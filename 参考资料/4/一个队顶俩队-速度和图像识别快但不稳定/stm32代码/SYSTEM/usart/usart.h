#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART3_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART3_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART3_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define USART1_REC_LEN	250
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART3_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬���
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART2_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         		//����״̬���	
extern u8 USART1_RX_BUF[USART1_REC_LEN];
extern __IO u8 USART1_Rx_CNT;

//����봮���жϽ��գ��벻Ҫע�����º궨��
void Lora_Usart3_Init(u32 bound);
void Raspberry_Usart2_Init(u32 bound);
void IMU_Usart1_Init(u32 bound);
extern int startswitch;
extern int isposeloopclosed;
extern int islineloopclosed;
extern int framecmd_send;
extern int frameans_receive;
extern float Hp206c_pressure;
extern int test_turn_angle;
extern int test_lag_cnt;
extern int test_speed;
#endif


