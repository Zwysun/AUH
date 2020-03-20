#include "pid_for_posture.h"
#include "dma.h"
#include "usart.h"
#include "delay.h"
#include "linefollow.h"
#include "JY901.h"
#include "HP206C.h"
#include "command_from_pi.h"
#include "linefollow.h"
#define SAFE 2
#define UNSAFE 0
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

void TIM6_PosPID_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM6中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
}
void TIM7_LinePID_Init(u16 arr,u16 psc)
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM7, ENABLE);  //使能TIMx					 
}

void TIM6_IRQHandler(void)   //TIM6中断，控制pitch，pitch为正的时候，前方翘起
{
	
	static int depth_modify,pitch_modify;
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM6更新中断发生与否
	{		
		GetHp206cData(&Hydrosnapper_hp206c);
		if(isposeloopclosed)
		{
			/*这两句尤其要注意正方向*/
			//平衡点平衡姿态下的小扰动PID修正 
			PID_setpoint(&PID_depth,Target_depth);
			PID_setpoint(&PID_pitch,Target_pitch);
			depth_modify=incPIDcalc(&PID_depth,(int)Hydrosnapper_hp206c.depth);			
			pitch_modify=incPIDcalc(&PID_pitch,pitch);

			forward=(depth_modify-pitch_modify);			
			behind=(depth_modify+pitch_modify);
			Setpwm_UD();
		}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}

void TIM7_IRQHandler(void)
{	
	static int nav_modify;
	static int pre_distance=0;
//  static int safety_index=SAFE;
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM7更新中断发生与否
	{
		if(islineloopclosed)
		{
			//前进方向到色带角度,指向右侧为正
			//中心到中线的像素数，机器人偏右侧时distance为正						
			//如果用IMU和摄像头联合巡线，则需要在开始巡线或刚开始下一条线时，yaw清零，以防yaw从±180跳变
			left=avr_leftright;right=avr_leftright;
			//距离灯带的数值距离23cm时dis/angle=-47,gain=3
			nav_modify = nav_gain/10.0*navi_distance/(float)nav_pro
									+nav_gain/10.0*(yaw+navi_angle_compensation);//这两个系数是有待调整的,比例要等于tan
			

//以下程序要应对的是偏出视角的情况			
			if(navi_distance==0&&(pre_distance>200||pre_distance<-200))
			{
				if(pre_distance<0)
					nav_modify=-avr_leftright/2;
				else if(pre_distance>0)
					nav_modify=avr_leftright/2;
			}
			if(navi_distance!=0&&navi_distance<321&&navi_distance>-321)//防止一开始的错误数据被存入
				pre_distance=navi_distance;
//			
			
			left-=nav_modify;
			right+=nav_modify;
			if(navi_angle<91&&navi_angle>-91)//避开异常数据点
				Setpwm_LR();

			//把更新角度数据放在末尾，最好地利用时间
			USART_SendData(USART2,CMD_NAV_ID);
//			printf("nav=%d\n",yaw+navi_angle_compensation);
		}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}
