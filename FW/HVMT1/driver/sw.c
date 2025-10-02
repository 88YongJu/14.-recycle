/*
 * sw.c
 *
 * Created: 2023-02-26 오전 8:59:16
 *  Author: YJKIM
 */ 

#include "SW.h"

void Save_SWSte(char *MSG)
{
	unsigned char data[8];
	data[0] = *MSG;
	data[1] = *MSG;
	sw[SW9] = data[0] & 0b00000001;
	sw[SW10] = data[0] >> 1 & 0b00000001;
	sw[SW11] = data[0] >> 2 & 0b00000001;
	sw[SW12] = data[0] >> 3 & 0b00000001;
	sw[SW1] = data[1] & 0b00000001;
	sw[SW2] = data[1] >> 1 & 0b00000001;
	sw[SW3] = data[1] >> 2 & 0b00000001;
	sw[SW4] = data[1] >> 3 & 0b00000001;
	sw[SW5] = data[1] >> 4 & 0b00000001;
	sw[SW6] = data[1] >> 5 & 0b00000001;
	sw[SW7] = data[1] >> 6 & 0b00000001;
	sw[SW8] = data[1] >> 7 & 0b00000001;
}

void SW_handler(void)
{
	static u_long SWSend_time = 0;
	static unsigned char act_SW[12], cnt_SW[12];

	SWSend_time = 0;
	if(PIN_SW1)
	{
		if(++cnt_SW[SW1] >= SW_WAIT)
		{
			cnt_SW[SW1] = SW_WAIT;
			sw[SW1] = 1;
		}
	}
	else
	{
		cnt_SW[SW1] = 0;
		sw[SW1] = 0;
	}
		
	if(PIN_SW2)
	{
		if(++cnt_SW[SW2] >= SW_WAIT)
		{
			cnt_SW[SW2] = SW_WAIT;
			sw[SW2] = 1;
		}
	}
	else
	{
		cnt_SW[SW2] = 0;
		sw[SW2] = 0;
	}
		
	if(PIN_SW3)
	{
		if(++cnt_SW[SW3] >= SW_WAIT)
		{
			cnt_SW[SW3] = SW_WAIT;
			sw[SW3] = 1;
		}
	}
	else
	{
		cnt_SW[SW3] = 0;
		sw[SW3] = 0;
	}
		
	if(PIN_SW4)
	{
		if(++cnt_SW[SW4] >= SW_WAIT)
		{
			cnt_SW[SW4] = SW_WAIT;
			sw[SW4] = 1;
		}
	}
	else
	{
		cnt_SW[SW4] = 0;
		sw[SW4] = 0;
	}
		
	if(PIN_SW5)
	{
		if(++cnt_SW[SW5] >= SW_WAIT)
		{
			cnt_SW[SW5] = SW_WAIT;
			sw[SW5] = 1;
		}
	}
	else
	{
		cnt_SW[SW5] = 0;
		sw[SW5] = 0;
	}
		
	if(PIN_SW6)
	{
		if(++cnt_SW[SW6] >= SW_WAIT)
		{
			cnt_SW[SW6] = SW_WAIT;
			sw[SW6] = 1;
		}
	}
	else
	{
		cnt_SW[SW6] = 0;
		sw[SW6] = 0;
	}
		
	if(PIN_SW7)
	{
		if(++cnt_SW[SW7] >= SW_WAIT)
		{
			cnt_SW[SW7] = SW_WAIT;
			sw[SW7] = 1;
		}
	}
	else
	{
		cnt_SW[SW7] = 0;
		sw[SW7] = 0;
	}
		
	if(PIN_SW8)
	{
		if(++cnt_SW[SW8] >= SW_WAIT)
		{
			cnt_SW[SW8] = SW_WAIT;
			sw[SW8] = 1;
		}
	}
	else
	{
		cnt_SW[SW8] = 0;
		sw[SW8] = 0;
	}
		
	if(PIN_SW9)
	{
		if(++cnt_SW[SW9] >= SW_WAIT)
		{
			cnt_SW[SW9] = SW_WAIT;
			sw[SW9] = 1;
		}
	}
	else
	{
		cnt_SW[SW9] = 0;
		sw[SW9] = 0;
	}
		
	if(PIN_SW10)
	{
		if(++cnt_SW[SW10] >= SW_WAIT)
		{
			cnt_SW[SW10] = SW_WAIT;
			sw[SW10] = 1;
		}
	}
	else
	{
		cnt_SW[SW10] = 0;
		sw[SW10] = 0;
	}
		
	if(PIN_SW11)
	{
		if(++cnt_SW[SW11] >= SW_WAIT)
		{
			cnt_SW[SW11] = SW_WAIT;
			sw[SW11] = 1;
		}
	}
	else
	{
		cnt_SW[SW11] = 0;
		sw[SW11] = 0;
	}
		
	if(PIN_SW12)
	{
		if(++cnt_SW[SW12] >= SW_WAIT)
		{
			cnt_SW[SW12] = SW_WAIT;
			sw[SW12] = 1;
		}
	}
	else
	{
		cnt_SW[SW12] = 0;
		sw[SW12] = 0;
	}

	if(++SWSend_time >= SW_WAIT_TIME)
	{
		SWSend_time = 0;
		struct MOb sendmsg1 = {CANID_SW, 0x00, CAN_STD, 2, {(sw[12]<<4 | sw[11]<<3 | sw[10]<<2 | sw[9]<<1 | sw[8]),
		(sw[7]<<7 | sw[6]<<6 | sw[5]<<5 | sw[4]<<4 | sw[3]<<3 | sw[2]<<2 | sw[1]<<1 | sw[0])}};
		can_tx(3,&sendmsg1,0x00);
	}
}