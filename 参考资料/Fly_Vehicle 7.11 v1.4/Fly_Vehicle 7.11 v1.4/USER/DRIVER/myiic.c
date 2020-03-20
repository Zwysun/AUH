#include "myiic.h"
 
	
void SDA_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
   
  GPIO_InitStruct.Pin = HP206F_IIC_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HP206F_IIC_PORT, &GPIO_InitStruct);
}
void SDA_In(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
   
  GPIO_InitStruct.Pin = HP206F_IIC_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HP206F_IIC_PORT, &GPIO_InitStruct);
}
//��ʼ��IIC
//void IIC_Init(void)
//{			
//  GPIO_InitTypeDef  GPIO_InitStructure;

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��

//  //GPIOB8,B9��ʼ������
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
//	IIC_SCL=1;
//	IIC_SDA=1;
//}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_Out();     //sda�����
	SDA(1);	  	  
	SCL(1);
	delay_sys_us(4);
 	SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_sys_us(4);
	SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_Out();//sda�����
	SCL(0);
	SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_sys_us(4);
	SCL(1); 
	SDA(1);//����I2C���߽����ź�
	delay_sys_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_In();      //SDA����Ϊ����  
	SDA(1);delay_sys_us(1);	   
	SCL(1);delay_sys_us(1);	 
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	SCL(0);
	SDA_Out();
	SDA(0);
	delay_sys_us(2);
	SCL(1);
	delay_sys_us(2);
	SCL(0);
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	SCL(0);
	SDA_Out();
	SDA(1);
	delay_sys_us(2);
	SCL(1);
	delay_sys_us(2);
	SCL(0);
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_Out(); 	    
	SCL(0);//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		SDA((txd&0x80)>>7);
		txd<<=1; 	  
		delay_sys_us(2);   //��TEA5767��������ʱ���Ǳ����
		SCL(1);
		delay_sys_us(2); 
		SCL(0);	
		delay_sys_us(2);
	}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_In();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		SCL(0); 
		delay_sys_us(2);
		SCL(1);
		receive<<=1;
		if(READ_SDA())receive++;   
		delay_sys_us(1); 
  }					 
	if (!ack)
			IIC_NAck();//����nACK
	else
			IIC_Ack(); //����ACK   
	return receive;
}



























