#include "Compass_Task.h"
IMUMsg_t imu;
void imu_Task_Function(void const * argument)
 {
  u8 i = 0; 	
//	portTickType tick = xTaskGetTickCount();
	while(1)
	{
		#ifdef DEBUG
 
		#else
		JY901_GetData(&imu);
		xQueueOverwrite(IMU_Message_Queue, &imu);
		#endif  
    osDelay(100);
//		vTaskDelayUntil(&tick,200);
	}
}
