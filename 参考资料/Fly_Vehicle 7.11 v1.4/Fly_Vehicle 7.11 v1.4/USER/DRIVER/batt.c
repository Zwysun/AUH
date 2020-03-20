#include "batt.h"
/**
  ******************************************************************************
  * @file    batt.c
  * @author  LiuYang
  * @brief   ADC DMA��ȡ��ѹ������
  *          
  *
  ******************************************************************************
  * @attention
  *	��ص�ѹ:  22K:4.99K �����ѹ     (4.4:1)
	*
  * ��ص���:INA282
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
*�� �� ��: Voltage_RecursionFilter
*����˵��: ��ѹADC�ɼ�ԭʼ���ݾ�ֵ�˲�
*��    ��: uint32_t *filterbuf      uint32_t ADValue 
*                  ��ѹ������               �²ɼ�������  
*              
*�� �� ֵ: ��ֵ�˲���ĵ�ѹԭʼֵ
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
*�� �� ��: Current_RecursionFilter
*����˵��: ����ADC�ɼ�ԭʼ���ݾ�ֵ�˲�
*��    ��: uint32_t *filterbuf      uint32_t ADValue 
*                  ����������               �²ɼ�������  
*              
*�� �� ֵ: ��ֵ�˲���ĵ���ԭʼֵ
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
*�� �� ��: GetVoltage
*����˵��: ��ȡ��ص�ѹ  (  110k   
*��    ��: ��                10K       11:1   
*�� �� ֵ: ��ѹֵ        )
**********************************************************************************************************/
float GetVoltage(void)
{
    adcVoltage = Voltage_RecursionFilter(BatVoltageBuf,ADC_Value[BAT_V_CHANNEL]);    
    return (adcVoltage*3.3f/4096*(22/4.99+1)*1.0128);            
}
/**********************************************************************************************************
*�� �� ��: GetCurrent
*����˵��: ��ȡ��ص���   ( ACS758KCB-150B-PFF-T   ��100A     20mV/A    5V���� )
*��    ��: ��
*�� �� ֵ: ��ѹֵ
**********************************************************************************************************/
float GetCurrent(void)
{
//	adcmA = Current_RecursionFilter(BatCurrentBuf,ADC_Value[BAT_C_CHANNEL]);
	adcmA = ADC_Value[BAT_C_CHANNEL];
	return (adcmA*3.3f/4096/50/0.01);                            
}

/**********************************************************************************************************
*�� �� ��: Get_Batt_Data
*����˵��: ���ݵ�ѹ�����ṹ�� 
*��    ��: PowerMsg_t 
*�� �� ֵ: �� 
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