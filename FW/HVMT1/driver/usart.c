//=============================================================================
// Microcontroller용 Serial 포트 제어 API 함수
// by (nexp76@naver.com)
// [serial.c]
//=============================================================================

/*
 - [2007.01.02] 
 - 모든 마이컴에 적용 될 수 있는 API함수 제작 프로젝트
  - 시리얼 포트 API함수 제작
  - _UART0_ENABLE으로 UART0사용

 - [2007.03.25]   
  - U0_Init()함수에서 디폴트 초기화 U0_SetBaud(BAUD_57600) 지움
   -> 보레이트 변경 않되는 현상 때문
   
 - [2007.04.01]
  - serial.h -> serial.c로 변경 serial_hal.h에서 hal을 선택 할 수 있도록함
  - (serial_hal.h)파일을 추가 system.h에서 HAL을 선택 할 수 있도록 하고...
  - 각 halmegxx.h파일 에서 _HAL_MEGA128_UART, _HAL_MEGA8_UART를 이용하여 HAL선택
*/

#include "usart.h"

#ifdef _UART0_ENABLE
//-----------------------------------------------------------------------------
// Serial Prot0 Utility Fuction Routine
void initUsart0(unsigned char baud)
{
	U0_SetBaud(baud);
	u0_hal_init();
	init_Queue(USART0_TX);
	init_Queue(USART0_RX);
}

void U0_SetBaud(unsigned char baud)
{
	u0_baudrate(baud);
}

unsigned char U0_GetByte(void)
{
	u0_WaitForReceive();
	
	return u0_RxData();
}

void U0_PutByte(unsigned char Data)
{
	push(USART0_TX, Data);
	UCSR0B |= 1<<UDRIE0;
}

void U0_PutBuf(unsigned char *str, unsigned char len)
{
	unsigned char i =0;
	
    for(i=0;i<len;i++)
    {
        U0_PutByte(*(str+i));
    }
}

void putStringUsart0(char *str)
{
    while(*str != 0)
    {
        U0_PutByte(*str);
        str++;
    }
}

unsigned char U0_IsGetByte(void)
{
	return u0_IsReceiveData();
}

void u0_TxData(unsigned char Data)
{
	u0_WaitForSend();
	_u0_TxData(Data);
}

void u0_hal_init(void)
{
	// enable serial uart & receiver
	//u0_EnableTx();
	u0_EnableTxIsr();
	u0_EnableRxIsr();
	
	//at 16Mhz
	U0R_CSRA |= UART_DB_BAUDRATE;
#if DEBUGSERIAL == _USART0
	fdevopen((void*)u0_TxData,  0);
#endif
}

void u0_baudrate(unsigned char baud)
{
	unsigned char baud_rate = 0;
	
	switch (baud)
	{
		case BAUD_9600:
		baud_rate = BAUDRATE_VAL_9600;
		break;
		case BAUD_14400:
		baud_rate = BAUDRATE_VAL_14400;
		break;
		case BAUD_19200:
		baud_rate = BAUDRATE_VAL_19200;
		break;
		case BAUD_28800:
		baud_rate = BAUDRATE_VAL_28800;
		break;
		case BAUD_38400:
		baud_rate = BAUDRATE_VAL_38400;
		break;
		case BAUD_57600:
		baud_rate = BAUDRATE_VAL_57600;
		break;
		case BAUD_76800:
		baud_rate = BAUDRATE_VAL_76800;
		break;
		case BAUD_115200:
		baud_rate = BAUDRATE_VAL_115200;
		break;
		default :
		baud_rate = BAUDRATE_VAL_DEFAULT;
		break;
	}
	
	U0R_UBRR = baud_rate;
}

ISR(USART0_UDRE_vect)
{
	//cli();
	//DebugPrint("%d",get_size(0));
	if(get_size(USART0_TX))
	{
		//tmptail0 = (UART_TxTail0 + 1)&UART_TX_BUFFER_MASK0;
		//UART_TxTail0 = tmptail0;

		UDR0 = pull(USART0_TX);//UART_TxBuf0[tmptail0];
	}
	else
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
	//sei();
}

ISR(USART0_RX_vect)
{
	uint8_t char_buf = 0;
	char_buf = UDR0;	//dummy read and throw away
	push(USART0_RX, char_buf);
}

//------------------------------------------------------------------------------
#endif	//end UART0 ENABLE


#ifdef _UART1_ENABLE
//-----------------------------------------------------------------------------
// USART Fuction Declaration
// Serial Prot1 Utility Fuction Routine
void U1_Init(unsigned char baud)
{
	U1_SetBaud(baud);
	u1_hal_init();
	init_Queue(USART1_TX);
	init_Queue(USART1_RX);
}

void U1_SetBaud(unsigned char baud)
{
	u1_baudrate(baud);
}

unsigned char U1_GetByte(void)
{
	u1_WaitForReceive();
	
	return u1_RxData();
}

void U1_PutByte(unsigned char Data)
{
	push(USART1_TX, Data);
	UCSR1B |= 1<<UDRIE1;
}

void U1_PutBuf(unsigned char *str, unsigned char len)
{
	unsigned char i =0;
	
    for(i=0;i<len;i++)
    {
        U1_PutByte(*(str+i));
    }
}

void U1_PutStr(unsigned char *str)
{
    while(*str != 0)
    {
        U1_PutByte(*str);
        str++;
    }
}

unsigned char U1_IsGetByte(void)
{
	return u1_IsReceiveData();
}

void u1_TxData(unsigned char Data)
{
	u1_WaitForSend();
	_u1_TxData(Data);
}

void u1_hal_init(void)
{
	// enable serial uart & receiver
	u1_EnableTxIsr();
	u1_EnableRxIsr();
	//at 16Mhz
	UCSR1A |= UART_DB_BAUDRATE;
#if DEBUGSERIAL == _USART1
	fdevopen(u1_TxData,  0);
#endif
}

void u1_baudrate(unsigned char baud)
{
	switch (baud)
	{
		case BAUD_9600:
		UBRR1L = BAUDRATE_VAL_9600;
		break;
		case BAUD_14400:
		UBRR1L = BAUDRATE_VAL_14400;
		break;
		case BAUD_19200:
		UBRR1L = BAUDRATE_VAL_19200;
		break;
		case BAUD_28800:
		UBRR1L = BAUDRATE_VAL_28800;
		break;
		case BAUD_38400:
		UBRR1L = BAUDRATE_VAL_38400;
		break;
		case BAUD_57600:
		UBRR1L = BAUDRATE_VAL_57600;
		break;
		case BAUD_76800:
		UBRR1L = BAUDRATE_VAL_76800;
		break;
		case BAUD_115200:
		UBRR1L = BAUDRATE_VAL_115200;
		break;
		default :
		UBRR1L = BAUDRATE_VAL_DEFAULT;
		break;
	}
}

ISR(USART1_UDRE_vect)
{
	//cli();
	//DebugPrint("%d",get_size(0));
	if(get_size(USART1_TX))
	{
		UDR1 = pull(USART1_TX);//UART_TxBuf0[tmptail0];
	}
	else
	{
		UCSR1B &= ~(1<<UDRIE1);
	}
	//sei();
}

ISR(USART1_RX_vect)
{
	uint8_t char_buf = 0;
	char_buf = UDR1;	//dummy read and throw away
	push(USART1_RX, char_buf);
}
//------------------------------------------------------------------------------
#endif	//end _UART1_ENABLE
