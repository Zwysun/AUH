#ifndef __BOARD_H
#define __BOARD_H

#include "includes.h"

#define SHT35_IIC             hi2c2
#define JY901_USART           huart1
#define PI_USART              huart2
#define SX1278_USART          huart3

#ifdef FREERTOS_SYS
	#define GET_TICK_COUNT()    xTaskGetTickCount()
#else
	#define GET_TICK_COUNT()    HAL_GetTick()
#endif

#define RX_LEN 256  
typedef struct  
{  
uint8_t  RX_flag:1;        //IDLE receive flag
uint16_t RX_Size;          //receive length
uint8_t  RX_pData[RX_LEN]; //DMA receive buffer
}UsartRec_t;  

void Task_Queue_Semaphore_Timers_Create(void);//消息队列创建
void delay_ms(int nms);
void BSP_Init(void);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart); 
#endif
