/*
 * MAX11162.h
 *
 * Created: 2023-03-30 오후 2:13:41
 *  Author: YJKIM
 */ 


#ifndef MAX11162_H_
#define MAX11162_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "exiom.h"
#include "spi.h"

#define MAX11162_CS1	portc.bit0
#define MAX11162_CS2	portc.bit1
#define MAX11162_CONV	porte.bit2

#define AD_MAX_VOLTAGE	1000	//AD MAX Voltage 55.55V
#define AD_MAX_ADC		65535	//AD MAX ADC Value 16bit

#define AD_AVR_MAX	5

#define MAX11162_DELAY	5
#define MAX11162_CHIP_MAX	2
#define MAX11162_CH_MAX	4

enum{MAX11162_CHIP1, MAX11162_CHIP2};

#define HVAD1	(((unsigned long)max11162Data[MAX11162_CHIP1]*1560)/10000)
#define HVAD2	(((unsigned long)max11162Data[MAX11162_CHIP2]*1560)/10000)

unsigned int max11162Data[MAX11162_CHIP_MAX];

void initMax11162(void);
void readMax11162(void);

#endif /* MAX11162_H_ */