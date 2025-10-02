/*
 * sw.h
 *
 * Created: 2023-02-26 오전 8:59:29
 *  Author: YJKIM
 */ 
#ifndef SW_H_
#define SW_H_

#include "exiom.h"
#include "can.h"
#include "checksum.h"

#define CANID_SW		0x20

#define SW_WAIT			20
#define SW_WAIT_TIME		100

enum{SW1, SW2, SW3,
	 SW4, SW5, SW6,
	 SW7, SW8, SW9,
	 SW10, SW11, SW12,
	 SW_NOT_SEND,
	 };

#define SW_OFF	0x00
#define SW_ON	0xFF

//enum{LED1, LED2};

#define PIN_SW1		!(pine.bit2)
#define PIN_SW2		!(pine.bit6)
#define PIN_SW3		!(pine.bit7)
#define PIN_SW4		!(pinf.bit4)
#define PIN_SW5		!(pinf.bit5)
#define PIN_SW6		!(pinf.bit6)
#define PIN_SW7		!(pinf.bit7)
#define PIN_SW8		!(ping.bit0)
#define PIN_SW9		!(ping.bit1)
#define PIN_SW10	!(ping.bit2)
#define PIN_SW11	!(ping.bit3)
#define PIN_SW12	!(ping.bit4)

struct MOb;

bool sw[12];

void Send_SWSte(void);
void Save_SWSte(char *MSG);
void SW_handler(void);

#endif /* SW_H_ */