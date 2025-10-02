/*
 * cb350m.c
 *
 * Created: 2025-09-10 오후 3:32:27
 *  Author: YJKIM
 */ 

#include "cb350m.h"

struct CB350M cb350m[4];

void saveCb350m(unsigned int index, char *data)
{
	cb350m[index].unit = (data[1] >> UNITFLAG) & 0b00000001;
	cb350m[index].current = (long)data[2] << 24;
	cb350m[index].current |= (long)data[3] << 16;
	cb350m[index].current |= (long)data[4] << 8;
	cb350m[index].current |= (long)data[5];
}