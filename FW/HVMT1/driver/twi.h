#ifndef	TWI_H_
#define TWI_H_

#include <avr/io.h>

void Twi_initialize0(void);
void TWI_write0(unsigned char addr, unsigned char data);
void TWI_Read0(void);
void TWI_InOut0(void);
void TWI_Command0(void);

#define TWI_RX_BUFFER_SIZE0 32
#define TWI_RX_BUFFER_MASK0 (TWI_RX_BUFFER_SIZE0 - 1)
#if (TWI_RX_BUFFER_SIZE0 & TWI_RX_BUFFER_MASK0)
#error RX buffer size is not a power of 2
#endif

#define TWI_TX_BUFFER_SIZE0 32
#define TWI_TX_BUFFER_MASK0 (TWI_TX_BUFFER_SIZE0 - 1)
#if (TWI_TX_BUFFER_SIZE0 & TWI_TX_BUFFER_MASK0)
#error TX buffer size is not a power of 2
#endif


#endif

extern volatile unsigned char TWI_TxHead0;
extern volatile unsigned char TWI_TxTail0;
extern volatile unsigned char TWI_busy0;

extern volatile unsigned char T_IO_FLAG;
extern volatile unsigned int T_DEST_ID;
extern volatile unsigned int T_CDS1;
extern volatile unsigned int T_CDS2;
extern volatile unsigned int T_TMP_SET;
extern volatile unsigned int T_TMP;
extern volatile unsigned int T_PIR_SET;
extern volatile unsigned int T_PIR_TIME;
extern volatile unsigned int T_LED1_OUT;
extern volatile unsigned int T_LED2_OUT;
extern volatile unsigned int T_LED1_COM;
extern volatile unsigned int T_LED2_COM;
extern volatile unsigned int T_LED1_PIR;
extern volatile unsigned int T_LED2_PIR;
extern volatile unsigned long T_IROUT;
