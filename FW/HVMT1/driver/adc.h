#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"

#define ADC_MAX_CHANNEL	8
#define ADC_MAX_AVR		20	//Æò±Õ »êÃâ È½¼ö
#define ADC_MAXVAL		1024
#define ADC_HIGH		896
#define ADC_LOW			128

volatile unsigned int ADC_Data[ADC_MAX_CHANNEL][ADC_MAX_AVR];
unsigned int ADC_avr[ADC_MAX_CHANNEL];

void ADCInitialize(void);
void ADC_in(void);
void ADC_average(u_char AD_CH);

#endif