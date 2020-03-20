#ifndef __MYIIC_H
#define __MYIIC_H
#include "includes.h" 
 
//引脚定义
#define HP206F_IIC_PORT                GPIOB
#define HP206F_IIC_SCL_PIN             GPIO_PIN_6
#define HP206F_IIC_SDA_PIN             GPIO_PIN_7
//#define delay_us(x)                Tim6_DelaynUS(x) 
//IO方向设置
void SDA_In(void);
void SDA_Out(void);
 
//IO操作函数	
#define SCL(x)        (x?HAL_GPIO_WritePin(HP206F_IIC_PORT, HP206F_IIC_SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(HP206F_IIC_PORT, HP206F_IIC_SCL_PIN, GPIO_PIN_RESET))
#define SDA(x)        (x?HAL_GPIO_WritePin(HP206F_IIC_PORT, HP206F_IIC_SDA_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(HP206F_IIC_PORT, HP206F_IIC_SDA_PIN, GPIO_PIN_RESET))
#define READ_SDA()     HAL_GPIO_ReadPin(HP206F_IIC_PORT,HP206F_IIC_SDA_PIN)
 
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				        //发送IIC开始信号
void IIC_Stop(void);	  			      //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			    //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				      //IIC等待ACK信号
void IIC_Ack(void);					        //IIC发送ACK信号
void IIC_NAck(void);				        //IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















