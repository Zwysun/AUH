/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����tim3�ĸ�ͨ�������ͬռ�ձȵ�pwm��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */  
	
	/*

��Ȩ����:

������ΪMINI-AUH���������ƴ��룬���ι���������С��ΧѧԺ�ڲ�ѧϰ��
������ʽ�����в��ø��Ʊ������κ�һ�δ���,��ֹ������ҵ��;.
����Ȩ֮����κ���;�����Ŷӽ�����׷����������ε�Ȩ����

�㽭��ѧ����ѧԺ������
ˮ��ֱ����������
½�� ��ѫ

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


void Delay(__IO u32 nCount);  //�򵥵���ʾ����
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
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
	
	
	
	USART1_Config(); //����֤
	
	USART2_Config(); //����֤
	USART3_Config();	//����֤
	
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
