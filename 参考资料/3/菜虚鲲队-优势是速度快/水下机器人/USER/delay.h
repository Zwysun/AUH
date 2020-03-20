#ifndef __DELAY_H
#define __DELAY_H
#include "my_scc.h"
#include "stm32f10x.h"
void delay_init(void);//������ʱ����ǰ�����ʼ��
void delay(u32 time);//��ѯ��ʽ ���Χ 1 864 135 us
void delay_sec(u32 second);//����ʱ �����ж� С���������жϳ�ͻ

#define delay_ms(time) delay(time*9000)
#define delay_us(time) delay(time*9)

#endif
