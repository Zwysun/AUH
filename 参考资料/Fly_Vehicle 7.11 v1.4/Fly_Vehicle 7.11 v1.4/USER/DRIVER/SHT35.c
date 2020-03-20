#include "SHT35.h"
/**
  ******************************************************************************
  * @file    SHT35.c
  * @author  LiuYang
  * @brief   SHT35 温湿度读取函数
  *
  *
  ******************************************************************************
  * @attention
  *
  *
  *
  ******************************************************************************
  */
uint8_t SHT3X_Modecommand_Buffer[2]={0x22,0x36};  //periodic mode commands 
uint8_t SHT3X_Fetchcommand_Bbuffer[2]={0xE0,0x00};//读取测量结果

uint8_t SHT3X_Data_Buffer[6];                     //byte0,1为温度 byte4,5为湿度
float Humidity;                                //湿度 正数
float Temperature;                             //温度 可能为负数
/******************************************************************
 *@brief  SHT35_Init                    :  发送periodic mode commands，传感器周期性的进行温湿度转换 
 *@parm   无                                  
 *@retval 无 
 *@note   
 ******************************************************************/  
void SHT35_Init(void)
{
	HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Modecommand_Buffer,2,0x10); //第一步，发送periodic mode commands，传感器周期性的进行温湿度转换
}
/******************************************************************
 *@brief  SHT35_ReadData                :  温湿度读取
 *@parm   TempHumiMsg_t *tempTempHum    :  >temp 温度
 *@parm                                 :  >humi 湿度
 *@retval 无 
 *@note    
 ******************************************************************/  
void SHT35_ReadData(TempHumiMsg_t *tempTempHum)
{
	  HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Fetchcommand_Bbuffer,2,0x10); //第二步，随时读取传感器的数据 
    HAL_I2C_Master_Receive(&SHT35_IIC,(0x44<<1)+1,SHT3X_Data_Buffer,6,0x10); 
    Temperature =(float)((((SHT3X_Data_Buffer[0]<<8)+SHT3X_Data_Buffer[1])*175)/65535.0f)-45; //得到摄氏度温度 
    Humidity =(((SHT3X_Data_Buffer[3]<<8)+SHT3X_Data_Buffer[4])*100)/65535.0f; //可以得到相对湿度
//  printf("Temperature:%.2f℃,Humidity:%.2f%%\r\n",Temperature_S1,Humidity_S1);
	  tempTempHum->temp = Temperature; 
	  tempTempHum->humi = Humidity;
}
/******************************************************************
 *@brief  SHT35_TestDemo      :  温湿度读取测试程序
 *@parm   无
 *@retval 无 
 *@note    
 ******************************************************************/  
void SHT35_TestDemo(void)
{
	HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Modecommand_Buffer,2,0x10); //第一步，发送periodic mode commands，传感器周期性的进行温湿度转换
  while (1)
  {
    HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Fetchcommand_Bbuffer,2,0x10); //第二步，随时读取传感器的数据 
    HAL_I2C_Master_Receive(&SHT35_IIC,(0x44<<1)+1,SHT3X_Data_Buffer,6,0x10); 
    Temperature=(float)((((SHT3X_Data_Buffer[0]<<8)+SHT3X_Data_Buffer[1])*175)/65535.0f)-45; //得到摄氏度温度 
    Humidity=(((SHT3X_Data_Buffer[3]<<8)+SHT3X_Data_Buffer[4])*100)/65535.0f; //可以得到相对湿度
    //printf("Temperature:%.2f℃,Humidity:%.2f%%\r\n",Temperature,Humidity);
    delay_ms(1000);
  }
}	
