#ifndef __PID_H__
#define __PID_H__
#include "stm32f10x.h"
typedef struct PID
{   
   float kp;         // �������� Proportional Const
   float ki;         // ���ֳ��� Integral Const
   float kd;         // ΢�ֳ��� Derivative Const
   
   float point;      // �趨��Ŀ�� Desired Value
   float ek;         // e[k]   ����ƫ��
   float ek_1;       // e[k-1] �ϴ�ƫ��
   float ek_2;       // e[k-2] ���ϴ�ƫ��
   float uk;         //����PID���ֵ
   float uk_1;       //�ϴ�PID���ֵ
   double ek_sum;     //ƫ���ܺ�
}PID; 
extern PID angle_x_pid;
extern PID angle_y_pid;
extern PID angle_z_pid;
extern PID high_pid;

void pid_INIT(void);
void Position_PID(PID *pp,float error);
void Increase_PID(PID *pp, float feedbackValue);
#endif

