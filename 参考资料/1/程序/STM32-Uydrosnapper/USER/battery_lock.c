#include "sys.h"
#include "delay.h"
void Battery_lock_release(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 EXTI_InitTypeDef EXTI_InitStructure;
 NVIC_InitTypeDef NVIC_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);

 delay_ms(100);
 GPIO_SetBits(GPIOC,GPIO_Pin_3);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);
	
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);

 EXTI_InitStructure.EXTI_Line=EXTI_Line2;
 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
 NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//使能按键KEY2所在的外部中断通道
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级2， 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级2
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
 NVIC_Init(&NVIC_InitStructure);
}

void EXTI2_IRQHandler(void)
{
//	static u8 sw = 0;
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)//中断标志位
	{
		EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE4上的中断标志位  
	/*	if(PCin(3)==0)
		{
			if(sw == 0)
			{
				PCout(3)=1;//开机 - 锁存
				sw = 1;
			}
			else
			{
				PCout(3)=0;//关机	
				sw = 0;			
			}
		}*/
		if(PCin(3)==0)
			PCout(3)=1;
		else
			PCout(3)=0;		
	}
}
 

