#include "sx1278.h"
/**
  ******************************************************************************
  * @file    sx1278.c
  * @author  LiuYang
  * @brief   Loraģ��
  *
  *
  ******************************************************************************
  * @attention
  *	Loraģ�� ����3 Ĭ�ϲ�����: 9600
	* E32-170T30D 170MHz 0x0000 0x28 2.4kbps 9600 8N1 1W
  *	ģ�������ֹ���ģʽ�������� M0�� M1 ���ã���ϸ������±���ʾ��
	*	ģʽ��0-3��       M0      M1                  ģʽ����                                               ��ע
	*	0 һ��ģʽ        0       0            ���ڴ򿪣����ߴ򿪣�͸������                           ���շ�������ģʽ 0�� 1
	*
	*	1 ����ģʽ        1       0						 ���ڴ򿪣����ߴ򿪣�                                  ���շ�������ģʽ 0
	*	                                       ��ģʽ 0 Ψһ�������ݰ�����ǰ���Զ����ӻ����룬       ���շ�������ģʽ 1
	*                                        �������ܻ��ѹ�����ģʽ 2 �Ľ��շ�
  *
	*	2 ʡ��ģʽ        0       1            ���ڽ��չرգ����ߴ��ڿ��л���ģʽ���յ�������          ���䷽����ģʽ 1
	* 																			 �ݺ󣬴򿪴��ڷ������ݡ�                               ��ģʽ�²��ܷ���
	* 
	* 
	* 3 ����ģʽ        1        1           ģ��������ߣ����Խ��ղ�����������                     
	��������������
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
