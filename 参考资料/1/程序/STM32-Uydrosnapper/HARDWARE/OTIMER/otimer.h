#ifndef __OTIMER_H
#define __OTIMER_H

#include "sys.h"
#include "stm32f10x_tim.h"
void TIM6_PosPID_Init(u16 arr,u16 psc);
void TIM7_LinePID_Init(u16 arr,u16 psc);
#endif
