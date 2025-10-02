#include "PWM.h"

volatile unsigned int ui_Pwm[6] = {0,};
volatile unsigned int ui_Def_Cur[10];
volatile unsigned int ui_Cut_time[10];
/*
void Init_Timer()
*/
void Init_PWM(void)
{
	TCCR1A = ((1<<COM1A1) | (0<<COM1A0)
			| (1<<COM1B1) | (0<<COM1B0)
			| (1<<COM1C1) | (0<<COM1C0)
			| (1<<WGM11) | (0<<WGM10));
	TCCR1B = ((1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10));
	
	ICR1H = (ICR_DATA1 >> 8);
	ICR1L = (ICR_DATA1 & 0b11111111);
	
	TCNT1H = (TCNT_DATA1 >> 8);
	TCNT1L = (TCNT_DATA1 & 0b11111111);
	
	OCR1AH = (OCR_DATA1A >> 8);
	OCR1AL = (OCR_DATA1A & 0b11111111);

	OCR1BH = (OCR_DATA1B >> 8);
	OCR1BL = (OCR_DATA1B & 0b11111111);

	OCR1CH = (OCR_DATA1C >> 8);
	OCR1CL = (OCR_DATA1C & 0b11111111);
	
	ui_Pwm[PWM_1] = PWM_OFF;
	ui_Pwm[PWM_2] = PWM_OFF;
	ui_Pwm[PWM_3] = PWM_OFF;
	Out_PWM(PWM_1);
	Out_PWM(PWM_2);
	Out_PWM(PWM_3);
	
	TCCR3A = ((1<<COM3A1) | (0<<COM3A0) | (1<<COM3B1) | (0<<COM3B0) | (1<<COM3C1) | (0<<COM3C0) | (1<<WGM31) | (0<<WGM30));
	TCCR3B = ((1<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30));

	ICR3H = (ICR_DATA3 >> 8);
	ICR3L = (ICR_DATA3 & 0b11111111);

	TCNT3H = (TCNT_DATA3 >> 8);
	TCNT3L = (TCNT_DATA3 & 0b11111111);

	OCR3AH = (OCR_DATA3A >> 8);
	OCR3AL = (OCR_DATA3A & 0b11111111);

	OCR3BH = (OCR_DATA3B >> 8);
	OCR3BL = (OCR_DATA3B & 0b11111111);

	OCR3CH = (OCR_DATA3C >> 8);
	OCR3CL = (OCR_DATA3C & 0b11111111);
	
	ui_Pwm[PWM_4] = PWM_OFF;
	ui_Pwm[PWM_5] = PWM_OFF;
	ui_Pwm[PWM_6] = PWM_OFF;
	Out_PWM(PWM_4);
	Out_PWM(PWM_5);
	Out_PWM(PWM_6);
}

void Out_PWM(unsigned char channel)
{
	switch(channel)
	{
		case PWM_1 :
		if((IN_VOL-2000) <= FOUT1_VOL)
		{
			if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME)
			{
				TCCR1A |= ((1<<COM1A1) | (0<<COM1A0));
				OCR1AH = (ui_Pwm[channel] >> 8);
				OCR1AL = (ui_Pwm[channel] & 0b11111111);
				if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME + 5)
				{
					if((DO1_CUR > (ui_Def_Cur[channel] + DO1_CUTCURRENT)))
					{
						TCCR1A &= ~((1<<COM1A1) | (1<<COM1A0));
						OCR1AH = 0x00;
						OCR1AL = 0x00;
						portb.bit5 = 0;
						ui_Cut_time[channel] = millis();
					}
					else
					{
						ui_Cut_time[channel] = millis() - (CUT_WAIT_TIME + 5);
					}
				}
			}
		}
		else
		{
			TCCR1A &= ~((1<<COM1A1) | (1<<COM1A0));
			OCR1AH = 0x00;
			OCR1AL = 0x00;
			portb.bit5 = 0;
		}
		break;
		
		case PWM_2 :
		if((IN_VOL-2000) <= FOUT2_VOL)
		{
			if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME)
			{
				TCCR1A |= ((1<<COM1B1) | (0<<COM1B0));
				OCR1BH = (ui_Pwm[channel] >> 8);
				OCR1BL = (ui_Pwm[channel] & 0b11111111);
				if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME + 5)
				{
					if((DO2_CUR > (ui_Def_Cur[channel] + DO2_CUTCURRENT)))
					{
						TCCR1A &= ~((1<<COM1B1) | (1<<COM1B0));
						OCR1BH = 0x00;
						OCR1BL = 0x00;
						portb.bit6 = 0;
						ui_Cut_time[channel] = millis();
					}
					else
					{
						ui_Cut_time[channel] = millis() - (CUT_WAIT_TIME + 5);
					}
				}
			}
		}
		else
		{
			TCCR1A &= ~((1<<COM1B1) | (1<<COM1B0));
			OCR1BH = 0x00;
			OCR1BL = 0x00;
			portb.bit6 = 0;
		}
		break;
		
		case PWM_3 :
		if((IN_VOL-2000) <= FOUT3_VOL)
		{
			if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME)
			{
				TCCR1A |= ((1<<COM1C1) | (0<<COM1C0));
				OCR1CH = (ui_Pwm[channel] >> 8);
				OCR1CL = (ui_Pwm[channel] & 0b11111111);
				if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME + 5)
				{
					if((DO3_CUR > (ui_Def_Cur[channel] + DO3_CUTCURRENT)))
					{
						TCCR1A &= ~((1<<COM1C1) | (1<<COM1C0));
						OCR1CH = 0x00;
						OCR1CL = 0x00;
						portb.bit7 = 0;
						ui_Cut_time[channel] = millis();
					}
					else
					{
						ui_Cut_time[channel] = millis() - (CUT_WAIT_TIME + 5);
					}
				}
			}
		}
		else
		{
			TCCR1A &= ~((1<<COM1C1) | (1<<COM1C0));
			OCR1CH = 0x00;
			OCR1CL = 0x00;
			portb.bit7 = 0;
		}
		
		break;
		
		case PWM_4 :
		if((IN_VOL-2000) <= FOUT4_VOL)
		{
			if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME)
			{
				TCCR3A |= ((1<<COM3A1) | (0<<COM3A0));
				OCR3AH = (ui_Pwm[channel] >> 8);
				OCR3AL = (ui_Pwm[channel] & 0b11111111);
				if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME + 5)
				{
					if((DO4_CUR > (ui_Def_Cur[PWM_4] + DO4_CUTCURRENT)))
					{
						TCCR3A &= ~((1<<COM3A1) | (1<<COM3A0));
						OCR3AH = 0x00;
						OCR3AL = 0x00;
						porte.bit3 = 0;
						ui_Cut_time[channel] = millis();
					}
					else
					{
						ui_Cut_time[channel] = millis() - (CUT_WAIT_TIME + 5);
					}
				}
			}
		}
		else
		{
			TCCR3A &= ~((1<<COM3A1) | (1<<COM3A0));
			OCR3AH = 0x00;
			OCR3AL = 0x00;
			porte.bit3 = 0;
		}
		break;
		
		case PWM_5 :
		if(((IN_VOL-2000) <= FOUT5_VOL))
		{
			if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME)
			{
				TCCR3A |= (1<<COM3B1) | (0<<COM3B0);
				OCR3BH = 0x00;
				OCR3BL = 0x00;
				if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME + 5)
				{
					if((DO5_CUR > (ui_Def_Cur[PWM_5] + DO5_CUTCURRENT)))
					{
						TCCR3A &= ~((1<<COM3B1) | (1<<COM3B0));
						porte.bit4 = 0;
						ui_Cut_time[channel] = millis();
					}
					else
					{
						ui_Cut_time[channel] = millis() - (CUT_WAIT_TIME + 5);
					}
				}
			}
		}
		else
		{
			TCCR3A &= ~((1<<COM3B1) | (1<<COM3B0));
			OCR3BH = 0x00;
			OCR3BL = 0x00;
			porte.bit4 = 0;
		}
		break;
		
		case PWM_6 :
		if((IN_VOL-2000) <= FOUT6_VOL)
		{
			if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME)
			{
				TCCR3A |= (1<<COM3C1) | (0<<COM3C0);
				OCR3CH = (ui_Pwm[channel] >> 8);
				OCR3CL = (ui_Pwm[channel] & 0b11111111);
				
				if(millis() - ui_Cut_time[channel] >= CUT_WAIT_TIME + 5)
				{
					if((DO6_CUR > (ui_Def_Cur[channel] + DO6_CUTCURRENT)))
					{
						TCCR3A &= ~((1<<COM3C1) | (1<<COM3C0));
						OCR3CH = 0x00;
						OCR3CL = 0x00;
						porte.bit5 = 0;
						ui_Cut_time[channel] = millis();
					}
					else
					{
						ui_Cut_time[channel] = millis() - (CUT_WAIT_TIME + 5);
					}
				}
			}
		}
		else
		{
			TCCR3A &= ~((1<<COM3C1) | (1<<COM3C0));
			OCR3CH = 0x00;
			OCR3CL = 0x00;
			porte.bit5 = 0;
		}
		break;
		
		default :
		break;
	}
}
