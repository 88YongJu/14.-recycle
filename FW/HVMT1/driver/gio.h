#ifndef GIO_H_
#define GIO_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "exiom.h"

#define RY_1		porta.bit0
#define RY_2		porta.bit1
#define RY_3		porta.bit2
#define RY_4		porta.bit3

void initPort(void);

#endif