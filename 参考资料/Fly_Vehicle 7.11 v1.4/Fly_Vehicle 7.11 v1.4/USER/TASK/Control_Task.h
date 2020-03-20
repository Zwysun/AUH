#ifndef __CONTROL_TASK_H
#define __CONTROL_TASK_H

#include "includes.h"
extern QueueHandle_t Control_Message_Queue; 
void control_Task_Function(void const * argument);

#endif
