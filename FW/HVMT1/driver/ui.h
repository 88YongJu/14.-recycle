/*
 * UI.h
 *
 * Created: 2023-03-09 오전 11:12:18
 *  Author: YJKIM
 */ 


#ifndef UI_H_
#define UI_H_

#include "w5100.h"

#define STX	0xFFFF02
#define ETX	0xFFFF03

#define HEAD_BAT1	0
#define HEAD_LDC1	1
#define HEAD_LDC2	2

#define HEAD_FC01	3

#define HEAD_INV1	4
#define HEAD_INV2	5
#define HEAD_ICM1	6
#define HEAD_CON1	7

#define HEAD_CTR1	8

void Send_UI_Start(void);
void Send_UI_End(void);
void Send_UI_Header(char head_num);
void Send_UI_Data(unsigned int num);



#endif /* UI_H_ */