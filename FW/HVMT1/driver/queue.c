/*
 * Queue.c
 *
 * Created: 2023-03-03 오후 2:47:26
 *  Author: YJKIM
 */ 

#include "Queue.h"

/* Q 선언 */
TYPE_RECV_Q  PRCESS_Q[MAX_Q_CNT];

/* Q 초기화 */
void init_Queue(unsigned char queue_num)
{
	PRCESS_Q[queue_num].qTail = 0;
	PRCESS_Q[queue_num].qHead = 0;
}

/* Q에 넣기 */
void push(unsigned char queue_num, char recv_data)
{
	if (PRCESS_Q[queue_num].qHead == MAX_Q_SIZE-1) PRCESS_Q[queue_num].qHead = 0;
	PRCESS_Q[queue_num]._data[PRCESS_Q[queue_num].qHead++] = recv_data;
}

/* Q에서 꺼내기 */
char pull(unsigned char queue_num)
{
	// Q 처리
	if(PRCESS_Q[queue_num].qTail != PRCESS_Q[queue_num].qHead)
	{
		if (PRCESS_Q[queue_num].qTail == MAX_Q_SIZE-1) PRCESS_Q[queue_num].qTail = 0;
		return PRCESS_Q[queue_num]._data[PRCESS_Q[queue_num].qTail++];
	}
	else return -1;
}

unsigned int get_size(unsigned char queue_num)
{
	unsigned int size = 0;
	if(PRCESS_Q[queue_num].qHead < PRCESS_Q[queue_num].qTail)
	{
		size = MAX_Q_SIZE -1 - PRCESS_Q[queue_num].qTail + PRCESS_Q[queue_num].qHead;
	}
	else
	{
		size = PRCESS_Q[queue_num].qHead - PRCESS_Q[queue_num].qTail;
	}
	return size;
}