/*
 * EVCC.c
 *
 * Created: 2023-05-17 오후 2:36:42
 *  Author: YJKIM
 */ 

#include "EVCC.h"

volatile unsigned int heartBeatTimer = 0;
volatile unsigned int evcc10msTimer = 0;
volatile unsigned int evcc1000msTimer = 0;
volatile unsigned int evccRuntimer = 0;
volatile unsigned int commandRunTime = 0;
volatile unsigned int currentCheckTimer = 0;

uint8 chargeControlBatteryIndex = INDEX_BAT2;
uint8 lowSocBatteryIndex = INDEX_BAT1;
uint8 chargeControl = EVS_NONE;

uint16 targetVoltage = 0;
uint16 targetCurrent = 0;
uint8 ev_SOC = 0;
uint8 dcMainConrtactorStatus = MAIN_RELAY_NONE;
boolean isolationMonitoring = 1;
boolean chargeParameterConfirm = 0;
boolean preChargeLoopProcess = 0;
boolean f_weldingDetection = 0;

uint8 heartBeat = 0;

struct EVCC evcc;

void saveEvccStatus(char *data)
{
	evcc.status = data[0];
}

void savePlugStatus(char *data)
{
	evcc.plugStatus = data[4];
}

void savePresentValue(char *data)
{
	evcc.outputVoltage = data[0];
	evcc.outputVoltage |= data[1]<<8;
	
	evcc.outputCurrent = data[2];
	evcc.outputCurrent |= data[3]<<8;
}

void saveEvseStatus(char *data)
{
	evcc.maxCurrent = data[0];
	evcc.maxCurrent |= data[1]<<8;
}

void sendConfigValue(void)
{	//2-01
	struct MOb configMessage = {EVS_CONFIG,CAN_EXT,{0x95, 0x02, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00},8};
	sendCan(&configMessage);
}

void processEvcc(void)
{
	if(millis() - heartBeatTimer >= HEART_TIME)
	{
		heartBeatTimer = millis();
		heartBeat++;
	}

	if(evcc.plugStatus == PLUG_OUT)
	{
		#ifdef DBG
		printf("PLUG OUT\r\n");
		#endif
		chargeControl = EVS_NONE;
		evccRuntimer = millis();
	}
	
	if(evcc.status)
	{
		if(millis() - evccRuntimer >= RUNTIME_OUT)
		{
			evccRuntimer = millis();
			chargeControl = EVS_EV_INIT_E_STOP;
		}
	}
	
	switch(evcc.status)
	{
		case EVCC_IDLE:
		#ifdef DBG
		printf("IDLE\r\n");
		#endif
		isolationMonitoring = 1;
		dcMainConrtactorStatus = MAIN_RELAY_NONE;
		chargeParameterConfirm = 0;
		preChargeLoopProcess = 0;
		f_weldingDetection = 0;

		if(evcc.plugStatus == PLUG_IN)
		{
			#ifdef DBG
			printf("PLUG IN\r\n");
			#endif
			chargeControl = EVS_INIT_EVCC;
			dcMainConrtactorStatus = MAIN_RELAY_OPEN;
			evccRuntimer = millis();
		}
		break;
		
		case EVCC_INIT:
		#ifdef DBG
		printf("EVCC_INIT\r\n");
		#endif
		break;
		
		case EVCC_HLC_READY:
		#ifdef DBG
		printf("EVCC_READY\r\n");
		#endif
		chargeControl = EVS_CHARGE_READY;
		evccRuntimer = millis();
		break;
		
		case EVCC_AUTH_FINISHED:
		#ifdef DBG
		printf("EVCC_Confirm\r\n");
		#endif
		chargeParameterConfirm = 1;
		evccRuntimer = millis();
		break;
		
		case EVCC_CPD_FINISHED:
		#ifdef DBG
		printf("EVCC_CPD_FINISHED\r\n");
		#endif
		chargeParameterConfirm = 0;
		isolationMonitoring = 0;
		evccRuntimer = millis();
		break;
		
		case EVCC_CABLE_CHECK_FINISHED:
		#ifdef DBG
		printf("EVCC_StePre\r\n");
		#endif
		isolationMonitoring = 1;
		
		if(bms[INDEX_BAT1].rackVoltage > bms[INDEX_BAT2].rackVoltage)
		{
			chargeControlBatteryIndex = INDEX_BAT1;
		}
		else
		{
			chargeControlBatteryIndex = INDEX_BAT2;
		}

		targetVoltage = bms[chargeControlBatteryIndex].rackVoltage;
		targetCurrent = 0;
		
		break;
		
		case EVCC_PRECHARGE:
		#ifdef DBG
		DebugPrint("Target Voltage : %d\r\n", targetVoltage);
		DebugPrint("evcc.outputVoltage : %d\r\n", evcc.outputVoltage);
		#endif
		if(evcc.outputVoltage >= targetVoltage - PRE_VOL_GAP)
		{
			dcMainConrtactorStatus = MAIN_RELAY_CLOSE;
			preChargeLoopProcess = 1;
			evccRuntimer = millis();
		}
		evccRuntimer = millis();
		break;

		case EVCC_POWER_DELIVERY_START:
		preChargeLoopProcess = 0;
		targetVoltage = CHARGE_VALTAGE;
		targetCurrent = CHARGE_CURRENT;
		break;

		case EVCC_CURRENTDEMAND:
		#ifdef DBG
		printf("EVCC_CURRENTDEMAND\r\n");
		#endif
		commandRunTime = millis();
		evccRuntimer = millis();
		

		if(millis() - currentCheckTimer >= 10000)
		{
			currentCheckTimer = millis();
			if(bms[INDEX_BAT1].rackCurrent < bms[INDEX_BAT2].rackCurrent)
			{
				chargeControlBatteryIndex = INDEX_BAT2;
			}
			else
			{
				chargeControlBatteryIndex = INDEX_BAT1;
			}
			if(targetCurrent < (evcc.maxCurrent - 50))
			{
				if(bms[chargeControlBatteryIndex].rackCurrent <= CHARGE_CURRENT - 50)
				{
					if(targetCurrent >= 1200)
					{
						targetCurrent = 1200;
					}
					else
					{
						targetCurrent = targetCurrent + 50;
					}
				}
			}
			else if(evcc.maxCurrent < targetCurrent)
			{
				targetCurrent = targetCurrent - 50;
			}
		}
		break;
		
		case EVCC_POWER_DELIVERY_STOP:
		targetVoltage = 0;
		targetCurrent = 0;
		chargeControl = EVS_EV_INIT_N_STOP;
		if(millis() - commandRunTime >= TIME_CNT_E_STOP)
		{
			commandRunTime = millis();
			f_weldingDetection = 1;
			#ifdef DBG
			printf("EVCC_RY_OPEN\r\n");
			#endif
			dcMainConrtactorStatus = MAIN_RELAY_OPEN;
		}
		evccRuntimer = millis();
		break;
		
		case EVCC_WELDING_DETECTION:
		evccRuntimer = millis();
		#ifdef DBG
		printf("UN_LUCK\r\n");
		#endif
		chargeControl = EVS_FORCE_UNLOCK;
		if(millis() - commandRunTime >= TIME_CNT_E_STOP)
		{
			commandRunTime = millis();
			f_weldingDetection = 1;
			dcMainConrtactorStatus = MAIN_RELAY_OPEN;
		}
		break;

		case EVCC_ERROR_SUSPENDED:
		targetVoltage = 0;
		targetCurrent = 0;
		dcMainConrtactorStatus = MAIN_RELAY_OPEN;
		chargeControl = EVS_FORCE_UNLOCK;
		break;
		
		case EVCC_ERROR_SELF_RECOVERABLE:
		chargeControl = EVS_RENEGOTIATE;
		evccRuntimer = millis();
		break;
		
		default :
		break;
	}
	
	#ifdef DBG
	DebugPrint("status : %d\r\n",evcc.status);
	#endif
	
	if(millis() - evcc10msTimer >= TIME_CNT_10ms)
	{
		evcc10msTimer = millis();
		if(evcc.plugStatus == PLUG_IN)
		{
			struct MOb evsStatusMessage = {EVS_STATUS,CAN_EXT,{chargeControl, dcMainConrtactorStatus,
				(isolationMonitoring << 0 | chargeParameterConfirm << 1 | preChargeLoopProcess << 2 | f_weldingDetection << 3),
			0x00, 0x00, 0x00, 0x00, heartBeat},8};
			sendCan(&evsStatusMessage);
		}
		if(evcc.status)
		{
			struct MOb evsTargetMessage = {EVS_TARGET,CAN_EXT,{targetVoltage, targetVoltage >> 8, targetCurrent,	targetCurrent >>8, 0x00, 0x00, 0x00, 0x00},8};
			sendCan(&evsTargetMessage);
		}
	}
	if(evcc.status)
	{
		if(millis() - evcc1000msTimer >= TIME_CNT_1000ms)
		{
			evcc1000msTimer = millis();
			
			if(bms[INDEX_BAT1].rackSoc < bms[INDEX_BAT2].rackSoc)
			{
				lowSocBatteryIndex = INDEX_BAT2;
			}
			else
			{
				lowSocBatteryIndex = INDEX_BAT1;
			}
			ev_SOC = (bms[lowSocBatteryIndex].rackSoc/10);
			
			if(ev_SOC >= 1000)
			{
				ev_SOC = 99;
				if(bms[INDEX_BAT1].rackCurrent < CUT_OFF_CURRENT)
				{
					if(bms[INDEX_BAT2].rackCurrent < CUT_OFF_CURRENT)
					{
						ev_SOC = 100;
						commandRunTime = millis();
					}
				}
			}
			
			struct MOb evsSocMessage = {EVS_SOC, CAN_EXT, {ev_SOC, FULL_SOC, BULK_SOC, 0x00, 0x00, 0x00, 0x00, 0x00},8};
			sendCan(&evsSocMessage);
			
			struct MOb batParameterMessage = {EVS_BAT_PARAMETER,CAN_EXT,{0x84, 0x03, 0x18, 0x15, 0x88, 0x13, 0x00, 0x00},8};
			sendCan(&batParameterMessage);
			
			struct MOb evsTimeOutMessage = {EVS_TIME_CONFIG, CAN_EXT, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD8, 0xD6},8};
			sendCan(&evsTimeOutMessage);
		}
	}
}