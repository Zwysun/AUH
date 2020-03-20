#ifndef __PID_H
#define __PID_H

#include "includes.h"
typedef struct{
 
	float Kp;
	float Ki;
	float Kd;

	float p_Out;
	float i_Out;
	float d_Out;
	
	float p_OutMAX;
	float p_OutMIN;
	
	float i_OutMAX;
	float i_OutMIN;
	
	float d_OutMAX;
	float d_OutMIN;
  
	float Bias;
	float Last_Bias;
	float Last_Last_Bias;
	
	float OutPut;
	float OutPutMAX;
	float OutPutMIN;
	
  float dead_zone;
}PID_t;

 
extern PID_t Deep_pid;
extern PID_t Heading_pid;
extern PID_t Pitch_pid;
extern void PID_Init(void);                                        //PID初始化
extern void PID_Param_Init(PID_t *pid, float *pid_param);            //PID参数设置
extern float Incremental_PID_Cal(float tar,float input,PID_t *pid);    //增量式PID计算 
extern float Position_PID_Cal(float tar,float input,PID_t *pid);
extern void PID_Clear(PID_t *pid);							                   //PID 累计量清空 
 
#endif
