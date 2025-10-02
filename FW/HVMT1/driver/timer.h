#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"

/********************************************************************************
								TIMER DEFINE
********************************************************************************/
#define TCNT_DATA0		0x00//0x06	/** 1024only Pwm Check */
#define TCNT_DATA1		0
#define TCNT_DATA2		0	//6=2ms 130=1ms	/** 64/25< 1ms at 16MHz */
#define OCR2_1ms		124	//250=2ms
#define OCR2_2ms		249	//250=2ms

#define TCNT_DATA3		0

#define MAX_TIMER2_CNT	10	// MAX Timer2 Handler Count
#define MAX_TIMER0_CNT	0	// MAX Timer0 Handler Count

#define MAX_TIMER_CNT	(MAX_TIMER0_CNT+MAX_TIMER2_CNT)	// Max Timer Handler
/********************************************************************************
TIMER2 Handler
********************************************************************************/
#define RESET_CHECK_TIMER	0
#define AD_CHECK_TIMER		1
#define UDP_CHECK_TIMER		3
#define DHCP_CHECK_TIMER	4

//#define INV_CHECK_TIMER		6
/********************************************************************************
TIMER0 Handler
********************************************************************************/

/********************************************************************************
WAIT TIME (10ms)
********************************************************************************/

#define	RUNLED_WAIT_TIME	500
#define	ERRLED_WAIT_TIME	250
#define	DEBUG_WAIT_TIME		500
#define	HVMT_WAIT_TIME		500
#define HVB_WAIT_TIME		500
#define	LED_WAIT_TIME		1000

#define	TCPCONNECT_WAIT_TIME		1000
#define	UDPCONNECT_WAIT_TIME		1000

enum TIMERNUM{TIMER0, TIMER1, TIMER2, TIMER3};

/*Interrupt enable define*/
#define OVIE		0
#define OCIE		1	//timer0, 2 only
#define OCAIE		1
#define OCBIE		2
#define OCCIE		3
#define ICIE		4

/*Generation Mode define*/
#define NORMAL		0
#define PWM			1
#define CTC			2
#define FAST_PWM	3

/*Compare Output Mode*/
#define TOGGLE		1
#define CLEAR_COMP	2
#define SET_COMP	3

/*timer prescale*/
#define STOPPED		0
#define CLK_1		1
#define CLK_8		2
#define CLK_32		3
#define CLK_64		4
#define CLK_128		5
#define CLK_256		6
#define CLK_1024	7
#define CLK_FAL		8
#define CLK_RISI	9

/* Timer handler */
typedef struct _TIMER_IRQ
{
	void (*user_timer_handler)(void);
}TIMER_IRQ;

volatile u_long Timer0[MAX_TIMER0_CNT];
volatile unsigned int reccon1_time;
volatile unsigned int reccon2_time;
volatile unsigned int run_time;
volatile unsigned int debug_time;
volatile u_long inv_time;

volatile u_long dhcp_time;			/**< DHCP Timer tick count */
volatile boolean f_Start;

volatile unsigned int ui_time_tcpconnect;
volatile unsigned int ui_time_udpconnect_r;
volatile unsigned int ui_time_udpconnect_t;

volatile unsigned char 	w5100_act;

void initTimer0(void);
void initTimer1(void);
void initTimer2(void);
void initTimer3(void);

void Delay_us(unsigned char time_us);
void Delay_ms(unsigned int time_ms);

unsigned int millis(void);

#endif