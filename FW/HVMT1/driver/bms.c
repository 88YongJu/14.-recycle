/*
 * BMS.c
 *
 * Created: 2023-02-03 오후 3:33:53
 *  Author: YJKIM
 */ 

#include "bms.h"

volatile unsigned int ui_Bat1_runtime = 0;
volatile unsigned int ui_Bat2_runtime = 0;

volatile unsigned int runtime[MAX_BAT];

void saveRackStatus(uint8 index, char *data)
{
	bms[index].rackVoltage = data[0];
	bms[index].rackVoltage |= data[1]<<8;
	
	bms[index].rackCurrent = data[2];
	bms[index].rackCurrent |= data[3]<<8;
	
	bms[index].rackSoc = data[4];
	bms[index].rackSoc |= data[5]<<8;
	
	bms[index].rackSoh = data[6];
	bms[index].rackSoh |= data[7]<<8;
}

void saveBmsStatus(uint8 index, char *data)
{
	bms[index].run = (data[0] >> 0) & 0b00000001;
	bms[index].fault = (data[0] >> 1) & 0b00000001;
	bms[index].warning = (data[0] >> 2) & 0b00000001;
	bms[index].rackImbalance = (data[0] >> 3) & 0b00000001;

	bms[index].chargeRelay = (data[1] >> 0) & 0b00000001;
	bms[index].dischargeRelay = (data[1] >> 1) & 0b00000001;
	bms[index].cellBalance = (data[1] >> 2) & 0b00000001;
	
	bms[index].sensingCharge = (data[1] >> 3) & 0b00000001;
	bms[index].sensingDischarge = (data[1] >> 4) & 0b00000001;
	
	bms[index].warnings = data[2];
	bms[index].faults1= data[3];
	bms[index].faults2= (data[4] & 0b00011111);

	bms[index].vocFault = data[5];
	bms[index].vocFault |= data[6]<<8;
	
	if (bms[index].vocFault > 0)
	{
		bms[index].faults2 |= (1 << 5);
	}
}

void saveBmsFan(uint8 index, char *data)
{
	bms[index].fanFault = data[0];
	bms[index].fanFault |= data[1]<<8;
	
//	bms[index].fan_fault = 0b00001010;

	if (bms[index].fanFault > 0)
	{
		bms[index].faults2 |= (1 << 6);
	}
}

void saveCellTemperatere(uint8 index, char *data)
{
	bms[index].cellMaxTemperature = data[0];
	bms[index].cellMaxTemperature |= data[1]<<8;

	bms[index].cellMinTemperature = data[4];
	bms[index].cellMinTemperature |= data[5]<<8;
}

void saveCellVoltage(uint8 index, char *data)
{
	bms[index].cellMaxVoltage = data[0];
	bms[index].cellMaxVoltage |= data[1]<<8;

	bms[index].cellMinVoltage = data[4];
	bms[index].cellMinVoltage |= data[5]<<8;
}

void saveModuleTemperature(uint8 index, char *data)
{
	bms[index].moduleMaxTemperature = data[0];
	bms[index].moduleMaxTemperature |= data[1]<<8;

	bms[index].moduleMinTemperature = data[4];
	bms[index].moduleMinTemperature |= data[5]<<8;
}

void SaveModuleHumidity(uint8 index, char *data)
{
	bms[index].moduleMaxHumidity = data[0];
	bms[index].moduleMaxHumidity |= data[1]<<8;

	bms[index].moduleMinHumidity = data[4];
	bms[index].moduleMinHumidity |= data[5]<<8;
}

void sendBmsHeartbeat(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + HEARTBIT, CAN_STD, {CAN_CMD1, CAN_CMD2, bms[INDEX_BAT1].rackVoltage, bms[INDEX_BAT1].rackVoltage>>8, 0x00, 0x00, 0x00, 0x00},8};
	struct MOb sendmsg2 = {CANID_BAT2 + HEARTBIT, CAN_STD, {CAN_CMD1, CAN_CMD2, bms[INDEX_BAT2].rackVoltage, bms[INDEX_BAT2].rackVoltage>>8, 0x00, 0x00, 0x00, 0x00},8};
	
	if(index == INDEX_BAT1)	sendCan(&sendmsg1);
	else if(index == INDEX_BAT2) sendCan(&sendmsg2);
}

void Send_on_Chage(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + CHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, ON_CHAGE,}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + CHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, ON_CHAGE,}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_off_Chage(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + CHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, OFF_CHAGE,}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + CHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, OFF_CHAGE,}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_on_DisChage(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + DISCHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, ON_DISCHAGE,}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + DISCHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, ON_DISCHAGE,}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_off_DisChage(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + DISCHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, OFF_DISCHAGE,}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + DISCHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, OFF_DISCHAGE,}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_on_PreChage(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + PRECHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, ON_PRECHAGE, 0x00, 0x00, 0x00, 0x00}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + PRECHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, ON_PRECHAGE, 0x00, 0x00, 0x00, 0x00}, 8};
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_off_PreChage(uint8 index)
{
	struct MOb sendmsg1 = {CANID_BAT1 + PRECHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, OFF_PRECHAGE, 0x00, 0x00, 0x00, 0x00}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + PRECHARGE, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, OFF_PRECHAGE, 0x00, 0x00, 0x00, 0x00}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_on_Emc(uint8 index, unsigned char status)
{
	struct MOb sendmsg1 = {CANID_BAT1 + EMERGENCY, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, SET_EMERYGENCY, 0x00, 0x00, 0x00, 0x00}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + EMERGENCY, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, SET_EMERYGENCY, 0x00, 0x00, 0x00, 0x00}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}

void Send_off_Emc(uint8 index, unsigned char status)
{
	struct MOb sendmsg1 = {CANID_BAT1 + EMERGENCY, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, RESET_EMERYGENCY, 0x00, 0x00, 0x00, 0x00}, 8};
	struct MOb sendmsg2 = {CANID_BAT2 + EMERGENCY, CAN_STD, {CAN_CMD1, CAN_CMD2, CAN_CMD3, RESET_EMERYGENCY, 0x00, 0x00, 0x00, 0x00}, 8};
	
	if(index == 0)	sendCan(&sendmsg1);
	else if(index == 1)	sendCan(&sendmsg2);
}