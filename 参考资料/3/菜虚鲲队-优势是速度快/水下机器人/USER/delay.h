#ifndef __DELAY_H
#define __DELAY_H
#include "my_scc.h"
#include "stm32f10x.h"
void delay_init(void);//调用延时函数前必须初始化
void delay(u32 time);//查询方式 最大范围 1 864 135 us
void delay_sec(u32 second);//秒延时 采用中断 小心与其他中断冲突

#define delay_ms(time) delay(time*9000)
#define delay_us(time) delay(time*9)

#endif
