#include "ledTasks.h"
 
extern enum WORK_STATE{FORWARD=0,ROTATE,STOP}work;

extern void LED_SW(u8 num,u8 sta);
 
void led_Task_Function(void const * argument)
{
	while(1)
	{
		#ifdef DEBUG
  
		#else 
	
		#endif 
		if(work == STOP){LED_SW(1, 1);LED_SW(2, 1);}
		else{LED_SW(1, 0);LED_SW(2, 0);}
//		osDelay(50);		
	}

}
