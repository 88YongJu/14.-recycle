#include "adc.h"

volatile unsigned int ADC_Data[ADC_MAX_CHANNEL][ADC_MAX_AVR];
unsigned int ADC_avr[ADC_MAX_CHANNEL];

void ADCInitialize(void)
{
   ADMUX = 0X00;
   ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
}

void ADC_in(void)
{
	static unsigned int ADC_result_cnt;
	static unsigned char ADC_channel = 0;
	static unsigned int ADC_cnt;

	ADCSRA |= (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

	if(ADC_cnt < 10)
	{
	}
	else if(ADC_cnt < 11)
	{
		ADC_Data[ADC_channel][ADC_result_cnt] = (ADCL) | (ADCH<<8);
	}
	else if(ADC_cnt == 11)
	{
		ADC_average(ADC_channel);
	}
	else
	{
		ADC_cnt = 0;
		ADC_channel++;
		
		if(ADC_channel >= ADC_MAX_CHANNEL)
		{
			ADC_result_cnt++;
			if (ADC_result_cnt >= ADC_MAX_AVR)
			{
				ADC_result_cnt = 0;
			}
			ADC_channel = 0;
		}
		ADMUX = ADC_channel;
	}
	ADC_cnt++;
}

void ADC_average(u_char AD_CH)
{
	unsigned long Ada_sum = 0;
	
	for(unsigned char uc_cnt=0;uc_cnt<ADC_MAX_AVR;uc_cnt++)
	{
		Ada_sum += ADC_Data[AD_CH][uc_cnt];
	}
	//DebugPrint("Adc_result%d\r", Adc_result);
	ADC_avr[AD_CH] = Ada_sum/ADC_MAX_AVR;
}

ISR(ADC_vect)
{
	static unsigned int ADC_dly;
	
	ADC_in();
	if (ADC_dly < 60000)
	{
		ADC_dly++;
	}
}