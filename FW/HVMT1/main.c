/*
 * HVMT.c
 *
 * Created: 2025-08-14 오후 3:54:29
 * Author : YJKIM
 */ 

#include <avr/io.h>
#include "main.h"

unsigned int bmsTimer = 0;
unsigned int debugPrintTime = 0;
unsigned int runningLedTimer = 0;
unsigned int hvmtStatus1SendTimer = 0;
unsigned int hvmtStatus2SendTimer = 0;

unsigned int runMode = MODE_NOMAL;

struct MOb hvmtStatus1 = {CANID_HVMT1_STE1, CAN_STD, {0}, 8};
struct MOb hvmtStatus2 = {CANID_HVMT1_STE2, CAN_STD, {0}, 8};

//can receive parsing process
void canParsing(void)
{
	if(receiveMessageHead != receiveMessageTail)
	{
		switch(receiveMessage[receiveMessageTail].id)
		{
			case CANID_BAT1 :
			bms1Runtime = BMS_TIMEOUT;
			saveRackStatus(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_TEMP :
			saveCellTemperatere(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_MOD_TEMP :
			saveModuleTemperature(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_HUM :
			saveModuleHumidity(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_STATUS :
			saveBmsStatus(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_BAT2 :
			bms2Runtime = BMS_TIMEOUT;
			saveRackStatus(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_TEMP :
			saveCellTemperatere(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_MOD_TEMP :
			saveModuleTemperature(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_HUM :
			saveModuleHumidity(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_STATUS :
			saveBmsStatus(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;

			case EVCC_STATUS :
			saveEvccStatus(receiveMessage[receiveMessageTail].data);
			break;
			case PLUG_STATUS :
			savePlugStatus(receiveMessage[receiveMessageTail].data);
			break;
			case EVSE_PRESENT :
			savePresentValue(receiveMessage[receiveMessageTail].data);
			break;
			case EVCC_MAC :
			sendConfigValue();
			break;
			case EVSE_STATUS :
			saveEvseStatus(receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT1_CMD1:
			saveHvmtCommand1(INDEX_HVMT1, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT2_CMD1:
			saveHvmtCommand1(INDEX_HVMT2, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT3_CMD1:
			saveHvmtCommand1(INDEX_HVMT3, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT4_CMD1:
			saveHvmtCommand1(INDEX_HVMT4, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT5_CMD1:
			saveHvmtCommand1(INDEX_HVMT5, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT6_CMD1:
			saveHvmtCommand1(INDEX_HVMT6, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT7_CMD1:
			saveHvmtCommand1(INDEX_HVMT7, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_CB350M_1 : 
			saveCb350m(INDEX_CB350M_1, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_CB350M_2 : 
			saveCb350m(INDEX_CB350M_2, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_CB350M_3 : 
			saveCb350m(INDEX_CB350M_3, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_CB350M_4 : 
			saveCb350m(INDEX_CB350M_4, receiveMessage[receiveMessageTail].data);
			break;
			
			default:
			break;
		}
		receiveMessageTail = (receiveMessageTail + 1) % MAX_RECEIVE_MESSAGE_BOX;
	}
}

int main(void)
{
	initPort();
	initUsart0(BAUD_115200);
	initTimer2();
	initCan(b500k);	//b500k
	initMax11162();
	printf("start\r\n");
    
    while (1)
    {
		readMax11162();	//high Voltage ADC Process
		trySendCan();	//can Message Send Process
		canParsing();	//can Message Receive Process
		//processEvcc();	//EVCC Control Process
		
		//run Check LED Process
		if(millis() - runningLedTimer > RUN_LED_TIME)
		{
			runningLedTimer = millis();
			LED_1 = ~LED_1;
			LED_2 = ~LED_2;
			LED_3 = ~LED_3;
		}
		
		RY_1 = hvmtCommand[HVMT_NUM].relay1;
		RY_2 = hvmtCommand[HVMT_NUM].relay2;
		RY_3 = hvmtCommand[HVMT_NUM].relay3;
		RY_4 = hvmtCommand[HVMT_NUM].relay4;
		
		/*
		if(hvmtCommand[INDEX_HVMT1].keyStatus != MODE_RUN)	//runMode Check Command protocal?
		{
			if(((396 < HVAD1) && (HVAD1 < 540)) && ((0 <= cb350m[INDEX_CB350M_1].current) && (cb350m[INDEX_CB350M_1].current <= 18000)))
			{
				RY_1 = 1;
				RY_2 = 1;
			}
			else
			{
				RY_1 = 0;
				RY_2 = 0;
			}
			if(((396 < HVAD2) && (HVAD2 < 540)) && ((0 <= cb350m[INDEX_CB350M_2].current) && (cb350m[INDEX_CB350M_2].current <= 18000)))
			{
				RY_3 = 1;
				RY_4 = 1;
			}
			else
			{
				RY_3 = 0;
				RY_4 = 0;
			}
		}
		*/
		
		if(millis() - hvmtStatus1SendTimer >= HVMT_SEND_TIME)
		{
			hvmtStatus1SendTimer = millis();
			//hvmt1Status1.data[0] = bms[INDEX_BAT1].chargeRelay | bms[INDEX_BAT1].dischargeRelay << 1 | bms[INDEX_BAT1].precharge;
			hvmtStatus1.data[0] = RY_1 | RY_2 << 1 | RY_3 << 2 | RY_4 << 3;
			hvmtStatus1.data[1] = HVAD1;
			hvmtStatus1.data[2] = HVAD1 >> 8;
			hvmtStatus1.data[3] = HVAD2;
			hvmtStatus1.data[4] = HVAD2 >> 8;
			sendCan(&hvmtStatus1);
		}
		
		//HVB charge process for main Relay control test code
		/*
		if(millis() - bmsTimer >= 100)
		{
			bmsTimer = millis();
			if(runtime[INDEX_BAT1])
			{
				sendBmsHeartbeat(INDEX_BAT1);
				if(dcMainConrtactorStatus == MAIN_RELAY_CLOSE)	sendChargeOnCommand(INDEX_BAT1);
				else sendChargeOffCommand(INDEX_BAT1);
			}
			if(bms2Runtime)
			{
				sendBmsHeartbeat(INDEX_BAT2);
				if(dcMainConrtactorStatus == MAIN_RELAY_CLOSE)	sendChargeOnCommand(INDEX_BAT2);
				else sendChargeOffCommand(INDEX_BAT2);
			}
		}
		*/
		
		//debug Print process
		if(millis() - debugPrintTime >= DBG_PRT_TIME)
		{
			debugPrintTime = millis();
			/*
			printf("chargeControl:%d\r\n",chargeControl);
			printf("evcc.status:%d\r\n",evcc.status);
			switch(evcc.status)
			{
				case EVCC_IDLE:
				printf("IDLE\r\n");
				if(evcc.plugStatus == PLUG_IN)
				{
					printf("PLUG IN\r\n");
				}
				break;
				
				case EVCC_INIT:
				printf("EVCC_INIT\r\n");
				break;
				
				case EVCC_HLC_READY:
				printf("EVCC_READY\r\n");
				break;
				
				case EVCC_AUTH_FINISHED:
				printf("EVCC_AUTH_FINISHED\r\n");
				break;
				
				case EVCC_CPD_FINISHED:
				printf("EVCC_CPD_FINISHED\r\n");
				break;
				
				case EVCC_CABLE_CHECK_FINISHED:
				printf("EVCC_CABLE_CHECK_FINISHED\r\n");
				break;
				
				case EVCC_PRECHARGE:
				printf("EVCC_PRECHARGE\r\n");
				DebugPrint("Target Voltage : %d\r\n", targetVoltage);
				DebugPrint("evcc.outputVoltage : %d\r\n", evcc.outputVoltage);
				break;

				case EVCC_POWER_DELIVERY_START:
				printf("EVCC_POWER_DELIVERY_START\r\n");
				break;

				case EVCC_CURRENTDEMAND:
				printf("EVCC_CURRENTDEMAND\r\n");
				break;
				
				case EVCC_POWER_DELIVERY_STOP:
				printf("EVCC_POWER_DELIVERY_STOP\r\n");
				break;
				
				case EVCC_WELDING_DETECTION:
				printf("EVCC_WELDING_DETECTION\r\n");
				break;

				case EVCC_ERROR_SUSPENDED:
				printf("GUN_SW_ON\r\n");
				break;
				
				case EVCC_ERROR_SELF_RECOVERABLE:
				printf("EVCC_ERROR_SELF_RECOVERABLE\r\n");
				break;
				
				default :
				break;
			}
			*/
		}
    }
}