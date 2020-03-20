#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

typedef struct 
{
	USART_TypeDef *usart_n;
	u8 tx_busy;//发送忙标志位
	u8 tx_length;//发送长度
	u8 tx_numb;//当前发送第几个字节
	u8 tx_buff[50];//发送缓冲区 最大50个字节
}USART_DATA;

extern USART_DATA uart1;
extern USART_DATA uart2;
extern USART_DATA uart3;
void usart_INIT(void);
void tx_it_deal(USART_DATA *uart_data_struct);
void uart_tx_start(USART_DATA *uart_data_struct,u8 data[],u8 length);
#endif
