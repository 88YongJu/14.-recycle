#include "TIMER.h"

volatile unsigned int _millis = 0;

volatile boolean f_Start = 0;

volatile u_long Timer2[MAX_TIMER2_CNT];
volatile u_long Timer0[MAX_TIMER0_CNT];

volatile unsigned int reccon1_time = 0, reccon2_time = 0;
volatile unsigned int tcp_send_time = 0;
volatile unsigned int run_time = 0;
volatile unsigned int debug_time = 0;

volatile unsigned char send_cnt = 0;
volatile u_long dhcp_time;			/**< DHCP Timer tick count */

volatile unsigned int ui_time_tcpconnect = 0;
volatile unsigned int ui_time_udpconnect_r = 0;
volatile unsigned int ui_time_udpconnect_t = 0;

volatile unsigned char 	w5100_act = 0;

/**
 @brief	Timer handler table 
*/
struct _TIMER_IRQS
{
	TIMER_IRQ timer_irq[MAX_TIMER_CNT];
}TIMER_IRQS;

/**
@brief	This function initialize AVR timer.
*/

void initTimer0(void)
{
	TIMSK0 |= ((1<<TOIE0) | (0<<OCIE0A));
	TCCR0A = ((0<<WGM01) | (0<<WGM00) | (0<<COM0A1) | (0<<COM0A0) | (1<<CS02) | (0<<CS01) | (0<<CS00));
	
	TCNT0 = TCNT_DATA0;
}

void initTimer1(void)
{
	TIMSK1 |= ((1<<TOIE1) | (0<<OCIE1A));
	TCCR1A = ((0<<WGM11) | (0<<WGM10) | (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0));
	TCCR1B = ((0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10));
	
	TCNT1H = (TCNT_DATA1 >> 8);
	TCNT1L = (TCNT_DATA1 & 0b11111111);
}

void initTimer2(void)
{
	TIMSK2 |= ((0<<TOIE2) | (1<<OCIE2A));
	TCCR2A = ((1<<WGM21) | (0<<WGM20) | (0<<COM2A1) | (0<<COM2A0) | (1<<CS22) | (0<<CS21) | (1<<CS20));

	//TCNT2 = TCNT_DATA2;
	OCR2A = OCR2_1ms;
}

void initTimer3(void)
{
	TIMSK3 |= ((1<<TOIE3) | (0<<OCIE3A));
	TCCR3A = ((0<<WGM31) | (0<<WGM30) | (0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0));
	TCCR3B = ((0<<WGM33) | (0<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30));
	
	TCNT3H = (TCNT_DATA3 >> 8);
	TCNT3L = (TCNT_DATA3 & 0b11111111);
}
//Init_timer(TIMER2, OCIE, NORMAL, CLK_1024);

/*timernum, interrupt, Generationmode, Output mode, Prescale*/ 
void Init_timer(int num, int intrrupt, int Gmode, int Omode, int Pre)
{
	switch(num)
	{
		case TIMER0 : 
			switch(intrrupt)
			{
				case OVIE:
					TIMSK0 |= 1 << TOIE0;
				break;
				case OCIE:
					TIMSK0 |= 1 << OCIE0A;
				break;
				case ICIE:
				TIMSK0 |= 1 << TOIE0;
				break;
			}
			switch(Gmode)
			{
				case NORMAL:
					TCCR0A &= ~(1 << WGM01 | 1 << WGM00);
				break;
				case PWM :
					TCCR0A |= (1 << WGM00);
					TCCR0A &= ~(1 << WGM01);
				break;
				case CTC:
					TCCR0A &= ~(1 << WGM00);
					TCCR0A |= (1 << WGM01);
				case FAST_PWM:
					TCCR0A |= (1 << WGM00);
					TCCR0A |= (1 << WGM01);
				break;
			}
			break;
			switch(Omode)
			{
				case NORMAL:
					TCCR0A &= ~((1<<COM0A1) | (1<<COM0A0));
				break;
				case TOGGLE:
					TCCR0A |= (1<<COM0A0);
					TCCR0A &= ~(1<<COM0A1);
				break;
				case CLEAR_COMP:
					TCCR0A &= ~(1<<COM0A0);
					TCCR0A |= (1<<COM0A1);
				break;
				case SET_COMP:
					TCCR0A |= (1<<COM0A0);
					TCCR0A |= (1<<COM0A1);
				break;
			}
			switch(Pre)
			{
				case STOPPED:
					TCCR0A &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
				break;
				case CLK_1:
				TCCR0A &= ~((1<<CS02) | (1<<CS01));
				TCCR0A |= (1<<CS00);
				break;
				case CLK_8:
				TCCR0A &= ~((1<<CS02) | (1<<CS00));
				TCCR0A |= (1<<CS01);
				break;
				case CLK_64:
				TCCR0A &= ~((1<<CS02));
				TCCR0A |= (1<<CS01) | (1<<CS00);
				break;
				case CLK_256:
				TCCR0A &= ~((1<<CS01));
				TCCR0A |= (1<<CS02 | (1<<CS00));
				break;
				case CLK_FAL:
				TCCR0A &= ~((1<<CS00));
				TCCR0A |= (1<<CS02 | (1<<CS01));
				break;
				case CLK_RISI:
				TCCR0A |= (1<<CS02 | (1<<CS01) | 1<<CS00);
				break;
			}
		case TIMER1 :
			switch(intrrupt)
			{
				case OVIE:
				TIMSK1 |= 1 << TOIE1;
				break;
				case OCAIE:
				TIMSK1 |= 1 << OCIE1A;
				break;
				case OCBIE:
				TIMSK1 |= 1 << OCIE1B;
				break;
				case OCCIE:
				TIMSK1 |= 1 << OCIE1C;
				break;
				case ICIE:
				TIMSK1 |= 1 << TOIE1;
				default :
				break;
			}
			switch(Gmode)
			{
				case NORMAL:
				TCCR1A &= ~(1 << WGM11 | 1 << WGM10);
				TCCR1B &= ~(1 << WGM13 | 1 << WGM12);
				break;
				case PWM :
				TCCR1A |= (1 << WGM00);
				TCCR1A &= ~(1 << WGM01);
				TCCR1B &= ~(1 << WGM13 | 1 << WGM12);
				break;
				case CTC:
				TCCR0A &= ~(1 << WGM00);
				TCCR0A |= (1 << WGM01);
				case FAST_PWM:
				TCCR0A |= (1 << WGM00);
				TCCR0A |= (1 << WGM01);
				break;
			}
			break;
			break;
		
		case TIMER2 :
			switch(intrrupt)
			{
				case OVIE:
				TIMSK2 |= 1 << TOIE2;
				break;
				case OCIE:
				TIMSK2 |= 1 << OCIE2A;
				break;
			}
			switch(Gmode)
			{
				case NORMAL:
				TCCR2A &= ~(1 << WGM21 | 1 << WGM20);
				break;
				case PWM :
				TCCR2A |= (1 << WGM20);
				TCCR2A &= ~(1 << WGM21);
				break;
				case CTC:
				TCCR2A &= ~(1 << WGM20);
				TCCR2A |= (1 << WGM21);
				case FAST_PWM:
				TCCR2A |= (1 << WGM20);
				TCCR2A |= (1 << WGM21);
				break;
			}
			break;
			switch(Omode)
			{
				case NORMAL:
				TCCR2A &= ~((1<<COM2A1) | (1<<COM2A0));
				break;
				case TOGGLE:
				TCCR2A |= (1<<COM2A0);
				TCCR2A &= ~(1<<COM2A1);
				break;
				case CLEAR_COMP:
				TCCR2A &= ~(1<<COM2A0);
				TCCR2A |= (1<<COM2A1);
				break;
				case SET_COMP:
				TCCR2A |= (1<<COM2A0);
				TCCR2A |= (1<<COM2A1);
				break;
			}
			switch(Pre)
			{
				case STOPPED:
				TCCR2A &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));
				break;
				case CLK_1:
				TCCR2A &= ~((1<<CS22) | (1<<CS21));
				TCCR2A |= (1<<CS20);
				break;
				case CLK_8:
				TCCR2A &= ~((1<<CS22) | (1<<CS20));
				TCCR2A |= (1<<CS21);
				break;
				case CLK_64:
				TCCR2A &= ~((1<<CS22));
				TCCR2A |= (1<<CS21) | (1<<CS20);
				break;
				case CLK_256:
				TCCR2A &= ~((1<<CS21));
				TCCR2A |= (1<<CS22 | (1<<CS20));
				break;
				case CLK_FAL:
				TCCR2A &= ~((1<<CS20));
				TCCR2A |= (1<<CS22 | (1<<CS21));
				break;
				case CLK_RISI:
				TCCR2A |= (1<<CS22 | (1<<CS21) | 1<<CS20);
				break;
			}
		break;
	}
}


/************Delay**************/
void Delay_us(unsigned char time_us)  // delay time. us
{
	register unsigned char i;
	for(i = 0 ; i < time_us ; i++ )
	{
		asm volatile("PUSH	R0");
		asm volatile("POP	R0");
		asm volatile("PUSH	R0");
		asm volatile("POP	R0");
		asm volatile("PUSH	R0");
		asm volatile("POP	R0");	// 1us for 20Mhz
	}
}

void Delay_ms(unsigned int time_ms)  //delay time. ms
{
	register unsigned int i;
	for( i = 0 ; i < time_ms ; i++)
	{
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);		// 1ms for 20Mhz
	}
}

unsigned int millis(void)
{
	cli();
	unsigned int m = _millis;
	sei();
	return m;
}

ISR(TIMER2_COMP_vect)
{
	_millis++;
}