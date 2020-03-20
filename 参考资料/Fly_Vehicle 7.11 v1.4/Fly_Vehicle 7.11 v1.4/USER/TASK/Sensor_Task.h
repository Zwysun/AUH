#ifndef __SENSOR_TASK_H
#define __SENSOR_TASK_H

#include "includes.h"
 
extern QueueHandle_t TempHum_Message_Queue; 
extern QueueHandle_t Battery_Message_Queue;
void sensor_TaskFunction(void *pvParameters);
#endif
