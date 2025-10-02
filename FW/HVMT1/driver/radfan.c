/*
 * RADFAN.c
 *
 * Created: 2023-03-15 오전 10:58:05
 *  Author: YJKIM
 */ 

#include "RADFAN.h"

void Run_LH_FAN(unsigned int rpm)
{
	struct MOb msg = {CANID_TRA_LH,0,CAN_EXT,8,{0x00,0x01,(rpm & 0xff),(rpm>>8),0x00,0x00,0x00,0x00}};
	can_tx(8, &msg, 0);
}

void Run_RH_FAN(unsigned int rpm)
{
	struct MOb msg = {CANID_TRA_RH,0,CAN_EXT,8,{0x00,0x01,(rpm & 0xff),(rpm>>8),0x00,0x00,0x00,0x00}};
	can_tx(8, &msg, 0);
}

void Stop_LH_FAN(void)
{
	struct MOb msg = {CANID_TRA_LH,0,CAN_EXT,8,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	can_tx(8, &msg, 0);
}

void Stop_RH_FAN(void)
{
	struct MOb msg = {CANID_TRA_RH,0,CAN_EXT,8,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	can_tx(8, &msg, 0);
}