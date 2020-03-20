#include "led.h"

int led_blink=0;

//LED IO��ʼ��
//IO����ߵ�ƽʱLED����
//��Ҫ������ָʾ��Դ�ʹ����
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);
 GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM7, ENABLE);  //ʹ��TIMx
}

void TIM2_IRQHandler(void)
{
	 static int led_pattern=LIGHTEN_ALL;
	 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM7�����жϷ������
	 {
		if(led_blink)
		{
			if(led_pattern==LIGHTEN_ALL)
				led_pattern=SHUTDOWN_ALL;
			else
				led_pattern=LIGHTEN_ALL;
			
			LED_Modify(led_pattern);
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	 }
}
