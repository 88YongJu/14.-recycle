/*
 * tcps1.h
 *
 * Created: 2022-12-13 오후 5:07:25
 *  Author: YJKIM
 */ 


#ifndef TCPS1_H_
#define TCPS1_H_

#include <avr/io.h>
//#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "w5100.h"
#include "spi.h"
#include "serial.h"

//-----------------------------------------------------------------------------

//SYSTEM Fuction Declaration

// Enter interrupt
#define enable()  __asm__ __volatile__ ("sei" ::)
#define disable() __asm__ __volatile__ ("cli" ::)

//-----------------------------------------------------------------------------

unsigned char InitSocket(unsigned int Socket, unsigned Port);
void EthernetTest(unsigned char *pRcvBuffer, unsigned int len);
void ProcessTcpSever(SOCKET s);
void ProcessTcpClient(u_char ch);
void Gpioinit(void);
void Cpuinit(void);
void SystemInit(void);


#endif /* TCPS1_H_ */