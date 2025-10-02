/*
 * invter.c
 *
 * Created: 2023-02-09 오전 10:10:26
 *  Author: YJKIM
 */ 
#include "DANFOSSINV.h"

volatile struct DANFOSSINV danfossinv[DANFOSSINV_MAXNUM];


boolean Ste_Inv = FALSE;
unsigned int ui_Danfoss_setsys_sendtime = 0;
unsigned int ui_Danfoss_setmot1_sendtime = 0;
unsigned int ui_Danfoss_setmot2_sendtime = 0;

volatile uint8_t CMD_SYS1[BUFFER_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t CMD_MOT1[BUFFER_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t CMD_MOT2[BUFFER_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void Save_danfossinv1(char *data)
{
	danfossinv[0].inv_run = (data[0] >> 0) & 0b00000001;
	danfossinv[0].inv_fault = (data[0] >> 2) & 0b00000001;
	danfossinv[0].inv_warn = (data[0] >> 4) & 0b00000001;
}

void Save_danfossinv2(char *data)
{
	danfossinv[0].temp_j = data[0];
	danfossinv[0].temp_max = data[1];
	danfossinv[0].temp_min = data[2];
}

void Save_danfossinv3(char *data)
{
	if(data[0])	DebugPrint("I3-0\r\n");
	if(data[1])	DebugPrint("I3-1\r\n");
	if(data[2])	DebugPrint("I3-2\r\n");
	if(data[3])	DebugPrint("I3-3\r\n");
	if(data[4])	DebugPrint("I3-4\r\n");
	if(data[5])	DebugPrint("I3-5\r\n");
	if(data[6])	DebugPrint("I3-6\r\n");
	if(data[7])	DebugPrint("I3-7\r\n");
	danfossinv[0].faults1 = data[0];
	danfossinv[0].faults1 |= ((data[1] & 0b11111000) << 5);
	danfossinv[0].faults1 |= ((data[2] & 0b00000111) << 13);
	danfossinv[0].faults2 = ((data[2] & 0b11111000) >> 3);
	danfossinv[0].faults2 |= ((data[3] & 0b11111111) << 5);
	danfossinv[0].faults2 |= ((data[4] & 0b00000100) << 11);
	danfossinv[0].faults2 |= ((data[4] & 0b00110000) << 10);
	danfossinv[0].faults3 = ((data[4] & 0b11000000) >> 6);
	if ((data[5] & 0b00111111) > 0)
	{
		danfossinv[0].faults3 |= 0b00000010;
	}
	danfossinv[0].faults3 |= ((data[5] & 0b01000000) >> 4);
	danfossinv[0].faults3 |= ((data[6] & 0b00001000) >> 0);
	danfossinv[0].faults3 |= ((data[7] & 0b11111110) << 3);
}

void Save_danfossinv4(char *data)
{
	if(data[0])	DebugPrint("I4-0\r\n");
	if(data[2])	DebugPrint("I4-2\r\n");
	if(data[3])	DebugPrint("I4-3\r\n");
	danfossinv[0].faults3 |= ((data[0] & 0b00000011) << 11);
	danfossinv[0].faults3 |= ((data[2] & 0b11000000) << 7);
	danfossinv[0].faults3 |= ((data[3] & 0b00000010) << 14);
}

void Save_danfossinv5(char *data)
{
	danfossinv[0].speed = data[0];
	danfossinv[0].speed |= data[1] << 8;

	danfossinv[0].volt = data[4];
	danfossinv[0].volt |= data[5] << 8;
	danfossinv[0].pwr = data[6];
	danfossinv[0].pwr |= data[7] << 8;
}

void Save_danfossinv6(char *data)
{
	danfossinv[0].amp = data[2];
	danfossinv[0].amp |= data[3] << 8;
}

/*uint8 run_stop, uint8 crtmode, uint8 val*/
void Control_Inverter(uint8 i, uint8 ctrmode, unsigned int val)
{	
	switch(i)
	{
		case INV_STOP:
			CMD_SYS1[0] = INV_DISABLE;
			CMD_MOT1[0]	= STOP;
			CMD_MOT2[0]	= 0x00;
			CMD_MOT2[1]	= 0x00;
			CMD_MOT2[2]	= 0x00;
			CMD_MOT2[3]	= 0x00;
			CMD_MOT2[4]	= 0x00;
			CMD_MOT2[5]	= 0x00;
			CMD_MOT2[6]	= 0x00;
			CMD_MOT2[7] = 0x00;
		break;
		case INV_RUN:
			CMD_SYS1[0] = INV_ENABLE;
			CMD_MOT1[0] = (MOT_RUN | ctrmode);
			switch(ctrmode)
			{
				case MODE_MOTOR_SPEED :
					CMD_MOT2[0] = val;
					CMD_MOT2[1] = val>>8;
					CMD_MOT2[2] = 0x00;
					CMD_MOT2[3] = 0x00;
					CMD_MOT2[4] = 0x00;
					CMD_MOT2[5] = 0x00;
					CMD_MOT2[6] = 0x00;
					CMD_MOT2[7] = 0x00;
				break;
				case MODE_MOTOR_TORQUE :
					CMD_MOT2[0] = 0x00;
					CMD_MOT2[1] = 0x00;
					CMD_MOT2[2] = val;
					CMD_MOT2[3] = val>>8;
					CMD_MOT2[4] = 0x00;
					CMD_MOT2[5] = 0x00;
					CMD_MOT2[6] = 0x00;
					CMD_MOT2[7] = 0x00;
				break;
				case MODE_DCLINK_VOLTAGE :
					CMD_MOT2[0] = 0x00;
					CMD_MOT2[1] = 0x00;
					CMD_MOT2[2] = 0x00;
					CMD_MOT2[3] = 0x00;
					CMD_MOT2[4] = val;
					CMD_MOT2[5] = val>>8;
					CMD_MOT2[6] = 0x00;
					CMD_MOT2[7] = 0x00;
				break;
				case MODE_MOTOR_POWER :
					CMD_MOT2[0] = 0x00;
					CMD_MOT2[1] = 0x00;
					CMD_MOT2[2] = 0x00;
					CMD_MOT2[3] = 0x00;
					CMD_MOT2[4] = 0x00;
					CMD_MOT2[5] = 0x00;
					CMD_MOT2[6] = val;
					CMD_MOT2[7] = val>>8;
				break;
				default :
				
				break;
			}
			break;
		case INV_Faultreset :
			CMD_SYS1[0] = INV_Faultreset;
			CMD_MOT1[0]	= STOP;
			CMD_MOT2[0] = 0x00;
			CMD_MOT2[1] = 0x00;
			CMD_MOT2[2] = 0x00;
			CMD_MOT2[3] = 0x00;
			CMD_MOT2[4] = 0x00;
			CMD_MOT2[5] = 0x00;
			CMD_MOT2[6] = 0x00;
			CMD_MOT2[7] = 0x00;
		break;
	}
}