/*
 * Modtemp.c
 *
 * Created: 2023-03-13 오전 10:04:02
 *  Author: YJKIM
 */ 

#include "Modtemp.h"

struct TEMPS temps;

void Save_Top_Temp(struct MOb *msg)
{
	temps.TL_Ste = msg->data[2] & 0x02;
	temps.TL_Temp = msg->data[2]>>2;
	temps.TL_Temp |= msg->data[3]<<6;
	temps.TL_Temp |= msg->data[4]<<14;
	
	temps.TR_Ste = msg->data[5] & 0x02;
	temps.TR_Temp = msg->data[5]>>2;
	temps.TR_Temp |= msg->data[6]<<6;
	temps.TR_Temp |= msg->data[7]<<14;
}

void Save_Bottom_Temp(struct MOb *msg)
{
	temps.BL_Ste = msg->data[2] & 0x02;
	temps.BL_Temp = msg->data[2]>>2;
	temps.BL_Temp |= msg->data[3]<<6;
	temps.BL_Temp |= msg->data[4]<<14;
	
	temps.BR_Ste = msg->data[5] & 0x02;
	temps.BR_Temp = msg->data[5]>>2;
	temps.BR_Temp |= msg->data[6]<<6;
	temps.BR_Temp |= msg->data[7]<<14;
}

unsigned long Get_Temp_TL(void)
{
	return ((temps.TL_Temp*7813)-16384);
}

unsigned long  Get_Temp_TR(void)
{
	return ((temps.TR_Temp*7813)-16384);
}

unsigned long  Get_Temp_BL(void)
{
	return ((temps.BL_Temp*7813)-16384);
}

unsigned long  Get_Temp_BR(void)
{
	return ((temps.BR_Temp*7813)-16384);
}
/*
	if(!temps.TL_Ste)	DebugPrint("TL_Temp : %lu\r", (((temps.TL_Temp*0.007813)-16384)));
	if(!temps.TR_Ste)	DebugPrint("TR_Temp : %lu\r", (((temps.TR_Temp*0.007813)-16384)));
		
	if(!temps.BL_Ste)	DebugPrint("BL_Temp : %lu\r", (((temps.BL_Temp*0.007813)-16384)));
	if(!temps.BR_Ste)	DebugPrint("BR_Temp : %lu\r", (((temps.BR_Temp*0.007813)-16384)));
*/