#ifndef __MYIIC_H
#define __MYIIC_H
#include "my_scc.h"
 
//���Ŷ���
#define HP206F_IIC_PORT                GPIOB
#define HP206F_IIC_SCL_PIN             GPIO_Pin_6
#define HP206F_IIC_SDA_PIN             GPIO_Pin_7
//#define delay_us(x)                Tim6_DelaynUS(x) 
//IO��������
void SDA_In(void);
void SDA_Out(void);
 
//IO��������	
#define SCL(x)        (x?GPIO_WriteBit(HP206F_IIC_PORT, HP206F_IIC_SCL_PIN, Bit_SET):GPIO_WriteBit(HP206F_IIC_PORT, HP206F_IIC_SCL_PIN, Bit_RESET))
#define SDA(x)        (x?GPIO_WriteBit(HP206F_IIC_PORT, HP206F_IIC_SDA_PIN, Bit_SET):GPIO_WriteBit(HP206F_IIC_PORT, HP206F_IIC_SDA_PIN, Bit_RESET))
#define READ_SDA()     GPIO_ReadInputDataBit(HP206F_IIC_PORT,HP206F_IIC_SDA_PIN)
 
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				        //����IIC��ʼ�ź�
void IIC_Stop(void);	  			      //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			    //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				      //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					        //IIC����ACK�ź�
void IIC_NAck(void);				        //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















