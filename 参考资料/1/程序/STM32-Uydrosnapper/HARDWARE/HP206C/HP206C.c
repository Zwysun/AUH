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
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//SLC/SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
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

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_Out();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}


//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_Out();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_In();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_Out(); 	    
	IIC_SCL=0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_In();//SDA设置为输入
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
			IIC_NAck();//发送nACK
	else
			IIC_Ack(); //发送ACK   
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

// @brief HP206C状态检测.
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
// @brief 读取气压和温度. 
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
    while(RDY_Detect()&&t<2)//最多只进行两次检测
			t++;
		if(t>1)
			HP206F_OK = 1;
    OSR_CFG=HP20X_CONVERT_OSR1024;
    OSR_ConvertTime=25;//	25MS
		for(t=0;t<20;t++)
			ReadPressureAndTemperature();
		delay_ms(500);
		ReadPressureAndTemperature();//这次开机必须在水上
		Hydrosnapper_hp206c.init_value = Hp206c_pressure;
		printf("the initial pressure= %d mbar\n",(int)Hp206c_pressure);
}


// @brief HP206C 数据读取

void GetHp206cData(PressureMsg_t *dev)
{
	//读取压强
	ReadPressureAndTemperature();
	dev->depth = Hp206c_pressure-dev->init_value;
	dev->Temperature = (int)Temperature/100.f;
}
// @brief HP206C 测试Demo

