#include "usart.h"
USART_DATA uart1;
USART_DATA uart2;
USART_DATA uart3;
static void USART_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
static void USART_Configuration(void)//串口1配置 波特率115200
{  
  USART_InitTypeDef USART_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	
  USART_InitStructure.USART_BaudRate =115200; 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;  
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_Init(USART1, &USART_InitStructure); 
	USART_Init(USART2, &USART_InitStructure);
	USART_InitStructure.USART_BaudRate =9600; 
	USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_TC,ENABLE);	
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_TC,ENABLE);	
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_TC,ENABLE);	
	
	USART_ClearITPendingBit(USART1,USART_IT_TC);
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	USART_ClearFlag(USART1,USART_FLAG_ORE); 
	
	USART_ClearITPendingBit(USART2,USART_IT_TC);
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	USART_ClearFlag(USART2,USART_FLAG_ORE);
	
	USART_ClearITPendingBit(USART3,USART_IT_TC);
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	USART_ClearFlag(USART3,USART_FLAG_ORE);
	
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
}
void usart_INIT(void)//串口初始化
{
	USART_GPIO();
	USART_Configuration();
	uart1.usart_n=USART1;
	uart2.usart_n=USART2;
	uart3.usart_n=USART3;
	uart1.tx_busy=0;
	uart2.tx_busy=0;
	uart3.tx_busy=0;
}
void uart_tx_start(USART_DATA *uart_data_struct,u8 data[],u8 length)//串口中断发送数据
{
	u8 i;
	
	if(length>100) uart_data_struct->tx_length=100;
	else uart_data_struct->tx_length=length;
	for(i=0;i<uart_data_struct->tx_length;i++)
	{
		uart_data_struct->tx_buff[i]=data[i];
	}
	while(uart_data_struct->tx_busy);
	uart_data_struct->tx_busy=1;
	USART_SendData(uart_data_struct->usart_n,uart_data_struct->tx_buff[0]);
	uart_data_struct->tx_numb=1;
}
void tx_it_deal(USART_DATA *uart_data_struct)
{
	if(uart_data_struct->tx_busy)
  { 
	  USART_SendData(uart_data_struct->usart_n,uart_data_struct->tx_buff[uart_data_struct->tx_numb]);
	  uart_data_struct->tx_numb++;
	  if(uart_data_struct->tx_numb>=uart_data_struct->tx_length) uart_data_struct->tx_busy=0;
	}
}
