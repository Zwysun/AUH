#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "pid_for_posture.h"
#include "command_from_pi.h"
#include "linefollow.h"
#include "beep.h"
#include "otimer.h"
#include "HP206C.h"
#include "led.h"
#include "JY901.h"
void Board_Init(void);
void Battery_lock_release(void);
int test_turn_angle=0;
int test_lag_cnt=0;
int test_speed=1;
int startswitch=0;


int main(void)
{		 
	 Board_Init();
	 //Target_depth=25;Target_pitch=0;
	 while(!startswitch)
		 delay_ms(200);	 

//准备下沉
		Target_depth=25;Target_pitch=0;
		yaw_Reset();
		while(yaw!=0){};
		isposeloopclosed=1;
		isyawloopclosed=1;
		Depth_Disturb_Suppress();

 
		/*开始运动*/	
		isyawloopclosed=0;
		LED_Modify(SHUTDOWN_ALL);		
		islineloopclosed=1;
			
		/*第一个打卡点*/
		while(brake_button!=1000)
			delay_ms(10);	
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		
//+如果要在第一个点停下来↓
		left=left_couple;right=right_couple;Setpwm_LR();
		brake_button=0;
		delay_ms(1000);
		left=0;right=0;Setpwm_LR();
		delay_ms(1000);
		LED_Modify(SHUTDOWN_ALL);
//+如果要在第一个点停下来↑
		
		/*冲过第一个打卡点*/
		left=avr_leftright;right=avr_leftright;Setpwm_LR();
		delay_ms(1000);
		delay_ms(1000);

		/*奔向第二个打卡点*/
		islineloopclosed=1;	
		while(brake_button!=1000)
			delay_ms(10);
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		Brake(brake_angle,brake_lagtime);
		brake_button=0;
		Pitch_Disturb_Suppress();//这个结束后必须确保能拍到黄球呀
		/*第二个打卡点*/		
				
		Macroturn_leftright(90,3);
		LED_Modify(SHUTDOWN_ALL);
		left=avr_leftright;right=avr_leftright;Setpwm_LR();
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		
		/*奔向第三个打卡点*/		
		islineloopclosed=1;			
		while(brake_button!=1000)
			delay_ms(10);	
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		Brake(brake_angle,brake_lagtime);
		brake_button=0;
		Pitch_Disturb_Suppress();
		
		/*字母获取*/
		USART_SendData(USART2,CMD_ALPHABET_ID);
				
		/*第三个打卡点*/	
		Macroturn_leftright(135+10,3);
		LED_Modify(SHUTDOWN_ALL);
		/*奔向第四个打卡点*/
		left=avr_leftright;right=avr_leftright;Setpwm_LR();
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		islineloopclosed=1;	

		/*第四个打卡点*/	
		while(brake_button!=1000)
			delay_ms(10);	
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		Brake(brake_angle,brake_lagtime);
		brake_button=0;
		Pitch_Disturb_Suppress();
		
		while(1)delay_ms(100);

}









void Board_Init(void)
{
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 delay_init();
	 delay_ms(200);//据说加一个延时对电池更好
	
	 Battery_lock_release();
	 LED_Init();//用来指示芯片已经上电，指示电源已经亮起

	 Raspberry_Usart2_Init(9600);//串口初始化为9600,接收Raspberry pi数据
	//开机时厂家的预置树莓派程序会发过来乱七八糟的东西

	 PWM_Configuration(19999,71);
//
//	TIM2_LEDIndicator_Init(1999,35999);//1s一次
	//这些初始化如果放在电调的前面，解锁就会失败
	 Lora_Usart3_Init(115200);	//串口初始化为115200，接收无线串口指令,发送中断开着，虽然不知道有啥用
	 IMU_Usart1_Init(9600);	 //串口初始化为9600,接收JY901数据
	 IIC_Init();HP206C_Init();//压力传感器初始化必须在水面上开机，以确定大气压
	 PID_pos_init();	 
	 TIM4_LocSettlement_Init(4999,7199);

//这个定时器中断关掉的话，压力数据就不会更新了
	 TIM6_PosPID_Init(1499,7199);//10Khz的计数频率，计数到1499为150ms,姿态深度PID计算	
	 TIM7_LinePID_Init(7499,7199);//10Khz的计数频率，循环周期为0.75s,巡线角度距离PID计算
}
