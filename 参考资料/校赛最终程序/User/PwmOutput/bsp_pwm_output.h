#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H

#include "stm32f10x.h"

void TIM3_PWM_Init(void);

void motor_speed_set(unsigned char motor_ch,int motor_traget_speed);// 设置推进器的速度

#define Motor_1 4
#define Motor_2 3
#define Motor_3 2
#define Motor_4 1



#endif /* __PWM_OUTPUT_H */

