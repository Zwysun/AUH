#include "my_scc.h"
float speed=70;
u8 motor_start=0;
u16 second=0;

u16 daka_count=0;
void set_nvic_irq(u8 Channel,u8 PreemptionPriority,u8 SubPriority)
{
	NVIC_InitTypeDef a;
	a.NVIC_IRQChannel=Channel;
	a.NVIC_IRQChannelPreemptionPriority=PreemptionPriority;
	a.NVIC_IRQChannelSubPriority=SubPriority;
	a.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&a);
}


u8 key_old=0;
u8 key_cnt=0;
u8 key_sign=0;
void key_scanf(void)
{
	u8 key_status;
  key_status=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
  if(!key_status)//本次按键状态为0 而上次状态为1 视为按下 
  {
    if(key_old) key_sign=1;
    else 
    {
      key_cnt++;//如果本次按键状态和上次状态都为0 视为长按 开始计数 长按超过800ms后 每隔80ms写一次按键标志位（20ms一次循环）
      if(key_cnt==40) key_sign=2;
      if(key_cnt>42) key_cnt=39;
    }
  }
  else key_cnt=0;
  key_old=key_status;//上次按键状态值等于本次
}

float bat_filter_old=0;
float bat_filter=0;
float battery;
void calculate_bat(void)
{
	bat_filter=ADC_ConvertedValue[1]*0.01+bat_filter_old*0.99;
	bat_filter_old=bat_filter;
	battery=bat_filter*3.3f/4096*(22/4.99+1)*1.0128;
}
