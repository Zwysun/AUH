#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x.h"
#define cnt0 0x7fff//脉冲初始值
#define set_ch1_duty(duty) TIM3->CCR1=duty;
#define set_ch2_duty(duty) TIM3->CCR2=duty;
#define set_ch3_duty(duty) TIM3->CCR3=duty;
#define set_ch4_duty(duty) TIM3->CCR4=duty;


void motor_init(void);//电机所有功能初始化
void set_up_down(s16 duty1,s16 duty2);
void motor_control(void);
#endif
