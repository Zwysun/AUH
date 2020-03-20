#include "batt.h"
/**
  ******************************************************************************
  * @file    batt.c
  * @author  LiuYang
  * @brief   ADC DMA读取电压、电流
  *          
  *
  ******************************************************************************
  * @attention
  *	电池电压:  22K:4.99K 电阻分压     (4.4:1)
	*
  * 电池电流:INA282
  *
  ******************************************************************************
  */
uint32_t ADC_Value[2];

uint32_t BatCurrentBuf[BAT_C_FILTER_N + 1] = {0};
uint32_t BatVoltageBuf[BAT_V_FILTER_N + 1] = {0};

/**
  * @brief  
  * @param  
  * @note  
  */
void Batt_Init(void)
{
	int i = 0;
//	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_Value,2);//??
  for(i = 0; i < BAT_V_FILTER_N + 1; i++)
	{
			BatVoltageBuf[i] = ADC_Value[BAT_V_CHANNEL];
      delay_ms(1);//
	}
	
//	for(i = 0; i < BAT_C_FILTER_N + 1; i++)
//	{
//			BatCurrentBuf[i] = ADC_Value[BAT_C_CHANNEL];
//			delay_ms(1);
//	}	
}
/**********************************************************************************************************
*函 数 名: Voltage_RecursionFilter
*功能说明: 电压ADC采集原始数据均值滤波
*形    参: uint32_t *filterbuf      uint32_t ADValue 
*                  电压总数组               新采集的数据  
*              
*返 回 值: 均值滤波后的电压原始值
**********************************************************************************************************/
static uint32_t Voltage_RecursionFilter(uint32_t *filterbuf,uint32_t ADValue)
{
	uint32_t filtersum = 0;
	int i= 0; 
	filterbuf[BAT_V_FILTER_N] = ADValue;
	for( i = 0; i < BAT_V_FILTER_N; i++) 
	{
		filterbuf[i] = filterbuf[i + 1]; // 
		filtersum += filterbuf[i];
	}
	return (int)(filtersum / BAT_V_FILTER_N);
}
/**********************************************************************************************************
*函 数 名: Current_RecursionFilter
*功能说明: 电流ADC采集原始数据均值滤波
*形    参: uint32_t *filterbuf      uint32_t ADValue 
*                  电流总数组               新采集的数据  
*              
*返 回 值: 均值滤波后的电流原始值
**********************************************************************************************************/
static uint32_t Current_RecursionFilter(uint32_t *filterbuf,uint32_t ADValue)
{
	uint32_t filtersum = 0;
	int i= 0; 
	filterbuf[BAT_V_FILTER_N] = ADValue;
	for( i = 0; i < BAT_C_FILTER_N; i++) 
	{
		filterbuf[i] = filterbuf[i + 1]; // 
		filtersum += filterbuf[i];
	}
	return (int)(filtersum / BAT_C_FILTER_N);
}
uint32_t adcVoltage = 0;
uint32_t adcmA = 0;
/**********************************************************************************************************
*函 数 名: GetVoltage
*功能说明: 获取电池电压  (  110k   
*形    参: 无                10K       11:1   
*返 回 值: 电压值        )
**********************************************************************************************************/
float GetVoltage(void)
{
    adcVoltage = Voltage_RecursionFilter(BatVoltageBuf,ADC_Value[BAT_V_CHANNEL]);    
    return (adcVoltage*3.3f/4096*(22/4.99+1)*1.0128);            
}
/**********************************************************************************************************
*函 数 名: GetCurrent
*功能说明: 获取电池电流   ( ACS758KCB-150B-PFF-T   ±100A     20mV/A    5V供电 )
*形    参: 无
*返 回 值: 电压值
**********************************************************************************************************/
float GetCurrent(void)
{
//	adcmA = Current_RecursionFilter(BatCurrentBuf,ADC_Value[BAT_C_CHANNEL]);
	adcmA = ADC_Value[BAT_C_CHANNEL];
	return (adcmA*3.3f/4096/50/0.01);                            
}

/**********************************************************************************************************
*函 数 名: Get_Batt_Data
*功能说明: 传递电压电流结构体 
*形    参: PowerMsg_t 
*返 回 值: 无 
**********************************************************************************************************/ 
void Get_Batt_Data(PowerMsg_t *BattStruct)
{
	BattStruct->voltage = GetVoltage();
	BattStruct->current = GetCurrent();
} 
static PowerMsg_t    power_data;
void Batt_Test_Demo(void)
{
	Batt_Init();
	while(1)
	{
    Get_Batt_Data(&power_data);
		delay_ms(500);		
	}
}