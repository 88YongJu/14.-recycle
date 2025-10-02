/*
 * ldc.c
 *
 * Created: 2023-02-20 오전 10:04:22
 *  Author: YJKIM
 */ 
#include "LDC.h"

volatile LDC ldc[MAX_LDC];

void initLdc(void)
{
	ldc[INDEX_LDC1].run = 1;
	ldc[INDEX_LDC1].enable = 1;
	
	ldc[INDEX_LDC2].run = 1;
	ldc[INDEX_LDC2].enable = 1;
	
	ldc[INDEX_LDC3].run = 0;
	ldc[INDEX_LDC3].enable = 0;
}

void saveLdcStatus(unsigned char index, char *data)
{
	ldc[index].ready = (data[0] >> 0) & 0b00000001;
	ldc[index].run = (data[0] >> 1) & 0b00000001;
	ldc[index].keyOn = (data[0] >> 2) & 0b00000001;
	ldc[index].fault = (data[0] >> 3) & 0b00000001;
	ldc[index].inputVoltage = data[1];
	ldc[index].inputVoltage |= data[2] << 8;
	ldc[index].outputVoltage = data[3];
	ldc[index].outputVoltage |= data[4] << 8;
	ldc[index].outputCurrent = data[5];
	ldc[index].outputCurrent |= data[6] << 8;
	ldc[index].temperature = data[7];//-40;
}
//voltage 100mV, current 100mA
//EX>1000->100.0V
//EX>1000->100.0A

void saveLdcFaults(unsigned char index, char *data)
{
	ldc[index].faults = data[0];
	ldc[index].faults |= data[1] << 8;
}