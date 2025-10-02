/*
 * exram.h
 *
 * Created: 2023-02-01 오전 11:27:38
 *  Author: YJKIM
 */ 


#ifndef EXRAM_H_
#define EXRAM_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

enum {DISABLE, ENABLE};

//Upper Start Address
enum {UP_N_A, UP_2000, UP_4000, UP_6000,
	UP_8000, UP_A000, UP_C000, UP_E000};

enum {NO_WAIT, ONE_CYCLE_LOAD, TWO_CYCLE_LOAD, TWO_CYCLE_LOAD_WAIT_ONE_CYCLE};
//Free PortC strat to High pin
enum {NOT, C7, C6, C5, C4, C3, C2, ALL};
	
/*enable, upperaddress, uppercycle, lowercycle, cportfree*/
void Init_ExSRAM(int i_Enable, ...);

#endif /* EXRAM_H_ */