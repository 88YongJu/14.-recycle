#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "exiom.h"
#include "AD7923.h"

#define TCNT_DATA1		0
#define TCNT_DATA3		0

#define ICR_DATA1		2000	//±‚¡ÿ¡÷∆ƒºˆ
#define ICR_DATA3		2000	//±‚¡ÿ¡÷∆ƒºˆ

#define OCR_DATA1A		0	//OCR1Aµ‡∆º∫Ò
#define OCR_DATA1B		0	//OCR1Bµ‡∆º∫Ò
#define OCR_DATA1C		0	//OCR1Cµ‡∆º∫Ò
#define OCR_DATA3A		0	//OCR3Aµ‡∆º∫Ò
#define OCR_DATA3B		0	//OCR3Bµ‡∆º∫Ò
#define OCR_DATA3C		0	//OCR3Cµ‡∆º∫Ò

#define PWM_ON		ICR_DATA1
#define PWM_OFF		0

#define CUT_WAIT_TIME	1000

#define DO1_CUTCURRENT	5000
#define DO2_CUTCURRENT	5000
#define DO3_CUTCURRENT	5000
#define DO4_CUTCURRENT	2000
#define DO5_CUTCURRENT	2000
#define DO6_CUTCURRENT	2000

#define RY1_CUTCURRENT	2000
#define RY2_CUTCURRENT	2000
#define RY3_CUTCURRENT	2000
#define RY4_CUTCURRENT	2000

#define DO1_CUR	((AD_DO1_CUR*3)/10)
#define DO2_CUR	((AD_DO2_CUR*3)/10)
#define DO3_CUR	((AD_DO3_CUR*3)/10)
#define DO4_CUR	((AD_DO4_CUR*3)/10)
#define DO5_CUR	((AD_DO5_CUR*3)/10)
#define DO6_CUR	((AD_DO6_CUR*3)/10)

#define RY1_CUR	((AD_RY1_CUR*3)/10)
#define RY2_CUR	((AD_RY2_CUR*3)/10)
#define RY3_CUR	((AD_RY3_CUR*3)/10)
#define RY4_CUR	((AD_RY4_CUR*3)/10)

enum{PWM_1, PWM_2, PWM_3, PWM_4, PWM_5, PWM_6};

volatile unsigned int ui_Pwm[6];
volatile unsigned int ui_Def_Cur[10];
volatile unsigned int ui_Cut_time[10];
	
void Init_PWM(void);
void Out_PWM(unsigned char channel);

#endif