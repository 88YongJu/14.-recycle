/*
 * throttle.h
 *
 * Created: 2023-01-31 오후 2:30:50
 *  Author: YJKIM
 */ 
#ifndef THROTTLE_H_
#define THROTTLE_H_

#include "EXINT.h"
#include "CAN.h"
#include "ADC.h"
#include "TIMER.h"

#define THROTTLE_WAIT_TIME	100

#define CANID_THROTTLE	0x10
#define THROTTLE_FRONT_VALUD	250
#define THROTTLE_REAR_VALUD		100

#define DUTY_MAX			1000
#define THROTTLE_AD_MAX		900//791
#define THROTTLE_AVR_MAX	4
#define THROTTLE_ERR_MAX	10

#define THROTTLE_N	0x00
#define THROTTLE_F	0x01
#define THROTTLE_R	0x02

#define AD_THROTTLE	ADC_avr[3]

uint8 Ste_RunMode;
unsigned char Direction;
unsigned char Throttle_value;

void Init_Throttle(void);
void Get_Direction(void);
void Get_Duty(void);
void Send_Throttle(void);
void Save_Throttle(char *MSG);
void Throttle_handler(void);
#endif /* THROTTLE_H_ */