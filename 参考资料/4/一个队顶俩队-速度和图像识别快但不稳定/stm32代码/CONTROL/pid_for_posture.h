#ifndef __PID_POS_H
#define __PID_POS_H

#include "sys.h"
#include "stm32f10x_tim.h"

#define Kp_pitch       0.32//��������(0.32)
#define Ti_pitch      0.2  //����ʱ�䳣��
#define Td_pitch      0.0028//0.0028   ΢��ʱ�䳣��
#define Ts_pitch       0.1 //��������0.1ms������ٶȶ�ȡ��ʱ��ͬ��
#define Ki_pitch     Kp_pitch*(Ts_pitch/Ti_pitch)        // Kp_pitch Ki_pitch Kd_pitch ������Ҫ����
#define Kd_pitch     Kp_pitch*(Td_pitch/Ts_pitch)

#define P_pitch				0.3//0.3   Kp_pitch+Ki_pitch+Kd_pitch
#define I_pitch				0.02//0.02 Kp_pitch+2*Kd_pitch
#define D_pitch				0.1// 0.1    Kd_pitch  PITCH��D�Ҿ���Ҫ�ܴ󡭡���ΪPITCH����������׵���б������

#define Kp_depth       3.3
#define Ti_depth      20  //����ʱ�䳣��
#define Td_depth      0.0028//0.0028   ΢��ʱ�䳣��
#define Ts_depth       0.1 //��������0.1ms������ٶȶ�ȡ��ʱ��ͬ��
#define Ki_depth     Kp_depth*(Ts_depth/Ti_depth)        // Kp_depth Ki_depth Kd_depth ������Ҫ����
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
 int setpoint;//�趨Ŀ��
 float proportion ;//��������
 float integral ;//����
 float derivative;//΢��
 int last_error;//e[-1]
 int prev_error;//e[-2]
}PIDtypedef;

extern PIDtypedef PID_pitch;
extern PIDtypedef PID_depth;

void incPIDinit(PIDtypedef*PIDx);                //��ʼ������������
int incPIDcalc(PIDtypedef*PIDx,int nextpoint);           //PID����
void PID_setpoint(PIDtypedef*PIDx,int setvalue);  //�趨 PIDԤ��ֵ
void PID_setgain(PIDtypedef*PIDx,float pp,float ii,float dd);//�趨PID  Kp_pitch Ki_pitch Kd_pitch��������
void PID_pos_init(void);   //��ʼ������
void Depth_Disturb_Suppress(void);
void Pitch_Disturb_Suppress(void);
void Setpwm_LR(void);
void Setpwm_UD(void);
void Setpwm_NoCompensation(void);

#endif

