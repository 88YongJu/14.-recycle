/*
 * CAN.h
 *
 * Created: 2024-12-13 오전 9:48:39
 *  Author: yjkim88
 */ 
#ifndef CAN_H_
#define CAN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "types.h"
#include "timer.h"

#include "bms.h"
#include "ldc.h"
#include "evcc.h"
#include "hvmt.h"

#define MOB_RX_STD_START 0 // 표준형 수신 메일박스 시작 번호
#define MOB_RX_STD_END 4   // 표준형 수신 메일박스 끝 번호
#define MOB_RX_EXT_START 5 // 확장형 수신 메일박스 시작 번호
#define MOB_RX_EXT_END 9   // 확장형 수신 메일박스 끝 번호
#define MOB_TX_START 10 // 송신 메일박스 시작 번호
#define MOB_TX_END 14   // 송신 메일박스 끝 번호

#define BUFFER_SIZE 8

#define MAX_SEND_MESSAGE_BOX	20
#define MAX_RECEIVE_MESSAGE_BOX	50

// CAN 보레이트(baud rate)===============================
#define b1M		1
#define b500k 	2
#define b250k	3
#define b200k	4
#define	b125k	5
#define b100k	6

#define MOB0	0
#define MOB1	1
#define MOB2	2
#define MOB3	3
#define MOB4	4
#define MOB5	5
#define MOB6	6
#define MOB7	7
#define MOB8	0
#define MOB9	1
#define MOB10	2
#define MOB11	3
#define MOB12	4
#define MOB13	5
#define MOB14	6

#define CAN_STD	0
#define CAN_EXT	1

#define STD_WAIT_TIME	200
#define EXT_WAIT_TIME	200


struct MOb{
	uint32_t id;
	boolean extended;
	char data[BUFFER_SIZE];
	uint8_t length;
};

struct MOb sendMessage[MAX_RECEIVE_MESSAGE_BOX];
volatile unsigned int sendMessageHead;
volatile unsigned int sendMessageTail;

struct MOb receiveMessage[MAX_RECEIVE_MESSAGE_BOX];
volatile unsigned int receiveMessageHead;
volatile unsigned int receiveMessageTail;

void initCan(char baudRate);
void sendCan(struct MOb *mob);
void trySendCan(void);
void checkCanStanderdReceive(void);
void checkCanExtendedReceive(void);

#endif /* CAN_H_ */