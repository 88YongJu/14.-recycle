/*
 * LED.h
 *
 * Created: 2023-11-17 오전 9:49:13
 *  Author: YJKIM
 */ 


#ifndef LED_H_
#define LED_H_

#include "exiom.h"
#include "PWM.h"

#define CANID_STLED1	0x11
#define CANID_STLED2	0x12
#define CANID_STLED3	0x13
#define CANID_STLED4	0x14
#define CANID_STLED5	0x15
#define CANID_STLED6	0x16
#define CANID_STLED7	0x17
#define CANID_STLED8	0x18
#define CANID_STLED9	0x19

#define CANID_SWLED1	0x21
#define CANID_SWLED2	0x22
#define CANID_SWLED3	0x23
#define CANID_SWLED4	0x24
#define CANID_SWLED5	0x25
#define CANID_SWLED6	0x26
#define CANID_SWLED7	0x27
#define CANID_SWLED8	0x28
#define CANID_SWLED9	0x29
#define CANID_SWLED10	0x2A
#define CANID_SWLED11	0x2B
#define CANID_SWLED12	0x2C

#define LED_OFF	0x00
#define	LED_ON	0xFF

#define PORT_LED1	portb.bit4
#define PORT_LED2	portc.bit0
#define PORT_LED3	portc.bit1
#define PORT_LED4	portc.bit2
#define PORT_LED5	portc.bit3
#define PORT_LED6	portc.bit4
#define PORT_LED7	portc.bit5
#define PORT_LED8	portc.bit6
#define PORT_LED9	portc.bit7

#define PORT_LED10	porta.bit0
#define PORT_LED11	porta.bit1
#define PORT_LED12	porta.bit2
#define PORT_LED13	porta.bit3
#define PORT_LED14	porta.bit4
#define PORT_LED15	porta.bit5
#define PORT_LED16	porta.bit6
#define PORT_LED17	porta.bit7
#define PORT_LED18	portb.bit0
#define PORT_LED19	portb.bit1
#define PORT_LED20	portb.bit2
#define PORT_LED21	portb.bit3

enum{LED1, LED2, LED3,
	LED4, LED5, LED6,
	LED7, LED8, LED9,
	LED10, LED11, LED12,
	LED13, LED14, LED15,
	LED16, LED17, LED18,
	LED19, LED20, LED21,
	LED_NOT_SEND,
};

struct LED
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct LED led[LED_NOT_SEND];

void Send_LEDSte(unsigned char num, unsigned char red, unsigned char green, unsigned char blue);
//void Save_LEDSte(struct MOb *rx_msg);
void Save_LEDSte(char *MSG);
void SteLED_ALL_OFF(void);
void SWLED_ALL_OFF(void);

void SteLED_handler(void);
void SWLED_handler(void);

#endif /* LED_H_ */