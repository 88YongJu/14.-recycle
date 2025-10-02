/*
 * DUALTHROTTLE.c
 *
 * Created: 2023-12-18 오전 11:50:00
 *  Author: YJKIM
 */ 

#include "DUALTHROTTLE.h"

struct DUAL_THROTTLE dual_throttle;
unsigned int throttle_timeR = 0;
unsigned int trim_timeR = 0;

void Save_Right_Trim(char *data)
{
	dual_throttle.Trim[RIGHT] = data[1];
}

void Save_Left_Trim(char *data)
{
	dual_throttle.Trim[LEFT] = data[1];
}

void Save_Right_Throttle(char *data)
{
	dual_throttle.value[RIGHT] = data[3] << 8;
	dual_throttle.value[RIGHT] |= data[4];
	dual_throttle.Direction[RIGHT] = data[5];
}

void Save_Left_Throttle(char *data)
{
	dual_throttle.value[LEFT] = data[3] << 8;
	dual_throttle.value[LEFT] |= data[4];
	dual_throttle.Direction[LEFT] = data[5];
}