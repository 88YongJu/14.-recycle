/*
 * exint.h
 *
 * Created: 2023-01-30 오후 4:25:44
 *  Author: YJKIM
 */ 


#ifndef EXINT_H_
#define EXINT_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "exiom.h"

enum{LOW_LEVEL, ANY_LOGIC, FALLING, RISING};

void Enable_EXINT(unsigned int num, unsigned int control);

void Disable_EXINT(unsigned int num);

unsigned int ui_Hi_time_cnt;

#endif /* EXINT_H_ */