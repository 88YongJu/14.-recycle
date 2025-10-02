/*
 * ldc.h
 *
 * Created: 2023-02-20 오전 10:04:12
 *  Author: YJKIM
 */ 


#ifndef LDC_H_
#define LDC_H_

#include "types.h"
#include "CAN.h"
#include "TIMER.h"

#define MAX_LDC	3

#define CANID_LDC	0x18000000

#define LDC_0_16	0x000000
#define LDC_16_32	0x010000
#define LDC_32_52	0x020000
#define LDC_52_62	0x030000
#define LDC_62_78	0x040000
#define LDC_78_100	0x050000

#define CANID_CMD	0xF5E5
#define CNAID_RECIVE	0xE5F5

#define CANID_CMD_LDC_12	CANID_LDC + LDC_0_16 + CANID_CMD
#define CANID_CMD_LDC_24	CANID_LDC + LDC_16_32 + CANID_CMD
#define CANID_CMD_LDC_48	CANID_LDC + LDC_32_52 + CANID_CMD

#define CANID_REC_LDC_12	CANID_LDC + LDC_0_16 + CNAID_RECIVE
#define CANID_REC_LDC_24	CANID_LDC + LDC_16_32 + CNAID_RECIVE
#define CANID_REC_LDC_48	CANID_LDC + LDC_32_52 + CNAID_RECIVE

#define LDC_DISABLE	0x00
#define LDC_ENABLE	0x01

#define LDC12_CHAGE_VOL	146	//*0.1V
#define LDC12_CHAGE_CUR	1100	//*0.1A

#define LDC24_CHAGE_VOL	292	//*0.1V
#define LDC24_CHAGE_CUR	400	//*0.1A

#define LDC48_CHAGE_VOL	500	//*0.1V
#define LDC48_CHAGE_CUR	1000//1250	//*0.1A

#define LDC12_ON_VOL	12	//0.1V
#define LDC24_ON_VOL	24	//0.1V

#define LDC_CUT_CUR		150	//0.1A
//15A	이하시차단

#define	LDC_WAIT_TIME		20
#define LDC_CHECK_WAIT_TIME	1000

enum {
	INDEX_LDC1, INDEX_LDC2, INDEX_LDC3
};

struct LDC
{
	//ldc timer
	unsigned int timer;
	//ldc control flag
	boolean Run;	//CMD 전송여부
	boolean Enable;	//LDC 동작여부

	//ldc status flag
	boolean Rdy_fb;
	boolean Run_fb;
	boolean On_chk;
	boolean Fault;
	unsigned int IN_Vol;
	unsigned int OUT_Vol;
	unsigned int Cur;
	uint8 temp;

	unsigned int Faults;
};

volatile struct LDC ldc[MAX_LDC];

void init_LDC();
void Save_LDC_Status(unsigned char index, char *data);
void Save_LDC_Faults(unsigned char index, char *data);
#endif /* LDC_H_ */