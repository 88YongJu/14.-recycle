/*
 * PCBMS.c
 *
 * Created: 2024-01-09 오후 5:28:31
 *  Author: YJKIM
 */ 

#include "PCBMS.h"

struct PCBMS pcbms;

void Save_pcbms1(unsigned char *msg)
{
	unsigned char data = 0;
	data = *msg;
	data = *msg;
	pcbms.Run_status = *msg;
}

void Save_pcbms2(unsigned char *msg)
{
	unsigned char data = 0;
	pcbms.Voltage = *msg;
	pcbms.Voltage |= *msg << 8;
	pcbms.Current = *msg;
	data = *msg;
	pcbms.Fault_status = *msg;
}

void Save_pcbms3(unsigned char *msg)
{
	pcbms.SOC = *msg;
	pcbms.SOC |= *msg << 8;
}

void Send_pcbms(void)
{
	struct MOb sendmsg1 = {CANID_PCBMS_CMD1, 0, CAN_STD, 8, {pcbms.Run, 0x00, 0x00, 0x00, PC_BMS_SEND_CURRENT}};
	struct MOb sendmsg2 = {CANID_PCBMS_CMD2, 0, CAN_STD, 8, {pcbms.fault_reset, }};
	can_tx(5,&sendmsg1,0);
	can_tx(5,&sendmsg2,0);
};

void PC_BMS_Handeler(void)
{
	static unsigned int pc_bms_time = 0;
	if(++pc_bms_time >= PC_BMS_SEND_TIME)
	{
		pc_bms_time = 0;
		Send_pcbms();
	}
}