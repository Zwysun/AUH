#include "led.h"

int led_blink=0;

//LED IO初始化
//IO输出高电平时LED两起
//主要作用是指示电源和打卡情况
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}


void LED_Modify(int pattern)
{
	if(pattern==LIGHTEN_ODD_PORT)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4);
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		GPIO_SetBits(GPIOE,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5);	
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
	}	
	else if(pattern==LIGHTEN_EVEN_PORT)
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4);	
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
	}
	else if(pattern==LIGHTEN_ALL)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
		GPIO_SetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	}
	else if(pattern==SHUTDOWN_ALL)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
		GPIO_ResetBits(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
	}
}

void TIM2_LEDIndicator_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM7, ENABLE);  //使能TIMx
}

void TIM2_IRQHandler(void)
{
	 static int led_pattern=LIGHTEN_ALL;
	 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM7更新中断发生与否
	 {
		if(led_blink)
		{
			if(led_pattern==LIGHTEN_ALL)
				led_pattern=SHUTDOWN_ALL;
			else
				led_pattern=LIGHTEN_ALL;
			
			LED_Modify(led_pattern);
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
	 }
}
