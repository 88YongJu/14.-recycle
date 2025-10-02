/*
 * PCBMS.h
 *
 * Created: 2024-01-09 오후 5:29:03
 *  Author: YJKIM
 */ 


#ifndef PCBMS_H_
#define PCBMS_H_

#include "types.h"
#include "CAN.h"

#define CANID_PCBMS_STATUS1	0x100
#define CANID_PCBMS_STATUS2	0x101
#define CANID_PCBMS_STATUS3	0x110

#define CANID_PCBMS_CMD1	0x50
#define CANID_PCBMS_CMD2	0x51

#define PC_BMS_SEND_TIME	100

#define PC_BMS_SEND_CURRENT	200

struct PCBMS
{
	bool Run;
	bool fault_reset;
	
	bool Run_status;
	unsigned int Voltage;
	unsigned int Current;
	unsigned int Fault_status;
	unsigned int SOC;
	
	};

struct PCBMS pcbms;

void PC_BMS_Handeler(void);

#endif /* PCBMS_H_ */