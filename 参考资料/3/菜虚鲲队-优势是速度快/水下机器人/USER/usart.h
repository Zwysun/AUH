#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

typedef struct 
{
	USART_TypeDef *usart_n;
	u8 tx_busy;//����æ��־λ
	u8 tx_length;//���ͳ���
	u8 tx_numb;//��ǰ���͵ڼ����ֽ�
	u8 tx_buff[50];//���ͻ����� ���50���ֽ�
}USART_DATA;

extern USART_DATA uart1;
extern USART_DATA uart2;
extern USART_DATA uart3;
void usart_INIT(void);
void tx_it_deal(USART_DATA *uart_data_struct);
void uart_tx_start(USART_DATA *uart_data_struct,u8 data[],u8 length);
#endif
