#include "my_scc.h"
static void tim_3_pwm_init(void)//pwm初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
  TIM_OCInitTypeDef TIM_OCInitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOC,&GPIO_InitStructure);
	
  TIM_TimeBaseInitStructure.TIM_Period = 20000-1; 
  TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能        
  TIM_OCInitStructure.TIM_Pulse=1500;         
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
  TIM_Cmd(TIM3,ENABLE);
}

static void tim_1_pwm_init(void)//pwm初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
											   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);

  TIM_TimeBaseStructure.TIM_Period = 20000-1;       
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//初始化定时器 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	   
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 1500;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	 
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);	
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
  //配置pwm
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	//使能通道重载
	TIM_Cmd(TIM1, ENABLE);	
	//开启定时器
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void set_left_right(s16 duty1,s16 duty2)
{
	TIM1->CCR3=1500+duty1;
	TIM3->CCR3=1500+duty2;
}
void set_up_down(s16 duty1,s16 duty2)
{
	TIM1->CCR1=1500-duty1;
	TIM3->CCR2=1500-duty2;
}
void motor_control(void)
{
	float duty[4]={0};
	if(motor_start)
	{
		if(direction_uk>0) duty[1]-=direction_uk;
		else duty[0]+=direction_uk;
//		duty[0]+=direction_uk;
//		duty[1]-=direction_uk;
		duty[2]+=up_down_uk;
		duty[3]-=up_down_uk;
		
		duty[0]+=speed;
		duty[1]+=speed+10;
//		if(yaw_pid.angular_speed_uk>0)
//		{
//			duty[0]+=yaw_pid.angular_speed_uk*0.4;
//			duty[1]+=yaw_pid.angular_speed_uk*0.4;
//			duty[2]+=yaw_pid.angular_speed_uk;
//			duty[3]+=yaw_pid.angular_speed_uk;
//		}
//		else 
//		{
//			duty[0]+=yaw_pid.angular_speed_uk;
//			duty[1]+=yaw_pid.angular_speed_uk;
//			duty[2]+=yaw_pid.angular_speed_uk*0.4;
//			duty[3]+=yaw_pid.angular_speed_uk*0.4;
//		}
		
//		duty[0]+=deepth_pid_uk;
//		duty[1]+=deepth_pid_uk;
		duty[2]+=deepth_pid_uk;
		duty[3]+=deepth_pid_uk;
		
		if(duty[0]>350) duty[0]=350;
		else if(duty[0]<-350) duty[0]=-350;
		if(duty[1]>350) duty[1]=350;
		else if(duty[1]<-350) duty[1]=-350;
		if(duty[2]>350) duty[2]=350;
		else if(duty[2]<-350) duty[2]=-350;
		if(duty[3]>350) duty[3]=350;
		else if(duty[3]<-350) duty[3]=-350;
		TIM1->CCR3=1500+duty[0];
		TIM3->CCR3=1500+duty[1];
		TIM1->CCR1=1500+duty[2];
		TIM3->CCR2=1500+duty[3];
	}
	else 
	{
		TIM1->CCR3=1500+duty[0];
		TIM3->CCR3=1500+duty[1];
		TIM1->CCR1=1500+duty[2];
		TIM3->CCR2=1500+duty[3];
	}
}
void motor_init(void)
{
	tim_1_pwm_init();
	tim_3_pwm_init();    
}
