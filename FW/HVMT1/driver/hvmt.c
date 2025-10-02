/*
 * HVMT.c
 *
 * Created: 2024-07-02 오후 2:07:57
 *  Author: yjkim88
 */ 

#include "HVMT.h"

volatile struct HVMT_CMD	hvmtCommand[HVMT_MAX_CNT];

volatile struct MOb hvmt1Command1 = {CANID_HVMT1_CMD1, CAN_STD, {0}, 8};

volatile struct HVMT_STE1	hvmtste1[HVMT_MAX_CNT];
volatile struct HVMT_STE2	hvmtste2[HVMT_MAX_CNT];
volatile struct HVMT_STE3	hvmtste3[HVMT_MAX_CNT];

volatile unsigned int hvmt1Command1SendTime;
volatile unsigned int hvmt1Command2SendTime;
volatile unsigned int hvmt1Command3SendTime;
volatile unsigned int hvmt2Command1SendTime;
volatile unsigned int hvmt2Command2SendTime;
volatile unsigned int hvmt2Command3SendTime;

void saveHvmtCommand1(uint8 index, char *data)
{
	/*
	hvmtCommand[index].chargeRelay1 = data[0] & 0b00000001;
	hvmtCommand[index].dischargeRelay1 = (data[0] >> 1) & 0b00000001;
	hvmtCommand[index].chargeRelay2 = (data[0] >> 2) & 0b00000001;
	hvmtCommand[index].dischargeRelay2 = (data[0] >> 3) & 0b00000001;
	hvmtCommand[index].keyStatus = (data[0] >> 2) & 0b00000011;
	*/
	
	hvmtCommand[index].relay1 = data[0] & 0b00000001;
	hvmtCommand[index].relay2 = (data[0] >> 1) & 0b00000001;
	hvmtCommand[index].relay3 = (data[0] >> 2) & 0b00000001;
	hvmtCommand[index].relay4 = (data[0] >> 3) & 0b00000001;
	
	/*
	hvmtCommand[index].IMU_Check = (data[0] >> 4) & 0b00000001;
	hvmtCommand[index].EVCC_Emcstop = (data[0] >> 5) & 0b00000001;
	*/
}

void saveHvmtCommand2(uint8 index, char *data)
{
	hvmtCommand[index].ldc[0].run = data[0] & 0b00000001;
	hvmtCommand[index].ldc[0].outputVoltage = data[3];
	hvmtCommand[index].ldc[0].outputVoltage |= data[4] >> 8;
}

void Save_cmd3(uint8 index, char *data)
{
	hvmtCommand[index].ldc[1].run = data[0] & 0b00000001;
	hvmtCommand[index].ldc[1].outputCurrent = data[5];
	hvmtCommand[index].ldc[1].outputCurrent |= data[6] >> 8;
}

void saveHvmtCommand3(uint8 index, char *data)
{
	hvmtste1[index].chargeRelay1 = data[0] & 0b00000001;
	hvmtste1[index].dischargeRelay1 = (data[0] >> 1) & 0b00000001;
	hvmtste1[index].prechargeRelay1 = (data[0] >> 2)  & 0b00000001;
	hvmtste1[index].chargeRelay2 = (data[0] >> 3) & 0b00000001;
	hvmtste1[index].dischargeRelay2 = (data[0] >> 4) & 0b00000001;
	hvmtste1[index].prechargeRelay2 = (data[0] >> 5) & 0b00000001;
	//hvmtste1[index].evccGunStatus = (data[0] >> 6) & 0b00000011;
	hvmtste1[index].ad1 = data[1];
	hvmtste1[index].ad1 |= data[2] << 8;
	hvmtste1[index].ad2 = data[3];
	hvmtste1[index].ad2 |= data[4] << 8;
}

void saveHvmtCommand4(uint8 index, char *data)
{
	hvmtste2[index].imuRun = data[0] & 0b00000001;
	hvmtste2[index].imuDcVoltage = data[1];
	hvmtste2[index].imuPositiveGroundRegistance = data[2];
	hvmtste2[index].imuPositiveGroundRegistance |= data[3] << 8;
	hvmtste2[index].imuNegativeGroundRegistance = data[4];
	hvmtste2[index].imuNegativeGroundRegistance |= data[5] << 8;
}

void saveHvmtCommand5(uint8 index, char *data)
{
	hvmtste3[index].ldc.ready = data[0] & 0b00000001;
	hvmtste3[index].ldc.run = (data[0] >> 1) & 0b00000001;
	hvmtste3[index].ldc.ig = (data[0] >> 2) & 0b00000001;
	hvmtste3[index].ldc.fault = (data[0] >> 3) & 0b00000001;
	hvmtste3[index].ldc.inputVoltage = data[1];
	hvmtste3[index].ldc.inputVoltage |= data[2] >> 8;
	hvmtste3[index].ldc.outputVoltage = data[3];
	hvmtste3[index].ldc.outputVoltage |= data[4] >> 8;
	hvmtste3[index].ldc.outputCurrent = data[5];
	hvmtste3[index].ldc.outputCurrent |= data[6] >> 8;
	hvmtste3[index].ldc.temperature = data[7];
}