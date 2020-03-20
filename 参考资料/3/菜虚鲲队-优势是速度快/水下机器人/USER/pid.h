#ifndef __PID_H__
#define __PID_H__
#include "stm32f10x.h"
typedef struct PID
{   
   float kp;         // 比例常数 Proportional Const
   float ki;         // 积分常数 Integral Const
   float kd;         // 微分常数 Derivative Const
   
   float point;      // 设定的目标 Desired Value
   float ek;         // e[k]   本次偏差
   float ek_1;       // e[k-1] 上次偏差
   float ek_2;       // e[k-2] 上上次偏差
   float uk;         //本次PID输出值
   float uk_1;       //上次PID输出值
   double ek_sum;     //偏差总和
}PID; 
extern PID angle_x_pid;
extern PID angle_y_pid;
extern PID angle_z_pid;
extern PID high_pid;

void pid_INIT(void);
void Position_PID(PID *pp,float error);
void Increase_PID(PID *pp, float feedbackValue);
#endif

