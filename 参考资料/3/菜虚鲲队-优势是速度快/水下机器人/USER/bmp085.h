#ifndef __BMP085_H
#define __BMP085_H
#include "delay.h"
#define	BMP085_SlaveAddress   0xee	  //����������IIC�����еĴӵ�ַ                               

#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)
							   
typedef unsigned char  BYTE;
typedef unsigned short WORD;  	



void Init_BMP085(void);
#endif
