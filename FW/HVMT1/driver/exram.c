/*
 * exram.c
 *
 * Created: 2023-02-01 오전 11:27:49
 *  Author: YJKIM
 */ 


#include "EXRAM.h"
//enable, Memory Upper Address, Cycle, PoerC Free Set

void Init_ExSRAM(int i_Enable, ...)
{
	va_list argptr;
	va_start(argptr, i_Enable);
	//cli();
	int ui_Address = va_arg(argptr, int);
	int ui_UpperCycle = va_arg(argptr, int);
	int ui_LowerCycle = va_arg(argptr, int);
	int ui_PortCFree = va_arg(argptr, int);
	
	XMCRA = (i_Enable << SRE | ui_Address << SRL0 | ui_UpperCycle << SRW10 | ui_LowerCycle << SRW00);	//External Memory Control Register A
	XMCRB =	(1<<XMBK | ui_PortCFree << XMM0);	//External Memory Control Register B
	
	va_end(argptr);
	//sei();
}

