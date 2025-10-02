/*
 * UI.h
 *
 * Created: 2023-03-09 오전 11:12:18
 *  Author: YJKIM
 */ 


#ifndef TCP_H_
#define TCP_H_

#include <string.h>
#include "types.h"
#include "spi.h"
#include "serial.h"
#include "TIMER.h"
#include "w5100.h"

#include "BMS.h"
#include "LDC.h"
#include "ABBINV.h"
#include "HVMT.h"




volatile unsigned int tcp_send_time;

unsigned int ui_Sr_udp;
unsigned int ui_Sr_tcp;



#endif /* TCP_H_ */