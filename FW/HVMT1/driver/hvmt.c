/*
 * HVMT.c
 *
 * Created: 2024-07-02 오후 2:07:57
 *  Author: yjkim88
 */ 

#include "HVMT.h"

volatile struct HVMT_CMD	hvmtcmd[HVMT_MAX_CNT];

struct MOb hvmt1Command1 = {CANID_HVMT1_CMD1, CAN_STD, {0}, 8};

volatile struct HVMT_STE1	hvmtste1[HVMT_MAX_CNT];
volatile struct HVMT_STE2	hvmtste2[HVMT_MAX_CNT];
volatile struct HVMT_STE3	hvmtste3[HVMT_MAX_CNT];

uint8_t HVMT_CMD1[HVMT_MAX_CNT][8];
uint8_t HVMT_CMD2[HVMT_MAX_CNT][8];
uint8_t HVMT_CMD3[HVMT_MAX_CNT][8];

volatile unsigned long time_hvmt = 0;

void saveHvmtCommand1(uint8 index, char *data)
{
	hvmtcmd[index].chargeRelay1 = data[0] & 0b00000001;
	hvmtcmd[index].dischargeRelay1 = (data[0] >> 1) & 0b00000001;
	/*
	hvmtcmd[index].chargeRelay2 = (data[0] >> 2) & 0b00000001;
	hvmtcmd[index].dischargeRelay2 = (data[0] >> 3) & 0b00000001;
	hvmtcmd[index].keyStatus = (data[0] >> 2) & 0b00000011;
	*/
	
	hvmtcmd[index].relay1 = (data[0] >> 2) & 0b00000011;
	hvmtcmd[index].relay2 = (data[0] >> 3) & 0b00000001;
	hvmtcmd[index].relay3 = (data[0] >> 4) & 0b00000001;
	hvmtcmd[index].relay4 = (data[0] >> 5) & 0b00000001;
	
	/*
	hvmtcmd[index].IMU_Check = (data[0] >> 4) & 0b00000001;
	hvmtcmd[index].EVCC_Emcstop = (data[0] >> 5) & 0b00000001;
	*/
}

void Save_cmd2(uint8 index, char *data)
{
	hvmtcmd[index].ldc_cmd[0].LDC_run = data[0] & 0b00000001;
	hvmtcmd[index].ldc_cmd[0].LDC_Voltage = data[3];
	hvmtcmd[index].ldc_cmd[0].LDC_Voltage |= data[4] >> 8;
}

void Save_cmd3(uint8 index, char *data)
{
	hvmtcmd[index].ldc_cmd[1].LDC_run = data[0] & 0b00000001;
	hvmtcmd[index].ldc_cmd[1].LDC_Current = data[5];
	hvmtcmd[index].ldc_cmd[1].LDC_Current |= data[6] >> 8;
}

void Save_ste1(uint8 index, char *data)
{
	hvmtste1[index].chargeRelay1 = data[0] & 0b00000001;
	hvmtste1[index].dischargeRelay1 = (data[0] >> 1) & 0b00000001;
	hvmtste1[index].Precharge_rt1 = (data[0] >> 2)  & 0b00000001;
	hvmtste1[index].chargeRelay2 = (data[0] >> 3) & 0b00000001;
	hvmtste1[index].dischargeRelay2 = (data[0] >> 4) & 0b00000001;
	hvmtste1[index].Precharge_ry2 = (data[0] >> 5) & 0b00000001;
	//hvmtste1[index].EVCC_GUN = (data[0] >> 6) & 0b00000011;
	hvmtste1[index].Charge_voltage = data[1];
	hvmtste1[index].Charge_voltage |= data[2] << 8;
	hvmtste1[index].Discharge_voltage = data[3];
	hvmtste1[index].Discharge_voltage |= data[4] << 8;
}

void Save_IMU(uint8 index, char *data)
{
	hvmtste2[index].IMU_ry = data[0] & 0b00000001;
	hvmtste2[index].DC_vol = data[1];
	hvmtste2[index].P_RES = data[2];
	hvmtste2[index].P_RES |= data[3] << 8;
	hvmtste2[index].N_RES = data[4];
	hvmtste2[index].N_RES |= data[5] << 8;
}

void Save_LDC(uint8 index, char *data)
{
	hvmtste3[index].LDC_reaad = data[0] & 0b00000001;
	hvmtste3[index].LDC_run = (data[0] >> 1) & 0b00000001;
	hvmtste3[index].LDC_ig = (data[0] >> 2) & 0b00000001;
	hvmtste3[index].LDC_fault_check = (data[0] >> 3) & 0b00000001;
	hvmtste3[index].LDC_in_voltage = data[1];
	hvmtste3[index].LDC_in_voltage |= data[2] >> 8;
	hvmtste3[index].LDC_out_voltage = data[3];
	hvmtste3[index].LDC_out_voltage |= data[4] >> 8;
	hvmtste3[index].LDC_out_current = data[5];
	hvmtste3[index].LDC_out_current |= data[6] >> 8;
	hvmtste3[index].temp = data[7];
}

void Update_CMD_HVMT(unsigned char index)
{
	HVMT_CMD1[index][0] = (hvmtcmd[index].EVCC_Emcstop << 5 | hvmtcmd[index].IMU_Check << 4 | hvmtcmd[index].dischargeRelay2 << 3 | hvmtcmd[index].chargeRelay2 << 2| hvmtcmd[index].dischargeRelay1 << 1 | hvmtcmd[index].chargeRelay1);
	
	HVMT_CMD2[index][0] = hvmtcmd[index].ldc_cmd[0].LDC_run;
	HVMT_CMD2[index][1] = hvmtcmd[index].ldc_cmd[0].LDC_Voltage;
	HVMT_CMD2[index][2] = hvmtcmd[index].ldc_cmd[0].LDC_Voltage >> 8;
	HVMT_CMD2[index][3] = hvmtcmd[index].ldc_cmd[0].LDC_Current;
	HVMT_CMD2[index][4] = hvmtcmd[index].ldc_cmd[0].LDC_Current >> 8;
	
	HVMT_CMD3[index][0] = hvmtcmd[index].ldc_cmd[1].LDC_run;
	HVMT_CMD3[index][1] = hvmtcmd[index].ldc_cmd[1].LDC_Voltage;
	HVMT_CMD3[index][2] = hvmtcmd[index].ldc_cmd[1].LDC_Voltage >> 8;
	HVMT_CMD3[index][3] = hvmtcmd[index].ldc_cmd[1].LDC_Current;
	HVMT_CMD3[index][4] = hvmtcmd[index].ldc_cmd[1].LDC_Current >> 8;
}