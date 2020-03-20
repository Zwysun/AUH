#include "relay.h"

unsigned char relay_state[4]={0}; 

void motor_init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //无线发射线圈启动//强大的干扰源//前方高能预警//非战斗人员撤离
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	sw_ctrl(MOTOR_0_EN,sw_switch_on);  
	sw_ctrl(MOTOR_1_EN,sw_switch_on);
	sw_ctrl(MOTOR_2_EN,sw_switch_on);
	sw_ctrl(MOTOR_3_EN,sw_switch_on);
	sw_ctrl(COIL_EN,sw_switch_on);
	

}

void usbl_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //USBL 供电使能
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}


void sys_init(void)
{
	motor_init();
	motor_init();

}


void sw_ctrl(unsigned char sw_ch,unsigned char sw_stage)
{
		switch(sw_ch)
			{	
				case MOTOR_0_EN:
					if(sw_stage==1)
						{
							GPIO_SetBits(GPIOD,GPIO_Pin_11);
							relay_state[0]=1;
						}
					else
						{
							GPIO_ResetBits(GPIOD,GPIO_Pin_11);
							relay_state[0]=0;
						
						}
					break;
					
					case MOTOR_1_EN:
						if(sw_stage==1)
							{
								GPIO_SetBits(GPIOD,GPIO_Pin_10);
								relay_state[1]=1;
							}
						else
							{
								GPIO_ResetBits(GPIOD,GPIO_Pin_10);
								relay_state[1]=0;
							}
					break;
					
					case MOTOR_2_EN:
						if(sw_stage==1)
							{
								GPIO_SetBits(GPIOD,GPIO_Pin_9);
								relay_state[2]=1;
							}
						else
							{
								GPIO_ResetBits(GPIOD,GPIO_Pin_9);
								relay_state[2]=0;
							}
					break;
					
					case MOTOR_3_EN:
						if(sw_stage==1)
							{
								GPIO_SetBits(GPIOD,GPIO_Pin_8);
								relay_state[3]=1;
							}
						else
							{
								GPIO_ResetBits(GPIOD,GPIO_Pin_8);
								relay_state[3]=0;
							}
					break;
					
					case USBL_EN://逆超短基线控制
					if(sw_stage==1)
						GPIO_SetBits(GPIOD,GPIO_Pin_12);
					else
						GPIO_ResetBits(GPIOD,GPIO_Pin_12);
					break;
					
					
					
					default:
						;
				
			}

}