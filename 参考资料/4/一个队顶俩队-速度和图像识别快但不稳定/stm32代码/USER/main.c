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

//׼���³�
		Target_depth=25;Target_pitch=0;
		yaw_Reset();
		while(yaw!=0){};
		isposeloopclosed=1;
		isyawloopclosed=1;
		Depth_Disturb_Suppress();

 
		/*��ʼ�˶�*/	
		isyawloopclosed=0;
		LED_Modify(SHUTDOWN_ALL);		
		islineloopclosed=1;
			
		/*��һ���򿨵�*/
		while(brake_button!=1000)
			delay_ms(10);	
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		
//+���Ҫ�ڵ�һ����ͣ������
		left=left_couple;right=right_couple;Setpwm_LR();
		brake_button=0;
		delay_ms(1000);
		left=0;right=0;Setpwm_LR();
		delay_ms(1000);
		LED_Modify(SHUTDOWN_ALL);
//+���Ҫ�ڵ�һ����ͣ������
		
		/*�����һ���򿨵�*/
		left=avr_leftright;right=avr_leftright;Setpwm_LR();
		delay_ms(1000);
		delay_ms(1000);

		/*����ڶ����򿨵�*/
		islineloopclosed=1;	
		while(brake_button!=1000)
			delay_ms(10);
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		Brake(brake_angle,brake_lagtime);
		brake_button=0;
		Pitch_Disturb_Suppress();//������������ȷ�����ĵ�����ѽ
		/*�ڶ����򿨵�*/		
				
		Macroturn_leftright(90,3);
		LED_Modify(SHUTDOWN_ALL);
		left=avr_leftright;right=avr_leftright;Setpwm_LR();
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		
		/*����������򿨵�*/		
		islineloopclosed=1;			
		while(brake_button!=1000)
			delay_ms(10);	
		LED_Modify(LIGHTEN_ALL);
		islineloopclosed=0;
		Brake(brake_angle,brake_lagtime);
		brake_button=0;
		Pitch_Disturb_Suppress();
		
		/*��ĸ��ȡ*/
		USART_SendData(USART2,CMD_ALPHABET_ID);
				
		/*�������򿨵�*/	
		Macroturn_leftright(135+10,3);
		LED_Modify(SHUTDOWN_ALL);
		/*������ĸ��򿨵�*/
		left=avr_leftright;right=avr_leftright;Setpwm_LR();
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		islineloopclosed=1;	

		/*���ĸ��򿨵�*/	
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
	 delay_ms(200);//��˵��һ����ʱ�Ե�ظ���
	
	 Battery_lock_release();
	 LED_Init();//����ָʾоƬ�Ѿ��ϵ磬ָʾ��Դ�Ѿ�����

	 Raspberry_Usart2_Init(9600);//���ڳ�ʼ��Ϊ9600,����Raspberry pi����
	//����ʱ���ҵ�Ԥ����ݮ�ɳ���ᷢ�������߰���Ķ���

	 PWM_Configuration(19999,71);
//
//	TIM2_LEDIndicator_Init(1999,35999);//1sһ��
	//��Щ��ʼ��������ڵ����ǰ�棬�����ͻ�ʧ��
	 Lora_Usart3_Init(115200);	//���ڳ�ʼ��Ϊ115200���������ߴ���ָ��,�����жϿ��ţ���Ȼ��֪����ɶ��
	 IMU_Usart1_Init(9600);	 //���ڳ�ʼ��Ϊ9600,����JY901����
	 IIC_Init();HP206C_Init();//ѹ����������ʼ��������ˮ���Ͽ�������ȷ������ѹ
	 PID_pos_init();	 
	 TIM4_LocSettlement_Init(4999,7199);

//�����ʱ���жϹص��Ļ���ѹ�����ݾͲ��������
	 TIM6_PosPID_Init(1499,7199);//10Khz�ļ���Ƶ�ʣ�������1499Ϊ150ms,��̬���PID����	
	 TIM7_LinePID_Init(7499,7199);//10Khz�ļ���Ƶ�ʣ�ѭ������Ϊ0.75s,Ѳ�߽ǶȾ���PID����
}
