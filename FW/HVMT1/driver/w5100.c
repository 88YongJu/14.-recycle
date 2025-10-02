/*
@file		w5100.c
*/

#include "w5100.h"

static uint16 local_port;

static uint8 I_STATUS[MAX_SOCK_NUM];
static uint16 SMASK[MAX_SOCK_NUM]; /**< Variable for Tx buffer MASK in each channel */
static uint16 RMASK[MAX_SOCK_NUM]; /**< Variable for Rx buffer MASK in each channel */
static uint16 SSIZE[MAX_SOCK_NUM]; /**< Max Tx buffer size by each channel */
static uint16 RSIZE[MAX_SOCK_NUM]; /**< Max Rx buffer size by each channel */
static uint16 SBUFBASEADDRESS[MAX_SOCK_NUM]; /**< Tx buffer base address by each channel */
static uint16 RBUFBASEADDRESS[MAX_SOCK_NUM]; /**< Rx buffer base address by each channel */
static uint8 SUBN_VAR[4];

boolean f_Set_DHCP = 0;

/*///////////////////////////////////////////////////////
// INITIALIZE W5100
///////////////////////////////////////////////////////*/
void Init_SPI_W5100(void)
{
	unsigned char uc_SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0);
	unsigned char uc_SPSR = (0<<SPIF)|(1<<SPI2X);
	
	Init_SPI(uc_SPCR, uc_SPSR);
}

void Init_W5100(void)
{
	//unsigned char st=0;
	W5100_EN = 0;
	Delay_us(250);
	Delay_us(250);
	W5100_EN = 1;
	Delay_ms(50);
	Init_SPI_W5100();
	iinchip_init();
	Delay_ms(50);
	//f_Set_DHCP = Set_Dhcp(SOCK_1);
	NetInit();
	DisplayConfig();
	//return st;
}

void Reset_W5100(void)
{
	Init_SPI_W5100();
	iinchip_init();
	
	if(f_Set_DHCP)
	{
		set_DHCP_network();
	}
	else
	{
		NetInit();
	}
	//DisplayConfig();
}

void DisplayConfig(void)
{
	u_char i = 0;
	//u_long iaddr;
	
	DebugPrint("\r\n==============================================\r\n");
	DebugPrint("       Net Config Information\r\n");
	DebugPrint("==============================================\r\n");


	DebugPrint("MAC : ");
	for (i = 0; i < 5; i++) DebugPrint("0x%02X.", IINCHIP_READ(SHAR0+i));
	DebugPrint("0x%02X\r\n",IINCHIP_READ(SHAR0+5));

/*
	for(i=0; i < 6; i++)	addr[i] = IINCHIP_READ(SHAR0+i);
	printf("MAC ADDRESS      : 0x%02X.0x%02X.0x%02X.0x%02X.0x%02X.0x%02X\r\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
	*/
	DebugPrint("SUBNET MASK      : ");
	for(i=0; i < 4; i++)
	{
		DebugPrint("%d.", (char)IINCHIP_READ(SUBR0+i));
	}

	DebugPrint("\r\nG/W IP ADDRESS   : ");
	for(i=0; i < 4; i++)
	{
		DebugPrint("%d.", (char)IINCHIP_READ(GAR0+i));
	}

	DebugPrint("\r\nLOCAL IP ADDRESS : ");
	for(i=0; i < 4; i++)
	{
		DebugPrint("%d.", (char)IINCHIP_READ(SIPR0+i));
	}
	DebugPrint("\r\n==============================================\r\n");
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void NetInit(void)
{
	unsigned char mac[6]	= MY_NET_MAC;
	unsigned char gwip[4]	= MY_NET_GWIP;
	unsigned char sm[4]	= MY_SUBNET;
	unsigned char m_sip[4]	= MY_SOURCEIP;				//Source IP     	: 192.168.0.100
	//unsigned char m_dip[4] = MY_DESTINATIONIP;
	
	//Set MAC Address
	setSHAR(mac);
	//Set Gateway
	setGAR(gwip);
	//Set Subnet Mask
	setSUBR(sm);
	//Set My IP
	setSIPR(m_sip);
	
	sysinit(MY_NET_MEMALLOC, MY_NET_MEMALLOC);
}

uint8 getISR(uint8 s)
{
	return I_STATUS[s];
}

void putISR(uint8 s, uint8 val)
{
   I_STATUS[s] = val;
}

uint16 getIINCHIP_RxMAX(uint8 s)
{
   return RSIZE[s];
}
uint16 getIINCHIP_TxMAX(uint8 s)
{
   return SSIZE[s];
}
uint16 getIINCHIP_RxMASK(uint8 s)
{
   return RMASK[s];
}
uint16 getIINCHIP_TxMASK(uint8 s)
{
   return SMASK[s];
}
uint16 getIINCHIP_RxBASE(uint8 s)
{
   return RBUFBASEADDRESS[s];
}
uint16 getIINCHIP_TxBASE(uint8 s)
{
   return SBUFBASEADDRESS[s];
}

 /**
@brief	This function writes the data into W5100 registers.
*/
uint8 IINCHIP_WRITE(uint16 addr,uint8 data)
{
	W5100_CS = 0;                            // CS=0, waiting for SPI start
	SPI_Write_Data(CMD_WR);
	SPI_Write_Data((addr & 0xFF00) >> 8);
	SPI_Write_Data(addr & 0x00FF);
	SPI_Write_Data(data);
	W5100_CS = 1;							 // CS=1, SPI end
	return 1;
}

/**
@brief	This function reads the value from W5100 registers.
*/
uint8 IINCHIP_READ(uint16 addr)
{
	uint8 data;
	W5100_CS = 0;                            // CS=0, waiting for SPI start
	SPI_Write_Data(CMD_RD);
	SPI_Write_Data((addr & 0xFF00) >> 8);
	SPI_Write_Data(addr & 0x00FF);
	data = SPI_Read_Data();
	W5100_CS = 1;							 // CS=1, SPI end
	return data;
}

u_short swaps(u_int i)
{
	u_short ret=0;
	ret = (i & 0xFF) << 8;
	ret |= ((i >> 8)& 0xFF);
	return ret;
}

u_long swapl(u_long l)
{
	u_long ret=0;
	ret = (l & 0xFF) << 24;
	ret |= ((l >> 8) & 0xFF) << 16;
	ret |= ((l >> 16) & 0xFF) << 8;
	ret |= ((l >> 24) & 0xFF);
	return ret;
}

/**
@brief	htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian).
@return 	the value in TCP/IP network byte order
*/
unsigned long htonl(
unsigned long hostlong		/**< hostshort  - A 32-bit number in host byte order.  */
)
{
	#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return swapl(hostlong);
	#else
	return hostlong;
	#endif
}

/**
@brief	htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).
@return 	the value in TCP/IP network byte order
*/
unsigned short htons(
unsigned short hostshort	/**< A 16-bit number in host byte order.  */
)
{
	#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return swaps(hostshort);
	#else
	return hostshort;
	#endif
}

/**
@brief	ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
@return 	a 16-bit number in host byte order
*/
unsigned long ntohs(
unsigned short netshort	/**< netshort - network odering 16bit value */
)
{
	#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return htons(netshort);
	#else
	return netshort;
	#endif
}


/**
@brief	converts a unsigned long from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
@return 	a 16-bit number in host byte order
*/
unsigned long ntohl(unsigned long netlong)
{
	#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return htonl(netlong);
	#else
	return netlong;
	#endif
}

/**
@brief	This function writes into W5100 memory(Buffer)
*/ 
uint16 wiz_write_buf(uint16 addr,uint8* buf,uint16 len)
{
	uint16 idx = 0;
	//SPI MODE I/F
	for(idx=0;idx<len;idx++)
	{
		W5100_CS = 0;                             // CS=0, SPI start 
		SPI_Write_Data(0xF0);
		SPI_Write_Data(((addr+idx) & 0xFF00) >> 8);
		SPI_Write_Data((addr+idx) & 0x00FF);
		SPI_Write_Data(buf[idx]);
		W5100_CS = 1;                             // CS=1, SPI end 
	}
	return len;
}

void W5100_write_burst(uint16_t dst_ptr, uint8_t* buf, uint16_t size)
{
	// 루프에 들어가기 전에 CS를 한 번만 내리고, 명령어와 시작 주소를 전송
	W5100_CS = 0;                                 // CS=0, SPI start
	SPI_Write_Data(CMD_WR);                         // Write Opcode
	SPI_Write_Data(((uint16)dst_ptr & 0xFF00) >> 8); // Start Address High Byte
	SPI_Write_Data((uint16)dst_ptr & 0x00FF);      // Start Address Low Byte
	
	// for문 안에서는 순수 데이터만 연속으로 전송
	for(uint16_t idx=0; idx<size; idx++)
	{
		SPI_Write_Data(buf[idx]);
	}
	
	// 모든 데이터 전송이 끝나면 CS를 한 번만 올림
	W5100_CS = 1;                                 // CS=1, SPI end
}


/**
@brief	This function reads into W5100 memory(Buffer)
*/ 
uint16 wiz_read_buf(uint16 addr, uint8* buf,uint16 len)
{
	//SPI MODE I/F
	uint16 idx = 0;
	W5100_CS = 0;                             // CS=0, SPI start 
	SPI_Write_Data(CMD_RD);
	SPI_Write_Data(((addr+idx) & 0xFF00) >> 8);
	SPI_Write_Data((addr+idx) & 0x00FF);
	
	for (idx=0; idx<len; idx++)
	{
		buf[idx] = SPI_Read_Data();
	}
	W5100_CS = 1;                             // CS=1, SPI end
	return len;
}


/**
@brief	This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/ 
void iinchip_init(void)
{	
	setMR( MR_RST );
	#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	setMR( MR_IND | MR_AI );
	#ifdef __DEF_IINCHIP_DBG__
	printf("MR value is %d \r\n",IINCHIP_READ(MR));
	#endif
	#endif
}


/**
@brief	This function set the transmit & receive buffer size as per the channels is used

Note for TMSR and RMSR bits are as follows\n
bit 1-0 : memory size of channel #0 \n
bit 3-2 : memory size of channel #1 \n
bit 5-4 : memory size of channel #2 \n
bit 7-6 : memory size of channel #3 \n\n
Maximum memory size for Tx, Rx in the W5100 is 8K Bytes,\n
In the range of 8KBytes, the memory size could be allocated dynamically by each channel.\n
Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
and to data transmission and receiption from non-allocated channel may cause some problems.\n
If the 8KBytes memory is already  assigned to centain channel, \n
other 3 channels couldn't be used, for there's no available memory.\n
If two 4KBytes memory are assigned to two each channels, \n
other 2 channels couldn't be used, for there's no available memory.\n
*/
void sysinit(
uint8 tx_size, 	/**< tx_size Tx memory size (00 - 1KByte, 01- 2KBtye, 10 - 4KByte, 11 - 8KByte) */
uint8 rx_size		/**< rx_size Rx memory size (00 - 1KByte, 01- 2KBtye, 10 - 4KByte, 11 - 8KByte) */
)
{
	int16 i;
	int16 ssum,rsum;

	#ifdef __DEF_IINCHIP_DBG__
	printf("sysinit()\r\n");
	#endif

	ssum = 0;
	rsum = 0;
	
	IINCHIP_WRITE(TMSR,tx_size); /* Set Tx memory size for each channel */
	IINCHIP_WRITE(RMSR,rx_size);	 /* Set Rx memory size for each channel */

	SBUFBASEADDRESS[0] = (uint16)(__DEF_IINCHIP_MAP_TXBUF__);		/* Set base address of Tx memory for channel #0 */
	RBUFBASEADDRESS[0] = (uint16)(__DEF_IINCHIP_MAP_RXBUF__);		/* Set base address of Rx memory for channel #0 */

	#ifdef __DEF_IINCHIP_DBG__
	printf("Channel : SEND MEM SIZE : RECV MEM SIZE\r\n");
	#endif

	for (i = 0 ; i < MAX_SOCK_NUM; i++)       // Set the size, masking and base address of Tx & Rx memory by each channel
	{
		SSIZE[i] = (int16)(0);
		RSIZE[i] = (int16)(0);
		if (ssum < 8192)
		{
			switch((tx_size >> i*2) & 0x03)  // Set Tx memory size
			{
				case 0:
				SSIZE[i] = (int16)(1024);
				SMASK[i] = (uint16)(0x03FF);
				break;
				case 1:
				SSIZE[i] = (int16)(2048);
				SMASK[i] = (uint16)(0x07FF);
				break;
				case 2:
				SSIZE[i] = (int16)(4096);
				SMASK[i] = (uint16)(0x0FFF);
				break;
				case 3:
				SSIZE[i] = (int16)(8192);
				SMASK[i] = (uint16)(0x1FFF);
				break;
			}
		}
		if (rsum < 8192)
		{
			switch((rx_size >> i*2) & 0x03)     // Set Rx memory size
			{
				case 0:
				RSIZE[i] = (int16)(1024);
				RMASK[i] = (uint16)(0x03FF);
				break;
				case 1:
				RSIZE[i] = (int16)(2048);
				RMASK[i] = (uint16)(0x07FF);
				break;
				case 2:
				RSIZE[i] = (int16)(4096);
				RMASK[i] = (uint16)(0x0FFF);
				break;
				case 3:
				RSIZE[i] = (int16)(8192);
				RMASK[i] = (uint16)(0x1FFF);
				break;
			}
		}
		ssum += SSIZE[i];
		rsum += RSIZE[i];

		if (i != 0)             // Sets base address of Tx and Rx memory for channel #1,#2,#3
		{
			SBUFBASEADDRESS[i] = SBUFBASEADDRESS[i-1] + SSIZE[i-1];
			RBUFBASEADDRESS[i] = RBUFBASEADDRESS[i-1] + RSIZE[i-1];
		}
		#ifdef __DEF_IINCHIP_DBG__
		printf("%d : %.4x : %.4x : %.4x : %.4x\r\n", i, (uint16)SBUFBASEADDRESS[i], (uint16)RBUFBASEADDRESS[i], SSIZE[i], RSIZE[i]);
		#endif
	}
}


void setMR(uint8 val)
{
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)
	*((volatile uint8*)(MR)) = val;
#else
	/* 	DIRECT ACCESS	*/
	IINCHIP_WRITE(MR,val);
#endif
}


/**
@brief	This function sets up gateway IP address.
*/ 
void setGAR(
	uint8 * addr	/**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
	)
{
	IINCHIP_WRITE((GAR0 + 0),addr[0]);
	IINCHIP_WRITE((GAR0 + 1),addr[1]);
	IINCHIP_WRITE((GAR0 + 2),addr[2]);
	IINCHIP_WRITE((GAR0 + 3),addr[3]);
}

void getGWIP(uint8 * addr)
{
	addr[0] = IINCHIP_READ((GAR0 + 0));
	addr[1] = IINCHIP_READ((GAR0 + 1));
	addr[2] = IINCHIP_READ((GAR0 + 2));
	addr[3] = IINCHIP_READ((GAR0 + 3));
}


/**
@brief	It sets up SubnetMask address
*/ 
void setSUBR(
	uint8 * addr	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
	)
{
	IINCHIP_WRITE((SUBR0 + 0),addr[0]);
	IINCHIP_WRITE((SUBR0 + 1),addr[1]);
	IINCHIP_WRITE((SUBR0 + 2),addr[2]);
	IINCHIP_WRITE((SUBR0 + 3),addr[3]);
}


/**
@brief	This function sets up MAC address.
*/ 
void setSHAR(
	uint8 * addr	/**< a pointer to a 6 -byte array responsible to set the MAC address. */
	)
{
	IINCHIP_WRITE((SHAR0 + 0),addr[0]);
	IINCHIP_WRITE((SHAR0 + 1),addr[1]);
	IINCHIP_WRITE((SHAR0 + 2),addr[2]);
	IINCHIP_WRITE((SHAR0 + 3),addr[3]);
	IINCHIP_WRITE((SHAR0 + 4),addr[4]);
	IINCHIP_WRITE((SHAR0 + 5),addr[5]);
}


/**
@brief	This function sets up Source IP address.
*/
void setSIPR(
	uint8 * addr	/**< a pointer to a 4 -byte array responsible to set the Source IP address. */
	)
{
	IINCHIP_WRITE((SIPR0 + 0),addr[0]);
	IINCHIP_WRITE((SIPR0 + 1),addr[1]);
	IINCHIP_WRITE((SIPR0 + 2),addr[2]);
	IINCHIP_WRITE((SIPR0 + 3),addr[3]);
}

/**
@brief	This function gets Interrupt register in common register.
 */
uint8 getIR( void )
{
   return IINCHIP_READ(IR);
}



/**
@brief	This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission 
will be there as per RTR (Retry Time-value Register)setting
*/
void setRTR(uint16 timeout)
{
	IINCHIP_WRITE(RTR0,(uint8)((timeout & 0xff00) >> 8));
	IINCHIP_WRITE((RTR0 + 1),(uint8)(timeout & 0x00ff));
}


/**
@brief	This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time 
as per RTR & RCR register seeting then time out will occur.
*/
void setRCR(uint8 retry)
{
	IINCHIP_WRITE(RCR,retry);
}


/**
@brief	This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void setIMR(uint8 mask)
{
	IINCHIP_WRITE(IMR,mask); // must be setted 0x10.
}


/**
@brief	These below functions are used to get the Gateway, SubnetMask
		and Source Hardware Address (MAC Address) and Source IP address
*/
void getGAR(uint8 * addr)
{
	addr[0] = IINCHIP_READ(GAR0);
	addr[1] = IINCHIP_READ(GAR0+1);
	addr[2] = IINCHIP_READ(GAR0+2);
	addr[3] = IINCHIP_READ(GAR0+3);
}
void getSUBR(uint8 * addr)
{
	addr[0] = IINCHIP_READ(SUBR0);
	addr[1] = IINCHIP_READ(SUBR0+1);
	addr[2] = IINCHIP_READ(SUBR0+2);
	addr[3] = IINCHIP_READ(SUBR0+3);
}
void getSHAR(uint8 * addr)
{
	addr[0] = IINCHIP_READ(SHAR0);
	addr[1] = IINCHIP_READ(SHAR0+1);
	addr[2] = IINCHIP_READ(SHAR0+2);
	addr[3] = IINCHIP_READ(SHAR0+3);
	addr[4] = IINCHIP_READ(SHAR0+4);
	addr[5] = IINCHIP_READ(SHAR0+5);
}
void getSIPR(uint8 * addr)
{
	addr[0] = IINCHIP_READ(SIPR0);
	addr[1] = IINCHIP_READ(SIPR0+1);
	addr[2] = IINCHIP_READ(SIPR0+2);
	addr[3] = IINCHIP_READ(SIPR0+3);
}

/**************************************************
Set Hardware Address (MAC Address)
***************************************************/
void setSn_DHAR(SOCKET s, uint8 * addr)
{
	IINCHIP_WRITE((Sn_DHAR0(s) + 0),addr[0]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 1),addr[1]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 2),addr[2]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 3),addr[3]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 4),addr[4]);
	IINCHIP_WRITE((Sn_DHAR0(s) + 5),addr[5]);
}

/**************************************************
Get Hardware Address (MAC Address)
***************************************************/
void getSn_DHAR(SOCKET s, uint8 * addr)
{
	addr[0] = IINCHIP_READ(Sn_DHAR0(s));
	addr[1] = IINCHIP_READ(Sn_DHAR0(s)+1);
	addr[2] = IINCHIP_READ(Sn_DHAR0(s)+2);
	addr[3] = IINCHIP_READ(Sn_DHAR0(s)+3);
	addr[4] = IINCHIP_READ(Sn_DHAR0(s)+4);
	addr[5] = IINCHIP_READ(Sn_DHAR0(s)+5);
}

/**************************************************
Set Destination IP Address (IP Address)
***************************************************/
void setSn_DIPR(SOCKET s, uint8 * addr)
{
	IINCHIP_WRITE((Sn_DIPR0(s) + 0),addr[0]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
	IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
}

/**************************************************
Get Destination IP Address (IP Address)
***************************************************/
unsigned long GetDestAddr(SOCKET s)/**< Channel number which try to get destination IP Address */
{
	u_long addr=0;
	int i = 0;
	for(i=0; i < 4; i++)
	{
		addr <<=8;
		addr += IINCHIP_READ(Sn_DIPR0(s)+i);
	}
	return addr;
}

/**************************************************
Set Destination PORT Number (PORT Number)
***************************************************/
void setSn_DPORT(SOCKET s, uint8 * addr)
{
	IINCHIP_WRITE((Sn_DPORT0(s) + 0),addr[0]);
	IINCHIP_WRITE((Sn_DPORT0(s) + 1),addr[1]);
}

/**************************************************
Get Destination PORT Number (PORT Number)
***************************************************/
unsigned int GetDestPort(SOCKET s)/**< Channel number which try to get destination port */
{
	u_int port;
	port = ((u_int) IINCHIP_READ(Sn_DPORT0(s))) & 0x00FF;
	port <<= 8;
	port += ((u_int) IINCHIP_READ(Sn_DPORT0(s)+1)) & 0x00FF;
	return port;
}



/**
@brief	Convert 32bit Address(Host Ordering) into Dotted Decimal Format
@return 	a char pointer to a static buffer containing the text address in standard ".'' notation. Otherwise, it returns NULL.
*/
char* inet_ntoa(
unsigned long addr	/**< Pointer variable to store converted value(INPUT) */
)
{
	static char addr_str[16];
	memset(addr_str,0,16);
	sprintf(addr_str,"%d.%d.%d.%d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
	return addr_str;
}

/**
@brief	This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void setSn_MSS(SOCKET s, uint16 Sn_MSSR0)
{
	IINCHIP_WRITE(Sn_MSSR0(s),(uint8)((Sn_MSSR0 & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_MSSR0(s) + 1),(uint8)(Sn_MSSR0 & 0x00ff));
}

void setSn_TTL(SOCKET s, uint8 ttl)
{
   IINCHIP_WRITE(Sn_TTL(s), ttl);
}


/**
@brief	These below function is used to setup the Protocol Field of IP Header when
		executing the IP Layer RAW mode.
*/
void setSn_PROTO(SOCKET s, uint8 proto)
{
	IINCHIP_WRITE(Sn_PROTO(s),proto);
}


/**
@brief	get socket interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
uint8 getSn_IR(SOCKET s)
{
   return IINCHIP_READ(Sn_IR(s));
}


/**
@brief	 get socket status
*/
uint8 getSn_SR(SOCKET s)
{
   return IINCHIP_READ(Sn_SR(s));
}


/**
@brief	get socket TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
uint16 getSn_TX_FSR(SOCKET s)
{
	uint16 val=0,val1=0;
	do
	{
		val1 = IINCHIP_READ(Sn_TX_FSR0(s));
		val1 = (val1 << 8) + IINCHIP_READ(Sn_TX_FSR0(s) + 1);
      if (val1 != 0)
		{
   			val = IINCHIP_READ(Sn_TX_FSR0(s));
   			val = (val << 8) + IINCHIP_READ(Sn_TX_FSR0(s) + 1);
		}
	} while (val != val1);
   return val;
}


/**
@brief	 get socket RX recv buf size

This gives size of received data in receive buffer. 
*/
uint16 getSn_RX_RSR(SOCKET s)
{
	uint16 val=0,val1=0;
	do
	{
		val1 = IINCHIP_READ(Sn_RX_RSR0(s));
		val1 = (val1 << 8) + IINCHIP_READ(Sn_RX_RSR0(s) + 1);
      if(val1 != 0)
		{
   			val = IINCHIP_READ(Sn_RX_RSR0(s));
   			val = (val << 8) + IINCHIP_READ(Sn_RX_RSR0(s) + 1);
		}
	} while (val != val1);
   return val;
}


/**
@brief	 This function is being called by send() and sendto() function also. 

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void send_data_processing(SOCKET s, uint8 *data, uint16 len)
{
	uint16 ptr;
	ptr = IINCHIP_READ(Sn_TX_WR0(s));
	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR0(s) + 1);
	write_data(s, data, (uint8 *)(ptr), len);
	ptr += len;
	IINCHIP_WRITE(Sn_TX_WR0(s),(uint8)((ptr & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_TX_WR0(s) + 1),(uint8)(ptr & 0x00ff));
}


/**
@brief	This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void recv_data_processing(SOCKET s, uint8 *data, uint16 len)
{
	uint16 ptr;
	ptr = IINCHIP_READ(Sn_RX_RD0(s));
	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(s) + 1);
#ifdef __DEF_IINCHIP_DBG__
	printf("ISR_RX: rd_ptr : %.4x\r\n", ptr);
#endif
	read_data(s, (uint8 *)ptr, data, len); // read data
	ptr += len;
	IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
}


/**
@brief	for copy the data form application buffer to Transmite buffer of the chip.

This function is being used for copy the data form application buffer to Transmite
buffer of the chip. It calculate the actual physical address where one has to write
the data in transmite buffer. Here also take care of the condition while it exceed
the Tx memory uper-bound of socket.
*/
void write_data(SOCKET s, vuint8 * src, vuint8 * dst, uint16 len)
{
	uint16 size;
	uint16 dst_mask;
	uint8 * dst_ptr;

	dst_mask = (uint16)dst & getIINCHIP_TxMASK(s);
	dst_ptr = (uint8 *)(getIINCHIP_TxBASE(s) + dst_mask);
	
	if (dst_mask + len > getIINCHIP_TxMAX(s)) 
	{
		size = getIINCHIP_TxMAX(s) - dst_mask;
		wiz_write_buf((uint16)dst_ptr, (uint8*)src, size);
		dst += size;
		size = len - size;
		dst_ptr = (uint8 *)(getIINCHIP_TxBASE(s));
		wiz_write_buf((uint16)dst_ptr, (uint8*)src, size);
	} 
	else
	{
		wiz_write_buf((uint16)dst_ptr, (uint8*)src, len);
	}
}


/**
@brief	This function is being used for copy the data form Receive buffer of the chip to application buffer.

It calculate the actual physical address where one has to read
the data from Receive buffer. Here also take care of the condition while it exceed
the Rx memory uper-bound of socket.
*/
void read_data(SOCKET s, vuint8 * src, vuint8 * dst, uint16 len)
{
	uint16 size;
	uint16 src_mask;
	uint8 * src_ptr;

	src_mask = (uint16)src & getIINCHIP_RxMASK(s);
	src_ptr = (uint8 *)(getIINCHIP_RxBASE(s) + src_mask);
	
	if( (src_mask + len) > getIINCHIP_RxMAX(s) ) 
	{
		size = getIINCHIP_RxMAX(s) - src_mask;
		wiz_read_buf((uint16)src_ptr, (uint8*)dst,size);
		dst += size;
		size = len - size;
		src_ptr = (uint8 *)(getIINCHIP_RxBASE(s));
		wiz_read_buf((uint16)src_ptr, (uint8*) dst,size);
	} 
	else
	{
		wiz_read_buf((uint16)src_ptr, (uint8*) dst,len);
	}
}


#ifdef __DEF_IINCHIP_PPP__
#define PPP_OPTION_BUF_LEN 64
uint8 pppinit_in(uint8 * id, uint8 idlen, uint8 * passwd, uint8 passwdlen);


/**
@brief	make PPPoE connection
@return	1 => success to connect, 2 => Auth fail, 3 => timeout, 4 => Auth type not support

*/

uint8 pppinit(uint8 * id, uint8 idlen, uint8 * passwd, uint8 passwdlen)
{
	uint8 ret;
	uint8 isr;
	
	// PHASE0. W5100 PPPoE(ADSL) setup
	// enable pppoe mode
	printf("-- PHASE 0. W5100 PPPoE(ADSL) setup process --\r\n");
	printf("\r\n");
	IINCHIP_WRITE(MR,IINCHIP_READ(MR) | MR_PPPOE);

	// open socket in pppoe mode
	isr = IINCHIP_READ(Sn_IR(0));// first clear isr(0), W5100 at present time
	IINCHIP_WRITE(Sn_IR(0),isr);
	
	IINCHIP_WRITE(PTIMER,200); // 5sec timeout
	IINCHIP_WRITE(PMAGIC,0x01); // magic number
	IINCHIP_WRITE(Sn_MR(0),Sn_MR_PPPOE);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_OPEN);
	
	ret = pppinit_in(id, idlen, passwd, passwdlen);

	// close ppp connection socket
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_CLOSE);
	return ret;
}


uint8 pppinit_in(uint8 * id, uint8 idlen, uint8 * passwd, uint8 passwdlen)
{
	uint8 loop_idx = 0;
	uint8 isr = 0;
	uint8 buf[PPP_OPTION_BUF_LEN];
	uint16 len;
	uint8 str[PPP_OPTION_BUF_LEN];
	uint8 str_idx,dst_idx;

   // PHASE1. PPPoE Discovery
	// start to connect pppoe connection
	printf("-- PHASE 1. PPPoE Discovery process --");
	printf(" ok\r\n");
	printf("\r\n");
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCON);
	wait_10ms(100);

	loop_idx = 0;
	//check whether PPPoE discovery end or not
	while (!(IINCHIP_READ(Sn_IR(0)) & Sn_IR_PNEXT))
	{
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout before LCP\r\n"); 
			return 3;
		}
		wait_10ms(100);
	}

   // PHASE2. LCP process
	printf("-- PHASE 2. LCP process --");
		
	// send LCP Request
	{
		// Magic number option
		// option format (type value + length value + data)
	   // write magic number value
		buf[0] = 0x05; // type value
		buf[1] = 0x06; // length value
		buf[2] = 0x01; buf[3] = 0x01; buf[4] = 0x01; buf[5]= 0x01; // data
		// for MRU option, 1492 0x05d4  
		// buf[6] = 0x01; buf[7] = 0x04; buf[8] = 0x05; buf[9] = 0xD4;
	}
	send_data_processing(0, buf, 0x06);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send request 
	wait_10ms(100);

	while (!((isr = IINCHIP_READ(Sn_IR(0))) & Sn_IR_PNEXT))
	{
		if (isr & Sn_IR_PRECV) // Not support option
		{
			len = getSn_RX_RSR(0);
			if ( len > 0 )
			{
				recv_data_processing(0, str, len);
				IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
				// for debug
				//printf("LCP proc\r\n"); for (i = 0; i < len; i++) printf ("%02x ", str[i]); printf("\r\n");
				// get option length
				len = str[4]; len = ((len & 0x00ff) << 8) + str[5];
				len += 2;
				str_idx = 6; dst_idx = 0; // ppp header is 6 byte, so starts at 6.
				do 
				{
					if ((str[str_idx] == 0x01) || (str[str_idx] == 0x02) || (str[str_idx] == 0x03) || (str[str_idx] == 0x05))
					{
						// skip as length of support option. str_idx+1 is option's length.
						str_idx += str[str_idx+1];
					}
					else
					{
						// not support option , REJECT
						memcpy((uint8 *)(buf+dst_idx), (uint8 *)(str+str_idx), str[str_idx+1]);
						dst_idx += str[str_idx+1]; str_idx += str[str_idx+1];
					}
				} while (str_idx != len);
	   			// for debug
	   			// printf("LCP dst proc\r\n"); for (i = 0; i < dst_idx; i++) printf ("%02x ", dst[i]); printf("\r\n");
	   
	   			// send LCP REJECT packet
	   			send_data_processing(0, buf, dst_idx);
	   			IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCJ);
      			}
		}
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after LCP\r\n");
			return 3;
		}
		wait_10ms(100);
	}
	printf(" ok\r\n");
	printf("\r\n");

	printf("-- PHASE 3. PPPoE(ADSL) Authentication mode --\r\n");
	printf("Authentication protocol : %.2x %.2x, ", IINCHIP_READ(PATR0), IINCHIP_READ(PATR0+1));

	loop_idx = 0;
	if (IINCHIP_READ(PATR0) == 0xc0 && IINCHIP_READ(PATR0+1) == 0x23)
	{
		printf("PAP\r\n"); // in case of adsl normally supports PAP.
		// send authentication data
		// copy (idlen + id + passwdlen + passwd)
		buf[loop_idx] = idlen; loop_idx++;
		memcpy((uint8 *)(buf+loop_idx), (uint8 *)(id), idlen); loop_idx += idlen;
		buf[loop_idx] = passwdlen; loop_idx++;
		memcpy((uint8 *)(buf+loop_idx), (uint8 *)(passwd), passwdlen); loop_idx += passwdlen;
		send_data_processing(0, buf, loop_idx);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
		wait_10ms(100);
	}	
	else if (IINCHIP_READ(PATR0) == 0xc2 && IINCHIP_READ(PATR0+1) == 0x23)
	{
		uint8 chal_len;
		md5_ctx context;
		uint8  digest[16];

		len = getSn_RX_RSR(0);
		if ( len > 0 )
		{
			recv_data_processing(0, str, len);
			IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
#ifdef __DEF_IINCHIP_DBG__
			printf("recv CHAP\r\n");
			{
				int16 i;
				
				for (i = 0; i < 32; i++) 
					printf ("%02x ", str[i]);
			}
			printf("\r\n");
#endif
// str is C2 23 xx CHAL_ID xx xx CHAP_LEN CHAP_DATA
// index  0  1  2  3       4  5  6        7 ...

			memset(buf,0x00,64);
			buf[loop_idx] = str[3]; loop_idx++; // chal_id
			memcpy((uint8 *)(buf+loop_idx), (uint8 *)(passwd), passwdlen); loop_idx += passwdlen; //passwd
			chal_len = str[6]; // chal_id
			memcpy((uint8 *)(buf+loop_idx), (uint8 *)(str+7), chal_len); loop_idx += chal_len; //challenge
			buf[loop_idx] = 0x80;
#ifdef __DEF_IINCHIP_DBG__
			printf("CHAP proc d1\r\n");
			{
				int16 i;
				for (i = 0; i < 64; i++) 
					printf ("%02x ", buf[i]);
			}
			printf("\r\n");
#endif

			md5_init(&context);
			md5_update(&context, buf, loop_idx);
			md5_final(digest, &context);

#ifdef __DEF_IINCHIP_DBG__
			printf("CHAP proc d1\r\n");
			{
				int16 i;				
				for (i = 0; i < 16; i++) 
					printf ("%02x", digest[i]);
			}
			printf("\r\n");
#endif
			loop_idx = 0;
			buf[loop_idx] = 16; loop_idx++; // hash_len
			memcpy((uint8 *)(buf+loop_idx), (uint8 *)(digest), 16); loop_idx += 16; // hashed value
			memcpy((uint8 *)(buf+loop_idx), (uint8 *)(id), idlen); loop_idx += idlen; // id
			send_data_processing(0, buf, loop_idx);
			IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
			wait_10ms(100);
		}
	}
	else
	{
		printf("Not support\r\n");
#ifdef __DEF_IINCHIP_DBG__
		printf("Not support PPP Auth type: %.2x%.2x\r\n",IINCHIP_READ(PATR0), IINCHIP_READ(PATR0+1));
#endif
		return 4;
	}
	printf("\r\n");

	printf("-- Waiting for PPPoE server's admission --");
	loop_idx = 0;
	while (!((isr = IINCHIP_READ(Sn_IR(0))) & Sn_IR_PNEXT))
	{
		if (isr & Sn_IR_PFAIL)
		{
			printf("failed\r\nReinput id, password..\r\n");
			return 2;
		}
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after PAP\r\n");
			return 3;
		}
		wait_10ms(100);
	}
	printf("ok\r\n");
	printf("\r\n");
	printf("-- PHASE 4. IPCP process --");
	// IP Address
	buf[0] = 0x03; buf[1] = 0x06; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00;
	send_data_processing(0, buf, 6);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
	wait_10ms(100);

	loop_idx = 0;
	while (1)
	{
		if (IINCHIP_READ(Sn_IR(0)) & Sn_IR_PRECV)
		{
			len = getSn_RX_RSR(0);
			if ( len > 0 )
			{
				recv_data_processing(0, str, len);
				IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
	   			//for debug
	   			//printf("IPCP proc\r\n"); for (i = 0; i < len; i++) printf ("%02x ", str[i]); printf("\r\n");
	   			str_idx = 6; dst_idx = 0;
	   			if (str[2] == 0x03) // in case of NAK
	   			{
	   				do 
	   				{
	   					if (str[str_idx] == 0x03) // request only ip information
	   					{
	   						memcpy((uint8 *)(buf+dst_idx), (uint8 *)(str+str_idx), str[str_idx+1]);
	   						dst_idx += str[str_idx+1]; str_idx += str[str_idx+1];
	   					}
	   					else
	   					{
	   						// skip byte
	   						str_idx += str[str_idx+1];
	   					}
	   					// for debug
	   					//printf("s: %d, d: %d, l: %d", str_idx, dst_idx, len);
	   				} while (str_idx != len);
	   				send_data_processing(0, buf, dst_idx);
	   				IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send ipcp request
	   				wait_10ms(100);
	   				break;
	   			}
			}
		}
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after IPCP\r\n");
			return 3;
		}
		wait_10ms(100);
		send_data_processing(0, buf, 6);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); //ipcp re-request
	}

	loop_idx = 0;
	while (!(IINCHIP_READ(Sn_IR(0)) & Sn_IR_PNEXT))
	{
		printf(".");
		if (loop_idx++ == 10) // timeout
		{
			printf("timeout after IPCP NAK\r\n");
			return 3;
		}
		wait_10ms(100);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send ipcp request
	}
	printf("ok\r\n");
	printf("\r\n");
	return 1;
	// after this function, User must save the pppoe server's mac address and pppoe session id in current connection
}


/**
@brief	terminate PPPoE connection
*/
uint8 pppterm(uint8 * mac, uint8 * sessionid)
{
	uint16 i;
	uint8 isr;
#ifdef __DEF_IINCHIP_DBG__
	printf("pppterm()\r\n");
#endif
	/* Set PPPoE bit in MR(Common Mode Register) : enable socket0 pppoe */
	IINCHIP_WRITE(MR,IINCHIP_READ(MR) | MR_PPPOE);
	
	// write pppoe server's mac address and session id 
	// must be setted these value.
	for (i = 0; i < 6; i++) IINCHIP_WRITE((Sn_DHAR0(0)+i),mac[i]);
	for (i = 0; i < 2; i++) IINCHIP_WRITE((Sn_DPORT0(0)+i),sessionid[i]);
	isr = IINCHIP_READ(Sn_IR(0));
	IINCHIP_WRITE(Sn_IR(0),isr);
	
	//open socket in pppoe mode
	IINCHIP_WRITE(Sn_MR(0),Sn_MR_PPPOE);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_OPEN);
	wait_1us(1);
	// close pppoe connection
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PDISCON);
	wait_10ms(100);
	// close socket
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_CLOSE);

#ifdef __DEF_IINCHIP_DBG__
	printf("pppterm() end ..\r\n");
#endif

	return 1;
}

#endif



/**
 * @brief	set system infomation
*/
void set_sysinfo(SYSINFO* pSysInfo)
{
	eeprom_write_block((void*)pSysInfo,(void*)SYS_INFO, sizeof(SYSINFO));	
}


/**
 * @brief	get system infomation
*/
void get_sysinfo(
	SYSINFO* pSysInfo	/**< system infomation pointer */
	)
{
	if(eeprom_read_word((u_int*)SYS_TEST) != TEST_VALUE)
	{
		pSysInfo->test = TEST_VALUE;
		pSysInfo->ver  = FW_VERSION;
		pSysInfo->auto_reset = SYSTEM_MANUAL_RESET;
		pSysInfo->any_port = SYSTEM_ANY_PORT;
		pSysInfo->serinum[0] = '2';
		pSysInfo->serinum[1] = 'U';
		pSysInfo->serinum[2] = 'E';
		pSysInfo->serinum[3] = '1';
		pSysInfo->serinum[4] = '-';
		pSysInfo->serinum[5] = '7';
		pSysInfo->serinum[6] = 'D';
		pSysInfo->serinum[7] = 'C';
		pSysInfo->serinum[8] = '0';
		pSysInfo->serinum[9] = '0';
		pSysInfo->serinum[10] = '0';
		pSysInfo->serinum[11] = '0';
		pSysInfo->serinum[12] = '0';
		pSysInfo->serinum[13] = '3';
		pSysInfo->serinum[14] = '8';
		pSysInfo->serinum[15] = 'D';
		set_sysinfo(pSysInfo);
	}
	else
	{
		eeprom_read_block((void*)pSysInfo,(void*)SYS_INFO, sizeof(SYSINFO));

		pSysInfo->ver = FW_VERSION;
	}
}


/**
 * @brief		set channel configuration
 * @return	pointer to be saved
*/
void set_chconf(CHCONF* pChConf)
{
	eeprom_write_block((void*)pChConf,(void*)CH_CONF, sizeof(CHCONF));
}

/**
 * @brief		get channel configuration
 * @return	pointer to be saved
*/
void get_chconf(CHCONF* pChConf)
{
	if(eeprom_read_word((void*)CH_CONF) != TEST_VALUE)
	{
		//u_int i;
		pChConf->test = TEST_VALUE;
//		for(i=0;i < 4; i++)
//		{
//			pChConf->ch[i].type  = DEFAULT_CH_TYPE;
//			pChConf->ch[i].port = DEFAULT_LISTEN_PORT;
//			pChConf->ch[i].destip = 0;
//		}
		pChConf->ch[0].type  = DHCP_CLIENT;
		pChConf->ch[0].port = DEFAULT_LISTEN_PORT;
		pChConf->ch[0].destip = 0;
		pChConf->ch[1].type  = LB_TCPC;
		pChConf->ch[1].port = DEFAULT_CONNECT_PORT;
		pChConf->ch[1].destip = htonl(DEFAULT_CH_DESTIP);
		pChConf->ch[2].type  = DEFAULT_CH_TYPE;
		pChConf->ch[2].port = DEFAULT_LISTEN_PORT;
		pChConf->ch[2].destip = 0;
		pChConf->ch[3].type  = DEFAULT_CH_TYPE;
		pChConf->ch[3].port = DEFAULT_LISTEN_PORT;
		pChConf->ch[3].destip = 0;
	}
	else eeprom_read_block((void*)pChConf,(void*)CH_CONF, sizeof(CHCONF));
}

/**
 * @brief	set network configuration
*/
void set_netconf(NETCONF* pNetConf)
{
	eeprom_write_block((void*)pNetConf,(void*)NET_CONF, sizeof(NETCONF));	
}


/**
 * @brief		get network configuration
 * @return	pointer to be saved
*/
void get_netconf(NETCONF* pNetConf)
{
	if(eeprom_read_word((void*)NET_TEST) != TEST_VALUE)
	{
		pNetConf->test = TEST_VALUE;
		eeprom_read_block((void*)pNetConf->mac, (void*)NET_MAC,6);
		if(!memcmp(pNetConf->mac,"\xFF\xFF\xFF\xFF\xFF\xFF",6) || !memcmp(pNetConf->mac,"\x00\x00\x00\x00\x00\x00",6))
		{
			memcpy(pNetConf->mac,DEFAULT_NET_MAC,6);
		}

		pNetConf->sip	= htonl(DEFAULT_NET_SIP);
		pNetConf->gwip	= htonl(DEFAULT_NET_GWIP);
		pNetConf->sn	= htonl(DEFAULT_NET_SN);
		pNetConf->dns	= htonl(DEFAULT_NET_DNS);
		pNetConf->Mem_alloc = DEFAULT_NET_MEMALLOC;
		
		set_netconf(pNetConf);
	}
	else eeprom_read_block((u_char*)pNetConf,(void*)NET_CONF, sizeof(NETCONF));

}

//------------------------------------------------soket control

static uint16 local_port;

/**
@brief	This Socket function initialize the channel in perticular mode, and set the port and wait for W5100 done it.
@return 	1 for sucess else 0.
*/  
uint8 socket(
	SOCKET s, 		/**< for socket number */
	uint8 protocol, 	/**< for socket protocol */
	uint16 port, 		/**< the source port for the socket */
	uint8 flag		/**< the option for the socket */
	)
{
	uint8 ret;
#ifdef __DEF_IINCHIP_DBG__
	DebugPrint("socket()\r\n");
#endif
	if ((protocol == Sn_MR_TCP) || (protocol == Sn_MR_UDP) || (protocol == Sn_MR_IPRAW) || (protocol == Sn_MR_MACRAW) || (protocol == Sn_MR_PPPOE))
	{
		close(s);
		IINCHIP_WRITE(Sn_MR(s),protocol | flag);
		if (port != 0) {
			IINCHIP_WRITE(Sn_PORT0(s),(uint8)((port & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_PORT0(s) + 1),(uint8)(port & 0x00ff));
		} else {
			local_port++; // if don't set the source port, set local_port number.
			IINCHIP_WRITE(Sn_PORT0(s),(uint8)((local_port & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_PORT0(s) + 1),(uint8)(local_port & 0x00ff));
		}
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_OPEN); // run sockinit Sn_CR
		ret = 1;
	}
	else
	{
		ret = 0;
	}
#ifdef __DEF_IINCHIP_DBG__
	printf("Sn_SR = %.2x , Protocol = %.2x\r\n", IINCHIP_READ(Sn_SR(s)), IINCHIP_READ(Sn_MR(s)));
#endif
	return ret;
}


/**
@brief	This function close the socket and parameter is "s" which represent the socket number
*/ 
void close(SOCKET s)
{
#ifdef __DEF_IINCHIP_DBG__
	printf("close()\r\n");
#endif
	IINCHIP_WRITE(Sn_CR(s),Sn_CR_CLOSE);
}


/**
@brief	This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return	1 for success else 0.
*/ 
uint8 listen(SOCKET s)	/**< the socket number */
{
	uint8 ret;
#ifdef __DEF_IINCHIP_DBG__
DebugPrint("listen()\r\n");
#endif
	if (IINCHIP_READ(Sn_SR(s)) == SOCK_INIT)
	{
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_LISTEN);
		ret = 1;
	}
	else
	{
		ret = 0;
#ifdef __DEF_IINCHIP_DBG__
DebugPrint("Fail[invalid ip,port]\r\n");
#endif
	}
	return ret;
}


/**
@brief	This function established  the connection for the channel in Active (client) mode. 
		This function waits for the untill the connection is established.
		
@return	1 for success else 0.
*/ 
uint8 connect(SOCKET s, uint8 * addr, uint16 port)
{
	uint8 ret;
	if (
			((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
		 	((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
		 	(port == 0x00) 
		) 
 	{
 		ret = 0;
#ifdef __DEF_IINCHIP_DBG__
	printf("Fail[invalid ip,port]\r\n");
#endif
	}
	else
	{
		
		ret = 1;
		// set destination IP
		IINCHIP_WRITE(Sn_DIPR0(s),addr[0]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
		IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
		IINCHIP_WRITE(Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
		IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8)(port & 0x00ff));
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_CONNECT);
		
		#ifdef __DEF_IINCHIP_DBG__
		printf("connect()\r\n");
		printf("\r\nDESTINATION IP ADDRESS : ");
		for(unsigned char i=0; i < 4; i++)
		{
			printf("%d.", (char)IINCHIP_READ(Sn_DIPR0(SOCK_TCPC)+i));
		}
		#endif
		// wait for completion
		while (IINCHIP_READ(Sn_CR(s)))
		{
			if (IINCHIP_READ(Sn_SR(s)) == SOCK_CLOSED)
			{
#ifdef __DEF_IINCHIP_DBG__
			printf("SOCK_CLOSED.\r\n");
#endif
				ret = 0; break;
			}
		}
	}

	return ret;
}



/**
@brief	This function used for disconnect the socket and parameter is "s" which represent the socket number
@return	1 for success else 0.
*/ 
void disconnect(SOCKET s)
{
#ifdef __DEF_IINCHIP_DBG__
	printf("disconnect()\r\n");
#endif
	IINCHIP_WRITE(Sn_CR(s),Sn_CR_DISCON);
}


/**
@brief	This function used to send the data in TCP mode
@return	1 for success else 0.
*/ 
uint16 send(
	SOCKET s, 		/**< the socket index */
	const uint8 * buf, 	/**< a pointer to data */
	uint16 len		/**< the data size to be send */
	)
{
	uint8 status=0;
	uint16 ret=0;
	uint16 freesize=0;
	uint8 exit_tcp = 0;
#ifdef __DEF_IINCHIP_DBG__
	printf("send()\r\n");
#endif

   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

   // if freebuf is available, start.
	do 
	{
		freesize = getSn_TX_FSR(s);
		status = IINCHIP_READ(Sn_SR(s));
		if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
		{
			ret = 0;
			break;
		}
#ifdef __DEF_IINCHIP_DBG__
		printf("socket %d freesize(%d) empty or error\r\n", s, freesize);
#endif
	} while (freesize < ret);

      // copy data
	//send_data_processing(s, (uint8 *)buf, ret);
	IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);

	// wait for completion
	while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
	{
		status = IINCHIP_READ(Sn_SR(s));
		if ((status == SOCK_CLOSED) || (exit_tcp > 2))
		{
#ifdef __DEF_IINCHIP_DBG__
			printf("SOCK_CLOSED.\r\n");
#endif
			putISR(s, getISR(s) & (Sn_IR_RECV | Sn_IR_DISCON | Sn_IR_CON));
			IINCHIP_WRITE(Sn_IR(s), (Sn_IR_SEND_OK | Sn_IR_TIMEOUT));
			return 0;
		}
		exit_tcp++;
    }
    putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
	IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
//	printf("exit_tcp %d \r\n", exit_tcp);
  	return ret;
}

/**
@brief	This function is an application I/F function which is used to receive the data in TCP mode.
		It continues to wait for data as much as the application wants to receive.
		
@return	received data size for success else -1.
*/ 
uint16 recv(
	SOCKET s, 	/**< socket index */
	uint8 * buf, 	/**< a pointer to copy the data to be received */
	uint16 len	/**< the data size to be read */
	)
{
	uint16 ret=0;
#ifdef __DEF_IINCHIP_DBG__
	printf("recv()\r\n");
#endif


	if ( len > 0 )
	{
		recv_data_processing(s, buf, len);
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV);
		ret = len;
	}
	return ret;
}

/**
@brief	It sets up SubnetMask address
*/
void applySUBR(
void	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
)
{
	IINCHIP_WRITE((SUBR0 + 0), SUBN_VAR[0]);
	IINCHIP_WRITE((SUBR0 + 1), SUBN_VAR[1]);
	IINCHIP_WRITE((SUBR0 + 2), SUBN_VAR[2]);
	IINCHIP_WRITE((SUBR0 + 3), SUBN_VAR[3]);
}


/**
@brief	It sets up SubnetMask address
*/
void clearSUBR(
void	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
)
{
	IINCHIP_WRITE((SUBR0 + 0), 0);
	IINCHIP_WRITE((SUBR0 + 1), 0);
	IINCHIP_WRITE((SUBR0 + 2), 0);
	IINCHIP_WRITE((SUBR0 + 3), 0);
}


/**
@brief  This function is an application I/F function which is used to send the data for other then TCP mode.
    Unlike TCP transmission, The peer's destination address and the port is needed.

@return This function return send data size for success else -1.
*/
uint16 sendto(
  SOCKET s,     /**< socket index */
  const uint8 * buf,  /**< a pointer to the data */
  uint16 len,     /**< the data size to send */
  uint8 * addr,     /**< the peer's Destination IP address */
  uint16 port   /**< the peer's destination port number */
  )
{
//  uint8 status=0;
//  uint8 isr=0;
	uint16 ret=0;
	uint8 exit_udp = 0;

#ifdef __DEF_IINCHIP_DBG__
	printf("sendto(%s)\r\n", buf);
	//printf("sendto()\r\n");
#endif
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

  if
    (
      ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
      ((port == 0x00)) ||(ret == 0)
    )
  {
     /* +2008.01 [bj] : added return value */
     ret = 0;
#ifdef __DEF_IINCHIP_DBG__
  printf("[ch%d] Fail[%.3d.%.3d.%.3d.%.3d, %.d, %d]\r\n",s, addr[0], addr[1], addr[2], addr[3] , port, len);
  printf("Fail[invalid ip,port]\r\n");
#endif
  }
  else
  {
    IINCHIP_WRITE(Sn_DIPR0(s),addr[0]);
    IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
    IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
    IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
    IINCHIP_WRITE(Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
    IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8)(port & 0x00ff));
    // copy data
    send_data_processing(s, (uint8 *)buf, ret);
	
    // m2012.03.13 [ys]: set/clear subnet for ARP Errata
    applySUBR();
    IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);
    /* +20071122[chungs]:wait to process the command... */
	
    while( IINCHIP_READ(Sn_CR(s)) );
    /* ------- */

/* +2008.01 bj */
	while ( (IINCHIP_READ(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
	{
		if ((IINCHIP_READ(Sn_IR(s)) & Sn_IR_TIMEOUT) || (exit_udp > 2))
		{
#ifdef __DEF_IINCHIP_DBG__
        printf("send fail.\r\n");
#endif
/* +2008.01 [bj]: clear interrupt */
			IINCHIP_WRITE(Sn_IR(s), (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
			return 0;
		}
		exit_udp++;
	}

/* +2008.01 bj */
	IINCHIP_WRITE(Sn_IR(s), (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */

	clearSUBR();
	
	//printf("exit_udp %d \r\n", exit_udp);
  }

  return ret;
}

/**
 * @brief	initialize configuration variables
*/
void init_conf_variables(void)
{
	SYSINFO	SysInfo;		/**< system info.(version, ...) */
	NETCONF	NetConf;
	CHCONF	ChConf;
	
	memset((void*)&SysInfo,0,sizeof(SYSINFO));	
	memset((void*)&NetConf,0,sizeof(NETCONF));
	memset((void*)&ChConf,0,sizeof(CHCONF));
	get_sysinfo(&SysInfo);
	get_netconf(&NetConf);
	get_chconf(&ChConf);
}


/**
@brief	This function is an application I/F function which is used to receive the data in other then
	TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well. 
	
@return	This function return received data size for success else -1.
*/ 
uint16 recvfrom(
	SOCKET s, 	/**< the socket number */
	uint8 * buf, 	/**< a pointer to copy the data to be received */
	uint16 len, 	/**< the data size to read */
	uint8 * addr, 	/**< a pointer to store the peer's IP address */
	uint16 *port	/**< a pointer to store the peer's port number. */
	)
{
	uint8 head[8];
	uint16 data_len=0;
	uint16 ptr=0;
#ifdef __DEF_IINCHIP_DBG__
	printf("recvfrom()\r\n");
#endif

	if ( len > 0 )
	{
   	ptr = IINCHIP_READ(Sn_RX_RD0(s));
   	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(s) + 1);
#ifdef __DEF_IINCHIP_DBG__
   	printf("ISR_RX: rd_ptr : %.4x\r\n", ptr);
#endif
   	switch (IINCHIP_READ(Sn_MR(s)) & 0x07)
   	{
   	case Sn_MR_UDP :
   			read_data(s, (uint8 *)ptr, head, 0x08);
   			ptr += 8;
   			// read peer's IP address, port number.
    			addr[0] = head[0];
   			addr[1] = head[1];
   			addr[2] = head[2];
   			addr[3] = head[3];
   			*port = head[4];
   			*port = (*port << 8) + head[5];
   			data_len = head[6];
   			data_len = (data_len << 8) + head[7];
   			
#ifdef __DEF_IINCHIP_DBG__
   			printf("UDP msg arrived\r\n");
   			printf("source Port : %d\r\n", *port);
   			printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif

			read_data(s, (uint8 *)ptr, buf, data_len); // data copy.
			ptr += data_len;

			IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
   			break;
   
   	case Sn_MR_IPRAW :
   			read_data(s, (uint8 *)ptr, head, 0x06);
   			ptr += 6;
   
   			addr[0] = head[0];
   			addr[1] = head[1];
   			addr[2] = head[2];
   			addr[3] = head[3];
   			data_len = head[4];
   			data_len = (data_len << 8) + head[5];
   	
#ifdef __DEF_IINCHIP_DBG__
   			printf("IP RAW msg arrived\r\n");
   			printf("source IP : %d.%d.%d.%d\r\n", addr[0], addr[1], addr[2], addr[3]);
#endif
			read_data(s, (uint8 *)ptr, buf, data_len); // data copy.
			ptr += data_len;

			IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
			IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
   			break;
   	case Sn_MR_MACRAW :
   			read_data(s,(uint8*)ptr,head,2);
   			ptr+=2;
   			data_len = head[0];
   			data_len = (data_len<<8) + head[1] - 2;

   			read_data(s,(uint8*) ptr,buf,data_len);
   			ptr += data_len;
   			IINCHIP_WRITE(Sn_RX_RD0(s),(uint8)((ptr & 0xff00) >> 8));
   			IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8)(ptr & 0x00ff));
   			
#ifdef __DEF_IINCHIP_DGB__
			printf("MAC RAW msg arrived\r\n");
			printf("dest mac=%.2X.%.2X.%.2X.%.2X.%.2X.%.2X\r\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
			printf("src  mac=%.2X.%.2X.%.2X.%.2X.%.2X.%.2X\r\n",buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
			printf("type    =%.2X%.2X\r\n",buf[12],buf[13]); 
#endif			
			break;

   	default :
   			break;
   	}
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_RECV);
	}
#ifdef __DEF_IINCHIP_DBG__
	printf("recvfrom() end ..\r\n");
#endif
 	return data_len;
}


uint16 igmpsend(SOCKET s, const uint8 * buf, uint16 len)
{
	//uint8 status=0;
	uint8 isr=0;
	uint16 ret=0;
	
#ifdef __DEF_IINCHIP_DBG__
	printf("igmpsend()\r\n");
#endif
   if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
   else ret = len;

	if	(ret == 0) 
 	{
 	   
#ifdef __DEF_IINCHIP_DBG__
	printf("%d Fail[%d]\r\n", s, len);
#endif
	}
	else
	{
		// copy data
		send_data_processing(s, (uint8 *)buf, ret);
		IINCHIP_WRITE(Sn_CR(s),Sn_CR_SEND);
      
      		while (IINCHIP_READ(Sn_CR(s)))
		{
			// status = IINCHIP_READ(Sn_SR(s));

			if ((getISR(s) & Sn_IR_TIMEOUT) || (isr & Sn_IR_TIMEOUT))
			{
#ifdef __DEF_IINCHIP_DBG__
				printf("igmpsend fail.\r\n");
#endif
				putISR(s, getISR(s) & (Sn_IR_RECV | Sn_IR_DISCON | Sn_IR_CON));
				IINCHIP_WRITE(Sn_IR(s), (Sn_IR_SEND_OK | Sn_IR_TIMEOUT));
				return 0;
			}
		}
		putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
	   	IINCHIP_WRITE(Sn_IR(s), Sn_IR_SEND_OK);
	}
	return ret;
}

/**
 * @brief		get reset info.(use SYSINFO structure)
 * @return	get reset method.
*/
/*
u_char get_reset_flag(void)
{
	SysInfo.auto_reset = eeprom_read_byte((u_char*)SYS_AUTORESET);
	return SysInfo.auto_reset;
}*/


/**
 * @brief	set reset info.(use SYSINFO structure)
*/
void set_reset_flag(u_char flag)
{
	eeprom_write_byte((u_char*)SYS_AUTORESET,flag);	
}

void evb_soft_reset(void)
{
	printf("System Reset.");
	set_reset_flag(SYSTEM_AUTO_RESET);
	asm volatile("jmp 0x0000");
}

//Initialize client - 클라이언트 모드로 소켓 초기화
unsigned char InitSocket(unsigned int soket, unsigned int Type, unsigned Port)
{
	u_char addr[6] = MY_DESTINATIONIP;
	
	//initialize the socket
	if(socket(soket,Sn_MR_TCP, Port,0x20) == 0)
	{
		DebugPrint("Fail to create socket.");
	}
	else if(Type == SOCK_TCPS)
	{
		listen(soket);
		return 1;
	}
	else
	{
		connect(soket, addr, Port);
		return 1;
	}
	return 0;
}