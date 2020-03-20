#include "usart.h"
#include "delay.h"
#include "pid_for_posture.h"

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define  front_mid          1500
#define  back_mid           1500
#define  left_mid           1493
#define  right_mid          1507

//换下一台机器之前一定要把配重块的位置记下来，然后打乱配重块

//首先要确保内部空间完全进水再根据roll pitch来调节配重，没盖盖子的话就不需要考虑进水不均匀的问题
//如果没有完全进水的话，浮力太大，难以下潜(正面朝上晃一晃，背面朝上晃一晃，把气泡拿掉)
//首先要确保静止的时候可以稳在中心，以此为依据来调节配重
//目前前方比后方大7，如何确定差值（在水下打印pitch角）。目前来看好像是差一个const
//随后确定在什么值可以刚好补偿中性浮力的偏差(,),这个值和电量有关
//前进(,)
//问题？一开始转，就要下沉。方案：在机器人里面塞浮力材料（例如小球），使得随遇平衡需要较大的PWM
//但是小球太狠了，所以做法是在小球上贴一些配重块，以此来弥补

//深度控制的逻辑行了，但是有残差（原因可能是平衡点的pwm不精确,只有P控制没法贴近），也可能是roll角度的轻微偏差导致
	//加上I的系数后或许会好一些
//pitch的控制只要一加上，马上就不行了，有待解决！！！！

//前后两个的转速太大，对roll的抗干扰能力就会下降


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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_BaseInitStructure.TIM_Period = arr;    //除了arr和psc，别的都是默认配置
    TIM_BaseInitStructure.TIM_Prescaler = psc;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
		TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//定时器通道配置，大部分都是默认状态
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

    //主输出bit使能（MOE位使能）
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    //TIM1启动
    TIM_Cmd(TIM1, ENABLE);
		TIM_Cmd(TIM3, ENABLE);
		
		//解锁电调
		TIM_SetCompare1(TIM1,1500);
		TIM_SetCompare2(TIM1,1500);
		TIM_SetCompare3(TIM1,1507);
		TIM_SetCompare4(TIM1,1500);
		TIM_SetCompare1(TIM3,1500);
		TIM_SetCompare2(TIM3,1500);
		TIM_SetCompare3(TIM3,1493);
		TIM_SetCompare4(TIM3,1500);	
		delay_ms(100);//等待电调解锁
}


void Setpwm_UD(void)
{
	static int forward_pwm=0,behind_pwm=0;
	forward_pwm =  forward+91;//91;
	behind_pwm =  behind+79;//79;

	forward_pwm=constrain(forward_pwm,66,150);//确保始终在转
	behind_pwm=constrain(behind_pwm,54,150);
	TIM_SetCompare2(TIM3,1500+forward_pwm);
	TIM_SetCompare1(TIM1,1500+behind_pwm);
}

void Setpwm_LR(void)
{
	static int left_pwm=0,right_pwm=0;
	//左右需要同时前进or同时倒退，故两个方向都应该能使力相等
	left_pwm =  left+44*(left>0)-59*(left<0);//
	right_pwm =  right+55*(right>0)-47*(right<0);//

	left_pwm=constrain(left_pwm,-150,150);//arr设为20000，水下也许一点点推力就够了
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
