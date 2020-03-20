#include "MS5803.h"


void MS5803_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; //AD0->PB4,PS->PB3�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//�����趨������ʼ��GPIOB3/4
	
	GPIO_SetBits(GPIOB,GPIO_Pin_3);			//PB3 �����
	GPIO_SetBits(GPIOB,GPIO_Pin_4);			//PB4 �����
}

void MS5803_Reset(void)
{
			IIC_Start();                  //IIC��������
	IIC_Send_Byte(MS5803_WRITE);      //����д����
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��
	IIC_Send_Byte(MS5803_RESET);      //���͸�λ����
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��
			IIC_Stop();				            //����һ��ֹͣ����
}

u16 MS5803_Read_PROM(u8 addr)
{
	u8 buf[2];
	u16 res;
	
			IIC_Start();                  //IIC��������
	IIC_Send_Byte(MS5803_WRITE);      //����д����	
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��
	IIC_Send_Byte(addr);              //���� PROM ��ַ
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
			IIC_Stop();				            //����һ��ֹͣ����	

			IIC_Start();                  //IIC��������	
	IIC_Send_Byte(MS5803_READ);       //���Ͷ�����	
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
	buf[0]=IIC_Read_Byte(1);		      //��ȡ����,����ACK   
	buf[1]=IIC_Read_Byte(0);		      //��ȡ����,����nACK 
			IIC_Stop();				            //����һ��ֹͣ����
	
	res=((u16)buf[0]<<8)|buf[1]; 	
	
	return res;
} 

u32 MS5803_Read_ADC(u8 addr)
{
	u8 buf[3];	
	u32 res;
	
			IIC_Start();                  //IIC��������		
	IIC_Send_Byte(MS5803_WRITE);      //����д����
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
	IIC_Send_Byte(addr);              //�����¶Ȼ���ѹ��ת��
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
			IIC_Stop();				            //����һ��ֹͣ����
	
	delay_ms(10);                    //�ȴ�ADת������	
	
			IIC_Start();                  //IIC��������		
	IIC_Send_Byte(MS5803_WRITE);      //����д����
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
	IIC_Send_Byte(MS5803_ADC_READ);   //����ADC��ȡ����
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
			IIC_Stop();				            //����һ��ֹͣ����
	
			IIC_Start();                  //IIC��������	
	IIC_Send_Byte(MS5803_READ);       //���Ͷ�����	
	IIC_Wait_Ack();	                  //�ȴ�Ӧ��	
	buf[0]=IIC_Read_Byte(1);		      //��ȡ����,����ACK   
	buf[1]=IIC_Read_Byte(1);		      //��ȡ����,����ACK 
	buf[2]=IIC_Read_Byte(0);		      //��ȡ����,����nACK
			IIC_Stop();				            //����һ��ֹͣ����	
			
	res=((u32)buf[0]<<16)|((u16)buf[1]<<8)|buf[2]; 	    

	return res;               
}	

