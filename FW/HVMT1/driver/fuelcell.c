/*
 * fuelcell.c
 *
 * Created: 2023-03-15 오전 9:34:47
 *  Author: YJKIM
 */ 

#include "fuelcell.h"

volatile unsigned int fc_dis = 60000;

volatile unsigned char H2_LP;

volatile unsigned char fc_rdy[1];
volatile unsigned char fc_su[1];
volatile unsigned char fc_ldprm[1];
volatile unsigned char fc_sd[1];
volatile unsigned char fc_flt[1];

volatile unsigned char fc_FcNetMxPwrLim[2];
volatile unsigned char fc_FcNetPwr[2];

volatile unsigned char fc_StckDiagReq[1];
volatile unsigned char fc_GdsCspRunReq[1];

volatile unsigned char fc_v[2];
volatile unsigned char fc_a[2];
volatile unsigned char fc_Tr[2];
volatile unsigned char fc_Tp[2];
volatile unsigned char fc_Tt[2];
volatile unsigned char fc_Af[2];
volatile unsigned char fc_Ap[2];
volatile unsigned char fc_At[2];
volatile unsigned char fc_Cf[2];
volatile unsigned char fc_Cp[2];
volatile unsigned char fc_Ct[2];
volatile unsigned char fc_Sf[2];
volatile unsigned char fc_s[1];

void Send_H2MidPrs(unsigned char Press)
{
	struct MOb msg = {CANID_VCU_FCU_C1,0,CAN_EXT,8,{0x00,0x00,0x00,Press,0x00,0x00,0x00,0x00}};
	can_tx(4,&msg,0);
}

void Send_Fixedvalue_1(void)
{
	struct MOb msg = {CANID_VCU_None1,0,CAN_EXT,8,{0b00000001,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	can_tx(5, &msg, 0);
}

void Send_Fixedvalue_2(void)
{
	struct MOb msg = {CANID_VCU_None2,0,CAN_EXT,8,{0x00,0x00,0x00,0x00,0b00000100,0x00,0x00,0x00}};
	can_tx(6,&msg,0);
}

void Send_Fixedvalue_3(void)
{
	struct MOb msg = {CANID_VCU_None3,0,CAN_EXT,8,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	can_tx(7,&msg,0);
}

void Send_Cmd_Run(unsigned char val)
{
	if(val & CMD_RUN)
	{
		struct MOb msg = {0x0CFF27CC,0,CAN_EXT,8,{CMD_RUN,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
		can_tx(8,&msg,0);
	}
	else
	{
		struct MOb msg = {0x0CFF27CC,0,CAN_EXT,8,{CMD_STOP,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
		can_tx(8,&msg,0);
	}
}

void Send_PwrReq(unsigned int val)
{
	struct MOb msg = {CANID_VCU_FCU2,0,CAN_EXT,8,{0b00000001 | (0b11100000 & ((val)<<5)),(0b11111111 & ((val)>>3)),(0b00000001 & ((val)>>11)),0x00,0x00,0x00,0x00,0x00}};
	can_tx(9,&msg,0);
}

void Send_Temp(unsigned int val)
{
	struct MOb msg = {CANID_VCU_FCU2,0,CAN_EXT,8,{0b00000001 | (0b11100000 & ((val)<<5)),(0b11111111 & ((val)>>3)),(0b00000001 & ((val)>>11)),0x00,0x00,0x00,0x00,0x00}};
	can_tx(9,&msg,0);
}

void Send_SOC(unsigned int val)
{
	VCU_FCU3.data[3] = (0b11110000 & ((val)<<4));
	VCU_FCU3.data[4] = (0b00001111 & ((val)>>4));
}

void Send_CUR_LIM(unsigned int val)
{
	VCU_FCU3.data[5] = (0b11110000 & ((val)<<4));
	VCU_FCU3.data[6] = (0b00001111 & ((val)>>4));
}

void Send_GDS(unsigned int val)
{
	if(val)	VCU_FCU3.data[7] = (0b11110000 & (((val)<<6) | ((val)<<4)));
	else VCU_FCU3.data[7] = 0x00;
}

void Send_PW_Lim(unsigned int val)
{
	struct MOb msg = {CANID_VCU_FCU4,0,CAN_EXT,8,{0x00,(0b11100000 & ((val)<<5)),(0b11111111 & ((val)>>3)),(0b00000001 & ((val)>>11)) | (0b11111110 & ((val)<<1)),(0b00011111 & ((val)>>7)),0x00,0x00,0x00}};
	can_tx(3,&msg,0);
}