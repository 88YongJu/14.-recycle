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
#include "ldc.h"
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
	
	LDC ldc[3];
};

struct HVMT_STE1
{
	boolean chargeRelay1;
	boolean dischargeRelay1;
	boolean prechargeRelay1;
	boolean chargeRelay2;
	boolean dischargeRelay2;
	boolean prechargeRelay2;
	unsigned char evccGunStatus;
	unsigned int ad1;
	unsigned int ad2;
	};

struct HVMT_STE2
{
	boolean imuRun;
	unsigned char imuDcVoltage;
	unsigned int imuPositiveGroundRegistance;
	unsigned int imuNegativeGroundRegistance;
	};

struct HVMT_STE3
{
	LDC ldc;
};

extern volatile struct MOb hvmt1Command1;

extern volatile struct HVMT_CMD		hvmtCommand[HVMT_MAX_CNT];
extern volatile struct HVMT_STE1	hvmtste1[HVMT_MAX_CNT];
extern volatile struct HVMT_STE2	hvmtste2[HVMT_MAX_CNT];
extern volatile struct HVMT_STE3	hvmtste3[HVMT_MAX_CNT];

extern volatile unsigned int hvmt1Command1SendTime;
extern volatile unsigned int hvmt1Command2SendTime;
extern volatile unsigned int hvmt1Command3SendTime;
extern volatile unsigned int hvmt2Command1SendTime;
extern volatile unsigned int hvmt2Command2SendTime;
extern volatile unsigned int hvmt2Command3SendTime;

void saveHvmtCommand1(uint8 index, char *data);
void saveHvmtCommand2(uint8 index, char *data);
void saveHvmtCommand3(uint8 index, char *data);
void saveHvmtCommand4(uint8 index, char *data);
void saveHvmtCommand5(uint8 index, char *data);

#endif /* HVMT_H_ */