#include "twi.h"

volatile unsigned char TWI_data0;
volatile unsigned char TWI_RxBuf0[TWI_RX_BUFFER_SIZE0];
volatile unsigned char TWI_RxHead0;
volatile unsigned char TWI_RxTail0;
volatile unsigned char TWI_AdBuf0[TWI_TX_BUFFER_SIZE0];
volatile unsigned char TWI_AdOld0;
volatile unsigned char TWI_TxBuf0[TWI_TX_BUFFER_SIZE0];
volatile unsigned char TWI_TxHead0;
volatile unsigned char TWI_TxTail0;
volatile unsigned char TWI_Rxflag0 = 0;

volatile unsigned int TWI_WD = 0;
volatile unsigned char TWI_busy0 = 0;
volatile unsigned char TWI_err_cnt = 0;
volatile unsigned char TWIR_flag0 = 0;
volatile unsigned char TWIR_cnt0 = 0;
volatile unsigned char TWIR_data0[25];
volatile unsigned char TWIR_c0 = 0;
volatile unsigned char TS_flag = 0;
volatile unsigned char TWI_all0 = 0;

volatile unsigned int TWIR_dly0;

volatile unsigned char T_IO_FLAG;
volatile unsigned int T_DEST_ID;
volatile unsigned int T_CDS1;
volatile unsigned int T_CDS2;
volatile unsigned int T_TMP_SET;
volatile unsigned int T_TMP;
volatile unsigned int T_PIR_SET;
volatile unsigned int T_PIR_TIME;
volatile unsigned int T_LED1_OUT;
volatile unsigned int T_LED2_OUT;
volatile unsigned int T_LED1_COM;
volatile unsigned int T_LED2_COM;
volatile unsigned int T_LED1_PIR;
volatile unsigned int T_LED2_PIR;
volatile unsigned long T_IROUT;



void Twi_initialize0(void)
{
	TWBR = 25;
	TWSR = 0b00000011;
//	TWAR = (2<<1) + 1;
	TWAR = 0x00;
	TWCR = ((1<<TWEA) | (1<<TWEN) | (1<<TWIE));

	TWI_RxTail0 = 0;
	TWI_RxHead0 = 0;
	TWI_TxTail0 = 0;
	TWI_TxHead0 = 0;
}

void TWI_write0(unsigned char addr0, unsigned char data0)
{

	unsigned char tmphead0;

	tmphead0 = (TWI_TxHead0 + 1) & TWI_TX_BUFFER_MASK0;

	if (tmphead0 != TWI_TxTail0)
	{
		TWI_AdBuf0[tmphead0] = addr0;
		TWI_TxBuf0[tmphead0] = data0;
		TWI_TxHead0 = tmphead0;
	}
}

void TWI_Read0(void)
{
	unsigned char tmptail0;

	if(TWI_RxHead0 != TWI_RxTail0)
	{
		tmptail0 = (TWI_RxTail0 + 1) & TWI_RX_BUFFER_MASK0;
		TWI_RxTail0 = tmptail0;
		TWI_data0 = TWI_RxBuf0[tmptail0];
		TWI_Rxflag0 = 1;
	}
	else
	{
		TWI_Rxflag0 = 0;
	}
}

void TWI_InOut0(void)
{
	if(TWIR_flag0 == 0)
	{
		TWI_Read0();

		if(TWI_Rxflag0 == 1)
		{
			TWIR_data0[TWIR_cnt0] = TWI_data0;
			TWIR_cnt0++;
			
			if(TWIR_cnt0 > 24)
			{
				TWIR_cnt0 = 25;
			}
			
			switch(TWI_data0)
			{
				case 0x00 : 
					if (TWIR_cnt0 >= 4)
					{
						TWIR_flag0 = 1;
						TWIR_cnt0 = 0;
						TS_flag = 0;
					}
					break;

				default :
					break;
			}
		
			TWI_Rxflag0 = 0;
		}
	}	

	if (TWI_err_cnt == 0)
	{
		if ((TWI_busy0 == 0) && (TWI_TxHead0 != TWI_TxTail0))
		{
			TWCR = ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWIE));
		}
	}
	else if (TWI_err_cnt == 1)
	{
		TWCR = ((1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE));
		TWCR |= ((1<<TWINT) | (1<<TWSTA) | (1<<TWSTO) | (1<<TWIE));
		TWI_err_cnt = 0;
	}
	else
	{
		TWI_err_cnt--;
	}
/*
	if (TWI_WD > 40000)
	{
		TWCR = ((1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE));
		TWI_err_cnt = 1;
	}
	else
	{
		TWI_WD++;
	}
*/
}


void TWI_Command0(void)
{

	if(TWIR_flag0 == 1)
	{
		T_IO_FLAG = TWIR_data0[0];
	    T_DEST_ID = (TWIR_data0[2]) | (TWIR_data0[1]<<8);

		switch(TWIR_data0[3])
		{
			case 'A' : 

				TWIR_c0 = 1;
			break;


			default :
				TWIR_c0 = 0;
			break;
		}

		switch(TWIR_c0)
		{
			case 1 :

			break;

			case 14 :

			break;


			default :

			break;
		}

		TWIR_c0 = 0;

		TWIR_flag0 = 0;

		for(int i=0; i < 25; i++)
		{
			TWIR_data0[i] = 0;
		}
	}
}

ISR(TWI_vect)
{
	cli();

	TWI_busy0 = 1;

	unsigned char data0;
	unsigned char tmphead0;
	unsigned char tmptail0;

	switch((TWSR & 0xF8))
	{
		case 0x08 : // 0x08 TW_START

//			TransmitByte('A');

			if(TWI_TxHead0 != TWI_TxTail0)
			{
				tmptail0 = (TWI_TxTail0 + 1)&TWI_TX_BUFFER_MASK0;
				TWI_AdOld0 = TWI_AdBuf0[tmptail0];

				TWDR = (TWI_AdBuf0[tmptail0]<<1) + TWI_all0;
				TWCR &= ~(1<<TWSTA);
			}
			else
			{
				TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
				TWI_busy0 = 0;
			}

		break;

		case 0x10 : // 0x10 TW_REP_START

//			TransmitByte('B');

			if(TWI_TxHead0 != TWI_TxTail0)
			{
				tmptail0 = (TWI_TxTail0 + 1)&TWI_TX_BUFFER_MASK0;
				TWI_AdOld0 = TWI_AdBuf0[tmptail0];

				TWDR = (TWI_AdBuf0[tmptail0]<<1) + TWI_all0;
				TWCR &= ~(1<<TWSTA);
			}
			else
			{
				TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
				TWI_busy0 = 0;
			}

		break;

		case 0x18 : // 0x18 TW_MT_SLA_ACK

//			TransmitByte('C');

			if(TWI_TxHead0 != TWI_TxTail0)
			{
				tmptail0 = (TWI_TxTail0 + 1)&TWI_TX_BUFFER_MASK0;
				if (TWI_AdBuf0[tmptail0] == TWI_AdOld0)
				{
					TWI_TxTail0 = tmptail0;

					TWDR = TWI_TxBuf0[tmptail0];

					TWCR |= (1<<TWINT);
				}
				else
				{
					TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
					TWI_busy0 = 0;
				}
			}
			else
			{
				TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
				TWI_busy0 = 0;
			}

		break;

		case 0x20 : // 0x20 TW_MT_SLA_NACK

//			TransmitByte('D');
			
			TWCR = ((1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE));
			TWI_err_cnt = 250;
			TWI_busy0 = 0;

		break;

		case 0x28 : // 0x28 TW_MT_DATA_ACK

//			TransmitByte('E');

			if(TWI_TxHead0 != TWI_TxTail0)
			{
				tmptail0 = (TWI_TxTail0 + 1)&TWI_TX_BUFFER_MASK0;
				if (TWI_AdBuf0[tmptail0] == TWI_AdOld0)
				{
					TWI_TxTail0 = tmptail0;

					TWDR = TWI_TxBuf0[tmptail0];

					TWCR |= (1<<TWINT);

				}
				else
				{
					TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
					TWI_busy0 = 0;
				}
			}
			else
			{
				TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
				TWI_busy0 = 0;
			}

		break;

		case 0x30 : // 0x30 TW_MT_DATA_NACK

//			TransmitByte('F');

			TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
			TWI_busy0 = 0;

		break;

		case 0x38 : // 0x38 ARBITRATION ERR

//			TransmitByte('G');

//			TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWIE));
			TWCR = ((1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE));
			TWI_err_cnt = 250;

			TWI_busy0 = 0;

		break;

		case 0x40 : // 0x40 TW_MR_SLA_ACK

//			TransmitByte('H');

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;

		case 0x48 : // 0x48 TW_MR_SLA_NACK

//			TransmitByte('I');

			TWCR |= ((1<<TWINT) | (1<<TWSTA) | (1<<TWSTO));
			TWI_busy0 = 0;

		break;

		case 0x50 : // 0x50 TW_MR_DATA_ACK

//			TransmitByte('J');

			data0 = TWDR;
			tmphead0 = (TWI_RxHead0 + 1) & TWI_RX_BUFFER_MASK0;
			TWI_RxHead0 = tmphead0;
			TWI_RxBuf0[tmphead0] = data0;

			TWCR |= ((1<<TWINT) | (1<<TWEA));
			TWI_busy0 = 0;

		break;

		case 0x58 : // 0x58 TW_MR_DATA_NACK

//			TransmitByte('K');

			TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
			TWI_busy0 = 0;

		break;

		case 0x60: // 0x60 RCV_START

//			TransmitByte('L');

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;

		case 0x68: // 0x68 RCV_ARBITRATION ERR

//			TransmitByte('M');

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;
	
		case 0x70: // 0x70 GC_RCV_START

//			TransmitByte('N');

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;

		case 0x78: // 0x78 GC_RCV_ARBITRATION ERR

//			TransmitByte('O');

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;

		case 0x80: //0x80 RCV_DATA

//			TransmitByte('P');

			data0 = TWDR;
			tmphead0 = (TWI_RxHead0 + 1) & TWI_RX_BUFFER_MASK0;
			TWI_RxHead0 = tmphead0;
			TWI_RxBuf0[tmphead0] = data0;

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;

		case 0x88: //0x88 RCV_NACK

//			TransmitByte('Q');

			data0 = TWDR;
			tmphead0 = (TWI_RxHead0 + 1) & TWI_RX_BUFFER_MASK0;
			TWI_RxHead0 = tmphead0;
			TWI_RxBuf0[tmphead0] = data0;

			TWCR |= ((1<<TWINT) | (1<<TWEA));
			TWI_busy0 = 0;

		break;

		case 0x90: //0x90 GC_RCV_DATA

//			TransmitByte('R');

			data0 = TWDR;
			tmphead0 = (TWI_RxHead0 + 1) & TWI_RX_BUFFER_MASK0;
			TWI_RxHead0 = tmphead0;
			TWI_RxBuf0[tmphead0] = data0;

			TWCR |= ((1<<TWINT) | (1<<TWEA));

		break;

		case 0x98: //0x98 GC_RCV_NACK

//			TransmitByte('S');

			data0 = TWDR;
			tmphead0 = (TWI_RxHead0 + 1) & TWI_RX_BUFFER_MASK0;
			TWI_RxHead0 = tmphead0;
			TWI_RxBuf0[tmphead0] = data0;

			TWCR |= ((1<<TWINT) | (1<<TWEA));
			TWI_busy0 = 0;

		break;

		case 0xa0: //0xa0 RCV_ACK

//			TransmitByte('T');

			TWCR |= ((1<<TWINT) | (1<<TWEA));
			TWI_busy0 = 0;

		break;

		case 0x00:

//			TransmitByte('U');

			TWCR |= ((1<<TWINT) | (1<<TWEA) | (1<<TWSTO));
			TWI_busy0 = 0;

		break;

		default :

//			TransmitByte('V');

			TWCR = ((1<<TWINT) | (1<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (1<<TWEN) | (1<<TWIE));
			TWI_err_cnt = 250;
			TWI_busy0 = 0;

		break;
	}

//	TWI_WD = 0;

	sei();
}


