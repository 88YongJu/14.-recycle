/*
 * DUALTHROTTLE.h
 *
 * Created: 2023-12-18 오전 11:49:38
 *  Author: YJKIM
 */ 


#ifndef DUALTHROTTLE_H_
#define DUALTHROTTLE_H_

#include "types.h"

#define THROTTLE_NUM	2	//Dual, 
#define RIGHT			0
#define LEFT			1

#define CANID_DUALTHROTTLE_DATA1	0x0001FFF1
#define CANID_DUALTHROTTLE_DATA2	0x0001FFF2
#define CANID_DUALTHROTTLE_DATA3	0x0001FFF3
#define CANID_DUALTHROTTLE_DATA4	0x0001FFF4
#define CANID_DUALTHROTTLE_DATA5	0x0001FFF5
#define CANID_DUALTHROTTLE_DATA6	0x0001FFF6
#define CANID_DUALTHROTTLE_DATA7	0x0001FFF7
#define CANID_DUALTHROTTLE_DATA8	0x0001FFF8
#define CANID_DUALTHROTTLE_DATA9	0x0001FFF9
#define CANID_DUALTHROTTLE_DATA10	0x0001FFFA

struct DUAL_THROTTLE
{
	boolean run;
	int Direction[THROTTLE_NUM];
	unsigned char Trim[THROTTLE_NUM];
	unsigned int value[THROTTLE_NUM];
};

struct DUAL_THROTTLE dual_throttle;

unsigned int throttle_timeR;
unsigned int trim_timeR;

unsigned int steer[THROTTLE_NUM];
unsigned int trim[THROTTLE_NUM];

void Save_Right_Trim(char *data);
void Save_Left_Trim(char *data);
void Save_Right_Throttle(char *data);
void Save_Left_Throttle(char *data);

#endif /* DUALTHROTTLE_H_ */