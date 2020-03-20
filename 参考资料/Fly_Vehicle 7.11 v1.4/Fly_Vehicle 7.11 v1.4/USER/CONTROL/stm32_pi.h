#ifndef __STM32_PI_H
#define __STM32_PI_H

#include "includes.h"
/****************remote message id*****************/
#define MSG_TEMP_HUMI_ID    0x00
#define MSG_PRESSURE_ID     0x01
#define MSG_COMPASS_ID      0x02
#define MSG_POWER_ID        0x03
#define MSG_CMD_ID          0x04
#define MSG_GIMBAL_ID       0x05
#define MSG_LIGHT_ID        0x06
#define MSG_PID_ID          0x07
#define MSG_STATE_ID        0x08
#define MSG_TEST_ID         0x09
#define MSG_PHONE_ID  			0X22
#define MSG_GPS_ID          0x20
#define MSG_BLANCE_EN_ID    0X21

#define MSG_NAVI_ID         0X23
typedef struct 
{
    u16 max_count;
	  u16 temp_count;
	  bool  loss_flag;
}FeedCounter_t;
//Pi - stm32 ¶Ï¿ª¼ì²â
void Pi_Counter_Init(void);
void Pi_LossCounter(void);
void Pi_FeedCounter(void);
bool Pi_CounterCheck(void);
//µç×ÓÂÞÅÌ ¶Ï¿ª¼ì²â 
void Compass_Counter_Init(void);
void Compass_LossCounter(void);
void Compass_FeedCounter(void);
bool Compass_CounterCheck(void);

#define PI_RX_LEN 256  
typedef struct  
{  
	uint8_t  RX_flag:1;           //IDLE receive flag
	uint16_t RX_Size;             //receive length
	uint8_t  RX_pData[PI_RX_LEN]; //DMA receive buffer
}PI_UsartRec_t;  

void Stm32_Pi_Init(void);
void HAL_PI_To_UART_Receive_IDLE(UART_HandleTypeDef *huart);
void Pi_Stm32_unpackData(uint8_t *buf,u16 len);//Pi - stm32 ½âÎöº¯Êý
void PI_STM32_Test_Demo(void);
 
#endif
