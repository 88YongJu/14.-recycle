/*
 * ad7923.c
 *
 * Created: 2022-11-30 오후 5:47:27
 *  Author: YJKIM
 */ 

#include "AD7923.h"

unsigned long AD7923_data[AD7923_CHIP_MAX][AD7923_CH_MAX][AD_AVR_MAX];
unsigned long AD7923_Avr[AD7923_CHIP_MAX][AD7923_CH_MAX];
unsigned int adReadTime = 0;
unsigned int adAvrTime = 0;

void Init_AD7923(void)
{
	SPCR = AD7923_SPCR;
	SPSR = AD7923_SPSR;
	
	AD7923_CS1 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS1 = 1;
	
	AD7923_CS2 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS2 = 1;
	
	AD7923_CS3 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS3 = 1;
	
	AD7923_CS4 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS4 = 1;
	
	AD7923_CS5 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS5 = 1;
	
	AD7923_CS6 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS6 = 1;
	
	AD7923_CS7 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS7 = 1;
	
	AD7923_CS8 = 0;
	SPI_Write_Data(AD7923_SPI_SET1);
	SPI_Write_Data(AD7923_SPI_SET2);
	AD7923_CS8 = 1;
}

void Read_AD7923(void)
{
	unsigned int Read_DATA = 0;
	unsigned char Read_CH = 0;
	static unsigned char Read_AVR_CNT = 0;
	
	if(millis() - adReadTime >= AD_READ_DELAY)
	{
		adReadTime = millis();
		
		SPCR = AD7923_SPCR;
		SPSR = AD7923_SPSR;

		AD7923_CS1 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS1 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP1][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS1 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS1 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP1][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS1 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS1 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP1][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS1 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS1 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP1][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;

		AD7923_CS2 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS2 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP2][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS2 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS2 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP2][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS2 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS2 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP2][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS2 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS2 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP2][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		
		AD7923_CS3 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS3 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP3][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS3 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS3 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP3][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS3 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS3 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP3][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS3 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS3 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP3][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		
		AD7923_CS4 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS4 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP4][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS4 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS4 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP4][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS4 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS4 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP4][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS4 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS4 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP4][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		
		AD7923_CS5 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS5 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP5][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS5 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS5 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP5][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS5 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS5 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP5][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS5 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS5 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP5][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		
		AD7923_CS6 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS6 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP6][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS6 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS6 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP6][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS6 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS6 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP6][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS6 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS6 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP6][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		
		AD7923_CS7 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS7 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP7][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS7 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS7 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP7][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS7 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS7 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP7][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS7 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS7 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP7][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		
		AD7923_CS8 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS8 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP8][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS8 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS8 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP8][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS8 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS8 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP8][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;
		AD7923_CS8 = 0;
		Read_DATA = ((SPI_Read_Data()<<8)&0xFF00);
		Read_DATA |= ((SPI_Read_Data())&0xFF);
		AD7923_CS8 = 1;
		Read_CH = Read_DATA>>12;
		AD7923_data[AD7923_CHIP8][Read_CH][Read_AVR_CNT] = Read_DATA&0x0fff;

		if(++Read_AVR_CNT >= AD_AVR_MAX)
		{
			Read_AVR_CNT = 0;
		}
	}
}

void Avr_AD7923(void)
{
	if(millis() - adAvrTime >= AD_AVR_DELAY)
	{
		adAvrTime = millis();
		
		AD7923_Avr[0][0] = (unsigned long)(AD7923_data[0][0][0] + AD7923_data[0][0][1] + AD7923_data[0][0][2] + AD7923_data[0][0][3] + AD7923_data[0][0][4]/* + AD7923_data[0][0][5] + AD7923_data[0][0][6] + AD7923_data[0][0][7] + AD7923_data[0][0][8] + AD7923_data[0][0][9] + AD7923_data[0][0][10] + AD7923_data[0][0][11] + AD7923_data[0][0][12] + AD7923_data[0][0][13] + AD7923_data[0][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[0][1] = (unsigned long)(AD7923_data[0][1][0] + AD7923_data[0][1][1] + AD7923_data[0][1][2] + AD7923_data[0][1][3] + AD7923_data[0][1][4]/* + AD7923_data[0][1][5] + AD7923_data[0][1][6] + AD7923_data[0][1][7] + AD7923_data[0][1][8] + AD7923_data[0][1][9] + AD7923_data[0][1][10] + AD7923_data[0][1][11] + AD7923_data[0][1][12] + AD7923_data[0][1][13] + AD7923_data[0][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[0][2] = (unsigned long)(AD7923_data[0][2][0] + AD7923_data[0][2][1] + AD7923_data[0][2][2] + AD7923_data[0][2][3] + AD7923_data[0][2][4]/* + AD7923_data[0][2][5] + AD7923_data[0][2][6] + AD7923_data[0][2][7] + AD7923_data[0][2][8] + AD7923_data[0][2][9] + AD7923_data[0][2][10] + AD7923_data[0][2][11] + AD7923_data[0][2][12] + AD7923_data[0][2][13] + AD7923_data[0][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[0][3] = (unsigned long)(AD7923_data[0][3][0] + AD7923_data[0][3][1] + AD7923_data[0][3][2] + AD7923_data[0][3][3] + AD7923_data[0][3][4]/* + AD7923_data[0][3][5] + AD7923_data[0][3][6] + AD7923_data[0][3][7] + AD7923_data[0][3][8] + AD7923_data[0][3][9] + AD7923_data[0][3][10] + AD7923_data[0][3][11] + AD7923_data[0][3][12] + AD7923_data[0][3][13] + AD7923_data[0][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[1][0] = (unsigned long)(AD7923_data[1][0][0] + AD7923_data[1][0][1] + AD7923_data[1][0][2] + AD7923_data[1][0][3] + AD7923_data[1][0][4]/* + AD7923_data[1][0][5] + AD7923_data[1][0][6] + AD7923_data[1][0][7] + AD7923_data[1][0][8] + AD7923_data[1][0][9] + AD7923_data[1][0][10] + AD7923_data[1][0][11] + AD7923_data[1][0][12] + AD7923_data[1][0][13] + AD7923_data[1][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[1][1] = (unsigned long)(AD7923_data[1][1][0] + AD7923_data[1][1][1] + AD7923_data[1][1][2] + AD7923_data[1][1][3] + AD7923_data[1][1][4]/* + AD7923_data[1][1][5] + AD7923_data[1][1][6] + AD7923_data[1][1][7] + AD7923_data[1][1][8] + AD7923_data[1][1][9] + AD7923_data[1][1][10] + AD7923_data[1][1][11] + AD7923_data[1][1][12] + AD7923_data[1][1][13] + AD7923_data[1][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[1][2] = (unsigned long)(AD7923_data[1][2][0] + AD7923_data[1][2][1] + AD7923_data[1][2][2] + AD7923_data[1][2][3] + AD7923_data[1][2][4]/* + AD7923_data[1][2][5] + AD7923_data[1][2][6] + AD7923_data[1][2][7] + AD7923_data[1][2][8] + AD7923_data[1][2][9] + AD7923_data[1][2][10] + AD7923_data[1][2][11] + AD7923_data[1][2][12] + AD7923_data[1][2][13] + AD7923_data[1][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[1][3] = (unsigned long)(AD7923_data[1][3][0] + AD7923_data[1][3][1] + AD7923_data[1][3][2] + AD7923_data[1][3][3] + AD7923_data[1][3][4]/* + AD7923_data[1][3][5] + AD7923_data[1][3][6] + AD7923_data[1][3][7] + AD7923_data[1][3][8] + AD7923_data[1][3][9] + AD7923_data[1][3][10] + AD7923_data[1][3][11] + AD7923_data[1][3][12] + AD7923_data[1][3][13] + AD7923_data[1][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[2][0] = (unsigned long)(AD7923_data[2][0][0] + AD7923_data[2][0][1] + AD7923_data[2][0][2] + AD7923_data[2][0][3] + AD7923_data[2][0][4]/* + AD7923_data[2][0][5] + AD7923_data[2][0][6] + AD7923_data[2][0][7] + AD7923_data[2][0][8] + AD7923_data[2][0][9] + AD7923_data[2][0][10] + AD7923_data[2][0][11] + AD7923_data[2][0][12] + AD7923_data[2][0][13] + AD7923_data[2][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[2][1] = (unsigned long)(AD7923_data[2][1][0] + AD7923_data[2][1][1] + AD7923_data[2][1][2] + AD7923_data[2][1][3] + AD7923_data[2][1][4]/* + AD7923_data[2][1][5] + AD7923_data[2][1][6] + AD7923_data[2][1][7] + AD7923_data[2][1][8] + AD7923_data[2][1][9] + AD7923_data[2][1][10] + AD7923_data[2][1][11] + AD7923_data[2][1][12] + AD7923_data[2][1][13] + AD7923_data[2][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[2][2] = (unsigned long)(AD7923_data[2][2][0] + AD7923_data[2][2][1] + AD7923_data[2][2][2] + AD7923_data[2][2][3] + AD7923_data[2][2][4]/* + AD7923_data[2][2][5] + AD7923_data[2][2][6] + AD7923_data[2][2][7] + AD7923_data[2][2][8] + AD7923_data[2][2][9] + AD7923_data[2][2][10] + AD7923_data[2][2][11] + AD7923_data[2][2][12] + AD7923_data[2][2][13] + AD7923_data[2][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[2][3] = (unsigned long)(AD7923_data[2][3][0] + AD7923_data[2][3][1] + AD7923_data[2][3][2] + AD7923_data[2][3][3] + AD7923_data[2][3][4]/* + AD7923_data[2][3][5] + AD7923_data[2][3][6] + AD7923_data[2][3][7] + AD7923_data[2][3][8] + AD7923_data[2][3][9] + AD7923_data[2][3][10] + AD7923_data[2][3][11] + AD7923_data[2][3][12] + AD7923_data[2][3][13] + AD7923_data[2][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[3][0] = (unsigned long)(AD7923_data[3][0][0] + AD7923_data[3][0][1] + AD7923_data[3][0][2] + AD7923_data[3][0][3] + AD7923_data[3][0][4]/* + AD7923_data[3][0][5] + AD7923_data[3][0][6] + AD7923_data[3][0][7] + AD7923_data[3][0][8] + AD7923_data[3][0][9] + AD7923_data[3][0][10] + AD7923_data[3][0][11] + AD7923_data[3][0][12] + AD7923_data[3][0][13] + AD7923_data[3][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[3][1] = (unsigned long)(AD7923_data[3][1][0] + AD7923_data[3][1][1] + AD7923_data[3][1][2] + AD7923_data[3][1][3] + AD7923_data[3][1][4]/* + AD7923_data[3][1][5] + AD7923_data[3][1][6] + AD7923_data[3][1][7] + AD7923_data[3][1][8] + AD7923_data[3][1][9] + AD7923_data[3][1][10] + AD7923_data[3][1][11] + AD7923_data[3][1][12] + AD7923_data[3][1][13] + AD7923_data[3][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[3][2] = (unsigned long)(AD7923_data[3][2][0] + AD7923_data[3][2][1] + AD7923_data[3][2][2] + AD7923_data[3][2][3] + AD7923_data[3][2][4]/* + AD7923_data[3][2][5] + AD7923_data[3][2][6] + AD7923_data[3][2][7] + AD7923_data[3][2][8] + AD7923_data[3][2][9] + AD7923_data[3][2][10] + AD7923_data[3][2][11] + AD7923_data[3][2][12] + AD7923_data[3][2][13] + AD7923_data[3][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[3][3] = (unsigned long)(AD7923_data[3][3][0] + AD7923_data[3][3][1] + AD7923_data[3][3][2] + AD7923_data[3][3][3] + AD7923_data[3][3][4]/* + AD7923_data[3][3][5] + AD7923_data[3][3][6] + AD7923_data[3][3][7] + AD7923_data[3][3][8] + AD7923_data[3][3][9] + AD7923_data[3][3][10] + AD7923_data[3][3][11] + AD7923_data[3][3][12] + AD7923_data[3][3][13] + AD7923_data[3][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[4][0] = (unsigned long)(AD7923_data[4][0][0] + AD7923_data[4][0][1] + AD7923_data[4][0][2] + AD7923_data[4][0][3] + AD7923_data[4][0][4]/* + AD7923_data[4][0][5] + AD7923_data[4][0][6] + AD7923_data[4][0][7] + AD7923_data[4][0][8] + AD7923_data[4][0][9] + AD7923_data[4][0][10] + AD7923_data[4][0][11] + AD7923_data[4][0][12] + AD7923_data[4][0][13] + AD7923_data[4][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[4][1] = (unsigned long)(AD7923_data[4][1][0] + AD7923_data[4][1][1] + AD7923_data[4][1][2] + AD7923_data[4][1][3] + AD7923_data[4][1][4]/* + AD7923_data[4][1][5] + AD7923_data[4][1][6] + AD7923_data[4][1][7] + AD7923_data[4][1][8] + AD7923_data[4][1][9] + AD7923_data[4][1][10] + AD7923_data[4][1][11] + AD7923_data[4][1][12] + AD7923_data[4][1][13] + AD7923_data[4][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[4][2] = (unsigned long)(AD7923_data[4][2][0] + AD7923_data[4][2][1] + AD7923_data[4][2][2] + AD7923_data[4][2][3] + AD7923_data[4][2][4]/* + AD7923_data[4][2][5] + AD7923_data[4][2][6] + AD7923_data[4][2][7] + AD7923_data[4][2][8] + AD7923_data[4][2][9] + AD7923_data[4][2][10] + AD7923_data[4][2][11] + AD7923_data[4][2][12] + AD7923_data[4][2][13] + AD7923_data[4][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[4][3] = (unsigned long)(AD7923_data[4][3][0] + AD7923_data[4][3][1] + AD7923_data[4][3][2] + AD7923_data[4][3][3] + AD7923_data[4][3][4]/* + AD7923_data[4][3][5] + AD7923_data[4][3][6] + AD7923_data[4][3][7] + AD7923_data[4][3][8] + AD7923_data[4][3][9] + AD7923_data[4][3][10] + AD7923_data[4][3][11] + AD7923_data[4][3][12] + AD7923_data[4][3][13] + AD7923_data[4][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[5][0] = (unsigned long)(AD7923_data[5][0][0] + AD7923_data[5][0][1] + AD7923_data[5][0][2] + AD7923_data[5][0][3] + AD7923_data[5][0][4]/* + AD7923_data[5][0][5] + AD7923_data[5][0][6] + AD7923_data[5][0][7] + AD7923_data[5][0][8] + AD7923_data[5][0][9] + AD7923_data[5][0][10] + AD7923_data[5][0][11] + AD7923_data[5][0][12] + AD7923_data[5][0][13] + AD7923_data[5][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[5][1] = (unsigned long)(AD7923_data[5][1][0] + AD7923_data[5][1][1] + AD7923_data[5][1][2] + AD7923_data[5][1][3] + AD7923_data[5][1][4]/* + AD7923_data[5][1][5] + AD7923_data[5][1][6] + AD7923_data[5][1][7] + AD7923_data[5][1][8] + AD7923_data[5][1][9] + AD7923_data[5][1][10] + AD7923_data[5][1][11] + AD7923_data[5][1][12] + AD7923_data[5][1][13] + AD7923_data[5][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[5][2] = (unsigned long)(AD7923_data[5][2][0] + AD7923_data[5][2][1] + AD7923_data[5][2][2] + AD7923_data[5][2][3] + AD7923_data[5][2][4]/* + AD7923_data[5][2][5] + AD7923_data[5][2][6] + AD7923_data[5][2][7] + AD7923_data[5][2][8] + AD7923_data[5][2][9] + AD7923_data[5][2][10] + AD7923_data[5][2][11] + AD7923_data[5][2][12] + AD7923_data[5][2][13] + AD7923_data[5][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[5][3] = (unsigned long)(AD7923_data[5][3][0] + AD7923_data[5][3][1] + AD7923_data[5][3][2] + AD7923_data[5][3][3] + AD7923_data[5][3][4]/* + AD7923_data[5][3][5] + AD7923_data[5][3][6] + AD7923_data[5][3][7] + AD7923_data[5][3][8] + AD7923_data[5][3][9] + AD7923_data[5][3][10] + AD7923_data[5][3][11] + AD7923_data[5][3][12] + AD7923_data[5][3][13] + AD7923_data[5][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[6][0] = (unsigned long)(AD7923_data[6][0][0] + AD7923_data[6][0][1] + AD7923_data[6][0][2] + AD7923_data[6][0][3] + AD7923_data[6][0][4]/* + AD7923_data[6][0][5] + AD7923_data[6][0][6] + AD7923_data[6][0][7] + AD7923_data[6][0][8] + AD7923_data[6][0][9] + AD7923_data[6][0][10] + AD7923_data[6][0][11] + AD7923_data[6][0][12] + AD7923_data[6][0][13] + AD7923_data[6][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[6][1] = (unsigned long)(AD7923_data[6][1][0] + AD7923_data[6][1][1] + AD7923_data[6][1][2] + AD7923_data[6][1][3] + AD7923_data[6][1][4]/* + AD7923_data[6][1][5] + AD7923_data[6][1][6] + AD7923_data[6][1][7] + AD7923_data[6][1][8] + AD7923_data[6][1][9] + AD7923_data[6][1][10] + AD7923_data[6][1][11] + AD7923_data[6][1][12] + AD7923_data[6][1][13] + AD7923_data[6][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[6][2] = (unsigned long)(AD7923_data[6][2][0] + AD7923_data[6][2][1] + AD7923_data[6][2][2] + AD7923_data[6][2][3] + AD7923_data[6][2][4]/* + AD7923_data[6][2][5] + AD7923_data[6][2][6] + AD7923_data[6][2][7] + AD7923_data[6][2][8] + AD7923_data[6][2][9] + AD7923_data[6][2][10] + AD7923_data[6][2][11] + AD7923_data[6][2][12] + AD7923_data[6][2][13] + AD7923_data[6][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[6][3] = (unsigned long)(AD7923_data[6][3][0] + AD7923_data[6][3][1] + AD7923_data[6][3][2] + AD7923_data[6][3][3] + AD7923_data[6][3][4]/* + AD7923_data[6][3][5] + AD7923_data[6][3][6] + AD7923_data[6][3][7] + AD7923_data[6][3][8] + AD7923_data[6][3][9] + AD7923_data[6][3][10] + AD7923_data[6][3][11] + AD7923_data[6][3][12] + AD7923_data[6][3][13] + AD7923_data[6][3][14]*/)/AD_AVR_MAX;
		/*																																																											 */
		AD7923_Avr[7][0] = (unsigned long)(AD7923_data[7][0][0] + AD7923_data[7][0][1] + AD7923_data[7][0][2] + AD7923_data[7][0][3] + AD7923_data[7][0][4]/* + AD7923_data[7][0][5] + AD7923_data[7][0][6] + AD7923_data[7][0][7] + AD7923_data[7][0][8] + AD7923_data[7][0][9] + AD7923_data[7][0][10] + AD7923_data[7][0][11] + AD7923_data[7][0][12] + AD7923_data[7][0][13] + AD7923_data[7][0][14]*/)/AD_AVR_MAX;
		AD7923_Avr[7][1] = (unsigned long)(AD7923_data[7][1][0] + AD7923_data[7][1][1] + AD7923_data[7][1][2] + AD7923_data[7][1][3] + AD7923_data[7][1][4]/* + AD7923_data[7][1][5] + AD7923_data[7][1][6] + AD7923_data[7][1][7] + AD7923_data[7][1][8] + AD7923_data[7][1][9] + AD7923_data[7][1][10] + AD7923_data[7][1][11] + AD7923_data[7][1][12] + AD7923_data[7][1][13] + AD7923_data[7][1][14]*/)/AD_AVR_MAX;
		AD7923_Avr[7][2] = (unsigned long)(AD7923_data[7][2][0] + AD7923_data[7][2][1] + AD7923_data[7][2][2] + AD7923_data[7][2][3] + AD7923_data[7][2][4]/* + AD7923_data[7][2][5] + AD7923_data[7][2][6] + AD7923_data[7][2][7] + AD7923_data[7][2][8] + AD7923_data[7][2][9] + AD7923_data[7][2][10] + AD7923_data[7][2][11] + AD7923_data[7][2][12] + AD7923_data[7][2][13] + AD7923_data[7][2][14]*/)/AD_AVR_MAX;
		AD7923_Avr[7][3] = (unsigned long)(AD7923_data[7][3][0] + AD7923_data[7][3][1] + AD7923_data[7][3][2] + AD7923_data[7][3][3] + AD7923_data[7][3][4]/* + AD7923_data[7][3][5] + AD7923_data[7][3][6] + AD7923_data[7][3][7] + AD7923_data[7][3][8] + AD7923_data[7][3][9] + AD7923_data[7][3][10] + AD7923_data[7][3][11] + AD7923_data[7][3][12] + AD7923_data[7][3][13] + AD7923_data[7][3][14]*/)/AD_AVR_MAX;
	}
}