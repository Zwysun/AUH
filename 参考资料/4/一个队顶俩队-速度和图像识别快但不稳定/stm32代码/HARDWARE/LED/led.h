#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//用来指示字母识别or打卡的结果
#define LED1 PBout(8)
#define LED2 PBout(9)
#define LED3 PEout(0)	
#define LED4 PEout(1)
#define LED5 PEout(5)	
#define LED6 PEout(4)
#define LED7 PEout(3)
#define LED8 PEout(2)

#define LIGHTEN_ALL 0
#define SHUTDOWN_ALL 1
#define LIGHTEN_EVEN_PORT 2
#define LIGHTEN_ODD_PORT 3

void LED_Init(void);//初始化
void LED_Modify(int pattern);
void TIM2_LEDIndicator_Init(u16 arr,u16 psc);
		 				    
#endif
