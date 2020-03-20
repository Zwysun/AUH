#include "command_from_pi.h" 
#include "string.h"
#include "usart.h"
#include "JY901.h"
#include "linefollow.h"
#include "delay.h"
#define abs(data) ((data)<(0)?(-1*data):data)

int islocalization_enabled=0;
const float pi=3.1415;
long quan_navi_angle=0;
int navi_angle=0;
int navi_distance=0;
int navi_angle_compensation=0;
long alphabet_class;
int brake_button=0;
int x_error=0,y_error=0;//���Ϸ�Ϊ����С�����ͼ�����ġ�
int pre_y_error=0;
int letter_cnt=0;

//�������ʹ��֮ǰ��òⶨһ��һ��slightmove���þ�ͷ�����������ƶ����ٸ�"PIXEL"
//�Բ������ʽ������


void TIM4_LocSettlement_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx
}

void TIM4_IRQHandler(void)   //TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
	{		
		if(islocalization_enabled)
		{
			//�е�����ȡ�����Ҫһ��suppress����
			if(y_error!=1000)
					pre_y_error=y_error;
			if(y_error<-localization_threshold||y_error>localization_threshold)//��ǰ��50
			{	
				left=left_couple*(y_error<0)+1*(y_error>0);
				right=right_couple*(y_error<0)+1*(y_error>0);
				if(y_error==1000)
				{
					left=left_couple*(pre_y_error<0)+1*(pre_y_error>0);//û��Ӧ��pre_errorΪ0���Ĳ�������ʱ��preerrorӦΪ�������ɣ�����ֵ����ν
					right=right_couple*(pre_y_error<0)+1*(pre_y_error>0);				
				}	
				Setpwm_LR();
				if((abs(y_error))<2*localization_threshold)
					delay_ms(150);
				else if((abs(y_error))<3*localization_threshold)
					delay_ms(200);
				else 
					delay_ms(300);
			}
			left=0;
			right=0;
			Setpwm_LR();		
			USART_SendData(USART2,CMD_LOCALIZATION_ID);			
		}
		if(isyawloopclosed)
		{
			//������֮ǰ��һ��Ҫ��"�ⲿ"�����������
			//yaw_Reset();
			//while(yaw!=0){};
			
			if(yaw>yaw_threshold||yaw<-yaw_threshold)
			{
				left=left_couple*(yaw>0)+1*(yaw<0);
				right=right_couple*(yaw<0)+1*(yaw>0);
				Setpwm_LR();
				if(abs(yaw)<5)
					delay_ms(180);
				else if(abs(yaw)<15)
					delay_ms(200);
				else 
					delay_ms(300);
			}
			left=0;
			right=0;
			Setpwm_LR();
		}//��ʱ���������൱����ת������ĵ͵�ƽ

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
	}
}


void Pi_Stm32_unpackData(uint8_t *buf,u16 len)
{
	switch(buf[3])
  {
		case MSG_NAVI_ID:
				quan_navi_angle=(buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+buf[7];
				navi_angle=-1*quan_navi_angle/pi*180/1000;//���ĵ�ɫ���Ƕȣ��������ڵƴ��Ҳ�Ϊ��
				navi_distance=(buf[8]<<24)+(buf[9]<<16)+(buf[10]<<8)+buf[11];//���ĵ����ߵ���������������ƫ�Ҳ�ʱdistanceΪ��
//				printf("angle=%d distance=%d\n",navi_angle,navi_distance);
				
				navi_angle_compensation=navi_angle-yaw;
				//if(brake_button==0)//ֻ����button�ɿ�������£�button�����Ż�����µ�ֵ
				brake_button=(buf[12]<<24)+(buf[13]<<16)+(buf[14]<<8)+buf[15];

				break;
		case MSG_REGISTER_ID:
				break;
		case MSG_LOCALIZATION_ID:
				x_error=-1*((buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+buf[7]);//��x��y����long����ʽ��������
				y_error=(buf[8]<<24)+(buf[9]<<16)+(buf[10]<<8)+buf[11];
				//printf("x_error=%d y_error=%d\n",x_error,y_error);
				break;
		case MSG_ALPHABET_ID:
				letter_cnt++;
				alphabet_class=(buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+buf[7];
				/*���Ը��ݵ�ǰС����������ж��ĸ���ĸʶ����������ʵ�*/
				printf("Hydrosnapper: %c,%c,%c,%c\n",(int)((alphabet_class&(0xff<<24))>>24)+'@',
																								(int)((alphabet_class&(0xff<<16))>>16)+'@',
																								(int)((alphabet_class&(0xff<<8))>>8)+'@',
																								(int)(alphabet_class&0xff)+'@');
//				printf("the letters are: %c,%c,%c,%c\n",(int)(buf[7])+'@',(int)(buf[6])+'@',(int)(buf[5])+'@',(int)(buf[4])+'@');
				break;
    default:break;
  }
}
