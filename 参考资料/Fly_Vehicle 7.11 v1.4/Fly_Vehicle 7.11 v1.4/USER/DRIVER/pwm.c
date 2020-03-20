#include "pwm.h"
////摄像头使用
//void PWM_ServoInit(void)
//{
////	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
////	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,SERVO_INIT);
//}
//int GetAngle = 0;
//void SetCameraAngle(int value)
//{
////	GetAngle = value;
////	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(GetAngle-50)*6+SERVO_INIT);
//}

uint16_t pwm_left=1500;
uint16_t pwm_right=1500;
uint16_t pwm_front=1500;
uint16_t pwm_back=1500;

uint16_t test_pwm=1500;//所有默认值都是1500//小：上下正面出风，下沉；左右背面出风，前进。
uint16_t *test_pwm1=&pwm_front;//上下（13）pwm范围1456，1558之外
uint16_t test_pwm2=1500;
uint16_t *test_pwm3=&pwm_right;//上下（13）pwm范围1456，1558之外
uint16_t test_pwm4=1500;
uint16_t *test_pwm5=&pwm_back;
uint16_t test_pwm6=1500;
uint16_t *test_pwm7=&pwm_left;
uint16_t test_pwm8=1500;

void PWM_Init(void)
{
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,mid);
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,mid);
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,right_mid);//  水平  右推进器
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,mid);
//	
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,mid);
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,mid);
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,left_mid);//  水平  左推进器
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,mid);
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1490);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1490);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1490);//  水平  右推进器
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1490);
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1490);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1490);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1490);//  水平  左推进器
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1490);
	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	
	HAL_Delay(2000);
}

void PWM_Test_Demo(void)
{
	PWM_Init();
	
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1490);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1490);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1490);//  水平  右推进器
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1490);
	
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1490);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1490);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1490);//  水平  左推进器
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1490);
	
	HAL_Delay(3000);
	
	while(1)
	{
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   上下
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   前后
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);//
//		
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   上下
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);// 
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   前后
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);//

		HAL_Delay(500);
	}
}

void ForwardStop(void)
{
	pwm_left = 1500;
	pwm_right = 1500;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
}

void Cruise(void)
{
	pwm_right = 1449;//-2
	pwm_left = 1554;//+2
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
}
