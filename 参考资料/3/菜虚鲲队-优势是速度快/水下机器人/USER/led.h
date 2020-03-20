#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"

extern u8 led_int_flag;
extern u8 flash_count;
void LED_Init(void);//初始化		 				 
void led_flash(u16 i);//led快闪模式
void start_led(void);
void LED_SW(u8 num,u8 sta);
void led_flash_int(void);
#endif
