/*
 * exint.c
 *
 * Created: 2023-01-30 오후 4:30:02
 *  Author: YJKIM
 */ 

#include "EXINT.h"

unsigned int ui_Hi_time_cnt = 0;

/*
	EICRA = ((0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00));
	EICRB = ((0<<ISC71) | (0<<ISC70) | (0<<ISC61) | (1<<ISC60) | (0<<ISC51) | (0<<ISC50));
	EIMSK = ((0<<INT7) | (0<<INT6) | (0<<INT5) | (0<<INT1) | (0<<INT0));
*/

void Enable_EXINT(unsigned int num, unsigned int control)
{
	EIMSK |= (1 << num);					//interrupt enable
	if(num < 4)
	{
		EICRA = control << num *2;
	}
	else
	{
		EICRB = control << num *2;
	}
}

void Disable_EXINT(unsigned int num)
{
	EIMSK &= ~(1 << num);
}

ISR(INT1_vect)	//Throttle PWM Input	//High time count
{
	//DebugPrint("INT1\r");
	if (pind.bit1 == 1)
	{
		TCNT0 = 0;
	}
	else
	{
		ui_Hi_time_cnt = TCNT0;
	}
}