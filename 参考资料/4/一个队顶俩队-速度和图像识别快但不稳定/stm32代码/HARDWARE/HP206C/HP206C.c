#include "HP206C.h"
#include "delay.h"
#include "usart.h"
uchar	OSR_CFG;
uint	OSR_ConvertTime;

float Hp206c_pressure_pase = 0;
float Hp206c_pressure = 0.0;

static ulong    Temperature= 0;
static ulong    Pressure=0;

u8 HP206F_OK = 0;

PressureMsg_t  Hydrosnapper_hp206c;

void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//SLC/SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}

void SDA_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_Out();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}


//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_Out();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_In();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_Out();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_Out();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_Out(); 	    
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_In();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us(1); 
  }					 
	if (!ack)
			IIC_NAck();//����nACK
	else
			IIC_Ack(); //����ACK   
	return receive;
}

void I2C_ByteWrite(uint8_t* pBuffer)
{
	IIC_Start();
	IIC_Send_Byte(HP206C_W_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(*pBuffer);
	IIC_Wait_Ack();
	IIC_Stop();
}

void I2C_BufferRead(uint8_t* pBuffer,uint16_t NumByteToRead)
{
		IIC_Start();
		IIC_Send_Byte(HP206C_R_ADDRESS);
		IIC_Wait_Ack();     
    while(NumByteToRead)
    {
      if (NumByteToRead == 1)
				*pBuffer = IIC_Read_Byte(0);
			else
			{
				*pBuffer = IIC_Read_Byte(1);
				pBuffer++;
      }            
        NumByteToRead--;
    }  
		IIC_Stop();
}

// @brief HP206C״̬���.
uint8_t RDY_Detect(void)
{
    uint8_t cmd = 0x06;
    uint8_t buf[3]={0};
		I2C_ByteWrite(&cmd);
	  delay_ms(10);
    cmd = 0x0D|HP20X_RD_REG_MODE;

		I2C_ByteWrite(&cmd);
    delay_ms(10);
    cmd = HP20X_I2C_DEV_ID|I2C_DID_RD_MASK;
		I2C_ByteWrite(&cmd);
		I2C_BufferRead(buf,1);
    if(buf[0]&0x40)
			return 0;   
    return 1;
}
// @brief ��ȡ��ѹ���¶�. 
void ReadPressureAndTemperature(void){

        uchar buf[6]={0};
        uchar cmd = HP20X_WR_CONVERT_CMD|OSR_CFG;
				I2C_ByteWrite(&cmd);//adc convert
        delay_ms(OSR_ConvertTime*2);			//difference OSR_CFG will be difference OSR_ConvertTime
        cmd = HP20X_READ_PT;
        I2C_ByteWrite(&cmd);
				I2C_BufferRead(buf,6); 
        Temperature = (buf[0]<<16) + (buf[1]<<8) + buf[2];
        if(Temperature&0x800000){Temperature=Temperature|0xff000000;}
        Pressure = (buf[3]<<16) + (buf[4]<<8) + buf[5];
        if(Pressure&0x800000){Pressure|=0xff000000;}
				Hp206c_pressure = (int)Pressure/100;     //   mbar
}



void HP206C_Init(void)
{
    uint8_t t = 0;   
    while(RDY_Detect()&&t<2)//���ֻ�������μ��
			t++;
		if(t>1)
			HP206F_OK = 1;
    OSR_CFG=HP20X_CONVERT_OSR1024;
    OSR_ConvertTime=25;//	25MS
		for(t=0;t<20;t++)
			ReadPressureAndTemperature();
		delay_ms(500);
		ReadPressureAndTemperature();//��ο���������ˮ��
		Hydrosnapper_hp206c.init_value = Hp206c_pressure;
		printf("the initial pressure= %d mbar\n",(int)Hp206c_pressure);
}


// @brief HP206C ���ݶ�ȡ

void GetHp206cData(PressureMsg_t *dev)
{
	//��ȡѹǿ
	ReadPressureAndTemperature();
	dev->depth = Hp206c_pressure-dev->init_value;
	dev->Temperature = (int)Temperature/100.f;
}
// @brief HP206C ����Demo

