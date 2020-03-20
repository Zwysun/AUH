/**
  ******************************************************************************
  * @file    bsp_pwm_output.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   tim3Êä³öËÄÂ·pwm bsp
  ******************************************************************************
  * @attention
  *
  * ÊµÑéÆ½Ì¨:Ò°»ğ ISO-MINI STM32 ¿ª·¢°å 
  * ÂÛÌ³    :http://www.chuxue123.com
  * ÌÔ±¦    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_pwm_output.h" 

 /**
  * @brief  ÅäÖÃTIM3¸´ÓÃÊä³öPWMÊ±ÓÃµ½µÄI/O
  * @param  ÎŞ
  * @retval ÎŞ
  */
static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* ÉèÖÃTIM3CLK Îª 72MHZ */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOC enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //PORTCºÍÖØÓ³ÉäÊ±ÖÓ
  
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//ÍêÈ«ÖØÓ³Éä
	
  /*GPIOC Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // ¸´ÓÃÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

 
}

/**
  * @brief  ÅäÖÃTIM3Êä³öµÄPWMĞÅºÅµÄÄ£Ê½£¬ÈçÖÜÆÚ¡¢¼«ĞÔ¡¢Õ¼¿Õ±È
  * @param  ÎŞ
  * @retval ÎŞ
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ÖØĞÂ¼ÆÊı
 *                    TIMx_CCR(µçÆ½·¢Éú±ä»¯)
 * ĞÅºÅÖÜÆÚ=(TIMx_ARR +1 ) * Ê±ÖÓÖÜÆÚ
 * Õ¼¿Õ±È=TIMx_CCR/(TIMx_ARR +1)
 */
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 63999;       //µ±¶¨Ê±Æ÷´Ó0¼ÆÊıµ½63999£¬¼´Îª64000´Î£¬ÎªÒ»¸ö¶¨Ê±ÖÜÆÚ
  TIM_TimeBaseStructure.TIM_Prescaler = 17;	    //ÉèÖÃÔ¤·ÖÆµ, ·ÖÆµÎª17+1=18£¬·ÖÆµºóÆµÂÊÎª4MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//ÉèÖÃÊ±ÖÓ·ÖÆµÏµÊı£
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //ÏòÉÏ¼ÆÊıÄ£Ê½
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //ÅäÖÃÎªPWMÄ£Ê½1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 5951;	   //ÉèÖÃÌø±äÖµ£¬µ±¼ÆÊıÆ÷¼ÆÊıµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //µ±¶¨Ê±Æ÷¼ÆÊıÖµĞ¡ÓÚCCR1_ValÊ±Îª¸ßµçÆ½
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //Ê¹ÄÜÍ¨µÀ1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5951;	  //ÉèÖÃÍ¨µÀ2µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM 
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //Ê¹ÄÜÍ¨µÀ2
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	
	 /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5951;	  //ÉèÖÃÍ¨µÀ2µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM 
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	  //Ê¹ÄÜÍ¨µÀ2
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);


 /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 5951;	  //ÉèÖÃÍ¨µÀ2µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM 
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //Ê¹ÄÜÍ¨µÀ2
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	 TIM_ARRPreloadConfig(TIM3, ENABLE);			 // Ê¹ÄÜTIM3ÖØÔØ¼Ä´æÆ÷ARR
  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //Ê¹ÄÜ¶¨Ê±Æ÷3	
}

/**
  * @brief  TIM3 Êä³öPWMĞÅºÅ³õÊ¼»¯£¬Ö»Òªµ÷ÓÃÕâ¸öº¯Êı
  *         TIM3µÄËÄ¸öÍ¨µÀ¾Í»áÓĞPWMĞÅºÅÊä³ö
  * @param  ÎŞ
  * @retval ÎŞ
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
