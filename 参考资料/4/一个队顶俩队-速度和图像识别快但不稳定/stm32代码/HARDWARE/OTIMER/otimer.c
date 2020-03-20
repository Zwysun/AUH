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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM6�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx					 
}
void TIM7_LinePID_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM7, ENABLE);  //ʹ��TIMx					 
}

void TIM6_IRQHandler(void)   //TIM6�жϣ�����pitch��pitchΪ����ʱ��ǰ������
{
	
	static int depth_modify,pitch_modify;
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM6�����жϷ������
	{		
		GetHp206cData(&Hydrosnapper_hp206c);
		if(isposeloopclosed)
		{
			/*����������Ҫע��������*/
			//ƽ���ƽ����̬�µ�С�Ŷ�PID���� 
			PID_setpoint(&PID_depth,Target_depth);
			PID_setpoint(&PID_pitch,Target_pitch);
			depth_modify=incPIDcalc(&PID_depth,(int)Hydrosnapper_hp206c.depth);			
			pitch_modify=incPIDcalc(&PID_pitch,pitch);

			forward=(depth_modify-pitch_modify);			
			behind=(depth_modify+pitch_modify);
			Setpwm_UD();
		}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	}
}

void TIM7_IRQHandler(void)
{	
	static int nav_modify;
	static int pre_distance=0;
//  static int safety_index=SAFE;
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM7�����жϷ������
	{
		if(islineloopclosed)
		{
			//ǰ������ɫ���Ƕ�,ָ���Ҳ�Ϊ��
			//���ĵ����ߵ���������������ƫ�Ҳ�ʱdistanceΪ��						
			//�����IMU������ͷ����Ѳ�ߣ�����Ҫ�ڿ�ʼѲ�߻�տ�ʼ��һ����ʱ��yaw���㣬�Է�yaw�ӡ�180����
			left=avr_leftright;right=avr_leftright;
			//����ƴ�����ֵ����23cmʱ�ddis/angle=-47,gain=3
			nav_modify = nav_gain/10.0*navi_distance/(float)nav_pro
									+nav_gain/10.0*(yaw+navi_angle_compensation);//������ϵ�����д�������,����Ҫ����tan
			

//���³���ҪӦ�Ե���ƫ���ӽǵ����			
			if(navi_distance==0&&(pre_distance>200||pre_distance<-200))
			{
				if(pre_distance<0)
					nav_modify=-avr_leftright/2;
				else if(pre_distance>0)
					nav_modify=avr_leftright/2;
			}
			if(navi_distance!=0&&navi_distance<321&&navi_distance>-321)//��ֹһ��ʼ�Ĵ������ݱ�����
				pre_distance=navi_distance;
//			
			
			left-=nav_modify;
			right+=nav_modify;
			if(navi_angle<91&&navi_angle>-91)//�ܿ��쳣���ݵ�
				Setpwm_LR();

			//�Ѹ��½Ƕ����ݷ���ĩβ����õ�����ʱ��
			USART_SendData(USART2,CMD_NAV_ID);
//			printf("nav=%d\n",yaw+navi_angle_compensation);
		}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	}
}
