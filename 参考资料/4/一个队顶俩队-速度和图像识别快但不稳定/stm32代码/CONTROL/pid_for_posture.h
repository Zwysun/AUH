#ifndef __PID_POS_H
#define __PID_POS_H

#include "sys.h"
#include "stm32f10x_tim.h"

#define Kp_pitch       0.32//比例常数(0.32)
#define Ti_pitch      0.2  //积分时间常数
#define Td_pitch      0.0028//0.0028   微分时间常数
#define Ts_pitch       0.1 //采样周期0.1ms，想和速度读取的时间同步
#define Ki_pitch     Kp_pitch*(Ts_pitch/Ti_pitch)        // Kp_pitch Ki_pitch Kd_pitch 三个主要参数
#define Kd_pitch     Kp_pitch*(Td_pitch/Ts_pitch)

#define P_pitch				0.3//0.3   Kp_pitch+Ki_pitch+Kd_pitch
#define I_pitch				0.02//0.02 Kp_pitch+2*Kd_pitch
#define D_pitch				0.1// 0.1    Kd_pitch  PITCH的D我觉得要很大……因为PITCH歪掉后会很容易导致斜向喷射

#define Kp_depth       3.3
#define Ti_depth      20  //积分时间常数
#define Td_depth      0.0028//0.0028   微分时间常数
#define Ts_depth       0.1 //采样周期0.1ms，想和速度读取的时间同步
#define Ki_depth     Kp_depth*(Ts_depth/Ti_depth)        // Kp_depth Ki_depth Kd_depth 三个主要参数
#define Kd_depth     Kp_depth*(Td_depth/Ts_depth)

#define P_depth				8//Kp_depth+Ki_depth+Kd_depth
#define I_depth				0.6//Kp_depth+2*Kd_depth
#define D_depth				0//Kd_depth

extern int left,right,forward,behind;
extern int isposeloopclosed;
extern int Target_depth;
extern int Target_pitch;
typedef struct 
{
 int setpoint;//设定目标
 float proportion ;//比例常数
 float integral ;//积分
 float derivative;//微分
 int last_error;//e[-1]
 int prev_error;//e[-2]
}PIDtypedef;

extern PIDtypedef PID_pitch;
extern PIDtypedef PID_depth;

void incPIDinit(PIDtypedef*PIDx);                //初始化，参数清零
int incPIDcalc(PIDtypedef*PIDx,int nextpoint);           //PID计算
void PID_setpoint(PIDtypedef*PIDx,int setvalue);  //设定 PID预期值
void PID_setgain(PIDtypedef*PIDx,float pp,float ii,float dd);//设定PID  Kp_pitch Ki_pitch Kd_pitch三个参数
void PID_pos_init(void);   //初始化函数
void Depth_Disturb_Suppress(void);
void Pitch_Disturb_Suppress(void);
void Setpwm_LR(void);
void Setpwm_UD(void);
void Setpwm_NoCompensation(void);

#endif

