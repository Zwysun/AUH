#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//����ָʾ��ĸʶ��or�򿨵Ľ��
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

void LED_Init(void);//��ʼ��
void LED_Modify(int pattern);
void TIM2_LEDIndicator_Init(u16 arr,u16 psc);
		 				    
#endif
