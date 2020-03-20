#include "hp206c_10ba.h"
 
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
HAL_StatusTypeDef I2C_ByteWrite(uint8_t* pBuffer)
{
	IIC_Start();
	IIC_Send_Byte(HP206C_W_ADDRESS);
	IIC_Wait_Ack();
	IIC_Send_Byte(*pBuffer);
	IIC_Wait_Ack();
	IIC_Stop();
	//delay_ms(10);
	//return HAL_I2C_Master_Transmit(&HP206C_IIC,HP206C_W_ADDRESS,pBuffer,1,0x100);
}
/**
  * @brief 读数据.
  */
HAL_StatusTypeDef I2C_BufferRead(uint8_t* pBuffer,uint16_t NumByteToRead)
{
		u8 num = 0;
		u8 sta;

		IIC_Start();
		IIC_Send_Byte(HP206C_R_ADDRESS);
		sta = IIC_Wait_Ack();
		//if(sta == 1){ printf("err\n");}      
    while(NumByteToRead)
    {
       	if (NumByteToRead == 1)
				{
           *pBuffer = IIC_Read_Byte(0);
            //printf("pbuffer[1] = %d\n",*pbuffer);            
				}else{
           *pBuffer = IIC_Read_Byte(1);
            pBuffer++;
        }            
        NumByteToRead--;
    }  
		IIC_Stop();

	//return HAL_I2C_Master_Receive(&HP206C_IIC,HP206C_R_ADDRESS,pBuffer,NumByteToRead,0x100);;
}
/**
  * @brief 读寄存器.
  */
uint8_t HP206C_ReadReg(uint8_t reg)
{
        uchar buf[3]={0};
        uchar cmd =reg;
				I2C_ByteWrite(&cmd);
				I2C_BufferRead(buf,1);
        return buf[0];
}
/**
  * @brief 写一个寄存器.
  */
HAL_StatusTypeDef HP206C_WriteReg(uint8_t* pBuffer,uint8_t* data)
{
	 HAL_StatusTypeDef ret;
	 ret = I2C_ByteWrite(pBuffer); 
	 ret = I2C_ByteWrite(data);
	 return ret;
}
/**
  * @brief HP206C状态检测.
  */
uint8_t RDY_Detect(void)
{
    uint8_t cmd = 0x06;
    uint8_t buf[3]={0};
    delay_ms(150); //这里的延时很重要   
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
void ReadTemperature(void){
        uchar buf[3]={0};
        uchar cmd = HP20X_WR_CONVERT_CMD|OSR_CFG;
				I2C_ByteWrite(&cmd);      //adc convert
        delay_ms(OSR_ConvertTime);//difference OSR_CFG will be difference OSR_ConvertTime
        cmd = HP20X_READ_T;
				I2C_ByteWrite(&cmd); 
				I2C_BufferRead(buf,3);
        Temperature = (buf[0]<<16) + (buf[1]<<8) + buf[2];
        if(Temperature&0x800000){Temperature=Temperature|0xff000000;}
//        printf("Temperature=%.2f\n", (int)Temperature/100.0);         
}  
/**
  * @brief 读取气压.
  */ 
void ReadPressure(void){
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
				//printf("Pressure = %d\n",(int)Pressure);
}
/**
  * @brief 读取气压和温度.
  */ 
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
				//printf("Temperature=%.2f\n", (int)Temperature/100.0);
        Pressure = (buf[3]<<16) + (buf[4]<<8) + buf[5];
        if(Pressure&0x800000){Pressure|=0xff000000;}
				Hp206c_pressure = (int)Pressure/100;     //   mbar 
				//Hp206c_pressure = ((float)(int)Pressure - 52428.8)/41.94304;     //   mbar  
        //printf("Pressure = %.2f\n",(float)Pressure/100);        
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
    uint8_t cmd,buf[6]={0};  
    while(RDY_Detect()&&t<2){
			t++;
    }
		if(t>1){HP206F_OK = 1;}
    OSR_CFG=HP20X_CONVERT_OSR1024;
    OSR_ConvertTime=25;//	25MS
		for(int i =0;i<10;i++)
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
/**
  * @brief HP206C 测试Demo
  */
PressureMsg_t  dev0;
u32 test_delay = 0;
void HP206C_Test_Demo(void)
{
	HP206C_Init();
	while(1)
	{
		GetHp206cData(&dev0);
		HAL_Delay(500);
	}
}
