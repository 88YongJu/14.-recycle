/*
 * ABBCON.c
 *
 * Created: 2023-12-26 오후 2:46:01
 *  Author: YJKIM
 */ 

#include "ABBCON.h"

struct ABBCON abbcon[CONVERTOR_MAXNUM];
unsigned long conv_on_time = 0, pcbms_on_time = 0;

void ABB_Con_Handeler(void)
{
	static unsigned long Con_time = 0;
	
	conv_on_time++;
	if(++Con_time >= CON_WAIT_TIME)
	{
		Con_time = 0;
		Send_abbcnv(INDEX_ABBCON1);
	}
}

void Save_abbcnv1(unsigned char index, unsigned char *msg)
{
	unsigned char data = 0;
	data = *msg;
	data = *msg;
	data = *msg;
	data = *msg;
	data = *msg;
	data = *msg;
	abbcon[index].Current = *msg;
	abbcon[index].Current |= *msg << 8;
}

void Save_abbcnv2(unsigned char num, unsigned char *msg)
{
	abbcon[num].kW = *msg;
	abbcon[num].kW |= *msg << 8;
	abbcon[num].Temp = *msg;
	abbcon[num].Temp |= *msg << 8;
	abbcon[num].Indecer_Temp = *msg;
	abbcon[num].Indecer_Temp |= *msg << 8;
	abbcon[num].Code_error = *msg;
	abbcon[num].Code_error |= *msg << 8;
}

void Send_abbcnv(unsigned index)
{
	struct MOb msg_send = { CANID_CMD_ABB_CON1, 0, CAN_EXT, 8, {abbcon[index].run | abbcon[index].faultreset << 1,
																0x00, CON_VOL, CON_VOL >> 8, 0x00, 0x00, hvb[INDEX_BAT2].RACK_Vol, hvb[INDEX_BAT2].RACK_Vol >> 8}};
	can_tx(10,&msg_send,0);
}