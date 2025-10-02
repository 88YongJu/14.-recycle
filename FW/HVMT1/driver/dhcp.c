/**
 * @file		dhcp.c
 * @brief 		functions relative to dhcp
 */

#include <string.h>
#include <avr/wdt.h>

#include "dhcp.h"

u_char SRC_MAC_ADDR[6];				/**< Local MAC address */
u_char GET_SN_MASK[4];				/**< Subnet mask received from the DHCP server */
u_char GET_GW_IP[4];				/**< Gateway ip address received from the DHCP server */
u_char GET_DNS_IP[4] = "\x00\x00\x00\x00";	/**< DNS server ip address received from the DHCP server */
u_char GET_SIP[4] = {0,};				/**< Local ip address received from the DHCP server */

static u_char DHCP_SIP[4] = {0,};				/**< DNS server ip address is discovered */
static u_char DHCP_REAL_SIP[4] = {0,};			/**< For extract my DHCP server in a few DHCP servers */
static u_char OLD_SIP[4];				/**< Previous local ip address received from DHCP server */

static char dhcp_state;				/**< DHCP client status */
static char retry_count;				/**< retry count */

static u_char DHCP_timeout;				/**< DHCP Timeout flag */
static un_l2cval lease_time;				/**< Leased time */

static u_long DHCP_XID;				
static SOCKET DHCPC_SOCK;				/**< Socket for the DHCP client */
static RIP_MSG* pRIPMSG;					/**< Pointer for the DHCP message */

void (*dhcp_ip_update)(void) = 0;		/**< handler to be called when the IP address from DHCP server is updated */
void (*dhcp_ip_conflict)(void) = 0;		/**< handler to be called when the IP address from DHCP server is conflict */

static void send_DHCP_DISCOVER(SOCKET s);		/* Send the discovery message to the DHCP server */
static void send_DHCP_REQUEST(SOCKET s);		/* Send the request message to the DHCP server */
static void send_DHCP_RELEASE_DECLINE(SOCKET s,char msgtype);		/**< send the release message to the DHCP server */
static char parseDHCPMSG(SOCKET s, u_int length);	/* Receive the message from DHCP server and parse it. */
static char check_leasedIP(void);			/* Check the leased IP address	*/
static void check_DHCP_Timeout(void);			/* Check DHCP Timeout  */ 
static void proc_ip_conflict(void);			/* called when the leased IP address is conflict */

/**
 * @brief		This function sends DHCP DISCOVER message to DHCP server.
 */
static void send_DHCP_DISCOVER(
	SOCKET s	/**< a socket number. */
	)
{
	u_char ip[4];
	u_int i=0;
		
	//pRIPMSG = (RIP_MSG*)TX_BUF;
	pRIPMSG = (RIP_MSG*)TX1_BUF;
	
//	uint8 tx_buf[TX_RX_MAX_BUF_SIZE];
//	pRIPMSG = (RIP_MSG*)tx_buf;

	*((u_long*)DHCP_SIP)=0;
	*((u_long*)DHCP_REAL_SIP)=0;
	
	memset((void*)pRIPMSG,0,sizeof(RIP_MSG));

	pRIPMSG->op = DHCP_BOOTREQUEST;
	pRIPMSG->htype = DHCP_HTYPE10MB;
	pRIPMSG->hlen = DHCP_HLENETHERNET;
	pRIPMSG->hops = DHCP_HOPS;
	pRIPMSG->xid = htonl(DHCP_XID);
	pRIPMSG->secs = htons(DHCP_SECS);
	pRIPMSG->flags = htons(DHCP_FLAGSBROADCAST);
	pRIPMSG->chaddr[0] = SRC_MAC_ADDR[0];
	pRIPMSG->chaddr[1] = SRC_MAC_ADDR[1];
	pRIPMSG->chaddr[2] = SRC_MAC_ADDR[2];
	pRIPMSG->chaddr[3] = SRC_MAC_ADDR[3];
	pRIPMSG->chaddr[4] = SRC_MAC_ADDR[4];
	pRIPMSG->chaddr[5] = SRC_MAC_ADDR[5];

	// MAGIC_COOKIE
	pRIPMSG->OPT[i++] = (char)((MAGIC_COOKIE >> 24)& 0xFF);
	pRIPMSG->OPT[i++] = (char)((MAGIC_COOKIE >> 16)& 0xFF);
	pRIPMSG->OPT[i++] = (char)((MAGIC_COOKIE >> 8)& 0xFF);
	pRIPMSG->OPT[i++] = (char)(MAGIC_COOKIE& 0xFF);

	// Option Request Param.
	pRIPMSG->OPT[i++] = dhcpMessageType;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = DHCP_DISCOVER;

	// Client identifier
	pRIPMSG->OPT[i++] = dhcpClientIdentifier;
	pRIPMSG->OPT[i++] = 0x07;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];
	
	// host name
	pRIPMSG->OPT[i++] = hostName;
	pRIPMSG->OPT[i++] = strlen(HOST_NAME)+3; // length of hostname + 3
	strcpy((char*)&(pRIPMSG->OPT[i]),HOST_NAME);
	
	i+=strlen(HOST_NAME);
	
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];

	
	pRIPMSG->OPT[i++] = dhcpParamRequest;
	pRIPMSG->OPT[i++] = 0x06;
	pRIPMSG->OPT[i++] = subnetMask;
	pRIPMSG->OPT[i++] = routersOnSubnet;
	pRIPMSG->OPT[i++] = dns;
	pRIPMSG->OPT[i++] = domainName;
	pRIPMSG->OPT[i++] = dhcpT1value;
	pRIPMSG->OPT[i++] = dhcpT2value;
	pRIPMSG->OPT[i++] = endOption;

	// send broadcasting packet
	ip[0] = 255;
	ip[1] = 255;
	ip[2] = 255;
	ip[3] = 255;
	if(0 == sendto(s, (u_char *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT))
	{
		//DebugPrint("DHCP : Fatal Error(0).\r\n");
		if ( dhcp_ip_conflict != 0 )
			(*dhcp_ip_conflict)();
	}
	//DebugPrint("sent DHCP_DISCOVER\r\n");
}


/**
 * @brief		This function sends DHCP REQUEST message to DHCP server.
 */
static void send_DHCP_REQUEST(
	SOCKET s	/**<  socket number */
	)
{
	u_char ip[4];
	u_int i = 0;

	pRIPMSG = (RIP_MSG*)TX1_BUF;
//	uint8 tx_buf[TX_RX_MAX_BUF_SIZE];
//	pRIPMSG = (RIP_MSG*)tx_buf;
		
	memset((void*)pRIPMSG,0,sizeof(RIP_MSG));

	pRIPMSG->op = DHCP_BOOTREQUEST;
	pRIPMSG->htype = DHCP_HTYPE10MB;
	pRIPMSG->hlen = DHCP_HLENETHERNET;
	pRIPMSG->hops = DHCP_HOPS;
	pRIPMSG->xid = htonl(DHCP_XID);
	pRIPMSG->secs = htons(DHCP_SECS);

	if(dhcp_state < STATE_DHCP_LEASED)
		pRIPMSG->flags = htons(DHCP_FLAGSBROADCAST);
	else
	{
		pRIPMSG->flags = 0;		// For Unicast
		pRIPMSG->ciaddr[0] = GET_SIP[0];
		pRIPMSG->ciaddr[1] = GET_SIP[1];
		pRIPMSG->ciaddr[2] = GET_SIP[2];
		pRIPMSG->ciaddr[3] = GET_SIP[3];
	}		

	pRIPMSG->chaddr[0] = SRC_MAC_ADDR[0];
	pRIPMSG->chaddr[1] = SRC_MAC_ADDR[1];
	pRIPMSG->chaddr[2] = SRC_MAC_ADDR[2];
	pRIPMSG->chaddr[3] = SRC_MAC_ADDR[3];
	pRIPMSG->chaddr[4] = SRC_MAC_ADDR[4];
	pRIPMSG->chaddr[5] = SRC_MAC_ADDR[5];

	/* MAGIC_COOKIE */
	pRIPMSG->OPT[i++] = (u_char)((MAGIC_COOKIE >> 24) & 0xFF);
	pRIPMSG->OPT[i++] = (u_char)((MAGIC_COOKIE >> 16) & 0xFF);
	pRIPMSG->OPT[i++] = (u_char)((MAGIC_COOKIE >> 8) & 0xFF);
	pRIPMSG->OPT[i++] = (u_char)(MAGIC_COOKIE & 0xFF);

	/* Option Request Param. */
	pRIPMSG->OPT[i++] = dhcpMessageType;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = DHCP_REQUEST;

	pRIPMSG->OPT[i++] = dhcpClientIdentifier;
	pRIPMSG->OPT[i++] = 0x07;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];	

	if(dhcp_state < STATE_DHCP_LEASED)
	{
		pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
		pRIPMSG->OPT[i++] = 0x04;
		pRIPMSG->OPT[i++] = GET_SIP[0];
		pRIPMSG->OPT[i++] = GET_SIP[1];
		pRIPMSG->OPT[i++] = GET_SIP[2];
		pRIPMSG->OPT[i++] = GET_SIP[3];
	
		pRIPMSG->OPT[i++] = dhcpServerIdentifier;
		pRIPMSG->OPT[i++] = 0x04;
		pRIPMSG->OPT[i++] = DHCP_SIP[0];
		pRIPMSG->OPT[i++] = DHCP_SIP[1];
		pRIPMSG->OPT[i++] = DHCP_SIP[2];
		pRIPMSG->OPT[i++] = DHCP_SIP[3];
	}
	
	// host name
	pRIPMSG->OPT[i++] = hostName;
	pRIPMSG->OPT[i++] = strlen(HOST_NAME)+3; // length of hostname + 3
	strcpy((char*)&(pRIPMSG->OPT[i]),HOST_NAME);
	
	i+=strlen(HOST_NAME);
	
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];		
	
	pRIPMSG->OPT[i++] = dhcpParamRequest;
	pRIPMSG->OPT[i++] = 0x08;
	pRIPMSG->OPT[i++] = subnetMask;
	pRIPMSG->OPT[i++] = routersOnSubnet;
	pRIPMSG->OPT[i++] = dns;
	pRIPMSG->OPT[i++] = domainName;
	pRIPMSG->OPT[i++] = dhcpT1value;
	pRIPMSG->OPT[i++] = dhcpT2value;
	pRIPMSG->OPT[i++] = performRouterDiscovery;
	pRIPMSG->OPT[i++] = staticRoute;
	pRIPMSG->OPT[i++] = endOption;

	/* send broadcasting packet */
	if(dhcp_state < STATE_DHCP_LEASED)
	{
		ip[0] = 255;
		ip[1] = 255;
		ip[2] = 255;
		ip[3] = 255;
	}
	else
	{
		ip[0] = DHCP_SIP[0];
		ip[1] = DHCP_SIP[1];
		ip[2] = DHCP_SIP[2];
		ip[3] = DHCP_SIP[3];
	}

	if(0 == sendto(s, (u_char*)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT))
	{
		//DebugPrint("DHCP : Fatal Error(1).");
		if ( dhcp_ip_conflict != 0 )
			(*dhcp_ip_conflict)();
	}
	//DebugPrint("sent DHCP_REQUEST");
}


/**
 * @brief		This function sends DHCP RELEASE message to DHCP server.
 */
static void send_DHCP_RELEASE_DECLINE(
	SOCKET s,		/**< socket number */
	char msgtype	/**< 0 : RELEASE, Not Zero : DECLINE */
	)
{
	u_int i =0;
	u_char ip[4];
	
	pRIPMSG = (RIP_MSG*)TX1_BUF;
//	uint8 tx_buf[TX_RX_MAX_BUF_SIZE];
//	pRIPMSG = (RIP_MSG*)tx_buf;


	memset((void*)pRIPMSG,0,sizeof(RIP_MSG));

	pRIPMSG->op = DHCP_BOOTREQUEST;
	pRIPMSG->htype = DHCP_HTYPE10MB;
	pRIPMSG->hlen = DHCP_HLENETHERNET;
	pRIPMSG->hops = DHCP_HOPS;
	pRIPMSG->xid = htonl(DHCP_XID);
	pRIPMSG->secs = htons(DHCP_SECS);
	pRIPMSG->flags = 0;	//DHCP_FLAGSBROADCAST;

	pRIPMSG->chaddr[0] = SRC_MAC_ADDR[0];
	pRIPMSG->chaddr[1] = SRC_MAC_ADDR[1];
	pRIPMSG->chaddr[2] = SRC_MAC_ADDR[2];
	pRIPMSG->chaddr[3] = SRC_MAC_ADDR[3];
	pRIPMSG->chaddr[4] = SRC_MAC_ADDR[4];
	pRIPMSG->chaddr[5] = SRC_MAC_ADDR[5];


	/* MAGIC_COOKIE */
	pRIPMSG->OPT[i++] = (u_char)((MAGIC_COOKIE >> 24) & 0xFF);
	pRIPMSG->OPT[i++] = (u_char)((MAGIC_COOKIE >> 16) & 0xFF);
	pRIPMSG->OPT[i++] = (u_char)((MAGIC_COOKIE >> 8) & 0xFF);
	pRIPMSG->OPT[i++] = (u_char)(MAGIC_COOKIE & 0xFF);

	/* Option Request Param. */
	pRIPMSG->OPT[i++] = dhcpMessageType;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = ((!msgtype) ? DHCP_RELEASE : DHCP_DECLINE);

	pRIPMSG->OPT[i++] = dhcpClientIdentifier;
	pRIPMSG->OPT[i++] = 0x07;
	pRIPMSG->OPT[i++] = 0x01;
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[0];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[1];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[2];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[3];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[4];
	pRIPMSG->OPT[i++] = SRC_MAC_ADDR[5];	

	pRIPMSG->OPT[i++] = dhcpServerIdentifier;
	pRIPMSG->OPT[i++] = 0x04;
	pRIPMSG->OPT[i++] = DHCP_SIP[0];
	pRIPMSG->OPT[i++] = DHCP_SIP[1];
	pRIPMSG->OPT[i++] = DHCP_SIP[2];
	pRIPMSG->OPT[i++] = DHCP_SIP[3];

	if(msgtype)
	{
		pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
		pRIPMSG->OPT[i++] = 0x04;
		pRIPMSG->OPT[i++] = GET_SIP[0];
		pRIPMSG->OPT[i++] = GET_SIP[1];
		pRIPMSG->OPT[i++] = GET_SIP[2];
		pRIPMSG->OPT[i++] = GET_SIP[3];		
		pRIPMSG->OPT[i++] = endOption;
		//DebugPrint("sent DHCP_DECLINE");
	}
	else
	{
		pRIPMSG->OPT[i++] = endOption;
		//DebugPrint("sent DHCP_RELEASE");
	}
	
	if(!msgtype)
	{
		ip[0] = DHCP_SIP[0];
		ip[1] = DHCP_SIP[1];
		ip[2] = DHCP_SIP[2];
		ip[3] = DHCP_SIP[3];
	}
	else
	{
		ip[0] = 255;
		ip[1] = 255;
		ip[2] = 255;
		ip[3] = 255;
	}

	if(0 == sendto(s, (u_char *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT))
	{
		//DebugPrint("DHCP : Fatal Error(2).");
		
		if ( dhcp_ip_conflict != 0 )
			(*dhcp_ip_conflict)();
	}	
}


/**
 * @brief		This function parses the reply message from DHCP server.
 * @return	success - return type, fail - 0
 */
static char parseDHCPMSG(
	SOCKET s, 	/**< socket number */
	u_int length	/**< a size data to receive. */
	)
{
	u_char svr_addr[6];
	u_int  svr_port;

	pRIPMSG = (RIP_MSG*)RX1_BUF;
//	uint8 rx_buf[TX_RX_MAX_BUF_SIZE];
//	pRIPMSG = (RIP_MSG*)rx_buf;

	u_int len;
	u_char * p;
	u_char * e;
	u_char type, opt_len=0;

	len = recvfrom(s, (u_char *)pRIPMSG, length, svr_addr, &svr_port);

	//DebugPrint("DHCP_SIP:%d.%d.%d.%d",DHCP_SIP[0],DHCP_SIP[1],DHCP_SIP[2],DHCP_SIP[3]);
	//DebugPrint("DHCP_RIP:%d.%d.%d.%d",DHCP_REAL_SIP[0],DHCP_REAL_SIP[1],DHCP_REAL_SIP[2],DHCP_REAL_SIP[3]);
	//DebugPrint("svr_addr:%d.%d.%d.%d",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
	
	if(pRIPMSG->op != DHCP_BOOTREPLY)
	{
		//DebugPrint("DHCP : NO DHCP MSG");
	}
	else
	{
		if (svr_port == DHCP_SERVER_PORT)
		{
			if(memcmp(pRIPMSG->chaddr,SRC_MAC_ADDR,6) != 0 || pRIPMSG->xid != htonl(DHCP_XID))
			{
				//DebugPrint("No My DHCP Message. This message is ignored.");
				//DebugPrint("\tSRC_MAC_ADDR(%02X.%02X.%02X.",SRC_MAC_ADDR[0],SRC_MAC_ADDR[1],SRC_MAC_ADDR[2]);
				//DebugPrint("%02X.%02X.%02X)",SRC_MAC_ADDR[3],SRC_MAC_ADDR[4],SRC_MAC_ADDR[5]);
				//DebugPrint(", pRIPMSG->chaddr(%02X.%02X.%02X.",pRIPMSG->chaddr[0],pRIPMSG->chaddr[1],pRIPMSG->chaddr[2]);
				//DebugPrint("%02X.%02X.%02X)",pRIPMSG->chaddr[3],pRIPMSG->chaddr[4],pRIPMSG->chaddr[5]);
				//DebugPrint("\tpRIPMSG->xid(%08lX), DHCP_XID(%08lX)",pRIPMSG->xid,htonl(DHCP_XID));
				//DebugPrint("\tpRIMPMSG->yiaddr:%d.%d.%d.%d",pRIPMSG->yiaddr[0],pRIPMSG->yiaddr[1],pRIPMSG->yiaddr[2],pRIPMSG->yiaddr[3]);
				return 0;
			}

			if( *((u_long*)DHCP_SIP) != 0x00000000 )
			{
				if( *((u_long*)DHCP_REAL_SIP) != *((u_long*)svr_addr) && 
					*((u_long*)DHCP_SIP) != *((u_long*)svr_addr) ) 
				{
					//DebugPrint("Another DHCP sever send a response message. This is ignored.");
					//DebugPrint("\tIP:%d.%d.%d.%d",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
					return 0;								
				}
			}

			
			memcpy(GET_SIP,pRIPMSG->yiaddr,4);
			//DebugPrint("DHCP MSG received..");
			//DebugPrint("yiaddr : %d.%d.%d.%d",GET_SIP[0],GET_SIP[1],GET_SIP[2],GET_SIP[3]);
			
	
			type = 0;
			p = (u_char *)(&pRIPMSG->op);
			p = p + 240;
			e = p + (len - 240);
			//DebugPrint("p : 0x%08X  e : 0x%08X  len : %d\r\n", (u_int)p, (u_int)e, len);
			
	
			while ( p < e ) 
			{
				switch ( *p++ ) 
				{
				case endOption :
				 	return type;
					break;	
	       			case padOption :
					break;
				case dhcpMessageType :
					opt_len = *p++;
					type = *p;
					//DebugPrint("dhcpMessageType : %x", type);				
	
					break;
				case subnetMask :
					opt_len =* p++;
					memcpy(GET_SN_MASK,p,4);
					//DebugPrint("subnetMask : ");
					//DebugPrint("%d.%d.%d.%d",GET_SN_MASK[0],GET_SN_MASK[1],GET_SN_MASK[2],GET_SN_MASK[3]);				
	
					break;
				case routersOnSubnet :
					opt_len = *p++;
					memcpy(GET_GW_IP,p,4);
					//DebugPrint("routersOnSubnet : ");
					//DebugPrint("%d.%d.%d.%d",GET_GW_IP[0],GET_GW_IP[1],GET_GW_IP[2],GET_GW_IP[3]);			
	
					break;
				case dns :
					opt_len = *p++;
					memcpy(GET_DNS_IP,p,4);
					break;
				case dhcpIPaddrLeaseTime :
					opt_len = *p++;
					lease_time.lVal = ntohl(*((u_long*)p));
					//DebugPrint("dhcpIPaddrLeaseTime : %08lX", lease_time.lVal);
					
					break;
	
				case dhcpServerIdentifier :
					opt_len = *p++;
					//DebugPrint("DHCP_SIP : %d.%d.%d.%d", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
			
					
					if( *((u_long*)DHCP_SIP) == 0 || 
					    *((u_long*)DHCP_REAL_SIP) == *((u_long*)svr_addr) || 
					    *((u_long*)DHCP_SIP) == *((u_long*)svr_addr) )
					{
						memcpy(DHCP_SIP,p,4);
						memcpy(DHCP_REAL_SIP,svr_addr,4);	// Copy the real ip address of my DHCP server
						//DebugPrint("My dhcpServerIdentifier : ");
						//DebugPrint("%d.%d.%d.%d", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
						//DebugPrint("My DHCP server real IP address : ");
						//DebugPrint("%d.%d.%d.%d", DHCP_REAL_SIP[0], DHCP_REAL_SIP[1], DHCP_REAL_SIP[2], DHCP_REAL_SIP[3]);
					}
					else
					{
						//DebugPrint("Another dhcpServerIdentifier : ");
						//DebugPrint("\tMY(%d.%d.%d.%d) ", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
						//DebugPrint("Another(%d.%d.%d.%d) : ", svr_addr[0], svr_addr[1], svr_addr[2], svr_addr[3]);
					}

					break;
				default :
					opt_len = *p++;
					//DebugPrint("opt_len : %d", opt_len);
					break;
				} // switch
				p+=opt_len;
			} // while
		} // if
	}
	return 0;
}


/**
 * @brief		This function checks the state of DHCP.
 */
void check_DHCP_state(
	SOCKET s	/**< socket number */
	) 
{
	u_int len;
	u_char type;
	
	type = 0;

	if( s < MAX_SOCK_NUM && getSn_SR(s)!=SOCK_CLOSED)
	{
		if ((len = getSn_RX_RSR(s)) > 0)
		{
			 type = parseDHCPMSG(s, len);
		}
	}
	else if(!socket(s, Sn_MR_UDP, DHCP_CLIENT_PORT, 0x00))
	{
		//DebugPrint("Fail to create the DHCPC_SOCK(%d)\r\n", s);
	}
	
	switch ( dhcp_state )
	{
	case STATE_DHCP_DISCOVER :
		if (type == DHCP_OFFER) 
		{
			retry_count = 0;
			dhcp_time = 0;
			send_DHCP_REQUEST(s);
			dhcp_state = STATE_DHCP_REQUEST;
			DebugPrint("state : STATE_DHCP_REQUEST");
		}
		else check_DHCP_Timeout();
		break;

	case STATE_DHCP_REQUEST :
		if (type == DHCP_ACK) 
		{
			retry_count = 0;
			dhcp_time = 0;
			if (check_leasedIP()) 
			{
				set_DHCP_network();
				dhcp_state = STATE_DHCP_LEASED;
				DebugPrint("state : STATE_DHCP_LEASED");
			} 
			else 
			{
				dhcp_state = STATE_DHCP_DISCOVER;
				DebugPrint("state : STATE_DHCP_DISCOVER");
			}
		}
		else if (type == DHCP_NAK) 
		{
			retry_count = 0;
			dhcp_time = 0;
			dhcp_state = STATE_DHCP_DISCOVER;
			DebugPrint("state : STATE_DHCP_DISCOVER");
		}
		else check_DHCP_Timeout();
		break;

	case STATE_DHCP_LEASED :
		if ((lease_time.lVal != 0xffffffff) && ((lease_time.lVal/2) < dhcp_time)) 
		{
			type = 0;
			memcpy(OLD_SIP,GET_SIP,4);
			DHCP_XID++;
			send_DHCP_REQUEST(s);
			dhcp_state = STATE_DHCP_REREQUEST;
			DebugPrint("state : STATE_DHCP_REREQUEST");
			retry_count = 0;
			dhcp_time = 0;
		}
		break;

	case STATE_DHCP_REREQUEST :
		if (type == DHCP_ACK) 
		{
			if(memcmp(OLD_SIP,GET_SIP,4)!=0)	
			{
				DebugPrint("OLD_SIP=%s,GET_SIP=%s",inet_ntoa(ntohl(*((u_long*)OLD_SIP))), inet_ntoa(ntohl(*((u_long*)GET_SIP))));
					if ( dhcp_ip_update != 0 )
					(*dhcp_ip_update)();
				DebugPrint("The IP address from the DHCP server is updated.");
			}
			else
			{
				//DebugPrint("state : STATE_DHCP_LEASED : same IP");
			}
			retry_count = 0;
			dhcp_time = 0;
			dhcp_state = STATE_DHCP_LEASED;
		} 
		else if (type == DHCP_NAK) 
		{
			retry_count = 0;
			dhcp_time = 0;
			dhcp_state = STATE_DHCP_DISCOVER;
			//DebugPrint("state : STATE_DHCP_DISCOVER");
		} 
		else check_DHCP_Timeout();
		break;

	case STATE_DHCP_RELEASE :
		break;
	default :
		break;
	}
	
}


/**
 * @brief		This function checks the timeout of DHCP in each state.
 */
static void check_DHCP_Timeout(void)
{
	if (retry_count < MAX_DHCP_RETRY) 
	{
		//DebugPrint("dhcp_time : %d\r\n", dhcp_time);
		if (dhcp_time >= DHCP_WAIT_TIME) 
		{
			dhcp_time = 0;
			retry_count++;
			switch ( dhcp_state )
			{
			case STATE_DHCP_DISCOVER :
				DebugPrint("<<timeout>> state : STATE_DHCP_DISCOVER\r\n");
				send_DHCP_DISCOVER(DHCPC_SOCK);
				break;
	
			case STATE_DHCP_REQUEST :
				DebugPrint("<<timeout>> state : STATE_DHCP_REQUEST");
				send_DHCP_REQUEST(DHCPC_SOCK);
				break;

			case STATE_DHCP_REREQUEST :
				DebugPrint("<<timeout>> state : STATE_DHCP_REREQUEST");
				send_DHCP_REQUEST(DHCPC_SOCK);
				break;
			default :
				break;
			}
		}
	} 
	else 
	{
		retry_count = 0;
		dhcp_time = 0;
		DHCP_timeout = 1;
		
		send_DHCP_DISCOVER(DHCPC_SOCK);
		dhcp_state = STATE_DHCP_DISCOVER;
		DebugPrint("timeout\r\nstate : STATE_DHCP_DISCOVER");
	}
}


/**
 * @brief		This function loads network info. to iinChip
 */
void set_DHCP_network(void)
{
	iinchip_init();
	setGAR(GET_GW_IP);
	setSUBR(GET_SN_MASK);
	setSIPR(GET_SIP);
	setSHAR(SRC_MAC_ADDR);

	sysinit(0x55, 0x55);
	//DebugPrint("DHCP Set IP OK. %d.%d.%d.%d\r\n", GET_SIP[0], GET_SIP[1], GET_SIP[2], GET_SIP[3]);
}


/**
 * @brief		check if a leased IP is valid
 * @return	0 : conflict, 1 : no conflict
 */
static char check_leasedIP(void)
{

	u_int a;
	//DebugPrint("<Check the IP Conflict : ");
	// sendto is complete. that means there is a node which has a same IP.

	/* +200801 (hwkim) */
	//a = sendto(DHCPC_SOCK, (const u_char*)"CHECK_IP_CONFLICT", 17, GET_SIP, 5000); 
	a=0; // Skip checking IP Confilct ; W5100 reply itself to ARP requset with self-IP in non-switching network environment.
	/* ---------------*/
	if ( a> 0)
	{
		//DebugPrint(" Conflict>");
		send_DHCP_RELEASE_DECLINE(DHCPC_SOCK,1);
		if ( dhcp_ip_conflict != 0 )
			(*dhcp_ip_conflict)();
		return 0;
	}
	//DebugPrint(" No Conflict>");
	return 1;

}	


/**
 * @brief		Get an IP from the DHCP server.
 * @return	0 : timeout, 1 : get dhcp ip
 */

uint8 Set_Dhcp(unsigned char ch)
{
	uint8 st = 0;
	NETCONF	NetConf;
	char mac[6] = MY_NET_MAC;
	
	DebugPrint("DHCP Test Program. - START\r");	
	
	init_conf_variables();
	
	DebugPrint("%d : DHCP Client Start.\r\n", SOCK_DHCPC);
	
	get_netconf(&NetConf);
	
	memcpy(SRC_MAC_ADDR, mac, 6);

	init_dhcp_client(ch, evb_soft_reset, evb_soft_reset);//evb_soft_reset
	
	if(!getIP_DHCPS(ch))
	{
		DebugPrint("Fail to get a IP adress from DHCP server\r\n");
		DebugPrint("Apply the default network information!!!\r\n");
		NetInit();
		st = 0;
	}
	else
	{
		memcpy(&NetConf.sip, GET_SIP, 4);
		NetConf.sip  = *((u_long*)GET_SIP);
		NetConf.gwip = *((u_long*)GET_GW_IP);
		NetConf.sn   = *((u_long*)GET_SN_MASK);
		NetConf.dns  = *((u_long*)GET_DNS_IP);
		DebugPrint("Get network information from DHCP Server...\r\n");
		set_netconf(&NetConf);
		st = 1;
	}
	disconnect(ch);
	return st;
}

u_int getIP_DHCPS(unsigned char ch) 
{
	DebugPrint("DHCP SetIP..\r\n");
	
	send_DHCP_DISCOVER(ch);
	dhcp_state = STATE_DHCP_DISCOVER;

	retry_count = 0;
	dhcp_time = 0;
	DHCP_timeout = 0;

	while (dhcp_state != STATE_DHCP_LEASED)
	{
		//wdt_reset();
		//DebugPrint("%d",dhcp_state);
		if (DHCP_timeout == 1)
		{
			//kill_timer(DHCP_CHECK_TIMER);
			return 0;
		}
		check_DHCP_state(ch);
	}
	return 1;
}

/**
 * @brief		Get an IP from the DHCP server.
 */
void init_dhcp_client(
	SOCKET s,				/**< Socket number for the DHCP client */
	void(*ip_update)(void),	/**< handler called when the leased IP address is updated */
	void(*ip_conflict)(void)	/**< handler called when the leased IP address is conflict */
	)
{
	if(!ip_update)	dhcp_ip_update = set_DHCP_network;
	else		dhcp_ip_update = ip_update;

	if(!ip_conflict) dhcp_ip_conflict = proc_ip_conflict;
	else		 dhcp_ip_conflict = ip_conflict;

	init_dhcpc_ch(s);
}


/**
 * @brief		Get an IP from the DHCP server.
 */
static void proc_ip_conflict(void)
{
	//DebugPrint(	"The IP Address from DHCP server is CONFLICT!!!\r\n"
	//"Retry to get a IP address from DHCP server\r\n");
}

/**
 * @brief		Initialize the socket for DHCP client
 */
u_int init_dhcpc_ch(SOCKET s)
{
	u_int ret;

	DHCP_XID = 0x12345678;
	memset(GET_SIP,0,4);
	memset(GET_GW_IP,0,4);
	memset(GET_SN_MASK,0,4);
	
	iinchip_init();
	setSIPR(GET_SIP);
	setSHAR(SRC_MAC_ADDR);

	sysinit(0x55, 0x55);

	{
		//u_int i;
		//DebugPrint("MAC : ");
		//for (i = 0; i < 5; i++) DebugPrint("0x%02X.", IINCHIP_READ(SHAR0+i));
		//DebugPrint("0x%02X\r\n",IINCHIP_READ(SHAR0+5));
	}
	
	//DebugPrint("DHCP socket %d \r\n",s);
	if(!socket(s, Sn_MR_UDP, DHCP_CLIENT_PORT, 0x00))
	{
		//DebugPrint("fail..\r\n");
		ret = 0;
	}
	else
	{
		//DebugPrint("ok..\r\n");
		ret = 1;
	}
	DHCPC_SOCK = s;
	return ret;
}