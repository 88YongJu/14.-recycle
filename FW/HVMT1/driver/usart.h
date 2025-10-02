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

#ifndef _SERIAL_H_
#define	_SERIAL_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include "types.h"
#include "queue.h"
//------------------------------------------------------------------------------
// Varialble System Defination
//------------------------------------------------------------------------------
#define _HAL_MEGA128_UART
//#define _HAL_MEGA8_UART
//#define _HAL_USB128x_UART

//#define _SYS_CLK_AT147456MHZ
#define _SYS_CLK_AT16MHZ

//------------------------------------------------------------------------------
// Varialble UART Defination
//------------------------------------------------------------------------------
#define _UART0_ENABLE
#define _UART1_ENABLE
#define BAUDRATE_VAL_DEFAULT				BAUDRATE_VAL_57600
#define DEBUGSERIAL	_USART0
#define DBG_PRT_TIME	100

//------------------------------------------------------------------------------
// UART Defination
//------------------------------------------------------------------------------
#define _USART0	0
#define _USART1	1
#define BAUD_9600							0
#define BAUD_14400							1
#define BAUD_19200							2
#define BAUD_28800							3
#define BAUD_38400							4
#define BAUD_57600							5
#define BAUD_76800							6
#define BAUD_115200							7

#ifdef _SYS_CLK_AT16MHZ
#define BAUDRATE_VAL_9600					207
#define BAUDRATE_VAL_19200					103
#define BAUDRATE_VAL_14400					138
#define BAUDRATE_VAL_28800					68
#define BAUDRATE_VAL_38400					51
#define BAUDRATE_VAL_57600					34
#define BAUDRATE_VAL_76800					25
#define BAUDRATE_VAL_115200					16
#endif

#ifdef _SYS_CLK_AT147456MHZ
#define BAUDRATE_VAL_9600					95
#define BAUDRATE_VAL_19200					47
#define BAUDRATE_VAL_28800					31
#define BAUDRATE_VAL_38400					24
#define BAUDRATE_VAL_57600					15
#define BAUDRATE_VAL_76800					11
#define BAUDRATE_VAL_115200					7
#endif

/*
#define BAUD_RATE(xtal,baud,smod)       	(256L-xtal/(baud*smod*192L))
//#define T2RELOAD(Baudrate)   				(65536-(XTAL/32/(Baudrate)))
#define T2RELOAD(Baudrate)   				(65536-((XTAL/2)/32/(Baudrate)))
*/

#define SERIAL_MODE1                    	0x40  // SCON.6 and SCON.7
#define DOUBLE_BAUD_RATE                	0x80  // PCON.7
#define ENABLE_RECEIVE                  	0x10  // SCON.4

#define UART_RXINT_ENABLE           		0x80
#define UART_TXINT_ENABLE           		0x40
#define UART_RX_ENABLE              		0x10
#define UART_TX_ENABLE              		0x08

#define UART_RX_COMPLETE            		0x80
#define UART_TX_COMPLETE            		0x40
#define UART_DATA_EMPTY             		0x20

#define TXD0_SEL_EN							BIT1
#define RXD0_SEL_EN							BIT0
#define TXD1_SEL_EN							0x40
#define RXD1_SEL_EN							0x80

#define UART_DB_BAUDRATE					0x02

#ifdef _UART0_ENABLE
//------------------------------------------------------
//Atmega128 Serial Register
#ifdef _HAL_MEGA128_UART
#define U0R_DATA							UDR0
#define U0R_CSRA							UCSR0A
#define U0R_CSRB							UCSR0B
#define U0R_UBRR							UBRR0L

#define U1R_DATA							UDR1
#define U1R_CSRA							UCSR1A
#define U1R_CSRB							UCSR1B
#define U1R_UBRR							UBRR1L
#endif
//------------------------------------------------------
//Atmega8 Serial Register
#ifdef _HAL_MEGA8_UART
#define U0R_DATA							UDR
#define U0R_CSRA							UCSRA
#define U0R_CSRB							UCSRB
#define U0R_UBRR							UBRRL
#endif
//------------------------------------------------------------------------------
// Serial Prot0 Utility Fuction Routine
#define U0_printf							printf
#define u0_WaitForReceive()					while(!(UCSR0A & UART_RX_COMPLETE))
#define u0_RxData()		 					(U0R_DATA)
#define _u0_TxData(Data)					(U0R_DATA = Data)
#define u0_WaitForSend()					while(!(U0R_CSRA & UART_DATA_EMPTY))
#define u0_IsReceiveData()					((U0R_CSRA & UART_RX_COMPLETE))

#define u0_EnableRxIsr()					(U0R_CSRB |= (UART_RXINT_ENABLE|UART_RX_ENABLE))
#define u0_EnableTxIsr()					(U0R_CSRB |= (UART_DATA_EMPTY|UART_TX_ENABLE))
#define u0_EnableRx()						(U0R_CSRB |= UART_RX_ENABLE)
#define u0_EnableTx()						(U0R_CSRB |= UART_TX_ENABLE)

//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// USART0 Fuction Declaration
void u0_baudrate(unsigned char baud);
void u0_hal_init(void);
void u0_TxData(unsigned char Data);
void u0_hal_init(void);
void u0_baudrate(unsigned char baud);
unsigned char U0_IsGetByte(void);
void initUsart0(unsigned char baud);
void U0_SetBaud(unsigned char baud);
unsigned char U0_GetByte(void);
void U0_PutByte(unsigned char Data);
void putStringUsart0(char *str);
void U0_PutBuf(unsigned char *str, unsigned char len);
//------------------------------------------------------------------------------
#endif	//end UART0 ENABLE


#ifdef _UART1_ENABLE
//------------------------------------------------------------------------------
// Serial Prot1 Utility Fuction Routine
#define U1_printf							printf
#define u1_WaitForReceive()					while(!(UCSR1A & UART_RX_COMPLETE))
#define u1_RxData()		 					(UDR1)
#define _u1_TxData(Data)					(UDR1 = Data)
#define u1_WaitForSend()					while(!(UCSR1A & UART_DATA_EMPTY))
#define u1_IsReceiveData()					((UCSR1A & UART_RX_COMPLETE))

#define u1_EnableRxIsr()					(U1R_CSRB |= (UART_RXINT_ENABLE|UART_RX_ENABLE))
#define u1_EnableTxIsr()					(U1R_CSRB |= (UART_DATA_EMPTY|UART_TX_ENABLE))
#define u1_EnableRx()						(U1R_CSRB |= UART_RX_ENABLE)
#define u1_EnableTx()						(U1R_CSRB |= UART_TX_ENABLE)
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// USART1 Fuction Declaration
unsigned char U1_IsGetByte(void);
void U1_Init(unsigned char baud);
void U1_SetBaud(unsigned char baud);
unsigned char U1_GetByte(void);
void U1_PutByte(unsigned char Data);
void U1_PutStr(unsigned char *str);
void U1_PutBuf(unsigned char *str, unsigned char len);
void u1_baudrate(unsigned char baud);
void u1_TxData(unsigned char Data);
void u1_hal_init(void);
void u1_baudrate(unsigned char baud);
//------------------------------------------------------------------------------
#endif	//end _UART1_ENABLE

//-----------------------------------------------------------------------------
#if DEBUGSERIAL == _USART0
#define DebugPrint						printf
#define DebugInit						U0_Init
#define DebugGetByte					U0_GetByte
#define DebugIsByte						U0_IsGetByte
#endif

#if DEBUGSERIAL	== _USART1
#define DebugPrint						printf
#define DebugInit						U1_Init
#define DebugGetByte					U1_GetByte
#define DebugIsByte						U1_IsGetByte
#endif
//-----------------------------------------------------------------------------

#endif