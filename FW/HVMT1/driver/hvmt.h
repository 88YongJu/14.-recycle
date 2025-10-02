/*
 * HVMT.h
 *
 * Created: 2024-07-02 오후 2:07:46
 *  Author: yjkim88
 */ 


#ifndef HVMT_H_
#define HVMT_H_

#include "types.h"
#include "can.h"
#include "max11162.h"

#define HVMT_MAX_CNT	7

#define CANID_HVMT1_CMD1	0x60A
#define CANID_HVMT2_CMD1	0x61A
#define CANID_HVMT3_CMD1	0x62A
#define CANID_HVMT4_CMD1	0x63A
#define CANID_HVMT5_CMD1	0x64A
#define CANID_HVMT6_CMD1	0x65A
#define CANID_HVMT7_CMD1	0x66A

#define CANID_HVMT1_STE1	0x600
#define CANID_HVMT1_STE2	0x601

#define CANID_HVMT2_STE1	0x610
#define CANID_HVMT2_STE2	0x611

#define CANID_HVMT3_STE1	0x620
#define CANID_HVMT3_STE2	0x621

#define CANID_HVMT4_STE1	0x630
#define CANID_HVMT4_STE2	0x631

#define CANID_HVMT5_STE1	0x640
#define CANID_HVMT5_STE2	0x641

#define CANID_HVMT6_STE1	0x650
#define CANID_HVMT6_STE2	0x651

#define CANID_HVMT7_STE1	0x660
#define CANID_HVMT7_STE2	0x661

enum{INDEX_HVMT1, INDEX_HVMT2, INDEX_HVMT3, INDEX_HVMT4, INDEX_HVMT5, INDEX_HVMT6, INDEX_HVMT7};

#define	HVMT_NUM	INDEX_HVMT1

struct _LDC_CMD
{
	boolean LDC_run;
	unsigned int LDC_Voltage;
	unsigned int LDC_Current;
};

struct HVMT_CMD
{
	boolean chargeRelay1;
	boolean dischargeRelay1;
	boolean chargeRelay2;
	boolean dischargeRelay2;
	boolean IMU_Check;
	boolean EVCC_Emcstop;
	//
	unsigned int keyStatus;
	boolean relay1;
	boolean relay2;
	boolean relay3;
	boolean relay4;
	
	struct _LDC_CMD ldc_cmd[3];
};

struct HVMT_STE1
{
	boolean chargeRelay1;
	boolean dischargeRelay1;
	boolean Precharge_rt1;
	boolean chargeRelay2;
	boolean dischargeRelay2;
	boolean Precharge_ry2;
	unsigned char EVCC_GUN;
	unsigned int Charge_voltage;
	unsigned int Discharge_voltage;
	};

struct HVMT_STE2
{
	boolean IMU_ry;
	unsigned char DC_vol;
	unsigned int P_RES;
	unsigned int N_RES;
	};

struct HVMT_STE3
{
	boolean LDC_reaad;
	boolean LDC_run;
	boolean LDC_ig;
	boolean LDC_fault_check;
	unsigned int LDC_in_voltage;
	unsigned int LDC_out_voltage;
	unsigned int LDC_out_current;
	unsigned char temp;
};

volatile struct HVMT_CMD	hvmtcmd[HVMT_MAX_CNT];

volatile struct HVMT_STE1	hvmtste1[HVMT_MAX_CNT];
volatile struct HVMT_STE2	hvmtste2[HVMT_MAX_CNT];
volatile struct HVMT_STE3	hvmtste3[HVMT_MAX_CNT];

uint8_t HVMT_CMD1[HVMT_MAX_CNT][8];
uint8_t HVMT_CMD2[HVMT_MAX_CNT][8];
uint8_t HVMT_CMD3[HVMT_MAX_CNT][8];

volatile unsigned long time_hvmt1_cmd1;
volatile unsigned long time_hvmt1_cmd2;
volatile unsigned long time_hvmt1_cmd3;
volatile unsigned long time_hvmt2_cmd1;
volatile unsigned long time_hvmt2_cmd2;
volatile unsigned long time_hvmt2_cmd3;

void saveHvmtCommand1(uint8 index, char *data);
void Save_cmd2(uint8 index, char *data);
void Save_ste1(uint8 index, char *data);
void Save_IMU(uint8 index, char *data);
void Save_LDC(uint8 index, char *data);
void Update_CMD_HVMT(unsigned char index);

#endif /* HVMT_H_ */