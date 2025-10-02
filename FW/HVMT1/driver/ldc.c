/*
 * ldc.c
 *
 * Created: 2023-02-20 오전 10:04:22
 *  Author: YJKIM
 */ 
#include "LDC.h"

volatile struct LDC ldc[MAX_LDC];

void init_LDC(void)
{
	ldc[INDEX_LDC1].Run = 1;
	ldc[INDEX_LDC1].Enable = 1;
	
	ldc[INDEX_LDC2].Run = 1;
	ldc[INDEX_LDC2].Enable = 1;
	
	ldc[INDEX_LDC3].Run = 0;
	ldc[INDEX_LDC3].Enable = 0;
}

void Save_LDC_Status(unsigned char index, char *data)
{

	ldc[index].Rdy_fb = (data[0] >> 0) & 0b00000001;
	ldc[index].Run_fb = (data[0] >> 1) & 0b00000001;
	ldc[index].On_chk = (data[0] >> 2) & 0b00000001;
	ldc[index].Fault = (data[0] >> 3) & 0b00000001;
	ldc[index].IN_Vol = data[1];
	ldc[index].IN_Vol |= data[2] << 8;
	ldc[index].OUT_Vol = data[3];
	ldc[index].OUT_Vol |= data[4] << 8;
	ldc[index].Cur = data[5];
	ldc[index].Cur |= data[6] << 8;
	ldc[index].temp = data[7];//-40;
}
//voltage 100mV, current 100mA
//EX>1000->100.0V
//EX>1000->100.0A

void Save_LDC_Faults(unsigned char index, char *data)
{
	ldc[index].Faults = data[0];
	ldc[index].Faults |= data[1] << 8;
}