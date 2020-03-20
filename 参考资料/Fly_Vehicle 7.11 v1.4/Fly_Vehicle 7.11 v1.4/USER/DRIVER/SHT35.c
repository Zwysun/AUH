#include "SHT35.h"
/**
  ******************************************************************************
  * @file    SHT35.c
  * @author  LiuYang
  * @brief   SHT35 ��ʪ�ȶ�ȡ����
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
uint8_t SHT3X_Fetchcommand_Bbuffer[2]={0xE0,0x00};//��ȡ�������

uint8_t SHT3X_Data_Buffer[6];                     //byte0,1Ϊ�¶� byte4,5Ϊʪ��
float Humidity;                                //ʪ�� ����
float Temperature;                             //�¶� ����Ϊ����
/******************************************************************
 *@brief  SHT35_Init                    :  ����periodic mode commands�������������ԵĽ�����ʪ��ת�� 
 *@parm   ��                                  
 *@retval �� 
 *@note   
 ******************************************************************/  
void SHT35_Init(void)
{
	HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Modecommand_Buffer,2,0x10); //��һ��������periodic mode commands�������������ԵĽ�����ʪ��ת��
}
/******************************************************************
 *@brief  SHT35_ReadData                :  ��ʪ�ȶ�ȡ
 *@parm   TempHumiMsg_t *tempTempHum    :  >temp �¶�
 *@parm                                 :  >humi ʪ��
 *@retval �� 
 *@note    
 ******************************************************************/  
void SHT35_ReadData(TempHumiMsg_t *tempTempHum)
{
	  HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Fetchcommand_Bbuffer,2,0x10); //�ڶ�������ʱ��ȡ������������ 
    HAL_I2C_Master_Receive(&SHT35_IIC,(0x44<<1)+1,SHT3X_Data_Buffer,6,0x10); 
    Temperature =(float)((((SHT3X_Data_Buffer[0]<<8)+SHT3X_Data_Buffer[1])*175)/65535.0f)-45; //�õ����϶��¶� 
    Humidity =(((SHT3X_Data_Buffer[3]<<8)+SHT3X_Data_Buffer[4])*100)/65535.0f; //���Եõ����ʪ��
//  printf("Temperature:%.2f��,Humidity:%.2f%%\r\n",Temperature_S1,Humidity_S1);
	  tempTempHum->temp = Temperature; 
	  tempTempHum->humi = Humidity;
}
/******************************************************************
 *@brief  SHT35_TestDemo      :  ��ʪ�ȶ�ȡ���Գ���
 *@parm   ��
 *@retval �� 
 *@note    
 ******************************************************************/  
void SHT35_TestDemo(void)
{
	HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Modecommand_Buffer,2,0x10); //��һ��������periodic mode commands�������������ԵĽ�����ʪ��ת��
  while (1)
  {
    HAL_I2C_Master_Transmit(&SHT35_IIC,0x44<<1,SHT3X_Fetchcommand_Bbuffer,2,0x10); //�ڶ�������ʱ��ȡ������������ 
    HAL_I2C_Master_Receive(&SHT35_IIC,(0x44<<1)+1,SHT3X_Data_Buffer,6,0x10); 
    Temperature=(float)((((SHT3X_Data_Buffer[0]<<8)+SHT3X_Data_Buffer[1])*175)/65535.0f)-45; //�õ����϶��¶� 
    Humidity=(((SHT3X_Data_Buffer[3]<<8)+SHT3X_Data_Buffer[4])*100)/65535.0f; //���Եõ����ʪ��
    //printf("Temperature:%.2f��,Humidity:%.2f%%\r\n",Temperature,Humidity);
    delay_ms(1000);
  }
}	
