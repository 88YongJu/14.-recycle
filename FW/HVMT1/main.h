/*
 * main.h
 *
 * Created: 2025-08-14 오후 4:04:07
 *  Author: YJKIM
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "driver/gio.h"
#include "driver/timer.h"
#include "driver/usart.h"
#include "driver/can.h"
#include "driver/bms.h"
#include "driver/max11162.h"
#include "driver/hvmt.h"
#include "driver/cb350m.h"

#define RY_1	porta.bit0
#define RY_2	porta.bit1
#define RY_3	porta.bit2
#define RY_4	porta.bit3

#define LED_1	portg.bit0
#define LED_2	portg.bit1
#define LED_3	portg.bit2

#define RUN_LED_TIME	500
#define HVMT_SEND_TIME	500

enum MODE{MODE_NOMAL, MODE_ACC, MODE_RUN};

#endif /* MAIN_H_ */