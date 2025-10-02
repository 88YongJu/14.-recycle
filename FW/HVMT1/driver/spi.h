#ifndef SPI_H_
#define SPI_H_

////////////////////////////////////////////////////////////
// INCLUDE STANDERD HEAER FILE /////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>
#include "exiom.h"

#define SPI_CLK		portb.bit1
#define SPI_OUT		portb.bit2
#define SPI_IN		pinb.bit3

/*///////////////////////////////////////////////////////
// DEFINE I/O PORT
///////////////////////////////////////////////////////*/
#define SPI_PORT_DIR_R			DDRB
#define SPI_SCLK_DIR			DDB1
#define SPI_SCLK				PB1
#define SPI_TX_DIR				DDB2
#define SPI_TX					PB2
#define SPI_RX_DIR				DDB3
#define SPI_RX					PB3

/*///////////////////////////////////////////////////////
// DEFINE COMMAND
///////////////////////////////////////////////////////*/
#define SPI_INTERRUPT_OFF	0
#define SPI_INTERRUPT_ON	1

#define SPI_BIG				0
#define SPI_LITTLE			1

#define SPI_SLAVE	0
#define SPI_MASTER	1

#define FREQUNENCY_2	SPCR |= (0<<SPR1)|(0<<SPR0)\
						SPSR |= (1<<SPI2X)
#define FREQUNENCY_4	SPCR |= (0<<SPR1)|(0<<SPR0)\
						SPSR |= (0<<SPI2X)
#define FREQUNENCY_8	SPCR |= (0<<SPR1)|(1<<SPR0)\
						SPSR |= (1<<SPI2X)
#define FREQUNENCY_16	SPCR |= (0<<SPR1)|(1<<SPR0)\
						SPSR |= (0<<SPI2X)
#define FREQUNENCY_32	SPCR |= (1<<SPR1)|(0<<SPR0)\
						SPSR |= (1<<SPI2X)
#define FREQUNENCY_64	SPCR |= (1<<SPR1)|(0<<SPR0)
#define FREQUNENCY_128	SPCR |= (1<<SPR1)|(1<<SPR0)

// Erase, Program Instructions /////////////////////////////////
#define SPI_Write_Enable			0x06
#define SPI_Write_Disable			0x04
#define SPI_Read_Status_Reg1		0x05  // s7-s0
#define SPI_Read_Status_Reg2		0x35  // s15-s8
#define SPI_Write_Status_Reg		0x01  // s7-s0	  / s15-s8
#define SPI_Page_Program			0x02  // a23-a16 / a15-a8  / a7-a0	/ d7-d0
#define SPI_Sector_Erase_4KB		0x20  // a23-a16 / a15-a8  / a7-a0	/ d7-d0
#define SPI_Block_Erase_32KB		0x52  // a23-a16 / a15-a8  / a7-a0	/ d7-d0
#define SPI_Block_Erase_64KB		0xD8  // a23-a16 / a15-a8  / a7-a0	/ d7-d0
#define SPI_Chip_Erase1 			0xc7
#define SPI_Chip_Erase2 			0x60
#define SPI_Erase_Suspend			0x75
#define SPI_Erase_Resume			0x75
#define SPI_Power_Down				0xB9
#define SPI_CRM_Reset				0xff // continuous read mode reset
// Read Instructions ////////////////////////////////////////
#define Read_Data					0x03  // a23-a16 / a15-a8  / a7-a0	/ (d7-d0)
#define Fast_Read					0x0b  // a23-a16 / a15-a8  / a7-a0	/ dummy / (d7-d0)
// ID, Security instructions //////////////////////////////////
#define RPD_Device_ID				0xAB  // dummy / dummy / dummy / (ID7-ID0)
#define Manufactruer_Device_ID		0x90  // dummy / dummy / dummy / 00h		  / (MF7-MF0) / (ID7-ID0)
#define JEDEC_ID					0x9f  // (MF7-MF0)		 /	(ID15-ID8)	  / (ID7-ID0)
// manufactrure / memory type / capacity

/*///////////////////////////////////////////////////////
// FUNCTION  DEFINITION
///////////////////////////////////////////////////////*/
uint8_t writeSpi(uint8_t byte);

#endif

