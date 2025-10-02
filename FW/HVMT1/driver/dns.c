/**
 @file		dns.c
 @brief 		Realize Simple Domain Name System Protocol  \n
 			Detail DNS protocol refer to RFC 1034
 */

#include <string.h>
#include "dns.h"

static u_short dns_id = 0x1122;			/**< DNS query id  */
static u_char*  dns_buf;
static u_char* get_domain_name;
static u_long	get_domain_ip;				/**< Resolved ip address */
static QUERYDATA query_data;				/**< Query type */


static int dns_makequery(u_char op,char * qname);	/* make a DNS query */
static int dns_parse_reponse(void);			/* analyze a response from DNS sever */

static u_char * dns_parse_question(u_char * cp);	/* analyze a question part in resources recode */
static u_char * dns_answer(u_char *cp);		/* analyze a answer part in resources recode */
static int parse_name(char* cp,char* qname, u_int qname_maxlen);	/* analyze a qname in each part. */

volatile unsigned long con2u_ip = 0;	//DESC IP

/** 
 @brief	Transfer query message for user entered domain name to desiginated DNS Server
 @return	1 - DNS resolve Success, 0 - DNS resolve Failue
 */
u_char dns_query(
	SOCKET s, 				/**< socket handle */
	u_long dnsip, 			/**< DNS server ip address(32bit network ordering address) */
	u_char * domain_name, 	/**< if querydata value is BYNAME then use parameter for resolving domain ip \n
                                                  	BYIP   then use return value(resolved domain name from DNS) */
	u_long* domain_ip,		/**< if querydata value is BYNAME then use return value(resolved domain ip from DNS) \n
								BYIP   then use parameter for resolving domain name */
	QUERYDATA querydata, 	/**< BYNAME : use domain_name for resolving domain_ip  \n
								BYIP   : use domain_ip for resolving domain_name */
	u_int elapse				/**< wait for resopnse from DNS server (unit : 10ms) */
	)
{
	int len;
	u_int port;
	u_char reponse_received = 0;
	u_char* qname = 0;
	
	dns_buf = (u_char*) TX1_BUF;
//	uint8 tx_buf[TX_RX_MAX_BUF_SIZE];
//	dns_buf = tx_buf;

	get_domain_name = dns_buf + MAX_DNSMSG_SIZE;
	
	query_data = querydata;

	if(querydata==BYNAME)	qname = domain_name;
	else
	{
		qname = get_domain_name + MAX_QNAME_LEN;		
		
		////---- MODIFY_2006_06_05 :
		/*2. Host address to host name translation
		This function will often follow the form of previous
		functions.  Given a 32 bit IP address, the caller wants a
		character string.  The octets of the IP address are reversed,
		used as name components, and suffixed with "IN-ADDR.ARPA".  A
		type PTR query is used to get the RR with the primary name of
		the host.  For example, a request for the host name
		corresponding to IP address 1.2.3.4 looks for PTR RRs for
		domain name "4.3.2.1.IN-ADDR.ARPA". */

		// Check little or big endian
		//DebugPrint("domain_ip=%08lx",domain_ip);
		if(*domain_ip != ntohl(*domain_ip))		// if domain_ip is little-endian
			strcpy((char*)qname,inet_ntoa(*domain_ip));
		else	strcpy((char*)qname,inet_ntoa(swapl(*domain_ip))); // if domain_ip is big-endian then make the reverse ip address string.
		////---- MODIFY_END							

		strcat((char*)qname,".in-addr.arpa");		
	}
	if(socket(s, Sn_MR_UDP, 0, 0) == 0) return 0;

#ifdef	DEBUG_DNS	
	DebugPrint("Querying server %s by %s", inet_ntoa(ntohl(dnsip)), qname);
#endif

	len = dns_makequery(OP_QUERY,(char*)qname);				// create DNS query
	if(!len)
	{
#ifdef DEBUG_DNS
		DebugPrint("Fail to make query");		
#endif		
		return 0;
	}
#ifdef DEBUG_DNS
/*
{
	int i;
	DPRINT1("%d length dns query is made.",len);
	for(i = 0; i < len; i++)
	{
		 if(!(i % 0x10)) DebugPrint("");		
		 DPRINT1("%02X ",dns_buf[i]);
	}
	DebugPrint("\r\n");
}
*/
#endif
	sendto(s, dns_buf, len, (u_char*)&dnsip, IPPORT_DOMAIN);		// DNS query send to DNS server
#ifdef DEBUG_DNS
	DebugPrint("sent dns query to DNS server : [%s]",inet_ntoa(ntohl(dnsip)));		
#endif	
	while (elapse-- > 0) 
	{	
		Delay_ms(10);						// wait until Domain name resolution
		if ((len = getSn_RX_RSR(s)) > 0) 
		{
			if (len > MAX_DNSMSG_SIZE) len = MAX_DNSMSG_SIZE;
			len = recvfrom(s, dns_buf, len, (u_char*)&dnsip, &port);
			reponse_received = 1;
			break;			
		}
	}
	close(s);	
	if(!reponse_received) 
	{
#ifdef DEBUG_DNS		
		DebugPrint("No response from DNS server");
#endif		
		return 0;
	}
#ifdef DEBUG_DNS
/*
{
	int i;
	DPRINT2("%d received resonse from DNS Server[%s]\r\n",len,inet_ntoa(ntohl(dnsip)));
	for(i = 0; i < len ; i++)
	{
		if(!(i%0x10)) DebugPrint("");
		DPRINT1("%02X ",dns_buf[i]);
	}		
	DebugPrint("\r\n");
}	
*/
#endif	
	if(!dns_parse_reponse())					// Convert to local format
	{
#ifdef DEBUG_DNS
		DebugPrint("Fail to parse reponse from DNS server");
#endif		
		return 0;
	}
	if(query_data == BYNAME) *domain_ip = get_domain_ip;
	else strcpy((char*)domain_name, (char*)get_domain_name);
	
	return 1;
}


/** 
 @brief	Create query message for transferring to DNS server
 @return	size of query message
 */
static int dns_makequery(
	u_char op,		/**< queryÀÇ opcode (input) */
	char * qname	/**< size of query message */
	)
{
	u_char* query  = dns_buf;
	u_char* domain_tok;
	u_int domain_len;
	u_int qtype	= (query_data) ? TYPE_PTR : TYPE_A;	// Domain name pointer or Host Address 
	u_int qclass	= CLASS_IN; 				// Internet

	/* Make Qurey Header */
	memset(dns_buf,0,MAX_DNSMSG_SIZE);
	*((u_short*)query) = htons(dns_id);
#ifdef DEBUG_DNS
	DebugPrint("query : id = %02X%02X, ",query[0],query[1]);
#endif
	
	query += 2;

	*query = MAKE_FLAG0(QR_QUERY,op,0,0,1);			// Recursion desired
#ifdef DEBUG_DNS
	DebugPrint("opcode = %02X, ",*query);
#endif
	query++;
	*query = MAKE_FLAG1(0,0,0);
#ifdef DEBUG_DNS
	DebugPrint("rcode = %02X",*query);
#endif	
	query++;
	
	*((u_short*)query) = htons(1);
	
	query = dns_buf + DHDR_SIZE;

	/* Make Question Section */
	while(1)	// fill in QNAME filed with domain_name 
	{
		domain_tok = (u_char*)strchr((char*)qname,'.');
		if(domain_tok)	domain_len = ((u_int)domain_tok - (u_int)qname) & 0xFF;   
		else domain_len = strlen(qname);
		if(domain_len > 63)
		{
#ifdef DEBUG_DNS
			DebugPrint("Invalid label length because labels are restricted to 63 octets or less.");
#endif			
			return 0;		// since the label must begin with two zero bits because labels are restricted to 63 octets or less.
		}
		*query++ = domain_len;
		memcpy(query,qname,domain_len);
		qname += domain_len+1;
		query += domain_len;
		if(!domain_tok) break;
	}
	*query++ = '\0';			// terminate QNAME field with 'NULL'
	
	// fill in QTYPE field, for host address
	*query++ = qtype >> 8 & 0xFF;
	*query++ = qtype & 0xFF;
	
	// fill in QCLASS field, for internet
	*query++ = qclass >> 8 & 0xFF;
	*query++ = qclass & 0xFF;
	
	return (int)(query - dns_buf);		// return the size of generated query
}


/** 
 @brief	Analyze received DNS message packet and store it into structure
 @return	success - 1, fail - 0
 */
static int dns_parse_reponse(void)
{
	u_int i;
	DHDR dhdr;
	u_char* cur_ptr = dns_buf;
	
	
	dhdr.id = ntohs(*((u_short*)cur_ptr));
	if(dhdr.id != dns_id)
	{
#ifdef DEBUG_DNS
		DebugPrint("Responsed ID != Query ID : %d ! = %d",dhdr.id, dns_id); 
#endif 		
		return 0;
	}
	dns_id++;
	cur_ptr += 2;
	dhdr.flag0 = *cur_ptr++;
	dhdr.flag1 = *cur_ptr++;
	if(!(dhdr.flag0 & 0x80)|| !(dhdr.flag1 & 0x80) )
	{
#ifdef DEBUG_DNS
		DebugPrint("No reponse message, flag0 = 0x%02X, flag1 = 0x%02X",dhdr.flag0,dhdr.flag1);
#endif		
		return 0;
	}
	if(dhdr.flag1 & 0x0F)
	{
#ifdef DEBUG_DNS
		DebugPrint("Error of reponse : ");
		switch(dhdr.flag1 & 0x0F)
		{
		case RC_FORMAT_ERROR:
			DebugPrint("Format Error");
			break;
		case RC_SERVER_FAIL:
			DebugPrint("Server failure");
			break;
		case RC_NAME_ERROR:
			DebugPrint("Name Error");
			break;
		case RC_NOT_IMPL:
			DebugPrint("Not Implemented");
			break;
		case RC_REFUSED:
			DebugPrint("Refused");
			break;
		}
#endif		
		return 0;
	}	
	
	dhdr.qdcount = ntohs(*((u_short*)cur_ptr));
	cur_ptr += 2;
	dhdr.ancount = ntohs(*((u_short*)cur_ptr));
	cur_ptr += 2;
	dhdr.nscount = ntohs(*((u_short*)cur_ptr));
	cur_ptr += 2;
	dhdr.arcount = ntohs(*((u_short*)cur_ptr));	
	cur_ptr += 2;

#ifdef DEBUG_DNS
	DebugPrint("Response : question count = %d, answer count = %d",dhdr.qdcount,dhdr.ancount);
	DebugPrint("Response : authority count = %d, additiional count = %d",dhdr.nscount,dhdr.arcount);
#endif	

	/* Now parse the variable length sections */
	for(i = 0; i < dhdr.qdcount; i++) 
	{
		cur_ptr = dns_parse_question(cur_ptr);		// Question section
		if(!cur_ptr)
		{
#ifdef	DEBUG_DNS	
			DebugPrint("Fail to parse question section%d",i);	
#endif	
			return 0;
		}
	}
		
	/* parse resource records */
	
	for(i=0; i < dhdr.ancount; i++)
	{
		cur_ptr = dns_answer(cur_ptr);			// Answer section
		if(!cur_ptr)
		{
#ifdef	DEBUG_DNS	
			DebugPrint("Fail to parse answer section%d",i);	
#endif	
			 return 0;
		}
	}
		

	for(i = 0; i < dhdr.nscount; i++) 			// Name server (authority) section
	{			
		// if you need to authority section, insert user parse fuction into here.
	}

	for(i = 0; i < dhdr.arcount; i++) 			// Additional section
	{
		// if you need to additional section , insert user parse fuction into here.
	}
	return 1;
}


/** 
 @brief	Parse question section in the DNS query
 @return	success - 1, fail - 0 
 */
static u_char * dns_parse_question(
	u_char * cp	/**< curruent pointer to be parsed */
	)	
{
	int len;
	char name[MAX_QNAME_LEN];

	len = parse_name((char*)cp, name, sizeof(name));
	if(!len)
	{ 
#ifdef DEBUG_DNS
		DebugPrint("Fail to parse (Q)NAME field");
#endif				
		return 0;
	}

	cp += len;
	cp += 2;						// skip type
	cp += 2;						// skip class
#ifdef DEBUG_DNS
	DebugPrint("In question section, (Q)NAME field value : %s",name);
#endif	
	return cp;
}

/** 
 @brief	Parse answer section in the DNS query. Store resolved IP address into destination address
 @return	end address of answer section, fail - 0
 */
static u_char * dns_answer(
	u_char *cp	
	)
{
	int len, type;
	char qname[MAX_QNAME_LEN];
	u_long tip;

	len = parse_name((char*)cp, qname, sizeof(qname));
	
	if(!len) return 0;

	cp += len;
	type = *cp++;
	type = (type << 8) + (u_int)*cp++;			// type
	cp += 2;						// skip class
	cp += 4;						// skip ttl
	cp += 2;						// skip len

	switch(type)
	{
	case TYPE_A:			
		tip = 0;
		*((u_char*)&tip) = *cp++;			// Network odering
		*(((u_char*)&tip) + 1) = *cp++;
		*(((u_char*)&tip) + 2) = *cp++;
		*(((u_char*)&tip) + 3) = *cp++;
#ifdef DEBUG_DNS
		DebugPrint("RRs : TYPE_A = %s", inet_ntoa(ntohl(tip)));
#endif		
		if(query_data == BYNAME) get_domain_ip = tip;
		break;
	case TYPE_CNAME:
	case TYPE_MB:
	case TYPE_MG:
	case TYPE_MR:
	case TYPE_NS:
	case TYPE_PTR:
		len = parse_name((char*)cp, qname, sizeof(qname));	// These types all consist of a single domain name
		if(!len) return 0;				// convert it to ascii format
		cp += len;
		if(query_data == BYIP && type == TYPE_PTR)
		{
			 strcpy((char*)get_domain_name,qname);
#ifdef DEBUG_DNS
			DebugPrint("RRs : TYPE_PTR  = %s",qname);
#endif
		}						 
		break;
	case TYPE_HINFO:
		len = *cp++;
		cp += len;

		len = *cp++;
		cp += len;
		break;
	case TYPE_MX:
		cp += 2;
		len = parse_name((char*)cp, qname, sizeof(qname));	// Get domain name of exchanger
		if(!len)
		{
#ifdef DEBUG_DNS
			DebugPrint("TYPE_MX : Fail to get domain name of exechanger");
#endif					
			 return 0;
		}
		cp += len;
		break;
	case TYPE_SOA:
		len = parse_name((char*)cp, qname, sizeof(qname));	// Get domain name of name server
		if(!len)
		{
#ifdef DEBUG_DNS
			DebugPrint("TYPE_SOA : Fail to get domain name of name server");
#endif					
			 return 0;
		}

		cp += len;

		len = parse_name((char*)cp, qname, sizeof(qname));	// Get domain name of responsible person
		if(!len)
		{
#ifdef DEBUG_DNS
			DebugPrint("TYPE_SOA : Fail to get domain name of responsible person");
#endif					
			return 0;
		}
		cp += len;

		cp += 4;
		cp += 4;
		cp += 4;
		cp += 4;
		cp += 4;
		break;
	case TYPE_TXT:
		break;						// Just stash
	default:
		break;						// Ignore
	}
	return cp;
}


/** 
 @brief	Parse answer section in the DNS query. Store resolved IP address into destination address
 @return	end address of answer section, fail - 0
 */
static int parse_name(
	char* cp,				/**< Convert a compressed domain name to the human-readable form */
	char* qname, 		/**< store human-readable form(input,output); */
	u_int qname_maxlen	/**< qname_max_len - max length of qname(input) */
	)
{
	u_int slen;						// Length of current segment
	int clen = 0;						// Total length of compressed name
	int indirect = 0;					// Set if indirection encountered
	int nseg = 0;					// Total number of label in name

	for(;;)
	{
		slen = *cp++;				// Length of this segment
		if (!indirect) clen++;
		
		if ((slen & 0xc0) == 0xc0)			// Is used in compression scheme?
		{
			cp = (char*)&dns_buf[((slen & 0x3f)<<8) + *cp];	// Follow indirection
			if(!indirect)	clen++;
			indirect = 1;			
			slen = *cp++;
		}

		if (slen == 0)					// zero length == all done
			break;

		if (!indirect) clen += slen;

		if((qname_maxlen -= slen+1) < 0)
		{
#ifdef DEBUG_DNS
			DebugPrint("Not enough memory");
#endif			
			return 0;
		}
		while (slen-- != 0) *qname++ = (char)*cp++;
		*qname++ = '.';

		nseg++;
	}

	if(nseg == 0)	*qname++ = '.';				// Root name; represent as single dot
	else --qname;

	*qname = '\0';
#ifdef DEBUG_DNS
	DebugPrint1("Result of parsing (Q)NAME field : %s",qname);
#endif	
	return clen;						// Length of compressed message						// Length of compressed message
}