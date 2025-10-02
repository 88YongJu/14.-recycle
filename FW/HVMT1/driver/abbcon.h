/*
 * ABBCON.h
 *
 * Created: 2023-12-26 오후 2:46:15
 *  Author: YJKIM
 */ 


#ifndef ABBCON_H_
#define ABBCON_H_

#include "CAN.h"
#include "BMS.h"

#define CONVERTOR_MAXNUM	1

#define CANID_CMD_ABB_CON1	0xCFFF300
#define CANID_REC_ABB_CON1	0xCFFF003
#define CANID_REC_ABB_CON2	0xCFFF103

#define CON_WAIT_TIME	 10

#define INDEX_ABBCON1	0
#define CON_VOL		7000	//

struct ABBCON
{
	bool run;
	bool faultreset;
	//status
	unsigned int Current;
	unsigned int kW;
	unsigned int Temp;
	unsigned int Indecer_Temp;
	unsigned int Code_error;
	};

struct ABBCON abbcon[CONVERTOR_MAXNUM];


unsigned long conv_on_time, pcbms_on_time;

void ABB_Con_Handeler(void);
void Save_abbcnv1(unsigned char index, unsigned char *msg);
void Save_abbcnv2(unsigned char num, unsigned char *msg);
void Send_abbcnv(unsigned index);
#endif /* ABBCON_H_ */