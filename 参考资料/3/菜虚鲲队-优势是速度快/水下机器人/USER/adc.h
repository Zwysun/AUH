#ifndef __ADC_H
#define __ADC_H
#include "my_scc.h"
#define ADC1_DR_Address    ((u32)0x4001244c)
extern __IO u16 ADC_ConvertedValue[2];
u16 get_adc(void);
void adc_INIT(void);
#endif
