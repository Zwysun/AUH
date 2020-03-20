#ifndef __BATT_H
#define __BATT_H

#include "includes.h"

//µÁ¡ø
typedef struct 
{
    float current;
    float voltage;
}PowerMsg_t;
#define BAT_V_FILTER_N     ( 100 )
#define BAT_C_FILTER_N     ( 100 )
#define BAT_V_CHANNEL      (  1  )
#define BAT_C_CHANNEL      (  0  )

void Batt_Init(void);
void Get_Batt_Data(PowerMsg_t *BattStruct);
void Batt_Test_Demo(void);

#endif
