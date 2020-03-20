
/*******************************************************************************
// 延时程序
// 使用单片机STM32F103C8T6
// 晶振：8.00M
*******************************************************************************/

#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























