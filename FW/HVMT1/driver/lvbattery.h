/*
 * lvbattery.h
 *
 * Created: 2023-02-20 오전 9:17:29
 *  Author: YJKIM
 */ 


#ifndef LVBATTERY_H_
#define LVBATTERY_H_

#include "types.h"
#include "can.h"

#define CANID_LV	0x400

struct MOb;

struct LVB
{
	u_int Vol;
	u_int Cur;
	uint8 SOC;
	uint8 Temp;
};

struct LVB lv;

void Save_LV_Status(struct MOb *msg);

#endif /* LVBATTERY_H_ */