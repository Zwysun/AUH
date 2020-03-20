#ifndef __COMPASS_WARD_H
#define __COMPASS_WARD_H

#include "includes.h"
extern QueueHandle_t IMU_Message_Queue ;
void imu_Task_Function(void const * argument);
void delay_ms(int nms);
#endif
