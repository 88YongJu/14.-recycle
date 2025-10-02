/*
 * throtle.c
 *
 * Created: 2023-01-31 오후 2:31:00
 *  Author: YJKIM
 */
#include "THROTTLE.h"

unsigned char Direction = THROTTLE_N;		//방향값
unsigned char ui_Throttle_Data[THROTTLE_AVR_MAX]={0,};	//데이터 누적
unsigned char Throttle_value = 0;					//평균데이터

void Init_Throttle(void)
{
	Enable_INT1();
	Init_timer0();
	set_timer(THROTTLE_CHECK_TIMER, Throttle_handler);
}

void Save_Throttle(char *MSG)
{
	Ste_RunMode = *MSG;
	Direction = *MSG;
	Throttle_value = *MSG;
}

void Get_Direction(void)
{
	if(AD_THROTTLE >= THROTTLE_FRONT_VALUD)	Direction = THROTTLE_F;
	else if(AD_THROTTLE <= THROTTLE_REAR_VALUD)	Direction = THROTTLE_R;
	else Direction = THROTTLE_N;	//87~88
}

void Get_Duty(void)
{
	unsigned int ui_Throttle_Data_sum = 0;
	unsigned char throttle_cnt = 0;
	static unsigned char st = 0, Data_cnt = 0, err_cnt = 0;
	if(st)
	{
		if (((hi_time_cnt < Throttle_value + 10) && (hi_time_cnt > Throttle_value - 10)) || (err_cnt > THROTTLE_ERR_MAX))
		{
			ui_Throttle_Data[Data_cnt++] = hi_time_cnt;
			err_cnt = 0;
		}
		else
		{
			err_cnt++;
		}
		ui_Throttle_Data[Data_cnt++] = hi_time_cnt;
	}
	else
	{
		for(Data_cnt=0;Data_cnt<THROTTLE_AVR_MAX;Data_cnt++)
		{
			ui_Throttle_Data[Data_cnt] = hi_time_cnt;
		}
		st = 1;
	}
	for(throttle_cnt = 0; throttle_cnt <THROTTLE_AVR_MAX; throttle_cnt++)
	{
		ui_Throttle_Data_sum = ui_Throttle_Data[throttle_cnt];	
	}
	Throttle_value = ui_Throttle_Data_sum/THROTTLE_AVR_MAX;
	if(Data_cnt>=THROTTLE_AVR_MAX) Data_cnt = 0;
}

void Throttle_handler(void)
{
	static unsigned long throttle_time = 0;
	if (++throttle_time >= THROTTLE_WAIT_TIME)
	{
		throttle_time = 0;
		Get_Direction();
		Get_Duty();
		struct MOb sendmsg1 = {CANID_THROTTLE, 0x00, CAN_STD, 8, {Ste_RunMode, Direction, Throttle_value, 0x00, 0x00, 0x00, 0x00, 0x00}};
		can_tx(3,&sendmsg1,0x00);
	}
}