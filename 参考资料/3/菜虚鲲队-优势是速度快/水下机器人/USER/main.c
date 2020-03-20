#include "my_scc.h"
u8 data[2];

void tim4_init(void)
{
	TIM_TimeBaseInitTypeDef a;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	a.TIM_ClockDivision=TIM_CKD_DIV1;
	a.TIM_CounterMode=TIM_CounterMode_Up;
	a.TIM_Period=1000-1;
	a.TIM_Prescaler=72-1;
	a.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&a);
	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
	TIM_Cmd(TIM4, ENABLE);	
}

void power_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_3);  
}
char temp_buf[100]={0};
char set_huichuan[]={0xff,0xaa,0x02,0x0c,0x00};
char set_speed[]={0xff,0xaa,0x03,0x09,0x00};
char set_baud[]={0xff,0xaa,0x04,0x06,0x00};
char save[]={0xff,0xaa,0x00,0x00,0x00};
int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	set_nvic_irq(EXTI15_10_IRQn,2,2);
//	set_nvic_irq(TIM1_UP_IRQn,0,0);
	set_nvic_irq(USART1_IRQn,0,0);
	set_nvic_irq(USART2_IRQn,1,0);
	set_nvic_irq(USART3_IRQn,2,0);
//	set_nvic_irq(TIM2_IRQn,1,1);
	set_nvic_irq(TIM4_IRQn,3,0);
	
//	angular_speed_angle_pid_init(&pitch_pid,1.8,0.01,0.0,3,0,0);
//  angular_speed_angle_pid_init(&roll_pid,1.8,0.0,0.0,3,0,0);
//  angular_speed_angle_pid_init(&yaw_pid,8.0,0.0,0.0,0,0,0);
	roll_pid.angle_point=2;
	adc_INIT();
	usart_INIT();
	motor_init();
	delay_init();
  power_gpio_init();
	LED_Init();
	SX1278_Init();
	HP206C_Init();
	tim4_init();
	LED_SW(9,1);
  while(!key_sign);
	start_led();
	LED_SW(9,0);
	second=0;
	motor_start=0xff;
	key_sign=0;
	led_int_flag=1;
	
  while(1)
	{
		GetHp206cData(&hp206c);
//		led_flash(5);
    if(key_sign)
		{
			key_sign=0;
			motor_start=~motor_start;
			led_flash(5);
			GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		}
		sprintf(temp_buf,"bat:%.2f",battery);
		SX1278_SendData((uint8_t*)temp_buf,strlen(temp_buf));
	}
}

