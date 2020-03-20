#ifndef __SHT35_H
#define __SHT35_H

#include "includes.h"
typedef struct
{
    float temp;
    float humi;
}TempHumiMsg_t;
void SHT35_Init(void);
void SHT35_ReadData(TempHumiMsg_t *tempTempHum);

void SHT35_TestDemo(void);
#endif
