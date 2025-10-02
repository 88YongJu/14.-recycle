/*
 * LED.c
 *
 * Created: 2023-11-17 오전 9:48:59
 *  Author: YJKIM
 */ 

#include "LED.h"
/*
void Send_LEDSte(unsigned char num, unsigned char red, unsigned char green, unsigned char blue)
{
	struct MOb sendmsg = {CANID_LED, 0x00, CAN_STD, 4, {num, red, green, blue}};
	can_tx(3,&sendmsg,0x00);
}
*/
/*
void Save_LEDSte(struct MOb *rx_msg)
{
	led[rx_msg->data[0]].red = rx_msg->data[1];
	led[rx_msg->data[0]].green = rx_msg->data[2];
	led[rx_msg->data[0]].blue = rx_msg->data[3];
}
*/

void Save_LEDSte(char *MSG)
{
	char data[4];
	data[0] = *MSG;
	led[data[0]].red = *MSG;
	led[data[0]].green = *MSG;
	led[data[0]].blue = *MSG;
}

void SteLED_ALL_OFF(void)
{
	DDRB = (DDRB & 0b11101111);
	PORTB = (PORTB | 0b00010000);
	DDRC = 0b00000000;
	PORTC = 0b11111111;
}

void SWLED_ALL_OFF(void)
{
	DDRA = 0b00000000;
	PORTA = 0b00000000;
	DDRB = (DDRB & 0b11110000);
	PORTB = (PORTB & 0b00010000);
}

void SteLED_handler(void)
{
	static unsigned char led_cnt = LED1;

	SteLED_ALL_OFF();
	
	Out_PWM(PWM_3A, led[led_cnt].red);
	Out_PWM(PWM_3B, led[led_cnt].green);
	Out_PWM(PWM_3C, led[led_cnt].blue);
	
	switch(led_cnt++)
	{
		case LED1:
		//DebugPrint("LED1\r");
		ddrb.bit4 = 1;
		PORT_LED1 = 0;
		break;
		
		case LED2:
		ddrc.bit0 = 1;
		PORT_LED2 = 0;
		break;
		
		case LED3:
		ddrc.bit1 = 1;
		PORT_LED3 = 0;
		break;
		
		case LED4:
		ddrc.bit2 = 1;
		PORT_LED4 = 0;
		break;
		
		case LED5:
		ddrc.bit3 = 1;
		PORT_LED5 = 0;
		break;
		
		case LED6:
		ddrc.bit4 = 1;
		PORT_LED6 = 0;
		break;
		
		case LED7:
		ddrc.bit5 = 1;
		PORT_LED7 = 0;
		break;
		
		case LED8:
		ddrc.bit6 = 1;
		PORT_LED8 = 0;
		break;
		
		case LED9:
		ddrc.bit7 = 1;
		PORT_LED9 = 0;
		led_cnt = LED1;
		break;
		
		default:
		break;
	}
}

void SWLED_handler(void)
{
	static unsigned char led_cnt = LED10;
			
	SWLED_ALL_OFF();
	
	Out_PWM(PWM_1A, led[led_cnt].red);
	Out_PWM(PWM_1B, led[led_cnt].green);
	Out_PWM(PWM_1C, led[led_cnt].blue);
	
	switch(led_cnt++)
	{
		case LED10 :
		ddra.bit0 = 1;
		PORT_LED10 = 1;
		break;
		
		case LED11 :
		ddra.bit1 = 1;
		PORT_LED11 = 1;
		break;
		
		case LED12 :
		ddra.bit2 = 1;
		PORT_LED12 = 1;
		break;
		
		case LED13:
		ddra.bit3 = 1;
		PORT_LED13 = 1;
		break;
		
		case LED14:
		ddra.bit4 = 1;
		PORT_LED14 = 1;
		break;
		
		case LED15:
		ddra.bit5 = 1;
		PORT_LED15 = 1;
		break;
		
		case LED16:
		ddra.bit6 = 1;
		PORT_LED16 = 1;
		break;
		
		case LED17:
		ddra.bit7 = 1;
		PORT_LED17 = 1;
		break;
		
		case LED18:
		ddrb.bit0 = 1;
		PORT_LED18 = 1;
		break;
		
		case LED19:
		ddrb.bit1 = 1;
		PORT_LED19 = 1;
		break;
		
		case LED20:
		ddrb.bit2 = 1;
		PORT_LED20 = 1;
		break;
		
		case LED21:
		ddrb.bit3 = 1;
		PORT_LED21 = 1;
		led_cnt = LED10;
		break;
		
		default :
		break;
	}
}