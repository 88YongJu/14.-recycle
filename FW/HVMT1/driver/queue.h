/*
 * Queue.h
 *
 * Created: 2023-03-03 오후 2:47:11
 *  Author: YJKIM
 */ 
#ifndef QUEUE_H_
#define QUEUE_H_

#include "types.h"

#define MAX_Q_CNT   4
#define MAX_Q_SIZE  16

#define FALSE 0
#define TRUE 1

#define USART0_TX	0
#define USART0_RX	1
#define USART1_TX	2
#define USART1_RX	3

typedef struct {
	uint16 qHead; // 넣는 포인트
	uint16 qTail; // 꺼내는 포인트
	uint8 _data[MAX_Q_SIZE];
} TYPE_RECV_Q;

void init_Queue(unsigned char queue_num);
void push(unsigned char queue_num, char recv_data);
char pull(unsigned char queue_num);
unsigned int get_size(unsigned char queue_num);

#endif /* QUEUE_H_ */