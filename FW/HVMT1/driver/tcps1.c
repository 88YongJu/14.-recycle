//=============================================================================
// 90can128
// 
// [tcps1.c]
//=============================================================================

#include "tcps1.h"

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//Initialize client - 클라이언트 모드로 소켓 초기화
unsigned char InitSocket(unsigned int Socket, unsigned Port)
{
	u_char addr[6] = MY_DESTINATIONIP;
	//initialize the socket
	
	if(socket(Socket,Sn_MR_TCP, Port,0x20) == 0)
	{
		DebugPrint("Fail to create socket.");
		return 0;
	}
	else if(Socket == SOCK_TCPS)
	{
		listen(Socket);
	}
	else
	{
		connect(Socket, addr, Port);
	}
	return 1;
}

//-----------------------------------------------------------------------------
//수신데이터 처리
void EthernetTest(unsigned char *pRcvBuffer, unsigned int len)
{
	#ifdef __DEF_IINCHIP_DBG__
	unsigned int i;
	DebugPrint("Read Data[%d]\r\n", len);
	for(i=0;i<len;i++)
	{
		//if(i%16==0)DebugPrint("\r\n");
		DebugPrint("%c", pRcvBuffer[i]);
	}
	DebugPrint("\r\n");
	DebugPrint("\r\nReceive OK!\r\n");
	#endif
}

//TCP-Server 처리
void ProcessTcpSever(u_char ch)
{
	int len;
	u_char * data_buf = (u_char*)RX_BUF;
	uint16 port = MY_LISTEN_PORT;
	
	switch (getSn_SR(ch))
	{
		case SOCK_ESTABLISHED :
			#ifdef	__DEF_IINCHIP_DBG__
			static unsigned char st = 1;
			if(st)
			{
				DebugPrint("%d : Connected by %s(%u)",ch,inet_ntoa(GetDestAddr(ch)),GetDestPort(ch));
				st = 0;
			}
			#endif
			//check Rx data
			if((len = getSn_RX_RSR(ch)) > 0)
			{
				//if Rx data size is lager than TX_RX_MAX_BUF_SIZE 
				if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE-1;
			
				//read the received data
				len = recv(ch, data_buf, len);
				//send the received data - loop back test
				send(ch, data_buf, len);
				EthernetTest(data_buf, len);
			}
			break;
		
		case SOCK_CLOSE_WAIT:
			//If the client request to close
			disconnect(ch);
			break;
		
		case SOCK_CLOSED:
			//if a socket is closed
		
			//reinitialize the socket
			InitSocket(ch, port);
			break;
		default : 
			break;
	}
	#ifdef	__DEF_IINCHIP_DBG__
	DebugPrint("%x\r\n",getSn_SR(ch));
	#endif
}

void ProcessTcpClient(u_char ch)/**< channel(socket) index */
{
	static unsigned char st = 1;
	int len;
	u_char * data_buf = (u_char*)TX_BUF;
	//unsigned char temp[20];
	//unsigned char out1, out2;
	switch (getSn_SR(ch))
	{
		case SOCK_ESTABLISHED:						/* if connection is established */
		
			if(st == 1)
			{
				printf("%d : Connected by %s(%u)",ch,inet_ntoa(GetDestAddr(ch)),GetDestPort(ch));
				set_timer(USART1_CHECK_TIMER, USART1_handler);
				st = 0;
			}
			if ((len = getSn_RX_RSR(ch)) > 0) 			/* check Rx data */
			{
				if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */\
			
				//read the received data
				len = recv(ch, data_buf, len);
				//send the received data - loop back test
				send(ch, data_buf, len);
				EthernetTest(data_buf, len);
			}
			break;
		case SOCK_SYNSENT:	//If the client try to connect
			/*if (dis_cnt > 1000)
			{
				disconnect(ch);
				bchannel_start[ch] = 0;
				dis_cnt = 0;
			}
			else
			{
				dis_cnt++;
			}*/
			break;
		case SOCK_CLOSE_WAIT:                           		/* If the client request to close */
			disconnect(ch);
			/*
			DebugPrint("%d : CLOSE_WAIT", ch);
			bchannel_start[ch] = 0;
			*/
			break;
		case SOCK_LAST_ACK:
			//If the client request to close
			disconnect(ch);
			/*
			bchannel_start[ch] = 0;
			*/
			break;
		case SOCK_FIN_WAIT:
			//If the client try to connect
			/*
			if (dis_cnt > 1000)
			{
				close(ch);
				bchannel_start[ch] = 0;
				dis_cnt = 0;
			}
			else
			{
				dis_cnt++;
			}
			*/
			break;
		case SOCK_CLOSED:                                               /* if a socket is closed */
			/*
			if(!bchannel_start[ch])
			{
				PRINTLN1("%d :  TCP Client Started.",ch);
				bchannel_start[ch] = 1;
			}
			if(socket(ch,Sn_MR_TCP,get_system_any_port(),0x00) == 0)    // reinitialize the socket
			{
				PRINTLN1("\a%d : Fail to create socket.",ch);
				bchannel_start[ch] = 0;
			}
			//else	connect(ch,(u_char*)&ChConf.ch[ch].destip,ChConf.ch[ch].port);
			else
			{
				//		    printf("Connecting to...\r\n");
				//			connect(ch, destip,5000);
				//			connect(ch,(u_char*)&ChConf.ch[ch].destip,ChConf.ch[ch].port);
				conn_cnt = 0;
			}*/
			break;
		case SOCK_INIT:
			//reinitialize the socket
			/*
			if (conn_cnt >= conn_dly)
			{
				//			wdt_reset();
				printf("\r\n*** CHECK IP ***\r\n");

				if (gethostbyname("ec1.con2u.net") != 0 )
				{
					ChConf.ch[ch].destip = con2u_ip;
					printf("\r\DNS RESULT => %s\r\n", inet_ntoa(htonl(con2u_ip)));
				}
				else
				{
					printf("Can'g get IP Address.\r\n");
				}
				printf("\r\n*** THANK YOU ***\r\n");

				//			wdt_reset();


				printf("CONNECT CNT %lu \r\n", conn_cnt);
				printf("Connecting to...\r\n");
				//			connect(ch, destip,5000);
				connect(ch,(u_char*)&ChConf.ch[ch].destip,ChConf.ch[ch].port);
				//				conn_cnt = 0;
			}
			else
			{
				conn_cnt++;
			}
			dis_cnt = 0;
			*/
			break;
		default :
		break;
	}
}
//-----------------------------------------------------------------------------

void Cpuinit(void)
{
	cli();
	MCUCR = (0<<PUD);
}

void Gpioinit(void)
{
	PORTG = 0b01000;
	DDRG = 0b01111;//0xff;
	PORTB = 0b00000000;
	DDRB = 0b00000111;//0x01|0x02|0x04;
}

//-----------------------------------------------------------------------------
// SystemInit
//-----------------------------------------------------------------------------
void SystemInit(void)
{
	Cpuinit();
	Gpioinit();
	Init_SPI_W5100();
	/*DebugInit(BAUD_115200);//Serial Init
	DebugPrint("[TCP Server] Test Program.\r\n");*/
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
/*
int main(void)
{
	SystemInit();
	//W5100 Chip Initialize
	//----------------------------
	printf("[Mega128 EVM] Test Program. - START\r\n");
	NetInit();
	//----------------------------
	
	//Display Net Configuration..
	DisplayConfig();	//Net Config Data Usart Send
	
	//Initialize TCP Server
	InitSocketServer(SOCK_TCPS, MY_LISTEN_PORT);
	
	printf("[Mega128 EVM] Test Program. - TCP Server start\r\n");
	while (1)
	{
		//TCP Server Process
		ProcessTcpSever(SOCK_TCPS);
	}		
}*/
//-----------------------------------------------------------------------------
