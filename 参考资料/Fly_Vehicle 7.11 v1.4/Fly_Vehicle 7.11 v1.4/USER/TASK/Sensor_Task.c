#include "Sensor_Task.h"


static PowerMsg_t    power_data;
static TempHumiMsg_t TempHum;

void sensor_Task_Function(void *pvParameters)
 {
 
  //portTickType tick = xTaskGetTickCount(); 
 
	while(1)
	{
		#ifdef DEBUG
 
		#else
			SHT35_ReadData(&TempHum);
			Get_Batt_Data(&power_data);
			xQueueOverwrite(TempHum_Message_Queue, &TempHum);
		  xQueueOverwrite(Battery_Message_Queue,&power_data);
		#endif
		osDelay(1000);
		//vTaskDelayUntil(&tick,1000);
 
	}
}
 
