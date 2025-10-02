/*
 * CAN.c
 *
 * Created: 2024-12-13 오전 9:48:51
 *  Author: yjkim88
 */ 
#include "can.h"

struct MOb sendMessage[MAX_RECEIVE_MESSAGE_BOX];
volatile unsigned int sendMessageHead = 0;
volatile unsigned int sendMessageTail = 0;

struct MOb receiveMessage[MAX_RECEIVE_MESSAGE_BOX];
volatile unsigned int receiveMessageHead = 0;
volatile unsigned int receiveMessageTail = 0;

uint16 canStandardReceiveTime = 0;
uint16 canExtandedReceiveTime = 0;

// CAN 초기화 함수
void initCan(char baudRate)
{
	CANGCON = (1 << SWRES); // CAN 컨트롤러 Reset
	CANGCON |= (1 << ENASTB);	// CAN General Control Register
	switch(baudRate)
	{
		case b1M:
		CANBT1= 0x00;
		CANBT2= 0x0c;
		CANBT3= 0x37;
		break;
		case b500k:
		CANBT1= 0x02;
		CANBT2= 0x0c;
		CANBT3= 0x37;
		break;
		case b250k:
		CANBT1= 0x06;	// CAN보레이트 설정
		CANBT2= 0x0c;	// bit timing: datasheet 264 (check table)
		CANBT3= 0x37;	// 250kbps, 16 MHz CPU Clock(0.250usec)
		break;
		case b200k:
		CANBT1= 0x08;
		CANBT2= 0x0c;
		CANBT3= 0x37;
		break;
		case b125k:
		CANBT1= 0x0E;
		CANBT2= 0x0c;
		CANBT3= 0x37;
		break;
		case b100k:
		CANBT1= 0x12;
		CANBT2= 0x0c;
		CANBT3= 0x37;
		break;
		default :
		break;
	}
	// 표준형 메일박스 설정
	for (uint8_t i = MOB_RX_STD_START; i <= MOB_RX_STD_END; i++)
	{
		CANPAGE = (i << MOBNB0);
		CANCDMOB = 0x00;
		CANSTMOB = 0x00;					// 상태 초기화
		CANIDT1= 0x00;
		CANIDT2= 0x00;
		CANIDM1= 0x00;
		CANIDM2= 0x00;
		CANIDM4= 0x01;
		CANCDMOB |= (BUFFER_SIZE << DLC0);	// set data length
		CANCDMOB &= ~(1 << IDE);			// clear IDE =0, standard 11 bits
		CANCDMOB |= (1 << CONMOB1);			// 수신 모드
		if(i<8)
			CANIE2 |= (1<<i);				// 해당 MOb의 인터럽트를 인에이블 시킴
		else
			CANIE1 |= (1<<(i-8));			//
	}

	// 확장형 메일박스 설정
	for (uint8_t i = MOB_RX_EXT_START; i <= MOB_RX_EXT_END; i++)
	{
		CANPAGE = (i << MOBNB0);
		CANCDMOB = 0x00;
		CANSTMOB = 0x00;               // 상태 초기화
		CANIDT1= 0x00;
		CANIDT2= 0x00;
		CANIDT3= 0x00;
		CANIDT4= 0x00;
		CANIDM1= 0x00;
		CANIDM2= 0x00;
		CANIDM3= 0x00;
		CANIDM4= 0x01;
		CANCDMOB |= (BUFFER_SIZE & 0x0f);		// set data length
		CANCDMOB |= (1 << IDE);		// clear IDE =0, standard 11 bits
		CANCDMOB |= (1 << CONMOB1);  // 수신 모드
		if(i<8)
			CANIE2 |= (1<<i);		// 해당 MOb의 인터럽트를 인에이블 시킴
		else
			CANIE1 |= (1<<(i-8));	//
	}
	// 송신 메일박스 초기화
	for (uint8_t i = MOB_TX_START; i <= MOB_TX_END; i++)
	{
		CANPAGE = (i << MOBNB0);
		CANSTMOB = 0x00;            // 상태 초기화
		CANCDMOB = 0x00;            // 상태 초기화
		if(i<8)
		CANIE2 |= (1<<i);		// 해당 MOb의 인터럽트를 인에이블 시킴
		else
		CANIE1 |= (1<<(i-8));	//
	}
	
	while (!(CANGSTA & (1<<ENFG))); // CAN General Status Register (Enable Flag)
	
	CANGIE |= ((1 << ENIT)| (1 << ENRX) | (1 << ENTX)/* | (1 << ENERR) | (1 << ENBOFF) */); // 인터럽트 활성화
	
	// Wait until module ready
}

void checkCanStanderdReceive(void)
{
	if (millis() - canStandardReceiveTime >= STD_WAIT_TIME)
	{
		canStandardReceiveTime = millis();
		initCan(b500k);
	}
}

void checkCanExtendedReceive(void)
{
	if (millis() - canExtandedReceiveTime >= EXT_WAIT_TIME)
	{
		canExtandedReceiveTime = millis();
		initCan(b500k);
	}
}

void trySendCan(void)
{
	unsigned char saveCanPage = 0;
	if(sendMessageHead != sendMessageTail)
	{
		saveCanPage = CANPAGE;
		for(uint8 sendMessageBoxIndex = MOB_TX_START;sendMessageBoxIndex <= MOB_TX_END; sendMessageBoxIndex++)
		{
			CANPAGE = (sendMessageBoxIndex << MOBNB0);
			//DebugPrint ("CANCDMOB : %d\r\n",CANCDMOB);
			if (CANCDMOB == 0)
			{
				//DebugPrint ("sendCommand\r\n");
				if (sendMessage[sendMessageTail].extended)
				{
					// 확장형 ID 설정 (29비트)
					CANIDT1 = (sendMessage[sendMessageTail].id >> 21);
					CANIDT2 = (sendMessage[sendMessageTail].id >> 13);
					CANIDT3 = (sendMessage[sendMessageTail].id >> 5);
					CANIDT4 = (sendMessage[sendMessageTail].id << 3);
					CANCDMOB |= (1 << IDE);		// Set IDE bit 2.0B 29bits
				}
				else
				{
					// 표준형 ID 설정 (11비트)
					CANIDT1 = (sendMessage[sendMessageTail].id >> 3);
					CANIDT2 = (sendMessage[sendMessageTail].id << 5);
					CANIDT3 = 0x00;
					CANIDT4 = 0x00;
					CANCDMOB &= ~(1 << IDE);		// Set IDE bit 2.0A 11bits
				}
				
				CANCDMOB |= sendMessage[sendMessageTail].length;
				
				// 송신 데이터 복사
				CANMSG = sendMessage[sendMessageTail].data[0]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[1]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[2]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[3]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[4]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[5]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[6]; // 송신 데이터 로드
				CANMSG = sendMessage[sendMessageTail].data[7]; // 송신 데이터 로드
				
				//DebugPrint("sendMessageTail : %d\r\n", sendMessageTail);
				sendMessageTail = (sendMessageTail+1) % MAX_SEND_MESSAGE_BOX;
				//DebugPrint("sendMessageTail : %d\r\n", sendMessageTail);
				CANCDMOB |= (1 << CONMOB0); // 송신 모드 재설정
				// 다음 메일박스로 순환
				break;
			}
		}
		CANPAGE = saveCanPage;
	}
}

// 송신 시작 함수
void sendCan(struct MOb *mob)
{
	unsigned int nextHead = (sendMessageHead + 1) % MAX_SEND_MESSAGE_BOX;
	
	if(nextHead != sendMessageTail)
	{
		sendMessage[sendMessageHead].id = mob->id;
		sendMessage[sendMessageHead].length = mob->length;
		sendMessage[sendMessageHead].extended = mob->extended;
		sendMessage[sendMessageHead].data[0] = mob->data[0];
		sendMessage[sendMessageHead].data[1] = mob->data[1];
		sendMessage[sendMessageHead].data[2] = mob->data[2];
		sendMessage[sendMessageHead].data[3] = mob->data[3];
		sendMessage[sendMessageHead].data[4] = mob->data[4];
		sendMessage[sendMessageHead].data[5] = mob->data[5];
		sendMessage[sendMessageHead].data[6] = mob->data[6];
		sendMessage[sendMessageHead].data[7] = mob->data[7];
		sendMessageHead = nextHead;
	}
	else
	{
		//DebugPrint("SendMessageBufferFUll\r\n");
	}
	trySendCan();
}

/*parsing Exe
void parsing(void)
{
	if(receiveMessageHead != receiveMessageTail)
	{
		switch(receiveMessage[receiveMessageTail].id)
		{

			case CANID_BAT1 :
			bat1_time = BAT_TIMEOUT;
			Save_Rack_Status(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_TEMP :
			Save_Cell_Temp(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_MOD_TEMP :
			Save_Module_Temp(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_HUM :
			Save_Module_Hum(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT1_STATUS :
			Save_HVB_STS(INDEX_BAT1, receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_BAT2 :
			bat2_time = BAT_TIMEOUT;
			Save_Rack_Status(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_TEMP :
			Save_Cell_Temp(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_MOD_TEMP :
			Save_Module_Temp(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_HUM :
			Save_Module_Hum(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_BAT2_STATUS :
			Save_HVB_STS(INDEX_BAT2, receiveMessage[receiveMessageTail].data);
			break;

			case EVCC_STATUS :
			Save_EVCC_Status(receiveMessage[receiveMessageTail].data);
			break;
			case PLUG_STATUS :
			Save_PLUG_Status(receiveMessage[receiveMessageTail].data);
			break;
			case EVSE_PRESENT :
			Save_PRESENT_Value(receiveMessage[receiveMessageTail].data);
			break;
			case EVCC_MAC :
			Send_Config_Value();
			break;
			case EVSE_STATUS :
			Save_Evse_status(receiveMessage[receiveMessageTail].data);
			break;
			
			case CANID_HVMT2_CMD1:
			saveHvmtCommand1(HVMT2, receiveMessage[receiveMessageTail].data);
			break;
			case CANID_HVMT2_CMD2:
			saveHvmtCommand2(HVMT2, receiveMessage[receiveMessageTail].data);
			break;
						
			default:
			break;
		}
		receiveMessageTail = (receiveMessageTail + 1) % MAX_RECEIVE_MESSAGE_BOX;
	}
}
*/

// CAN 인터럽트 서비스 루틴
ISR(CANIT_vect)
{
	unsigned int receiveMessageNextHead = (receiveMessageHead + 1) % MAX_RECEIVE_MESSAGE_BOX;
	unsigned char saveCanPage = CANPAGE;
	CANPAGE = CANHPMOB;		// 현재 메일박스 번호 확인
	uint8_t mob_number = (CANPAGE >> 4);
	
	if (mob_number >= MOB_TX_START && mob_number <= MOB_TX_END)
	{
		if((CANSTMOB & ((1 << TXOK) | (1 << BERR) | (1 << SERR) | (1 << CERR) | (1 << FERR) | (1 << AERR)))) {
			CANSTMOB = 0x00; // 플래그 클리어
			CANCDMOB = 0x00; // MOB 비활성화
		}
	}
	if(CANSTMOB & (1<<RXOK))// 수신 처리
	{
		if(receiveMessageNextHead != receiveMessageTail)
		{
			if (mob_number >= MOB_RX_STD_START && mob_number <= MOB_RX_STD_END)
			{
				canStandardReceiveTime = 0;
				//DebugPrint("STDRXOK\r\n");
				receiveMessage[receiveMessageHead].id = ((unsigned long)CANIDT1 << 3)
														| (((unsigned long)CANIDT2 & 0xE0) >> 5);
			}
			else if (mob_number >= MOB_RX_EXT_START && mob_number <= MOB_RX_EXT_END)
			{
				canExtandedReceiveTime = 0;
				//DebugPrint("EXTRXOK\r\n");
				receiveMessage[receiveMessageHead].id = ((unsigned long)CANIDT1 << 21)
														| ((unsigned long)CANIDT2 << 13)
														| ((unsigned long)CANIDT3 << 5)
														| ((unsigned long)CANIDT4 >>3);
			}
			receiveMessage[receiveMessageHead].data[0] = CANMSG;
			receiveMessage[receiveMessageHead].data[1] = CANMSG;
			receiveMessage[receiveMessageHead].data[2] = CANMSG;
			receiveMessage[receiveMessageHead].data[3] = CANMSG;
			receiveMessage[receiveMessageHead].data[4] = CANMSG;
			receiveMessage[receiveMessageHead].data[5] = CANMSG;
			receiveMessage[receiveMessageHead].data[6] = CANMSG;
			receiveMessage[receiveMessageHead].data[7] = CANMSG;
			receiveMessageHead = receiveMessageNextHead;
		}
		CANSTMOB = 0x00;			// 상태 초기 화
		CANCDMOB |= (1<<CONMOB1); 	// 수신 IDE 유지하고 수신 모드 설정
	}
	CANPAGE = saveCanPage;
}
