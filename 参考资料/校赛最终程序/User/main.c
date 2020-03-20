/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试tim3四个通道输出不同占空比的pwm波
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 ISO-MINI STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */  
	
	/*

版权声明:

本代码为MINI-AUH的完整控制代码，本次公开仅限于小范围学院内部学习，
且在正式比赛中不得复制本程序任何一段代码,禁止用于商业用途.
此授权之外的任何用途，本团队将保留追究责相关责任的权利。

浙江大学海洋学院海工所
水下直升机课题组
陆培 刘勋

*/	
	
	
	
#include "stm32f10x.h"
#include "bsp_pwm_output.h"
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "relay.h"
#include "usbl.h"
#include "float_ctrl.h"
#include "navigation.h"

#include  <math.h>  
#include "i2c.h" 
#include "MS5803.h" 


float roll=0; 
float pitch=0; 
float yaw=0; 








extern float traget_yaw;   
extern float traget_pitch; 
extern float traget_roll;  

extern float traget_x_speed; 
extern float traget_y_speed; 
extern float traget_z_speed; 

extern float traget_speed_sys_x;

extern float sys_yaw;
extern float sys_pitch;
extern float sys_roll;

extern float sys_x_speed;
extern float  sys_y_speed;
extern float sys_z_speed;

extern unsigned char rec_motive_flag;
extern unsigned char rec_motive_mode;

extern unsigned char PID_en_flag;

extern float depth;


#define bar_pre 1000000 


void Delay(__IO u32 nCount);  //简单的演示函数
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */


	
int main(void)
{	
	int64_t C0,C1,C2,C3,C4,C5,C6;
	int64_t D1,D2;
	int64_t dT,TEMP,P,T2;
	int64_t OFF,SENS,OFF2,SENS2;
	
	
	
	int i=0;
	unsigned char sum;
	static int led_flag;
	
	delay_init();	    
	
	
	
	USART1_Config(); //已验证
	
	USART2_Config(); //已验证
	USART3_Config();	//已验证
	
	UART4_Config(36,115200);
	sys_init();
	
	

	
	NVIC_Configuration();

	SysTick_Init();
	
	TIM3_PWM_Init();

	float_ctrl_inti();
	
	
 
	Delay(0x0FFFFF);
	Delay(0x0FFFFF);
	Delay(0x0FFFFF);
	Delay(0x0FFFFF);
  Delay(0x0FFFFF);
	
	
	
	while(1)
		{
				
			
			
			
	
		
		
		dT  = D2-C5*256;
		TEMP= 2000+dT*C6/8388608;
		OFF = C2*65536+(C4*dT)/128;
		SENS= C1*32768+(C3*dT)/256;
		P   = ( D1*(SENS>>21) - OFF )/32768;
	
		depth=(P-bar_pre)/1/9.8*100; 
		
	
		if(led_flag)
			{
		
				led_flag=0;
			}
		else
			{
			
				led_flag=1;
			}
			  
		}
	
	
	
		 
}

void Delay(__IO uint32_t nCount)	 //???????
{
	for(; nCount != 0; nCount--);
}


/*********************************************END OF FILE**********************/
