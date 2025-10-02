/*
 * lvbattery.c
 *
 * Created: 2023-02-20 오전 9:17:03
 *  Author: YJKIM
 */ 

#include "lvbattery.h"

struct LVB lv;

void Save_LV_Status(struct MOb *msg)
{
	lv.Vol = msg->data[0]<<8;
	lv.Vol |= msg->data[1];
	//lv.Vol = lv.Vol*5;
	lv.Cur =  msg->data[2]<<8;
	lv.Cur |=  msg->data[3];
	//lv.Cur = ((511-lv.Cur)*5);
	lv.SOC = msg->data[4];
	lv.Temp =msg->data[5];//-55
}