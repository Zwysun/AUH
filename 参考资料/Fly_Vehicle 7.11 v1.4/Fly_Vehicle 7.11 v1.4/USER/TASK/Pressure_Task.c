#include "Pressure_Task.h"
 
PressureMsg_t pressure_raw;
void pressure_Task_Function(void const * argument)
 {
 
  //portTickType tick = xTaskGetTickCount(); 
	while(1)
	{
		#ifdef DEBUG
 
		#else
		GetHp206cData(&pressure_raw);
		xQueueOverwrite(Pressure_Message_Queue, &pressure_raw);
		#endif 
    osDelay(100);
		//vTaskDelayUntil(&tick,200);
	}
}
 