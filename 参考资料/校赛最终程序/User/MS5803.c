#include "MS5803.h"


void MS5803_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; //AD0->PB4,PS->PB3端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//根据设定参数初始化GPIOB3/4
	
	GPIO_SetBits(GPIOB,GPIO_Pin_3);			//PB3 输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_4);			//PB4 输出高
}

void MS5803_Reset(void)
{
			IIC_Start();                  //IIC启动命令
	IIC_Send_Byte(MS5803_WRITE);      //发送写命令
	IIC_Wait_Ack();	                  //等待应答
	IIC_Send_Byte(MS5803_RESET);      //发送复位命令
	IIC_Wait_Ack();	                  //等待应答
			IIC_Stop();				            //产生一个停止条件
}

u16 MS5803_Read_PROM(u8 addr)
{
	u8 buf[2];
	u16 res;
	
			IIC_Start();                  //IIC启动命令
	IIC_Send_Byte(MS5803_WRITE);      //发送写命令	
	IIC_Wait_Ack();	                  //等待应答
	IIC_Send_Byte(addr);              //发送 PROM 地址
	IIC_Wait_Ack();	                  //等待应答	
			IIC_Stop();				            //产生一个停止条件	

			IIC_Start();                  //IIC启动命令	
	IIC_Send_Byte(MS5803_READ);       //发送读命令	
	IIC_Wait_Ack();	                  //等待应答	
	buf[0]=IIC_Read_Byte(1);		      //读取数据,发送ACK   
	buf[1]=IIC_Read_Byte(0);		      //读取数据,发送nACK 
			IIC_Stop();				            //产生一个停止条件
	
	res=((u16)buf[0]<<8)|buf[1]; 	
	
	return res;
} 

u32 MS5803_Read_ADC(u8 addr)
{
	u8 buf[3];	
	u32 res;
	
			IIC_Start();                  //IIC启动命令		
	IIC_Send_Byte(MS5803_WRITE);      //发送写命令
	IIC_Wait_Ack();	                  //等待应答	
	IIC_Send_Byte(addr);              //开启温度或者压力转换
	IIC_Wait_Ack();	                  //等待应答	
			IIC_Stop();				            //产生一个停止条件
	
	delay_ms(10);                    //等待AD转换结束	
	
			IIC_Start();                  //IIC启动命令		
	IIC_Send_Byte(MS5803_WRITE);      //发送写命令
	IIC_Wait_Ack();	                  //等待应答	
	IIC_Send_Byte(MS5803_ADC_READ);   //发送ADC读取命令
	IIC_Wait_Ack();	                  //等待应答	
			IIC_Stop();				            //产生一个停止条件
	
			IIC_Start();                  //IIC启动命令	
	IIC_Send_Byte(MS5803_READ);       //发送读命令	
	IIC_Wait_Ack();	                  //等待应答	
	buf[0]=IIC_Read_Byte(1);		      //读取数据,发送ACK   
	buf[1]=IIC_Read_Byte(1);		      //读取数据,发送ACK 
	buf[2]=IIC_Read_Byte(0);		      //读取数据,发送nACK
			IIC_Stop();				            //产生一个停止条件	
			
	res=((u32)buf[0]<<16)|((u16)buf[1]<<8)|buf[2]; 	    

	return res;               
}	

