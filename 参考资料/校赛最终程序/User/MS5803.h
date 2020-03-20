
/*******************************************************************************
// MS5805 IIC���Գ���
// ʹ�õ�Ƭ��STM32F103C8T6
// ����8.00M
// ��ģ������ GPIOB6->SCL GPIOB7->SDA 
*******************************************************************************/

#ifndef __MS5803_H
#define __MS5803_H 			   
#include "sys.h"  
#include "i2c.h"
#include "sys.h"
#include "delay.h"
//#include "usart.h"

//MS5803 �Ĵ�������	   
#define	MS5803_WRITE		  0xEC	//MS5803 д��ַ
#define	MS5803_READ       0xED	//MS5803 ����ַ

#define	MS5803_ADC_READ   0x00	//MS5803 ��ADC
#define	MS5803_RESET      0x1E	//MS5803 Reset
#define	MS5803_D1         0x48	//MS5803 ѹ��ת������OSR=4096���ֱ���mbar
#define	MS5803_D2         0x58	//MS5803 �¶�ת������OSR=4096���ֱ��ʡ�C

#define	MS5803_PROM_C0    0xA0	//
#define	MS5803_PROM_C1    0xA2	//MS5803 Pressure sensitivity | SENS T1
#define	MS5803_PROM_C2    0xA4	//MS5803 Pressure offset | OFF T1
#define	MS5803_PROM_C3    0xA6	//MS5803 Temperature coefficient of pressure sensitivity | TCS
#define	MS5803_PROM_C4    0xA8	//MS5803 Temperature coefficient of pressure offset | TCO
#define	MS5803_PROM_C5    0xAA	//MS5803 Reference temperature | T REF
#define	MS5803_PROM_C6    0xAC	//MS5803 Temperature coefficient of the temperature | TEMPSENS



#define dT_min  -16776960
#define dT_max   16777216
#define OFF_min  -8589672450
#define OFF_max   12884705280
#define SENS_min  -4294836225
#define SENS_max   6442352640


void MS5803_Config(void); 			//����MS5803
void MS5803_Reset(void); 				//��λMS5803
u16 MS5803_Read_PROM(u8 addr);  //MS5803��ȡPROM
u32 MS5803_Read_ADC(u8 addr);   //MS5803��ȡ24λADת������

#endif
