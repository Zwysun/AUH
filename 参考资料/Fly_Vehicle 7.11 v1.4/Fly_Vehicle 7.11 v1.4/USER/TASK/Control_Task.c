#include "Control_Task.h"
#include "control.h"

extern ControlMsg_t control_data;
extern enum MODE_CHOOSE{MODE0=0,MODEt}mode;
extern void Control_0(void);
extern void Control_t(void);

float front_back_rocker = 0.0;                        //左边 - 前后摇杆位置
float left_right_rocker = 0.0;                        //左边 - 左右摇杆位置
float up_down_rocker = 0.0;                           //右边 - 上下摇杆位置
float left_right_adjust_rock = 0.0;                   //右边 - 左右摇杆位置

int16_t test_motor = 80;
float heading_pid_tar = 0;
float heading_pid_out = 0;
float pitching_pid_out = 0;
float deepth_pid_out = 0;
float deep_tar = 14;
float navi_position  = 0.0;
//俯仰使能
u8 pitch_en =1;
u8 deep_en = 1;
u8 roll_en =1;

void control_Task_Function(void const * argument)
{
 
  portTickType tick = xTaskGetTickCount();

	while(1)
	{
		#ifdef DEBUG
		
		#else
//		Flyer_Limit();
		if(xQueueReceive(Control_Message_Queue,&(control_data),portMAX_DELAY)) 
		{
			//俯仰角 PID 调节
			if(roll_en==1){
					
				if(control_data.rolling>3)
				{
//				if(deepth_pid_out>55)
//				{
//					deepth_pid_out=55;
//				}				
//       if(deepth_pid_out<-55)
//				{
//					deepth_pid_out=-55;
//				}		
					pitching_pid_out = Position_PID_Cal(0,control_data.rolling,&Pitch_pid);
					if(deepth_pid_out>180)//曾用参数都是155
					{
						deepth_pid_out=180;
					}				
					if(deepth_pid_out<-180)
					{
						deepth_pid_out=-180;
					}	
					SetUpDowm(0,pitching_pid_out);				
				}
				
				
				else if(control_data.rolling<-3)
				{
//				if(deepth_pid_out>55)
//				{
//					deepth_pid_out=55;
//				}				
//       if(deepth_pid_out<-55)
//				{
//					deepth_pid_out=-55;
//				}		
					pitching_pid_out = Position_PID_Cal(0,control_data.rolling,&Pitch_pid);
					if(deepth_pid_out>180)//曾用参数都是155
					{
						deepth_pid_out=180;
					}				
					if(deepth_pid_out<-180)
					{
						deepth_pid_out=-180;
					}	
					SetUpDowm((-1)*pitching_pid_out,0);
				}
				else{
					SetUpDowm(0,0);		
				}
			}
			
			
//			else{
//				SetUpDowm(0,0);		
//			}
       
			
			//深度 PID 调节
			//last value do not too far from|60|
			if(deep_en==1){
				if(deepth_pid_out>200)//曾用参数都是200
				{
					deepth_pid_out=200;
				}				
				if(deepth_pid_out<-55)//曾用参数都是55
				{
					deepth_pid_out=-55;
				}		
			
			
				deepth_pid_out = Position_PID_Cal(deep_tar,control_data.depth,&Deep_pid);	
				
				if(deepth_pid_out>200)//曾用参数都是200
				{
					deepth_pid_out=200;
				}				
        if(deepth_pid_out<-55)//曾用参数都是55
				{
					deepth_pid_out=-55;
				}	
				
				SetUpDowm(deepth_pid_out,deepth_pid_out);						
			  }
			
				else{
				SetUpDowm(0,0);		
			}
			
//			if(control_data.Control_Disable==true){
//				if(control_data.navi_angle>30){//偏右
//					navi_position = 20;
//				}else if(control_data.navi_angle<(-30)){
//					navi_position = -20;
//				}else{
//					navi_position =0.0;
//				}
//				SetHorizontal(test_motor,navi_position);
//			}else{
//						SetHorizontal(0,0);
//			}
		}
		#endif
	}
}
 
