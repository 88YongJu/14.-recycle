/*
@file		w5100.h
*/

#ifndef	_W5100_H_
#define	_W5100_H_

#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "types.h"
#include "exiom.h"
#include "spi.h"
#include "serial.h"
#include "TCP.h"
#include "TIMER.h"
#include "dhcp.h"

///////////////////////////////////////////////////////////
// DEFINE I/O PORT	///////////////////////////////////////
#define W5100_CS		portg.bit3
#define W5100_EN		portg.bit4
//-----------------------------------------------------------------------------
enum{SOCK_1, SOCK_2, SOCK_3, SOCK_4};

#define SOCK_DHCPC		SOCK_1
#define SOCK_TCPS		SOCK_2
#define SOCK_TCPC		SOCK_3

#define MY_NET_MAC				{0xEE, 0x08, 0xdc, 0x00, 0x00, 0x02}		// MY Mac Address : EE.08.DC.00.00.00
#define MY_NET_GWIP				{192, 168,  25,   1}	// MY Gateway IP    : 192.168.25.1
//#define MY_DESTINATIONIP		{ 59,   0, 251,  32}	// MY Destination   : 192.168.25.61
#define MY_DESTINATIONIP		{192, 168,  25,  100}	// MY Destination   : 192.168.25.61
//#define MY_DESTINATIONIP2		{192, 168,  25,  102}	// MY Destination   : 192.168.25.61
	
#define MY_SOURCEIP				{192, 168,  25,  102}	// MY Source IP     : 192.168.25.100
#define MY_DNSIP				{192, 168,  25,    1}	// MY Source IP     : 192.168.25.100
#define MY_SUBNET				{255, 255, 255,    0}
#define MY_NET_MEMALLOC			0x55					// MY iinchip memory allocation
#define MY_LISTEN_PORT			3000					// MY Listen Port  : 3000
#define MY_CONNECT_PORT			8888					// MY Connect Port : 5000

#define DEFAULT_NET_MAC			"\xEE\x08\xDC\x00\x00\x00"	/**< Default Mac Address : 00.08.DC.03.04.05 */
#define DEFAULT_NET_SN			0xFFFFFF00	// Default Subnet Mask   : 255.255.255.0
#define DEFAULT_NET_DNS			0xA87E3F01	/**< Default DNS Server IP : 0.0.0.0	  */
#define DEFAULT_NET_SIP			0xC0A80037	// Default Source IP     : 192.168.1.5
#define DEFAULT_NET_GWIP		0xC0A800FE	// Default Gateway IP    : 192.168.1.1
#define DEFAULT_NET_MEMALLOC	0x55		/**< Default iinchip memory allocation */

#define DEFAULT_CH_TYPE			NOTUSE		/**< TCP Loop-Back Server */
#define DEFAULT_LISTEN_PORT		5555		/**< Default Listen Port  : 5000 */
#define DEFAULT_CONNECT_PORT	3000		/**< Default Connect Port : 3000 */
#define DEFAULT_SOURCE_PORT		5000		/**< Default Source Port  : 3000 */
#define DEFAULT_HTTP_PORT		80		/**< Default HTTP Port    : 80 */
#define DEFAULT_CH_DESTIP		0x7D888573	/**< Default Destination IP : 192.168.0.3 */
//-----------------------------------------------------------------------------

#define SYSTEM_ANY_PORT			1000		/**< Default System Any Port Number  */
#define SYSTEM_AUTO_RESET		1
#define SYSTEM_MANUAL_RESET		0

#define SYS_INFO			0x00
#define SYS_TEST			(SYS_INFO)
#define SYS_VER				(SYS_TEST + 2)
#define SYS_AUTORESET		(SYS_VER + 4)
#define SYS_ANY_PORT		(SYS_AUTORESET + 1)
#define SYS_SERINUM			(SYS_ANY_PORT + 2)

#define NET_CONF			0x40
#define NET_TEST			(NET_CONF)
#define NET_MAC				(NET_TEST+2)
#define NET_SIP				(NET_MAC + 6)
#define NET_GWIP			(NET_SIP + 4)
#define NET_SN				(NET_GWIP + 4)
#define NET_DNS				(NET_SN + 4)
#define NET_MEMALLOC		(NET_DNS + 4)

#define CH_CONF				0x70
#define CH_TEST				(CH_CONF)
#define CH_TYPE_0			(CH_TEST + 2)
#define CH_PORT_0			(CH_TYPE_0 + 1)
#define CH_DESTIP_0			(CH_PORT_0 + 2)
#define CH_TYPE_1			(CH_DESTIP_0 + 4)
#define CH_PORT_1			(CH_TYPE_1 + 1)
#define CH_DESTIP_1			(CH_PORT_1 + 2)
#define CH_TYPE_2			(CH_DESTIP_1 + 4)
#define CH_PORT_2			(CH_TYPE_2 + 1)
#define CH_DESTIP_2			(CH_PORT_2 + 2)
#define CH_TYPE_3			(CH_DESTIP_2 + 4)
#define CH_PORT_3			(CH_TYPE_3 + 2)
#define CH_DESTIP_3			(CH_PORT_3 + 2)

#define TEST_VALUE	0xA5A5

#define MR __DEF_IINCHIP_MAP_BASE__
#define IDM_OR		((__DEF_IINCHIP_MAP_BASE__ + 0x00))
#define IDM_AR0		((__DEF_IINCHIP_MAP_BASE__ + 0x01))
#define IDM_AR1		((__DEF_IINCHIP_MAP_BASE__ + 0x02))
#define IDM_DR		((__DEF_IINCHIP_MAP_BASE__ + 0x03))

//Gateway IP Register address
#define GAR0		(COMMON_BASE + 0x0001)
//Subnet mask Register address
#define SUBR0		(COMMON_BASE + 0x0005)
//Source MAC Register address
#define SHAR0		(COMMON_BASE + 0x0009)
//Source IP Register address
#define SIPR0		(COMMON_BASE + 0x000F)
//Interrupt Register
#define IR			(COMMON_BASE + 0x0015)
//Interrupt mask register
#define IMR			(COMMON_BASE + 0x0016)
//Timeout register address( 1 is 100us )
#define RTR0		(COMMON_BASE + 0x0017)
//Retry count reigster
#define RCR			(COMMON_BASE + 0x0019)
//Receive memory size reigster
#define RMSR		(COMMON_BASE + 0x001A)
/**
 @brief Transmit memory size reigster
 */
#define TMSR		(COMMON_BASE + 0x001B)
/**
 @brief Authentication type register address in PPPoE mode
 */
#define PATR0		(COMMON_BASE + 0x001C)
//#define PPPALGO (COMMON_BASE + 0x001D)
#define PTIMER		(COMMON_BASE + 0x0028)
#define PMAGIC		(COMMON_BASE + 0x0029)

/**
 @brief Unreachable IP register address in UDP mode
 */
#define UIPR0		(COMMON_BASE + 0x002A)
/**
 @brief Unreachable Port register address in UDP mode
 */
#define UPORT0		(COMMON_BASE + 0x002E)

/**
 @brief socket register
*/
#define CH_BASE		(COMMON_BASE + 0x0400)
/**
 @brief	size of each channel register map
 */
#define CH_SIZE		0x0100
/**
 @brief socket Mode register
 */
#define Sn_MR(ch)	(CH_BASE + ch * CH_SIZE + 0x0000)
/**
 @brief channel Sn_CR register
 */
#define Sn_CR(ch)				(CH_BASE + ch * CH_SIZE + 0x0001)
/**
 @brief channel interrupt register
 */
#define Sn_IR(ch)			(CH_BASE + ch * CH_SIZE + 0x0002)
/**
 @brief channel status register
 */
#define Sn_SR(ch)			(CH_BASE + ch * CH_SIZE + 0x0003)
/**
 @brief source port register
 */
#define Sn_PORT0(ch)		(CH_BASE + ch * CH_SIZE + 0x0004)
/**
 @brief Peer MAC register address
 */
#define Sn_DHAR0(ch)			(CH_BASE + ch * CH_SIZE + 0x0006)
/**
 @brief Peer IP register address
 */
#define Sn_DIPR0(ch)			(CH_BASE + ch * CH_SIZE + 0x000C)
/**
 @brief Peer port register address
 */
#define Sn_DPORT0(ch)		(CH_BASE + ch * CH_SIZE + 0x0010)
/**
 @brief Maximum Segment Size(Sn_MSSR0) register address
 */
#define Sn_MSSR0(ch)					(CH_BASE + ch * CH_SIZE + 0x0012)
/**
 @brief Protocol of IP Header field register in IP raw mode
 */
#define Sn_PROTO(ch)			(CH_BASE + ch * CH_SIZE + 0x0014)

/** 
 @brief IP Type of Service(TOS) Register 
 */
#define Sn_TOS(ch)						(CH_BASE + ch * CH_SIZE + 0x0015)
/**
 @brief IP Time to live(TTL) Register 
 */
#define Sn_TTL(ch)						(CH_BASE + ch * CH_SIZE + 0x0016)

//not support
//#define RX_CH_DMEM_SIZE			(COMMON_BASE + 0x001E)
//#define TX_CH_DMEM_SIZE			(COMMON_BASE + 0x001F)

/**
 @brief Transmit free memory size register
 */
#define Sn_TX_FSR0(ch)	(CH_BASE + ch * CH_SIZE + 0x0020)
/**
 @brief Transmit memory read pointer register address
 */
#define Sn_TX_RD0(ch)			(CH_BASE + ch * CH_SIZE + 0x0022)
/**
 @brief Transmit memory write pointer register address
 */
#define Sn_TX_WR0(ch)			(CH_BASE + ch * CH_SIZE + 0x0024)
/**
 @brief Received data size register
 */
#define Sn_RX_RSR0(ch)	(CH_BASE + ch * CH_SIZE + 0x0026)
/**
 @brief Read point of Receive memory
 */
#define Sn_RX_RD0(ch)			(CH_BASE + ch * CH_SIZE + 0x0028)
/**
 @brief Write point of Receive memory
 */
#define Sn_RX_WR0(ch)			(CH_BASE + ch * CH_SIZE + 0x002A)

/* MODE register values */
#define MR_RST			0x80 /**< reset */
#define MR_PB			0x10 /**< ping block */
#define MR_PPPOE		0x08 /**< enable pppoe */
#define MR_LB  		0x04 /**< little or big endian selector in indirect mode */
#define MR_AI			0x02 /**< auto-increment in indirect mode */
#define MR_IND			0x01 /**< enable indirect mode */

/* IR register values */
#define IR_CONFLICT	0x80 /**< check ip confict */
#define IR_UNREACH	0x40 /**< get the destination unreachable message in UDP sending */
#define IR_PPPoE		0x20 /**< get the PPPoE close message */
#define IR_SOCK(ch)	(0x01 << ch) /**< check socket interrupt */

/* Sn_MR values */
#define Sn_MR_CLOSE		0x00		/**< unused socket */
#define Sn_MR_TCP		0x01		/**< TCP */
#define Sn_MR_UDP		0x02		/**< UDP */
#define Sn_MR_IPRAW		0x03		/**< IP LAYER RAW SOCK */
#define Sn_MR_MACRAW	0x04		/**< MAC LAYER RAW SOCK */
#define Sn_MR_PPPOE		0x05		/**< PPPoE */
#define Sn_MR_ND		0x20		/**< No Delayed Ack(TCP) flag */
#define Sn_MR_MULTI		0x80		/**< support multicating */


/* Sn_CR values */
#define Sn_CR_OPEN				0x01		/**< initialize or open socket */
#define Sn_CR_LISTEN			0x02		/**< wait connection request in tcp mode(Server mode) */
#define Sn_CR_CONNECT			0x04		/**< send connection request in tcp mode(Client mode) */
#define Sn_CR_DISCON			0x08		/**< send closing reqeuset in tcp mode */
#define Sn_CR_CLOSE				0x10		/**< close socket */
#define Sn_CR_SEND				0x20		/**< updata txbuf pointer, send data */
#define Sn_CR_SEND_MAC			0x21		/**< send data with MAC address, so without ARP process */
#define Sn_CR_SEND_KEEP			0x22		/**<  send keep alive message */
#define Sn_CR_RECV				0x40		/**< update rxbuf pointer, recv data */

#ifdef __DEF_IINCHIP_PPP__
	#define Sn_CR_PCON			0x23		 
	#define Sn_CR_PDISCON		0x24		 
	#define Sn_CR_PCR			0x25		 
	#define Sn_CR_PCN			0x26		
	#define Sn_CR_PCJ			0x27		
#endif

/* Sn_IR values */
#ifdef __DEF_IINCHIP_PPP__
	#define Sn_IR_PRECV			0x80		
	#define Sn_IR_PFAIL			0x40		
	#define Sn_IR_PNEXT			0x20		
#endif
#define Sn_IR_SEND_OK			0x10		/**< complete sending */
#define Sn_IR_TIMEOUT			0x08		/**< assert timeout */
#define Sn_IR_RECV				0x04		/**< receiving data */
#define Sn_IR_DISCON			0x02		/**< closed socket */
#define Sn_IR_CON				0x01		/**< established connection */

/* Sn_SR values */
#define SOCK_CLOSED				0x00		/**< closed */
#define SOCK_NOT_CONNECT		0x01		/**< closed */
#define SOCK_INIT 				0x13		/**< init state */
#define SOCK_LISTEN				0x14		/**< listen state */
#define SOCK_SYNSENT	   		0x15		/**< connection state */
#define SOCK_SYNRECV		   	0x16		/**< connection state */
#define SOCK_ESTABLISHED		0x17		/**< success to connect */
#define SOCK_FIN_WAIT			0x18		/**< closing state */
#define SOCK_CLOSING		   	0x1A		/**< closing state */
#define SOCK_TIME_WAIT			0x1B		/**< closing state */
#define SOCK_CLOSE_WAIT			0x1C		/**< closing state */
#define SOCK_LAST_ACK			0x1D		/**< closing state */
#define SOCK_UDP				0x22		/**< udp socket */
#define SOCK_IPRAW				0x32		/**< ip raw mode socket */
#define SOCK_MACRAW				0x42		/**< mac raw mode socket */
#define SOCK_PPPOE				0x5F		/**< pppoe socket */

/* IP PROTOCOL */
#define IPPROTO_IP              0           /**< Dummy for IP */
#define IPPROTO_ICMP            1           /**< Control message protocol */
#define IPPROTO_IGMP            2           /**< Internet group management protocol */
#define IPPROTO_GGP             3           /**< Gateway^2 (deprecated) */
#define IPPROTO_TCP             6           /**< TCP */
#define IPPROTO_PUP             12          /**< PUP */
#define IPPROTO_UDP             17          /**< UDP */
#define IPPROTO_IDP             22          /**< XNS idp */
#define IPPROTO_ND              77          /**< UNOFFICIAL net disk protocol */
#define IPPROTO_RAW             255         /**< Raw IP packet */

#define CMD_WR	0xF0
#define CMD_RD	0x0F

boolean f_Set_DHCP;

char* inet_ntoa(unsigned long addr);/**< Pointer variable to store converted value(INPUT) */
unsigned long htonl(
unsigned long hostlong		/**< hostshort  - A 32-bit number in host byte order.  */
);
unsigned long ntohl(unsigned long netlong);
unsigned short htons(
unsigned short hostshort	/**< A 16-bit number in host byte order.  */
);

void Init_SPI_W5100(void);
void Init_W5100(void);
void set_netconf(NETCONF* pNetConf);
void get_netconf(NETCONF* pNetConf);
//----------------sockrt control


/**
@brief	This Socket function initialize the channel in perticular mode, and set the port and wait for W5100 done it.
@return 	1 for sucess else 0.
*/  
uint8 socket(
	SOCKET s, 		/**< for socket number */
	uint8 protocol, 	/**< for socket protocol */
	uint16 port, 		/**< the source port for the socket */
	uint8 flag		/**< the option for the socket */
	);
	
/**
@brief	This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return	1 for success else 0.
*/ 
uint8 listen(SOCKET s);


/**
@brief	This function established  the connection for the channel in Active (client) mode. 
		This function waits for the untill the connection is established.
		
@return	1 for success else 0.
*/ 
uint8 connect(SOCKET s, uint8 * addr, uint16 port);



/**
@brief	This function used to send the data in TCP mode
@return	1 for success else 0.
*/ 
uint16 send(
	SOCKET s, 		/**< the socket index */
	const uint8 * buf, 	/**< a pointer to data */
	uint16 len		/**< the data size to be send */
	);

void W5100_write_burst(uint16_t dst_ptr, uint8_t* buf, uint16_t size);

/**
@brief	This function is an application I/F function which is used to receive the data in TCP mode.
		It continues to wait for data as much as the application wants to receive.
		
@return	received data size for success else -1.
*/ 
uint16 recv(
	SOCKET s, 	/**< socket index */
	uint8 * buf, 	/**< a pointer to copy the data to be received */
	uint16 len	/**< the data size to be read */
	);


/**
@brief	This function is an application I/F function which is used to send the data for other then TCP mode. 
		Unlike TCP transmission, The peer's destination address and the port is needed.
		
@return	This function return send data size for success else -1.
*/ 
uint16 sendto(
	SOCKET s, 		/**< socket index */
	const uint8 * buf, 	/**< a pointer to the data */
	uint16 len, 		/**< the data size to send */
	uint8 * addr, 		/**< the peer's Destination IP address */
	uint16 port		/**< the peer's destination port number */
	);


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
	);

uint16 igmpsend(SOCKET s, const uint8 * buf, uint16 len);

/*********************************************************
* iinchip access function
*********************************************************/
uint8 IINCHIP_READ(uint16 addr);
uint8 IINCHIP_WRITE(uint16 addr,uint8 data);
uint16 wiz_read_buf(uint16 addr, uint8* buf,uint16 len);
uint16 wiz_write_buf(uint16 addr,uint8* buf,uint16 len);

void iinchip_init(void); // reset iinchip
void sysinit(uint8 tx_size, uint8 rx_size); // setting tx/rx buf size
uint8 getISR(uint8 s);
void putISR(uint8 s, uint8 val);
uint16 getIINCHIP_RxMAX(uint8 s);
uint16 getIINCHIP_TxMAX(uint8 s);
uint16 getIINCHIP_RxMASK(uint8 s);
uint16 getIINCHIP_TxMASK(uint8 s);
uint16 getIINCHIP_RxBASE(uint8 s);
uint16 getIINCHIP_TxBASE(uint8 s);
void setGAR(uint8 * addr); // set gateway address
void setSUBR(uint8 * addr); // set subnet mask address
void setSHAR(uint8 * addr); // set local MAC address
void setSIPR(uint8 * addr); // set local IP address
void setRTR(uint16 timeout); // set retry duration for data transmission, connection, closing ...
void setRCR(uint8 retry); // set retry count (above the value, assert timeout interrupt)
void setIMR(uint8 mask); // set interrupt mask. 
void getGAR(uint8 * addr);
void getSUBR(uint8 * addr);
void getSHAR(uint8 * addr);
void getSIPR(uint8 * addr);
uint8 getIR( void );
void setSn_MSS(SOCKET s, uint16 Sn_MSSR0); // set maximum segment size
void setSn_PROTO(SOCKET s, uint8 proto); // set IP Protocol value using IP-Raw mode
uint8 getSn_IR(SOCKET s); // get socket interrupt status
uint8 getSn_SR(SOCKET s); // get socket status
uint16 getSn_TX_FSR(SOCKET s); // get socket TX free buf size
uint16 getSn_RX_RSR(SOCKET s); // get socket RX recv buf size
void setSn_DHAR(SOCKET s, uint8 * addr);
void setSn_DIPR(SOCKET s, uint8 * addr);
void setSn_DPORT(SOCKET s, uint8 * addr);
void getSn_DHAR(SOCKET s, uint8 * addr);
unsigned long GetDestAddr(SOCKET s);/**< Channel number which try to get destination IP Address */
unsigned int GetDestPort(SOCKET s);/**< Channel number which try to get destination port */
void setSn_TTL(SOCKET s, uint8 ttl);
void setMR(uint8 val);

#ifdef __DEF_IINCHIP_PPP__
uint8 pppinit(uint8 *id, uint8 idlen, uint8 *passwd, uint8 passwdlen);
uint8 pppterm(uint8 *mac,uint8 *sessionid);
#endif

void send_data_processing(SOCKET s, uint8 *data, uint16 len);
void recv_data_processing(SOCKET s, uint8 *data, uint16 len);
void read_data(SOCKET s, vuint8 * src, vuint8 * dst, uint16 len);
void write_data(SOCKET s, vuint8 * src, vuint8 * dst, uint16 len);


void NetInit(void);
void DisplayConfig(void);
void init_conf_variables(void);

extern void evb_soft_reset(void);
//----------------sockrt control

/**
@brief	This function used for disconnect the socket and parameter is "s" which represent the socket number
@return	1 for success else 0.
*/
void disconnect(SOCKET s);

/**
@brief	This function close the socket and parameter is "s" which represent the socket number
*/
void close(SOCKET s);

unsigned long ntohs(
unsigned short netshort	/**< netshort - network odering 16bit value */
);

u_long swapl(u_long l);
unsigned char InitSocket(unsigned int soket, unsigned int Type, unsigned Port);
void Reset_W5100(void);
#endif
