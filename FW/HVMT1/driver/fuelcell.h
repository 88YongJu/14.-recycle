/*
 * fuelcell.h
 *
 * Created: 2023-03-15 오전 9:34:56
 *  Author: YJKIM
 */ 


#ifndef FUELCELL_H_
#define FUELCELL_H_

#include "can.h"

#define CANID_HMU			0x0311

/*Recive FCU ID*/
#define CANID_FCU1_3		0x0CFF10C1
#define CANID_FCU2_4		0x0CFF21C2
#define CANID_FCU2_5		0x0CFF22C2
#define CANID_FCU2_9		0x0CFF26C2

/*Send FCU ID*/
#define CANID_VCU_None1		0x18FEF1EF
#define CANID_VCU_None2		0x18FFF0F4
#define CANID_VCU_None3		0x18FFF1F4
#define CANID_VCU_FCU2		0x0CFF27CC
#define CANID_VCU_FCU3		0x18FF28CC
#define CANID_VCU_FCU4		0x18FF17CC
#define CANID_VCU_FCU_C1	0x18FF29CC
#define CANID_FCU_HMU		0x51A

#define MAX_FUELCELL	1

#define CMD_RUN			0x01
#define CMD_STOP		0x00
#define HS_SOC			60
#define HV_CUR_LIMIT	50
#define TEMP			55 //-40
#define PW_LIMIT		1000
//1bar= 100kpa 15bar=1500 fact20 = 75
#define PRESS			75 //

#define BIT_SU	0
#define BIT_FCUREAD	1
#define BIT_FCLPER	2
#define BIT_FCFAULT	3
#define BIT_ISG		4

struct MOb VCU_FCU2;
struct MOb VCU_FCU3;

struct FC
{
	u_int RACK_Vol;
	int RACK_Cur;
	u_int RACK_SOC;
	u_int RACK_SOH;
	
	u_int CELL_MAX_Vol;
	u_int CELL_MIN_Vol;
	
	int CELL_MAX_Temp;
	u_int CELL_MAX_Temp_pos;
	int CELL_MIN_Temp;
	u_int CELL_MIN_Temp_pos;
	
	u_int MOD_MAX_HUM;
	u_int MOD_MAX_HUM_pos;
	u_int MOD_MIN_HUM;
	u_int MOD_MIN_HUM_pos;

	uint8 RACK_status;
	u_int CHA_status;
};

struct MOb FC[MAX_FUELCELL];

volatile unsigned int fc_dis;

volatile unsigned char H2_LP;

volatile unsigned char fc_su[1];
volatile unsigned char fc_rdy[1];
volatile unsigned char fc_ldprm[1];
volatile unsigned char fc_flt[1];
volatile unsigned char fc_sd[1];

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

void Send_H2MidPrs(unsigned char Press);
void Send_Fixedvalue_1(void);
void Send_Fixedvalue_2(void);
void Send_Fixedvalue_3(void);

void Send_Cmd_Run(unsigned char val);
void Send_PwrReq(unsigned int val);

void Send_Temp(unsigned int val);
void Send_SOC(unsigned int val);
void Send_CUR_LIM(unsigned int val);
void Send_GDS(unsigned int val);
void Send_PW_Lim(unsigned int val);
#endif /* FUELCELL_H_ */