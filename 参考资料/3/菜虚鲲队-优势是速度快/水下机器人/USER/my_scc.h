#ifndef __MY_SCC_H
#define __MY_SCC_H
#include "stm32f10x.h"
#include "delay.h"
#include "led.h" 
#include "motor.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "jy901.h"
#include "sx1278.h"
#include "stm32_pi.h"
#include "hp206c_10ba.h"
#include "math.h"
#include "adc.h"
#include "algorithm.h"
extern float speed;
extern u8 motor_start;
extern u16 second;
extern u16 daka_count;
void set_nvic_irq(u8 Channel,u8 PreemptionPriority,u8 SubPriority);//设定中断优先级

extern u8 key_old;
extern u8 key_cnt;
extern u8 key_sign;
void key_scanf(void);

extern float battery;
void calculate_bat(void);
#endif
