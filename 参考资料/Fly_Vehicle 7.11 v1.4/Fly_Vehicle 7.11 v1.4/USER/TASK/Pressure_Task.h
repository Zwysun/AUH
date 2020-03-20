#ifndef __PRESSURE_TASK_H
#define __PRESSURE_TASK_H

#include "includes.h"
extern QueueHandle_t Pressure_Message_Queue;
void pressure_Task_Function(void const * argument);
#endif
