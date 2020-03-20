#include "sx1278.h"
/**
  ******************************************************************************
  * @file    sx1278.c
  * @author  LiuYang
  * @brief   Lora模块
  *
  *
  ******************************************************************************
  * @attention
  *	Lora模块 串口3 默认波特率: 9600
	* E32-170T30D 170MHz 0x0000 0x28 2.4kbps 9600 8N1 1W
  *	模块有四种工作模式，由引脚 M0、 M1 设置；详细情况如下表所示：
	*	模式（0-3）       M0      M1                  模式介绍                                               备注
	*	0 一般模式        0       0            串口打开，无线打开，透明传输                           接收方必须是模式 0、 1
	*
	*	1 唤醒模式        1       0						 串口打开，无线打开；                                  接收方可以是模式 0
	*	                                       和模式 0 唯一区别：数据包发射前，自动增加唤醒码，       接收方可以是模式 1
	*                                        这样才能唤醒工作在模式 2 的接收方
  *
	*	2 省电模式        0       1            串口接收关闭，无线处于空中唤醒模式，收到无线数          发射方必须模式 1
	* 																			 据后，打开串口发出数据。                               该模式下不能发射
	* 
	* 
	* 3 休眠模式        1        1           模块进入休眠，可以接收参数设置命令                     
	详见工作参数详解
  *
  ******************************************************************************
  */
SX1278_Parse_State_t sx1278_parse; 
SX1278_Info_t        sx1278_info;

void SX1278_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_11|GPIO_Pin_12);
}

//static void unpackData(uint8_t *buf,u16 len)
//{
//	switch(sx1278_parse)
//	{
//		case SX1278_GET_WORK_STA:
//			if(buf[0]==0xC0){
//				sx1278_info.address = buf[1]<<8|buf[2];
//				sx1278_info.baud = buf[1]<<8|buf[2];
//			}
//			break;
//		case SX1278_GET_VERSION:break;
//		case SX1278_RESET:break;
//		default:break;
//	}
//}


void SX1278_GetData()
{

	 
}
void SX1278_SendData(uint8_t buf[],u16 len)
{
	uart_tx_start(&uart3,buf,len);
}
