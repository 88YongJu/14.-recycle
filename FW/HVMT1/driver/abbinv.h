/*
 * ABBINV.h
 *
 * Created: 2023-12-26 오후 1:31:19
 *  Author: YJKIM
 */

#ifndef ABBINV_H_
#define ABBINV_H_

#include "CAN.h"

#define ABB_INV_NUM		2

#define INDEX_ABBINV1	0
#define INDEX_ABBINV2	1

#define CANID_REC_ABB_1_0	0xCFFF001
#define CANID_REC_ABB_1_1	0xCFFF101

#define CANID_REC_ABB_2_0	0xCFFF002
#define CANID_REC_ABB_2_1	0xCFFF102

#define CANID_CMD_ABB_INV1	0xCFFF600
#define CANID_CMD_ABB_INV2	0xCFFF900

#define ABB_INV_WAIT_TIME	10

struct ABBINV
{
	//timer
	unsigned long time;
	
	//cmd
	boolean run;
	boolean fault_Reset;
	boolean reset;
	unsigned long abb_inv_check_time;
	unsigned int abb_reset_on_time;
	unsigned long abb_reset_wait_time;
	
	int RPM;
	
	//status
	unsigned int Speed;
	unsigned int Current;
	unsigned int kW;
	unsigned int Temp;
	unsigned int Mot_temp;
	unsigned int Code_error;
	};

struct ABBINV abbinv[ABB_INV_NUM];
unsigned long abb_inv1_check_time;
unsigned long abb_inv2_check_time;

void Save_abbinv1(unsigned char num);
void Save_abbinv2(unsigned char num);

void ABB_Inv_Handeler(void);
#endif /* ABBINV_H_ */