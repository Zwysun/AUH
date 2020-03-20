#ifndef __NRF24L01_H
#define __NRF24L01_H
#include "stm32f10x.h"
#define CE(a) if(a) GPIO_SetBits(GPIOA,GPIO_Pin_12);else GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define CSN(a) if(a) GPIO_SetBits(GPIOB,GPIO_Pin_12);else GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define IRQ GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define tx_mode 0x0e
#define rx_mode 0x0f
#define power_saving 0x00
#define RX_DR 6//���������ж�λ
#define TX_DS 5//����Ӧ���ж�λ(���Ƿ��ͳɹ��ж�)
#define MAX_RT 4//�ط�����ж�λ

extern u8 rx_add[5];//���͵�ַ
extern u8 nrf_data[32];//���߷�������
extern u8 rx_ok;

void nrf_rx(void);
void nrf_tx(void);
u8 Read_Status(void);       
void Clean_RXFifo(void);//�����������
void Clean_TXFifo(void);//�����������
void Read_RX(u8 pBuf[],u8 len);//����������
void Write_TX(u8 pBuf[],u8 len);//д��������
void tx_data_launch(void);//�������ݷ���
void deal_with_interrupt(void);
void NRF24L01_check(void);
void NRF24L01_Init(void);
#endif
