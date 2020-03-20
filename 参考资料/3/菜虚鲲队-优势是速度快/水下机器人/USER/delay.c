#include "delay.h"
void delay_init(void)
{
  NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1); 
  SysTick->VAL=0;                                         
	SysTick->CTRL=0;
}
void delay(u32 time)
{    
	u32 temp;	
  SysTick->LOAD=(time&SysTick_LOAD_RELOAD_Msk)-1;
	SysTick->VAL=0;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;//开启滴答定时器
	do
	{
		temp=SysTick->CTRL;
	}
	while(!(temp&SysTick_CTRL_COUNTFLAG_Msk));//等待时间到达
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//关闭滴答定时器
}
void delay_sec(u32 second)//秒延时
{
	u32 temp;
	u32 delay_count;
	delay_count=second;
	SysTick->LOAD=(9000000&SysTick_LOAD_RELOAD_Msk)-1;
	SysTick->VAL=0;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;//开启滴答定时器 开启中断
	do
	{
		do
	  {
		  temp=SysTick->CTRL;
	  }
		while(!(temp&SysTick_CTRL_COUNTFLAG_Msk));//等待时间到达
	}
	while(delay_count--);
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;//关闭滴答定时器
}

