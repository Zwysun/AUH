#ifndef __NRF24L01_H
#define __NRF24L01_H
#include "stm32f10x.h"
#define CE(a) if(a) GPIO_SetBits(GPIOA,GPIO_Pin_12);else GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define CSN(a) if(a) GPIO_SetBits(GPIOB,GPIO_Pin_12);else GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define IRQ GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define tx_mode 0x0e
#define rx_mode 0x0f
#define power_saving 0x00
#define RX_DR 6//接受数据中断位
#define TX_DS 5//接受应答中断位(就是发送成功中断)
#define MAX_RT 4//重发溢出中断位

extern u8 rx_add[5];//发送地址
extern u8 nrf_data[32];//无线发送数据
extern u8 rx_ok;

void nrf_rx(void);
void nrf_tx(void);
u8 Read_Status(void);       
void Clean_RXFifo(void);//清除接受数据
void Clean_TXFifo(void);//清除发送数据
void Read_RX(u8 pBuf[],u8 len);//读接受数据
void Write_TX(u8 pBuf[],u8 len);//写发送数据
void tx_data_launch(void);//无线数据发射
void deal_with_interrupt(void);
void NRF24L01_check(void);
void NRF24L01_Init(void);
#endif
