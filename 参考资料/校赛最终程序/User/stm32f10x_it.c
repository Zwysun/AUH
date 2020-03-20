/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart1.h"  
#include "bsp_pwm_output.h" 
#include "usbl.h"
#include "navigation.h"
#include "float_ctrl.h"
//#include "float_ctrl.h"


extern float roll; 
extern float pitch; 
extern float yaw; 

extern float traget_yaw;

extern float motor_err_set;

extern int AUH_speed;

extern int motor_1_speed; 
extern int motor_2_speed;

extern float KP; 
extern float KI; 
extern float KD;

extern unsigned char PID_en_flag;


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

extern unsigned char yaw_high,yaw_low,pitch_high,pitch_low,roll_high,roll_low;

extern float usbl_roll,usbl_pitch,usbl_yaw; 

extern unsigned char message_txd[13];
extern unsigned char message_rxd[13];

extern  unsigned char ctrl_message_txd[13]; 
extern  unsigned char ctrl_message_rxd[13];


extern int16_t motor_1_speed_set,motor_2_speed_set,motor_3_speed_set,motor_4_speed_set;
extern unsigned char float_state;


extern unsigned char rec_motive_flag;
extern unsigned char rec_motive_mode;


extern unsigned char motor_sleep_state[4];

extern int X_angle,Y_angle;//定义XY轴声线
extern float x_angle_f,y_angle_f;//定义xy轴声线

extern float traget_depth;
extern float sys_depth;
extern float depth;

extern unsigned char u1_u3_t_flag;


extern float match_depth_set; 
 
extern float depth_p_set;
extern float depth_d_set;

extern float match_pitch_set;


extern float pitch_p_set;
extern float pitch_d_set;

extern float  match_yaw_set;
extern float yaw_p_set;
extern float yaw_d_set;
extern float match_speed_set;

unsigned int motive_num=0;


float yaw_PID(float set_yaw,float sys_yaw) 
{
 
	float temp;
  volatile static float err=0,last_err=0,derr=0;
 
	
	
 
	err=set_yaw-sys_yaw;   
 	
	derr=err-last_err;                       
  
	
	temp=err*yaw_p_set+derr*yaw_d_set;
	
	
	
	last_err=err; 
  return(temp);
}


float pitch_PID(float set_pitch,float sys_pitch) 
{
 
	float temp;
  volatile static float err=0,last_err=0,derr=0;
  static float pitch_KP=1;
	static float pitch_KD=1;
	
	
 
	err=set_pitch-sys_pitch;   
  
	
	temp=err*pitch_p_set;
	

	
  return(temp);
}


float depth_PID(float set_depth,float sys_depth) //调节航向角的PID算法   
{
 
	float temp;
  volatile static float err=0,last_err=0,derr=0;
  static float depth_KP=1;
	static float depth_KD=1;
	
	
	
 
	err=set_depth-sys_depth;   //计算偏差、偏差的微分
  
		
	derr=err-last_err;                       
  
	
	temp=err*depth_p_set;

	last_err=err; //保存上一次偏差
  return(temp);
}



void motive_control(float yaw_control,int speed_control,int float_control)//控制航向，速度，浮力的函数
{
	
	 
	
	 
	
}


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USART3_IRQHandler(void)
{
	uint8_t ch;
	

  static unsigned char rx_flag=0;
	static unsigned char rx_i=0;
	 
	unsigned char  i;

  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
	   
			ch = USART_ReceiveData(USART3);
		  
				    
			if((rx_flag==0)&&(ch==start_frame)) 
				{
					rx_flag=1;

				}
			if(rx_flag==1)
				{ 
					ctrl_message_rxd[rx_i]=ch;
					
					rx_i++;
					
				}
			if(rx_i>12)
				{
					rx_i=0;
					rx_flag=0;
					
				
					 CTRL_command_handle();
						
					
				}
		
			/* 发送一个字节数据到USART1 */
	///	USART_SendData(USART1, ch);
		
		/* 等待发送完毕 */
	//	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	  	//printf( "%c", ch );    //将接受到的数据直接返回打印
	}
 /*
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
	    //ch = USART1->DR;
			ch = USART_ReceiveData(USART1);
		if(ch==0xff)
			{
				re_mark=1; 
			}
		if(re_mark==1)
			{
				temp[i++]=ch; 
				if(i>2)
					{
							i=0;
							re_mark=0;
						  duty_temp=temp[1]*100+temp[2];
						  printf("%d\n",duty_temp);
						 //  TIM_SetCompare1(TIM3,duty_temp);
					}
			}
	  	//printf( "%c", ch );    
	} */
	 
}







void USART1_IRQHandler(void)
{   
	  uint8_t ch;
		static unsigned char rx_flag=0;
	  static unsigned char rx_i=0;
	  unsigned char i;
	
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
			{ 	
	   
			ch = USART_ReceiveData(USART1);
	    
			if((rx_flag==0)&&(ch==start_frame)) 
				{
					rx_flag=1;

				}
			if(rx_flag==1)
				{ 
					message_rxd[rx_i]=ch;
					
					rx_i++;
					
				}
			if(rx_i>3)
				{
					rx_i=0;
					rx_flag=0;
					
			
					rxd_command_handle();
						
					
				}
				
				
			}				
}



#define step 20   



const int motive_level_sum_max[9]={
	500,		
	450,		
	500,		
	450,
	500,	
	600,
	500,		
	600,
	
	10000		
};

const int motive_vertical_sum_max[6]={
	6000,		
	600 ,		
	6000,	
	600,		
	
	2700,		
	10000,		

};


const int motive_rec_sum_max[11]={
	500,	
	450,	
	500,	
	450,	
	6000,
	500,	
	800,	
	6000,	
	500,	
	600,	
	10000
};

const int motive_iUSBL_sum_max[6]={
	600,		
	200, 		
	6000,		
	2000,		
	2700,		
	10000,		

};

const int motive_bun_match_sum_max[]={

		100,
	80,
	100,
	420,
	40,
	400,
	10000
};

const int motive_bun_pitch_sum_max[]={
	100,
	60,
	100,
	400,
	40,
	400,
	10000
};

const int motive_bun_pitch14_sum_max[]={
	60,
	400,
	400,
	80,
  300,
	100,
	10000
};

const int motive_bun_pitch15_sum_max[]={
	20,
	200,
	200,
	225,
  200,
	200,
	10000
};

const int motive_bun_pitch16_sum_max[]={
	200,
	200,
	300,
	250,
  300,
	200,
	10000
};

const int motive_bun_pitch17_sum_max[]={
	20,
	100,
	30,
	175,
  30,
	200,
	10000
};

const int motive_bun_pitch18_sum_max[]={
	30,
	50,
	50,
	250,
  30,
	200,
	10000
};

const int motive_bun_pitch19_sum_max[]={
	20,
	50,
	40,
	225,
  30,
	200,
	10000
};

const int motive_bun_pitch20_sum_max[]={
	20,
	40,
	30,
	250,
  30,
	200,
	10000
};

const int motive_bun_pitch21_sum_max[]={
	20,
	100,
	30,
	175,
  30,
	200,
	10000
};





                       
void USART2_IRQHandler(void)
{
  uint8_t ch;
	static unsigned char re_mark=0; 
	static unsigned char P_data[11]={0};
	static unsigned char i=0;
  int16_t temp=0;

	 static unsigned char inti_yaw_flag=1;
   static unsigned char int_sum;
   int motor_err_set;
   int motor_pitch_err_set;

	 int motor_depth_set;

   int motor_1_speed;
   int motor_2_speed;
   int motor_3_speed;
   int motor_4_speed;

   float traget_yaw_temp;
   
   static unsigned int float_up_sum;
   static unsigned int float_down_sum;
   
  static unsigned char motor_init_sum=0;

	static unsigned char motive_stage_num=0;

	static unsigned char motive_stage_init_flag=1;


	static unsigned char pitch_inti_flag=1;
  //static int navigation_sum[]={};

  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{ 	
	   
			ch = USART_ReceiveData(USART2); 
		
		
		if(ch==0x55 && re_mark==0)
			{
				re_mark=1; //接受开始
			}
		if(re_mark==1)
			{
				
				P_data[i]=ch; 
			
				i++;
				if(i>10)
					{
							i=0;   //计数归零
							re_mark=0; //接受归零
						
							if(P_data[1]==0x53)
								{
									
									
									temp=P_data[7]<<8;					
									temp|=P_data[6];   
																		
									yaw=((float)temp)/32768*180; 
									
									temp=P_data[5]<<8;					
									temp|=P_data[4];   
																		
									pitch=((float)temp)/32768*180; 
									
									temp=P_data[3]<<8;					
									temp|=P_data[2];   
																		
									roll=((float)temp)/32768*180; 
									
								  yaw_high=P_data[7];
									yaw_low=P_data[6];
									pitch_high=P_data[5];
									pitch_low=P_data[4];
									roll_high=P_data[3];
									roll_low=P_data[2];
									
									usbl_roll=roll;
									usbl_pitch=pitch;
									usbl_yaw=yaw;
									
									printf("yaw=%f    ",yaw);
									printf("pitch=%f \n",pitch);
									
								
									if(rec_motive_flag)
										{
										 
												switch (rec_motive_mode) //判断为哪种实验形式
													{	

																	
													case 4: //小笼包控制
																	motive_num++;
													
																if(motive_num>motive_bun_match_sum_max[motive_stage_num])
																	{
																		motive_num=0;
																		motive_stage_num++;
																		motive_stage_init_flag=1;
																	}
																switch(motive_stage_num)
																	{
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{
																				//	motive_control(0,0,1);
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					
																					motive_stage_init_flag=0;
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);
																					
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;
																					//motor_sleep_state[0]=1;
																					//motor_sleep_state[1]=1;
																				}
//																				
																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;
																					
																				}
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,1);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=30;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;
																				}
																			break;
																				
																				
																				
																		
																		
																	
																		default:
																			motive_num=0;
																			motive_stage_num=0;//归零
																		
																	}
													
															break;
													
														case 5: 
																traget_yaw=match_yaw_set;
																traget_speed_sys_x=0;
																traget_depth=0;
															break;
														
														case 6:
																traget_yaw=0;
																traget_speed_sys_x=20;
																traget_depth=0;
															break;
														
														case 7:
															
																	
																traget_yaw=0;
																traget_speed_sys_x=0;
																traget_depth=100;
															break;
														
														
														case 8:
														

																	motive_num++;
																if(motive_num>motive_bun_match_sum_max[motive_stage_num])
																	{
																		motive_num=0;
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=30;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=-45;
																					traget_yaw=0;
																					traget_speed_sys_x=20;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
															break;														
													
														
														case 9:
																motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch_sum_max[motive_stage_num])
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=30;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)//若第一次进入，则//保持
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)//若第一次进入，则//保持
																				{	
																					traget_pitch=-45;
																					traget_yaw=0;
																					traget_speed_sys_x=30;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6://保持
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
															break;


														
														
														
														case 10://分段下降 力小
																traget_yaw=0;
																traget_speed_sys_x=0;
																traget_depth=100;
															break;
														case 11://分段上升 力∞
																traget_yaw=0;
																traget_speed_sys_x=0;
																traget_depth=100;
															break;
														
														case 13://倾斜整套
																	motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch_sum_max[motive_stage_num])//若溢出当前阶段最大值
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;//进入下一阶段
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)//若第一次进入，则//定深前进
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=-45;
																					traget_yaw=0;
																					traget_speed_sys_x=20;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)//若第一次进入，则//保持
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
															break;
														
														case 12:
																traget_yaw=0;
																traget_speed_sys_x=0;
																traget_depth=100;
															break;
														
														
														case 14:
																motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch14_sum_max[motive_stage_num])
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=0;
																					traget_speed_sys_x=40;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					pitch_p_set=0.4;
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=90;
																					traget_pitch=0;
																					traget_speed_sys_x=0;
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=150;//150;
																				//	traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)//若第一次进入，则//保持
																				{	
																					traget_pitch=0;
																					traget_yaw=270;
																					traget_speed_sys_x=0;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)//若第一次进入，则//保持
																				{	
																	
																					traget_yaw=270;
																					traget_pitch=30;
																					traget_speed_sys_x=40;
																					pitch_p_set=0.3;
																					//traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
														
														
														
															break;																				

														
														  case 15:
																motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch15_sum_max[motive_stage_num])
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					pitch_p_set=0.4;
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=90;
																					traget_speed_sys_x=50;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=0;
																					traget_speed_sys_x=30;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=45;
																					traget_yaw=0;
																					traget_speed_sys_x=40;//150;
																				
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
														
															break;
																	
																	
														  case 16:
																		motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch16_sum_max[motive_stage_num])//若溢出当前阶段最大值
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)//不同阶段选择不同的运动配置
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=-40;
																					traget_yaw=0;
																					traget_speed_sys_x=200;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=50;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=30;
																					traget_yaw=90;
																					traget_speed_sys_x=50;//150;
																				
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
														
															break;
															
										
														  case 17:
																motive_num++;
																if(motive_num>motive_bun_pitch17_sum_max[motive_stage_num])
																	{
																		motive_num=0;
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=-30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_pitch=10;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
															break;
																	
															
                         case 18://*稳如蛇皮
																motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch18_sum_max[motive_stage_num])
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=-30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
														
														
														
															break;																				

														
														  case 19:
																motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch19_sum_max[motive_stage_num])
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;//进入下一阶段
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=45;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=30;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=-45;
																					traget_yaw=0;
																					traget_speed_sys_x=40;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
														
															break;
																	
																	
														  case 20:
																		motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch20_sum_max[motive_stage_num])
																	{
																		motive_num=0;
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)//不同阶段选择不同的运动配置
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_yaw=0;
																					traget_speed_sys_x=25;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=-30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
														
															break;
															
										
														  case 21:
																																					motive_num++;//计时变量累加
																if(motive_num>motive_bun_pitch21_sum_max[motive_stage_num])
																	{
																		motive_num=0;//计时变量累加清零
																		motive_stage_num++;
																		motive_stage_init_flag=1;//刷新标志置位
																	}
																switch(motive_stage_num)//不同阶段选择不同的运动配置
																	{																		
																		
																		case 0:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=0;
																					traget_yaw=0; 
																					traget_speed_sys_x=0;
																				//	traget_depth=0;
																					
																					motive_stage_init_flag=0;//清零
																				 
																				}
																			break;
																		case 1:	
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,0);//角度速度浮力
																					traget_pitch=30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					
																					//traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}
																			break; 
																		case 2:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					traget_pitch=0;
																					traget_yaw=90;
																					traget_speed_sys_x=0;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																				
																				}

																			break;
																		case 3:
																			if(motive_stage_init_flag)
																				{	
																				//	motive_control(0,0,2);//角度速度浮力
																					
																					traget_pitch=-10;
																					traget_yaw=0;
																					traget_speed_sys_x=20;
																					traget_depth=match_depth_set;
																					
																					motive_stage_init_flag=0;//清零
																					//motor_sleep_state[0]=1;//关掉电机
																					//motor_sleep_state[1]=1;//关掉电机
																				}
//																				if(y_angle_f>60&&y_angle_f<120)
	//																				motive_num=7000;//指定溢出
																			break;
																		case 4:
																			if(motive_stage_init_flag)
																				{	
																			//		motive_control(0,0,0);//角度速度浮力
																					traget_yaw=90;
																					traget_speed_sys_x=0;//150;
																					traget_depth=match_depth_set;
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 5:
																			if(motive_stage_init_flag)
																				{	
																					traget_pitch=-30;
																					traget_yaw=0;
																					traget_speed_sys_x=20;//150;
																				//	traget_depth=match_depth_set;
																				
																					motive_stage_init_flag=0;//清零
																					
																				}
																			break;
																		case 6:
																			if(motive_stage_init_flag)
																				{	
																	
																					traget_yaw=0;
																					traget_speed_sys_x=0;
																					traget_depth=0;
																					motive_stage_init_flag=0;//清零
																				}
																			break;
																		
																		default:
																			motive_num=0;//计时变量累加清零
																			motive_stage_num=0;//归零
																		
																	}
															break;																	
												











																	default:
															;
													}
										}
									else
										{
                  
												if(inti_yaw_flag<150) 
														{
															traget_yaw=yaw;
															inti_yaw_flag++;
														}  
												if(inti_yaw_flag==150)
														{
															traget_yaw=0;
															inti_yaw_flag++;

														}
											
										
									/*		switch (rec_motive_mode) //判断为哪种实验形式
													{
															case 4:
																motor_1_speed_set=300;
																motor_2_speed_set=300;
															break;
														
														case 5:
																motor_1_speed_set=200;
																motor_2_speed_set=200;
															break;
														
														case 6:
																motor_1_speed_set=100;
																motor_2_speed_set=100;
															break;
														
														case 7:
																motor_1_speed_set=50;
																motor_2_speed_set=50;
															break;
														default:
															;
														
													}*/
								
									
											//motor_err_set=yaw_PID(traget_yaw,yaw);
											//motor_pitch_err_set=pitch_PID(traget_pitch,pitch);
										//	motor_depth_set=depth_PID(traget_depth,depth);
									
								/*			if(PID_en_flag==1)  								
												{		
														motor_1_speed=traget_speed_sys_x+motor_err_set;
														motor_2_speed=traget_speed_sys_x-motor_err_set;
														motor_1_speed_set=motor_1_speed;
														motor_2_speed_set=motor_2_speed;
													
													  motor_3_speed=motor_depth_set+motor_pitch_err_set;
														motor_4_speed=motor_depth_set-motor_pitch_err_set;
														motor_3_speed_set=motor_3_speed;
														motor_4_speed_set=motor_4_speed;
												}
											else
												{
														motor_1_speed=motor_1_speed_set;
														motor_2_speed=motor_2_speed_set;
														motor_3_speed=motor_3_speed_set;
														motor_4_speed=motor_4_speed_set;
											
												}
									*/
											motive_num=0;//计时变量累加清零
											motive_stage_num=0;
										  motive_stage_init_flag=1;//刷新标志置位
											
										}
									if(motor_init_sum<100)
												{
													motor_init_sum++;
														
												}
									else
												{
															
                            

                             													
														if(rec_motive_flag)				
															{   //if(rec_motive_mode<3)//非声学导航实验
																	//	motive_control(traget_yaw,traget_speed_sys_x,float_state);
																  //else
										  					motor_err_set=yaw_PID(traget_yaw,yaw);
																motor_pitch_err_set=pitch_PID(traget_pitch,pitch);
																//motor_depth_set=depth_PID(traget_depth,depth);
									              
														/*		if(traget_depth>10)
																	motor_depth_set=0;
																else
																	motor_depth_set=-20;
										*/
																motor_1_speed=traget_speed_sys_x+motor_err_set;
																motor_2_speed=traget_speed_sys_x-motor_err_set;
															//	motor_1_speed_set=motor_1_speed;
															//	motor_2_speed_set=motor_2_speed;
													
																motor_3_speed=motor_depth_set+motor_pitch_err_set;
																motor_4_speed=motor_depth_set-motor_pitch_err_set;
															//	motor_3_speed_set=motor_3_speed;
															//	motor_4_speed_set=motor_4_speed;
																
																motor_speed_set(Motor_1,motor_1_speed); 
										   					motor_speed_set(Motor_2,motor_2_speed);
																motor_speed_set(Motor_3,motor_3_speed); 
										   					motor_speed_set(Motor_4,motor_4_speed);
															//	motor_speed_set(Motor_1,50);
																
															}
														else
															{	
																//rec_motive_flag=1;
																//rec_motive_mode=0;
												   			motor_speed_set(Motor_1,motor_1_speed);
										   					motor_speed_set(Motor_2,motor_2_speed);
																motor_speed_set(Motor_3,motor_3_speed); 
										   					motor_speed_set(Motor_4,motor_4_speed);
															}
												}
								}
						 
								
						 
					}
			}
	  
	} 
}
#define EN_UART4_RX	

#ifdef EN_UART4_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[64];     //接收缓冲,最大64个字节.
//接收状态
//bit7，接收完成标志
//bit6，接收到0x0d
//bit5~0，接收到的有效字节数目
u8 USART_RX_STA=0;       //接收状态标记	  
  
void UART4_IRQHandler(void)
{
	u8 res;	    
	if(UART4->SR&(1<<5))//接收到数据
	{	 
		res=UART4->DR; 
		if((USART_RX_STA&0x80)==0)//接收未完成
		{
			if(USART_RX_STA&0x40)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x80;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x40;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3F]=res;
					USART_RX_STA++;
					if(USART_RX_STA>63)USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	}  											 
} 
#endif	



extern void Delay(__IO uint32_t nCount);
/*

void  EXTI0_IRQHandler(void)
{
	unsigned char b_flag,c_flag;//用于标记B C的状态
	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
		{
			
			Delay(0x0FFFFF);
			Delay(0x0FFFFF);
			
			b_flag=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
			c_flag=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
	
			if(b_flag)//
				{
						if(c_flag)
								rec_motive_mode=0; // B 1 C 1
						else
								rec_motive_mode=1; // B 1 C 0
				}
			else
				{		
						if(c_flag)
								rec_motive_mode=2; // B 0 C 1
						else
								rec_motive_mode=3; // B 0 C 0
			
				}
		
		
			EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
		}  


}*/

void  EXTI0_IRQHandler(void)
{
	unsigned char a_flag;//用于标记B C的状态
	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //确保是否产生了EXTI Line中断
		{
			
			Delay(0x0FFFFF);
			Delay(0x0FFFFF);
			
			a_flag=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
			;
			if(a_flag)
				{
						motive_num=60000;
				}
			else
				{
					
				}
			
		EXTI_ClearITPendingBit(EXTI_Line0);     //清除中断标志位
			}  


}/**/



void  EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET) //确保是否产生了EXTI Line中断
	{
		float_ctrl_pr(float_stop);		
		Delay(0x0FFFFF);
		Delay(0x0FFFFF);
		
		
		EXTI_ClearITPendingBit(EXTI_Line3);     //清除中断标志位
	}  


}


void  EXTI4_IRQHandler(void) //触发上浮限位开关
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //确保是否产生了EXTI Line中断
	{
		Delay(0x0FFFFF);//延时消抖
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)==0)
			{
				float_ctrl_pr(float_stop);	
				Delay(0x0FFFFF);
		
				float_ctrl_pr(float_down);
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);		
				float_ctrl_pr(float_stop);
		}
		EXTI_ClearITPendingBit(EXTI_Line4);     //清除中断标志位
	}  


}

void  EXTI9_5_IRQHandler (void)//触发下潜限位开关
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) //确保是否产生了EXTI Line中断
	{
		float_ctrl_pr(float_stop);		
		
		EXTI_ClearITPendingBit(EXTI_Line5);     //清除中断标志位
	}  
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) //确保是否产生了EXTI Line中断
	{
		Delay(0x0FFFFF);//延时消抖
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==0)
			{
				float_ctrl_pr(float_stop);	
				Delay(0x0FFFFF);
		
				float_ctrl_pr(float_up);
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);	
				Delay(0x0FFFFF);
				float_ctrl_pr(float_stop);
			}
		EXTI_ClearITPendingBit(EXTI_Line7);     //清除中断标志位
	}  
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
