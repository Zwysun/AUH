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
int x_error=0,y_error=0;//右上方为正。小球相对图像中心。
int pre_y_error=0;
int letter_cnt=0;

//这个变量使用之前最好测定一下一个slightmove能让镜头里黄球的质心移动多少个"PIXEL"
//以补码的形式来传输


void TIM4_LocSettlement_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx
}

void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
	{		
		if(islocalization_enabled)
		{
			//有点像定深度……需要一个suppress函数
			if(y_error!=1000)
					pre_y_error=y_error;
			if(y_error<-localization_threshold||y_error>localization_threshold)//当前是50
			{	
				left=left_couple*(y_error<0)+1*(y_error>0);
				right=right_couple*(y_error<0)+1*(y_error>0);
				if(y_error==1000)
				{
					left=left_couple*(pre_y_error<0)+1*(pre_y_error>0);//没法应对pre_error为0，拍不到黄球时，preerror应为正负即可，绝对值无所谓
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
			//进入它之前，一定要在"外部"调用如下语句
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
		}//定时器的周期相当于是转弯脉冲的低电平

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}


void Pi_Stm32_unpackData(uint8_t *buf,u16 len)
{
	switch(buf[3])
  {
		case MSG_NAVI_ID:
				quan_navi_angle=(buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+buf[7];
				navi_angle=-1*quan_navi_angle/pi*180/1000;//中心到色带角度，机器人在灯带右侧为正
				navi_distance=(buf[8]<<24)+(buf[9]<<16)+(buf[10]<<8)+buf[11];//中心到中线的像素数，机器人偏右侧时distance为正
//				printf("angle=%d distance=%d\n",navi_angle,navi_distance);
				
				navi_angle_compensation=navi_angle-yaw;
				//if(brake_button==0)//只有在button松开的情况下，button变量才会接受新的值
				brake_button=(buf[12]<<24)+(buf[13]<<16)+(buf[14]<<8)+buf[15];

				break;
		case MSG_REGISTER_ID:
				break;
		case MSG_LOCALIZATION_ID:
				x_error=-1*((buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+buf[7]);//先x后y，以long的形式传输数据
				y_error=(buf[8]<<24)+(buf[9]<<16)+(buf[10]<<8)+buf[11];
				//printf("x_error=%d y_error=%d\n",x_error,y_error);
				break;
		case MSG_ALPHABET_ID:
				letter_cnt++;
				alphabet_class=(buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+buf[7];
				/*可以根据当前小球的坐标来判断哪个字母识别结果是最合适的*/
				printf("Hydrosnapper: %c,%c,%c,%c\n",(int)((alphabet_class&(0xff<<24))>>24)+'@',
																								(int)((alphabet_class&(0xff<<16))>>16)+'@',
																								(int)((alphabet_class&(0xff<<8))>>8)+'@',
																								(int)(alphabet_class&0xff)+'@');
//				printf("the letters are: %c,%c,%c,%c\n",(int)(buf[7])+'@',(int)(buf[6])+'@',(int)(buf[5])+'@',(int)(buf[4])+'@');
				break;
    default:break;
  }
}
