#include "hp206c_10ba.h"
#include "myiic.h"
uchar	OSR_CFG;
uint	OSR_ConvertTime;

float Hp206c_pressure_pase = 0;
float Hp206c_temperture = 0.0;
float Hp206c_pressure = 0.0;
float Hp206c_Deepth = 0.0;
static ulong    Temperature= 0;
static ulong    Pressure=0;
long     Altitude=0;

uint32_t pressure_buf[10 + 1] = {0};
/**
  * @brief 写一个字节.
  */
PressureMsg_t hp206c;
static void i2c1_init(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}
////ZRX          
////单字节写入*******************************************

//void Single_Write(u8 REG_Address,u8 REG_data)		    
//{
//  I2C_GenerateSTART(I2C1, ENABLE);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//  I2C_Send7bitAddress(I2C1, HP206C_W_ADDRESS, I2C_Direction_Transmitter);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//  I2C_SendData(I2C1, REG_Address);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//  I2C_SendData(I2C1, REG_data); 
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//  I2C_GenerateSTOP(I2C1, ENABLE);
//}

//单字节读取*****************************************
//u8 Single_Read(u8 REG_Address)
//{   
//	u8 REG_data;     	
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//	I2C_GenerateSTART(I2C1, ENABLE);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//  I2C_Send7bitAddress(I2C1, HP206C_W_ADDRESS, I2C_Direction_Transmitter);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//  I2C_SendData(I2C1, REG_Address);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//  I2C_GenerateSTART(I2C1, ENABLE);
//	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//  I2C_Send7bitAddress(I2C1, HP206C_W_ADDRESS, I2C_Direction_Receiver);
//  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//  I2C_AcknowledgeConfig(I2C1, DISABLE);
//	I2C_GenerateSTOP(I2C1, ENABLE);
//	while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
//	REG_data=I2C_ReceiveData(I2C1);
//  I2C_AcknowledgeConfig(I2C1, ENABLE);
//	return REG_data;
//}		
void I2C_ByteWrite(uint8_t* pBuffer)
{
//	IIC_Start();
//	IIC_Send_Byte(HP206C_W_ADDRESS);
//	IIC_Wait_Ack();
//	IIC_Send_Byte(*pBuffer);
//	IIC_Wait_Ack();
//	IIC_Stop();
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C1, HP206C_W_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C_SendData(I2C1, *pBuffer);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTOP(I2C1, ENABLE);
}
/**
  * @brief 读数据.
  */
void I2C_BufferRead(uint8_t* pBuffer,uint16_t NumByteToRead)
{
//		u8 num = 0;
//		u8 sta;

//		IIC_Start();
//		IIC_Send_Byte(HP206C_R_ADDRESS);
//		sta = IIC_Wait_Ack();
//		//if(sta == 1){ printf("err\n");}      
//    while(NumByteToRead)
//    {
//       	if (NumByteToRead == 1)
//				{
//           *pBuffer = IIC_Read_Byte(0);
//            //printf("pbuffer[1] = %d\n",*pbuffer);            
//				}else{
//           *pBuffer = IIC_Read_Byte(1);
//            pBuffer++;
//        }            
//        NumByteToRead--;
//    }  
//		IIC_Stop();
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, HP206C_W_ADDRESS, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	while(NumByteToRead)
	{
		if (NumByteToRead == 1)
		{
			I2C_AcknowledgeConfig(I2C1, DISABLE);
		}
		while(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
		*pBuffer=I2C_ReceiveData(I2C1); 
		pBuffer++;		
		NumByteToRead--;
	}
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
}

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
    if(buf[0]&0x40){
        //printf("Dev is Ready\n");
        return 0;
    }else{
       // printf("Dev ERROR\n");
    }    
    return 1;
}

/**
  * @brief 读取温度.
  */      
void ReadTemperature(void)
{
	uchar buf[3]={0};
	uchar cmd = HP20X_WR_CONVERT_CMD|OSR_CFG;
	I2C_ByteWrite(&cmd);      //adc convert
	delay_ms(OSR_ConvertTime);//difference OSR_CFG will be difference OSR_ConvertTime
	cmd = HP20X_READ_T;
	I2C_ByteWrite(&cmd); 
	I2C_BufferRead(buf,3);
	Temperature = (buf[0]<<16) + (buf[1]<<8) + buf[2];
	if(Temperature&0x800000){Temperature=Temperature|0xff000000;}       
}  
/**
  * @brief 读取气压.
  */ 
void ReadPressure(void)
{
	uchar buf[3]={0};
	uchar cmd = HP20X_WR_CONVERT_CMD|OSR_CFG;
	I2C_ByteWrite(&cmd);//adc convert
	delay_ms(OSR_ConvertTime);			//difference OSR_CFG will be difference OSR_ConvertTime
	cmd = HP20X_READ_P;
	I2C_ByteWrite(&cmd);
	I2C_BufferRead(buf,3);
	Pressure = (buf[0]<<16) + (buf[1]<<8) + buf[2];
	if(Pressure&0x800000){Pressure|=0xff000000;}
	Hp206c_pressure = ((float)(int)Pressure - 52428.8)/41.94304;     //   mbar  
}
/**
  * @brief 读取气压和温度.
  */ 
void ReadPressureAndTemperature(void)
{
	uchar buf[6]={0};
	uchar cmd = HP20X_WR_CONVERT_CMD|OSR_CFG;
	I2C_ByteWrite(&cmd);//adc convert
	delay_ms(OSR_ConvertTime*2);			//difference OSR_CFG will be difference OSR_ConvertTime
	cmd = HP20X_READ_PT;
	I2C_ByteWrite(&cmd);
	I2C_BufferRead(buf,6); 
	Temperature = (buf[0]<<16) + (buf[1]<<8) + buf[2];
	if(Temperature&0x800000){Temperature=Temperature|0xff000000;}
	//printf("Temperature=%.2f\n", (int)Temperature/100.0);
	Pressure = (buf[3]<<16) + (buf[4]<<8) + buf[5];
	if(Pressure&0x800000){Pressure|=0xff000000;}
	Hp206c_pressure = (int)Pressure/100;     //   mbar 
}
void HP206C_DiscardErrorData(void)
{
	
}
/**********************************************************************************************************
*函 数 名: 
*功能说明:  
*形    参:  
*                   
*              
*返 回 值:  
**********************************************************************************************************/
static uint32_t Deepth_RecursionFilter(uint32_t *filterbuf,uint32_t ADValue)
{
	uint32_t filtersum = 0;
	int i= 0; 
	filterbuf[10] = ADValue;
	for( i = 0; i < 10; i++) 
	{
		filterbuf[i] = filterbuf[i + 1]; // 
		filtersum += filterbuf[i];
	}
	return (int)(filtersum / 10);
}
u8 HP206F_OK = 0;
/**
  * @brief HP206C初始化
  */
void HP206C_Init(void)
{
    uint8_t t = 0;  
		int i;
	i2c1_init();
//	IIC_Init();
    while(RDY_Detect()&&t<2){t++;
    }
		if(t>1){HP206F_OK = 1;}
    OSR_CFG=HP20X_CONVERT_OSR1024;
    OSR_ConvertTime=25;//	25MS
		for(i =0;i<10;i++)
		{
			 delay_ms(100);
			 ReadPressureAndTemperature();
			 pressure_buf[i]=Hp206c_pressure;
		}
    Hp206c_pressure_pase = Deepth_RecursionFilter(pressure_buf,Hp206c_pressure);
}


/**
  * @brief HP206C 数据读取
  */
void GetHp206cData(PressureMsg_t *dev)
{
	//读取压强
	ReadPressureAndTemperature();
	
	Hp206c_Deepth = (Deepth_RecursionFilter(pressure_buf,Hp206c_pressure)-Hp206c_pressure_pase)*100/98;
	if(Hp206c_Deepth<0){Hp206c_Deepth = 0;}
	dev->depth =  Hp206c_Deepth;
	dev->init_value = Hp206c_pressure_pase;
	dev->Pressure = Hp206c_pressure;
	dev->Temperature = (int)Temperature/100.f;
}
