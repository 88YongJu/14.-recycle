/*
 * UI.c
 *
 * Created: 2023-03-09 오전 11:12:30
 *  Author: YJKIM
 */ 

#include "UI.h"

char HEADER[][4] = {"BAT1",
	"LDC1", "LDC2", "FC01",
	"INV1", "INV2", "ICM1",
	"CON1", "CTR1",
	};

void Send_UI_Start(void)
{
	char buff[3];
	buff[0] = STX>>16;
	buff[1] = STX>>8;
	buff[2] = STX;
	send(SOCK_TCPC, buff, 3);
}

void Send_UI_Header(char head_num)
{
	send(SOCK_TCPC, HEADER[head_num], 4);
}

void Send_UI_End(void)
{
	char buff[3];
	buff[0] = ETX>>16;
	buff[1] = ETX>>8;
	buff[2] = ETX;
	send(SOCK_TCPC, buff, 3);
	send(SOCK_TCPC, buff, 3);
}

void Send_UI_Data(unsigned int num)
{
	char buff[3];
	buff[0] = num;
	buff[1] = num>>8;
	buff[2] = '/';
	send(SOCK_TCPC, buff, 3);
}