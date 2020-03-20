#include "adc.h"
__IO u16 ADC_ConvertedValue[2];
void adc_GPIO_init(void)
{
	GPIO_InitTypeDef a;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	a.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	a.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&a);
}
void dma_init(void)//dma循环传输8个ad数据到数组
{
	DMA_InitTypeDef a;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
	DMA_DeInit(DMA1_Channel1); 
	a.DMA_BufferSize=2;
	a.DMA_DIR= DMA_DIR_PeripheralSRC;
	a.DMA_M2M=DMA_M2M_Disable;
	a.DMA_MemoryBaseAddr=(u32)ADC_ConvertedValue;
	a.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	a.DMA_MemoryInc=DMA_MemoryInc_Enable;
	a.DMA_Mode=DMA_Mode_Circular;
	a.DMA_PeripheralBaseAddr=ADC1_DR_Address;
	a.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	a.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	a.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&a);
	DMA_Cmd(DMA1_Channel1, ENABLE); 
}
void adc_init(void)//循环扫描获取2个ad数据
{
	ADC_InitTypeDef a;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	a.ADC_ContinuousConvMode=ENABLE;
	a.ADC_DataAlign=ADC_DataAlign_Right;
	a.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	a.ADC_Mode=ADC_Mode_Independent;
	a.ADC_NbrOfChannel=2;
	a.ADC_ScanConvMode=ENABLE;
	ADC_Init(ADC1,&a);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void adc_INIT(void)//adc初始化
{
	adc_GPIO_init();
	dma_init();
	adc_init();
}
