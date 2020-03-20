#ifndef __COMPASS_TASK_H
#define __COMPASS_TASK_H

#include "includes.h"
extern QueueHandle_t IMU_Message_Queue ;
void imu_Task_Function(void const * argument);
#endif
