/*
 * MAX11162.c
 *
 * Created: 2023-03-30 오후 2:13:29
 *  Author: YJKIM
 */ 

#include "MAX11162.h"

unsigned int max11162Data[MAX11162_CHIP_MAX];

void initMax11162(void)
{
	SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(1<<CPHA)|(0<<SPR1)|(0<<SPR0);
	SPSR = (0<<SPIF)|(0<<SPI2X);
	MAX11162_CS1 = 1;
	MAX11162_CS2 = 1;
	MAX11162_CONV = 0;
	MAX11162_CONV = 1;
}

void readMax11162(void)
{
	MAX11162_CS1 = 0;
	max11162Data[MAX11162_CHIP1] = ((writeSpi(0x00)<<8)&0xFF00);
	max11162Data[MAX11162_CHIP1] |= ((writeSpi(0x00))&0xFF);
	MAX11162_CS1 = 1;
	
	MAX11162_CS2 = 0;
	max11162Data[MAX11162_CHIP2] = ((writeSpi(0x00)<<8)&0xFF00);
	max11162Data[MAX11162_CHIP2] |= ((writeSpi(0x00))&0xFF);
	MAX11162_CS2 = 1;
	
	MAX11162_CONV = 0;
	MAX11162_CONV = 1;
}