/*
 * RADFAN.h
 *
 * Created: 2023-03-15 오전 10:58:19
 *  Author: YJKIM
 */ 


#ifndef RADFAN_H_
#define RADFAN_H_

#include "can.h"

#define CANID_REC_LH	0x4C3
#define CANID_REC_RH	0x4C4

#define CANID_TRA_LH	0x4CE
#define CANID_TRA_RH	0x4CF

#define RPM	500

void Run_LH_FAN(unsigned int rpm);
void Run_RH_FAN(unsigned int rpm);
void Stop_LH_FAN(void);
void Stop_RH_FAN(void);
#endif /* RADFAN_H_ */