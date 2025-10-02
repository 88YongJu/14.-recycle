/*
 * checksum.c
 *
 * Created: 2023-01-19 오전 9:24:49
 *  Author: YJKIM
 */ 
#include "checksum.h"

unsigned char Checksum(unsigned char *Byte)
{
	unsigned char chk=0;
	for(unsigned char i=0;i<7;i++)
	{
		chk += Byte[i];
	}
	return chk;
}