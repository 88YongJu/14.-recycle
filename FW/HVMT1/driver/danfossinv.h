/*
 * invter.h
 *
 * Created: 2023-02-09 오전 10:09:16
 *  Author: YJKIM
 */ 


#ifndef DANFOSSINV_H_
#define DANFOSSINV_H_

#include <stdio.h>
#include "types.h"
#include "can.h"

#define DANFOSSINV_MAXNUM		2
#define MAX_RPM					3500
#define MAX_THROTTLE			150
#define	DANFOSSINV_WAIT_TIME	50

#define CANID_DANFOSSINV1_STS		0x0CFF81FC
#define CANID_DANFOSSINV2_STS		0x0CFF82FC
#define CANID_DANFOSSINV3_STS		0x0CFF83FC
#define CANID_DANFOSSINV4_STS		0x0CFF84FC
#define CANID_DANFOSSINV5_STS		0x0CFF91FC
#define CANID_DANFOSSINV6_STS		0x0CFF92FC

#define CANID_DANFOSSINV1_SYSCMD	0x0CFF1128
#define CANID_DANFOSSINV1_MOTCMD1	0x0CFF2128
#define CANID_DANFOSSINV1_MOTCMD2	0x0CFF2228

#define OFFSET_RPM_VALUE			32128	//Center RPM0 value

#define RPM_PER_DUTY	((MAX_RPM/MAX_THROTTLE))	//MAXRPM/100(MAXDUTY)*(0.5(FACTOR))
#define DUTYTORPM(x)	(x*(RPM_PER_DUTY))

#define MODE_MOTOR_SPEED	0x00
#define MODE_MOTOR_TORQUE	0x01
#define MODE_DCLINK_VOLTAGE	0x02
#define MODE_MOTOR_POWER	0x04

#define INV_STOP	0x00
#define INV_RUN		0x01
#define INV_Faultreset	0x04

#define MOT_RUN	0x10

enum{INV_DISABLE, INV_ENABLE};
enum{STOP, RUN, FRONT, REAR};
enum{INDEX_INV1, INDEX_INV2};

struct DANFOSSINV{
	unsigned char Direction;
	unsigned int RPM;
	
	int speed;
	unsigned char temp_j;
	unsigned char temp_min;
	unsigned char temp_max;
	unsigned int volt;
	unsigned int amp;
	unsigned int pwr;
	boolean inv_run;
	boolean inv_fault;
	boolean inv_warn;

	unsigned int faults1;
	unsigned int faults2;
	unsigned int faults3;
	};

volatile struct DANFOSSINV danfossinv[DANFOSSINV_MAXNUM];

volatile uint8_t CMD_SYS1[8];
volatile uint8_t CMD_MOT1[8];
volatile uint8_t CMD_MOT2[8];

boolean Ste_Inv;
unsigned int ui_Danfoss_setsys_sendtime;
unsigned int ui_Danfoss_setmot1_sendtime;
unsigned int ui_Danfoss_setmot2_sendtime;

void Save_danfossinv1(char *data);
void Save_danfossinv2(char *data);
void Save_danfossinv3(char *data);
void Save_danfossinv5(char *data);
void Save_danfossinv4(char *data);
void Save_danfossinv6(char *data);
void Control_Inverter(uint8 i, uint8 ctrmode, unsigned int val);
#endif /* INVTER_H_ */