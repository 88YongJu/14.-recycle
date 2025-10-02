/*
 * Modtemp.h
 *
 * Created: 2023-03-13 오전 10:04:17
 *  Author: YJKIM
 */ 


#ifndef MODTEMP_H_
#define MODTEMP_H_

#include "can.h"

#define CANID_TOP_TEMP		0x601
#define CANID_BOTTOM_TEMP	0x602

struct MOb;

struct TEMPS
{
	unsigned char TL_Ste;
	unsigned char TR_Ste;
	unsigned char BL_Ste;
	unsigned char BR_Ste;
	
	unsigned long TL_Temp;
	unsigned long TR_Temp;
	unsigned long BL_Temp;
	unsigned long BR_Temp;
};

struct TEMPS temps;

void Save_Top_Temp(struct MOb *msg);
void Save_Bottom_Temp(struct MOb *msg);

unsigned long  Get_Temp_TL(void);
unsigned long  Get_Temp_TR(void);
unsigned long  Get_Temp_BL(void);
unsigned long  Get_Temp_BR(void);

#endif /* MODTEMP_H_ */