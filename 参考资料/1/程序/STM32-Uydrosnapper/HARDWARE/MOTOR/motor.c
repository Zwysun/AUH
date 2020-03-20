#include "usart.h"
#include "delay.h"
#include "pid_for_posture.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define  front_mid          1500
#define  back_mid           1500
#define  left_mid           1493
#define  right_mid          1507

//����һ̨����֮ǰһ��Ҫ�����ؿ��λ�ü�������Ȼ��������ؿ�

//����Ҫȷ���ڲ��ռ���ȫ��ˮ�ٸ���roll pitch���������أ�û�Ǹ��ӵĻ��Ͳ���Ҫ���ǽ�ˮ�����ȵ�����
//���û����ȫ��ˮ�Ļ�������̫��������Ǳ(���泯�ϻ�һ�Σ����泯�ϻ�һ�Σ��������õ�)
//����Ҫȷ����ֹ��ʱ������������ģ��Դ�Ϊ��������������
//Ŀǰǰ���Ⱥ󷽴�7�����ȷ����ֵ����ˮ�´�ӡpitch�ǣ���Ŀǰ���������ǲ�һ��const
//���ȷ����ʲôֵ���Ըպò������Ը�����ƫ��(,),���ֵ�͵����й�
//ǰ��(,)
//���⣿һ��ʼת����Ҫ�³����������ڻ������������������ϣ�����С�򣩣�ʹ������ƽ����Ҫ�ϴ��PWM
//����С��̫���ˣ�������������С������һЩ���ؿ飬�Դ����ֲ�

//��ȿ��Ƶ��߼����ˣ������ввԭ�������ƽ����pwm����ȷ,ֻ��P����û����������Ҳ������roll�Ƕȵ���΢ƫ���
	//����I��ϵ���������һЩ
//pitch�Ŀ���ֻҪһ���ϣ����ϾͲ����ˣ��д������������

//ǰ��������ת��̫�󣬶�roll�Ŀ����������ͻ��½�


int left=0,right=0,forward=0,behind=0;

void PWM_Configuration(u16 arr, u16 psc)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		
	  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_BaseInitStructure.TIM_Period = arr;    //����arr��psc����Ķ���Ĭ������
    TIM_BaseInitStructure.TIM_Prescaler = psc;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
		TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//��ʱ��ͨ�����ã��󲿷ֶ���Ĭ��״̬
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    //�����bitʹ�ܣ�MOEλʹ�ܣ�
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    //TIM1����
    TIM_Cmd(TIM1, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		
		//�������
		TIM_SetCompare1(TIM1,1500);
		TIM_SetCompare2(TIM1,1500);
		TIM_SetCompare3(TIM1,1507);
		TIM_SetCompare4(TIM1,1500);
		TIM_SetCompare1(TIM3,1500);
		TIM_SetCompare2(TIM3,1500);
		TIM_SetCompare3(TIM3,1493);
		TIM_SetCompare4(TIM3,1500);	
		delay_ms(100);//�ȴ��������
}


void Setpwm_UD(void)
{
	static int forward_pwm=0,behind_pwm=0;
	forward_pwm =  forward+91;//91;
	behind_pwm =  behind+79;//79;

	forward_pwm=constrain(forward_pwm,66,150);//ȷ��ʼ����ת
	behind_pwm=constrain(behind_pwm,54,150);
	TIM_SetCompare2(TIM3,1500+forward_pwm);
	TIM_SetCompare1(TIM1,1500+behind_pwm);
}

void Setpwm_LR(void)
{
	static int left_pwm=0,right_pwm=0;
	//������Ҫͬʱǰ��orͬʱ���ˣ�����������Ӧ����ʹ�����
	left_pwm =  left+44*(left>0)-59*(left<0);//
	right_pwm =  right+55*(right>0)-47*(right<0);//

	left_pwm=constrain(left_pwm,-150,150);//arr��Ϊ20000��ˮ��Ҳ��һ��������͹���
	right_pwm=constrain(right_pwm,-150,150);

	TIM_SetCompare3(TIM1,1500-left_pwm);
	TIM_SetCompare3(TIM3,1500+right_pwm);
}

void Setpwm_NoCompensation(void)
{
	TIM_SetCompare3(TIM1,1500-constrain(left,-100,100));
	TIM_SetCompare3(TIM3,1500+constrain(right,-100,100));
	TIM_SetCompare2(TIM3,1500+constrain(forward,-100,120));
	TIM_SetCompare1(TIM1,1500+constrain(behind,-100,120));
}
