/*
 * ABBINV.c
 *
 * Created: 2023-12-26 오후 1:31:33
 *  Author: YJKIM
 */ 

#include "ABBINV.h"

struct ABBINV abbinv[ABB_INV_NUM];

void Save_abbinv1(unsigned char num)
{
	unsigned char data = 0;
	data = CANMSG;
	data = CANMSG;
	abbinv[num].Speed = CANMSG;
	abbinv[num].Speed |= CANMSG << 8;
	data = CANMSG;
	data = CANMSG;
	abbinv[num].Current = CANMSG;
	abbinv[num].Current |= CANMSG << 8;
}

void Save_abbinv2(unsigned char num)
{
	unsigned char data = 0;
	abbinv[num].kW = CANMSG;
	abbinv[num].kW |= CANMSG << 8;
	abbinv[num].Temp = CANMSG;
	abbinv[num].Temp |= CANMSG << 8;
	abbinv[num].Mot_temp = CANMSG;
	abbinv[num].Mot_temp |= CANMSG << 8;
	abbinv[num].Code_error = CANMSG;
	abbinv[num].Code_error = CANMSG << 8;
}

void ABB_Inv_Handeler(void)
{
	/*
	if(abbinv[INDEX_ABBINV1].reset)
	{
		if(++abbinv[INDEX_ABBINV1].abb_reset_wait_time >= ABB_RESET_WAIT_TIME)
		{
			abbinv[INDEX_ABBINV1].abb_reset_wait_time = 0;
			abbinv[INDEX_ABBINV1].reset = 0;
		}
	}
	else
	{
		if(++abbinv[INDEX_ABBINV1].abb_inv_check_time >= ABB_CHECK_WAIT_TIME)
		{
			abbinv[INDEX_ABBINV1].abb_inv_check_time = ABB_CHECK_WAIT_TIME;
			if(++abbinv[INDEX_ABBINV1].abb_reset_on_time >= ABB_RESET_ON_WAIT_TIME)
			{
				RY_INVTOR1 = 0;
				abbinv[INDEX_ABBINV1].abb_reset_on_time = 0;
				abbinv[INDEX_ABBINV1].reset = 1;
			}
			else
			{
				RY_INVTOR1 = 1;
			}
		}
	}
	
	if(abbinv[INDEX_ABBINV2].reset)
	{
		if(++abbinv[INDEX_ABBINV2].abb_reset_wait_time >= ABB_RESET_WAIT_TIME)
		{
			abbinv[INDEX_ABBINV2].abb_reset_wait_time = 0;
			abbinv[INDEX_ABBINV2].reset = 0;
		}
	}
	else
	{
		if(++abbinv[INDEX_ABBINV2].abb_inv_check_time >= ABB_CHECK_WAIT_TIME)
		{
			abbinv[INDEX_ABBINV2].abb_inv_check_time = ABB_CHECK_WAIT_TIME;
			if(++abbinv[INDEX_ABBINV2].abb_reset_on_time >= ABB_RESET_ON_WAIT_TIME)
			{
				RY_INVTOR2 = 0;
				abbinv[INDEX_ABBINV2].abb_reset_on_time = 0;
				abbinv[INDEX_ABBINV2].reset = 1;
			}
			else
			{
				RY_INVTOR2 = 1;
			}
		}
	}
	*/
	if(++abbinv[INDEX_ABBINV1].time >= ABB_INV_WAIT_TIME)
	{
		struct MOb msg_send = { CANID_CMD_ABB_INV1, 0, CAN_EXT, 8, {abbinv[INDEX_ABBINV1].run | abbinv[INDEX_ABBINV1].fault_Reset << 1,
		0x00, abbinv[INDEX_ABBINV1].RPM, abbinv[INDEX_ABBINV1].RPM >> 8, 0x00, 0x00, 0x00, 0x00}};
		can_tx(10,&msg_send,0);
	}
	if(++abbinv[INDEX_ABBINV2].time >= ABB_INV_WAIT_TIME)
	{
		struct MOb msg_send = { CANID_CMD_ABB_INV2, 0, CAN_EXT, 8, {abbinv[INDEX_ABBINV2].run | abbinv[INDEX_ABBINV2].fault_Reset << 1,
		0x00, abbinv[INDEX_ABBINV2].RPM, abbinv[INDEX_ABBINV2].RPM >> 8, 0x00, 0x00, 0x00, 0x00}};
		can_tx(10,&msg_send,0);
	}
}