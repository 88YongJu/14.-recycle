/*
 * BMS.h
 *
 * Created: 2022-11-23 오전 10:07:53
 *  Author: YJKIM
 */ 

#include "types.h"
#include "can.h"

#ifndef BMS_H_
#define BMS_H_

#define MAX_BAT	2

// command to Hvb
#define CANID_BAT1	0x100
#define CANID_BAT2	0x200
#define CANID_BAT3	0x300
#define CANID_BAT4	0x400

#define CANID_CELL_VOL		0x01
#define CANID_CELL_TEMP		0x02
#define CANID_MOD_TEMP		0x04
#define CANID_HUM		0x05
#define CANID_STATUS	0x06
#define CANID_FAN	0x07

#define CANID_BAT1_VOL		(CANID_BAT1+CANID_CELL_VOL)
#define CANID_BAT1_TEMP		(CANID_BAT1+CANID_CELL_TEMP)
#define CANID_BAT1_MOD_TEMP	(CANID_BAT1+CANID_MOD_TEMP)
#define CANID_BAT1_HUM		(CANID_BAT1+CANID_HUM)
#define CANID_BAT1_STATUS	(CANID_BAT1+CANID_STATUS)
#define CANID_BAT1_FAN		(CANID_BAT1+CANID_FAN)

#define CANID_BAT2_VOL		(CANID_BAT2+CANID_CELL_VOL)
#define CANID_BAT2_TEMP		(CANID_BAT2+CANID_CELL_TEMP)
#define CANID_BAT2_MOD_TEMP	(CANID_BAT2+CANID_MOD_TEMP)
#define CANID_BAT2_HUM		(CANID_BAT2+CANID_HUM)
#define CANID_BAT2_STATUS	(CANID_BAT2+CANID_STATUS)
#define CANID_BAT2_FAN		(CANID_BAT2+CANID_FAN)

#define CHARGE		0x0A
#define DISCHARGE	0x0B
#define PRECHARGE	0x0C
#define EMERGENCY	0x0D
#define HEARTBIT	0x0E

#define BAT_DISABLE	0x00
#define BAT_ENABLE	0x01

#define CAN_CMD1	0xFA
#define CAN_CMD2	0xEB
#define CAN_CMD3	0xDC

#define ON_CHAGE		0x10
#define OFF_CHAGE		0x11

#define ON_DISCHAGE		0x12
#define OFF_DISCHAGE	0x13

#define ON_PRECHAGE		0x14
#define OFF_PRECHAGE	0x15

#define SET_EMERYGENCY	0x01
#define RESET_EMERYGENCY	0x02

#define BMS_TIMEOUT	500

enum {INDEX_BAT1, INDEX_BAT2, INDEX_BAT3, INDEX_BAT4};

struct BMS
{
	boolean charge;
	boolean discharge;
	boolean precharge;

	unsigned int rackVoltage;
	u_int rackCurrent;
	u_int rackSoc;
	u_int rackSoh;
	
	u_int cellMaxVoltage;
	u_int cellMaxPosition;
	u_int cellMinVoltage;
	u_int cellMinPosition;
	
	u_int cellMaxTemperature;
	u_int cellMaxTemperaturePosition;
	u_int cellMinTemperature;
	u_int cellMinTemperaturePosition;
	
	u_int moduleMaxTemperature;
	u_int moduleMinTemperature;
	
	u_int moduleMaxHumidity;
	u_int moduleMaxHumidityPosition;
	u_int moduleMinHumidity;
	u_int moduleMinHumidityPosition;

	boolean run;
	boolean fault;
	boolean warning;
	boolean rackImbalance;
	
	boolean chargeRelay;
	boolean dischargeRelay;
	boolean cellBalance;
	boolean sensingCharge;
	boolean sensingDischarge;

	unsigned char warnings;
	unsigned char faults1;
	unsigned int faults2;
	unsigned int vocFault;
	unsigned int fanFault;
};

volatile struct BMS bms[MAX_BAT];

volatile unsigned int runtime[MAX_BAT];

void saveRackStatus(unsigned char index, char *data);
void saveBmsStatus(uint8 index, char *data);
void saveBmsFan(uint8 index, char *data);
void saveCellTemperatere(uint8 index, char *data);
void saveCellVoltage(uint8 index, char *data);
void saveModuleTemperature(uint8 index, char *data);
void SaveModuleHumidity(uint8 index, char *data);

void sendBmsHeartbeat(uint8 index);
void Send_on_Chage(uint8 index);
void Send_off_Chage(uint8 index);
void Send_on_DisChage(uint8 index);
void Send_off_DisChage(uint8 index);
void Send_on_PreChage(uint8 index);
void Send_off_PreChage(uint8 index);
void Send_on_Emc(uint8 index, unsigned char status);
void Send_off_Emc(uint8 index, unsigned char status);
#endif /* BATTERY_H_ */