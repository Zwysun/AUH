#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "float_ctrl.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "float_ctrl.h"
//unsigned char float_state;

void EXTI_Config(void);

void float_ctrl_inti(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	EXTI_Config();
}

void float_ctrl_pr(int float_command)
{
	switch (float_command)
		{
		case float_up:
			//float_state=float_up;
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
		  GPIO_ResetBits(GPIOB,GPIO_Pin_2);
			break;
		case float_down:
			//float_state=float_down;
			GPIO_SetBits(GPIOB,GPIO_Pin_2);
		  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			break;
		case float_stop:
		//	float_state=float_stop;
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		  GPIO_ResetBits(GPIOB,GPIO_Pin_2);
			break;
		default:
			;
		}


}


void EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); //开B时钟，开复用
	
	
  
  /* Configure one bit for preemption priority */
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  
   /*配置P[A|B|C|D|E]3为中断源 */
 // NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 // NVIC_Init(&NVIC_InitStructure);
	
	


	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	 /*配置P[A|B|C|D|E]4为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	 /*配置P[A|B|C|D|E]3为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	


	
	

	/**/
	
	
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入
	//GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3); 
	
 /*EXTI_InitStructure.EXTI_Line = EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	*/
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource4); 
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 
	
  /*EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource7); 
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	

}