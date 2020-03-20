/**
  ******************************************************************************
  * @file    bsp_pwm_output.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   tim3�����·pwm bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_pwm_output.h" 

 /**
  * @brief  ����TIM3�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ����TIM3CLK Ϊ 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOC enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //PORTC����ӳ��ʱ��
  
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//��ȫ��ӳ��
	
  /*GPIOC Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

 
}

/**
  * @brief  ����TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * @param  ��
  * @retval ��
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 63999;       //����ʱ����0������63999����Ϊ64000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 17;	    //����Ԥ��Ƶ, ��ƵΪ17+1=18����Ƶ��Ƶ��Ϊ4MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ���
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 5951;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5951;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM 
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	
	 /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5951;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM 
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);


 /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5951;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM 
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	 TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/**
  * @brief  TIM3 ���PWM�źų�ʼ����ֻҪ�����������
  *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
  * @param  ��
  * @retval ��
  */
void TIM3_PWM_Init(void)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config();	
}

extern unsigned char motor_sleep_state[4];

#define motor1_dead_zone_forward   
#define motor1_dead_zone_opposite  

#define motor2_dead_zone_forward   
#define motor2_dead_zone_opposite  

#define motor3_dead_zone_forward   
#define motor3_dead_zone_opposite  

#define motor4_dead_zone_forward   
#define motor4_dead_zone_opposite  

void motor_speed_set(unsigned char motor_ch,int motor_traget_speed)
{
   
	
	
	 
	 if(motor_sleep_state[0])
			motor_traget_speed=0;
	 
	 switch (motor_ch)
		{
			case 1:
				if(motor_sleep_state[0])
					motor_traget_speed=0;
				
			
				
				TIM_SetCompare1(TIM3,5951+motor_traget_speed);
				break;
			case 2:
				if(motor_sleep_state[1])
					motor_traget_speed=0;
				
		
				TIM_SetCompare2(TIM3,5951+motor_traget_speed);
				break;
			case 3:
				if(motor_sleep_state[1])
					motor_traget_speed=0;
				
		
				TIM_SetCompare3(TIM3,5951+motor_traget_speed);
				break;
			case 4:
				if(motor_sleep_state[1])
					motor_traget_speed=0;
				
				TIM_SetCompare4(TIM3,5951+motor_traget_speed);
				break;
		  default:
	
			;
		}		 

}

/*********************************************END OF FILE**********************/
