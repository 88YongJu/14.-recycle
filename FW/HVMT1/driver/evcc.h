/*
 * EVCC.h
 *
 * Created: 2023-05-17 오후 2:36:36
 *  Author: YJKIM
 */

#ifndef EVCC_H_
#define EVCC_H_

//#define DBG

#include "types.h"
#include "CAN.h"
#include "BMS.h"

#define	EVCC_WAIT_TIME		10

#define MAX_POWER	10000000
#define CHARGE_VALTAGE	5410
#define MAX_CHARGE_CURRENT	920
#define CHARGE_CURRENT	600

#define CUT_OFF_CURRENT	350

#define PRE_VOL_GAP	100	//10V
#define FULL_SOC	100	//100%
#define BULK_SOC	85

/*Define Send Time*/
#define HEART_TIME	200
#define TIME_CNT_10ms	10
#define TIME_CNT_1000ms	1000

#define TIME_CNT_E_STOP		1000
#define RUNTIME_OUT	20000

/*Send CANID DEFINE*/
#define EVS_CONFIG			0x1EBCC001	//1000 ms
#define EVS_TIME_CONFIG		0x1EBCC002	//1000 ms
#define EVS_STATUS			0x1EBCC003	//10 ms

#define EVS_BAT_PARAMETER	0x1EBCC005	//100 ms
#define EVS_SOC				0x1EBCC006	//100 ms
#define EVS_TARGET			0x1EBCC008	//10ms

/*RECIVE CANID DEFINE*/
#define EVCC_STATUS		0x1EBCC101
#define PLUG_STATUS		0x1EBCC103
#define EVSE_STATUS		0x1EBCC104	//10ms
#define EVSE_PRESENT	0x1EBCC106
#define EVCC_MAC		0x1EBCC107

/*Define charging Control Step*/
#define EVS_NONE			0
#define EVS_INIT_EVCC		1
#define EVS_CHARGE_READY	2
#define EVS_EV_INIT_N_STOP	3
#define EVS_EV_INIT_E_STOP	4
#define EVS_FORCE_UNLOCK	5
#define EVS_RENEGOTIATE		6

/*define evcc Status*/
#define EVCC_IDLE		0
#define EVCC_INIT		1
#define EVCC_HLC_READY	2
#define EVCC_BS_READY	3

#define EVCC_SLAC	10
#define EVCC_SDP	11
#define EVCC_SAP	12
#define EVCC_SESSION_SETUP	13
#define EVCC_SERVICE_DISCOVERY	14
#define EVCC_PAYMENT_SERVICE_SELECTION	15
#define EVCC_AUTH_ONGOING	16

#define EVCC_AUTH_FINISHED	17
#define EVCC_CPD_ONFOING	18
#define	EVCC_CPD_FINISHED	19
#define EVCC_CABLE_CHECK_ONGOING	20
#define EVCC_CABLE_CHECK_FINISHED	21
#define EVCC_PRECHARGE		22
#define EVCC_POWER_DELIVERY_START	23
#define EVCC_CURRENTDEMAND	24
#define EVCC_POWER_DELIVERY_STOP	25
#define EVCC_WELDING_DETECTION	27
#define EVCC_ERROR_SELF_RECOVERABLE		30
#define EVCC_ERROR_SUSPENDED	31	//GunSw
#define EVCC_ERROR_BS	32
#define EVCC_TERMINATED	40

/*define MailRelay*/
#define MAIN_RELAY_NONE		0
#define MAIN_RELAY_OPEN		1
#define MAIN_RELAY_CLOSE	2
#define MAIN_RELAY_ERROR	3

/*Define Plug*/
#define PLUG_NONE	0
#define PLUG_OUT	1
#define PLUG_IN		2
#define LATCH		3

struct MOb;

struct EVCC
{
	uint8 status;
	uint8 plugStatus;
	uint16 outputVoltage;
	uint16 outputCurrent;
	uint16 maxCurrent;
};

struct EVCC evcc;
uint16 targetVoltage;
uint8 dcMainConrtactorStatus;
uint8 chargeControl;

void saveEvccStatus(char *data);
void savePlugStatus(char *data);
void savePresentValue(char *data);
void saveEvseStatus(char *data);
void sendConfigValue(void);

void processEvcc(void);
#endif /* CHARGE_H_ */