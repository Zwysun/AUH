#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#include "my_scc.h"


typedef struct
{
  float angular_speed_point;
  float angular_speed_kp;
  float angular_speed_ki;
  float angular_speed_kd;
  float angular_speed_ek_1;
  float angular_speed_ek_sum;
  float angular_speed_ek_k_1;
  float angular_speed_uk;
  float angle_point;
  float angle_kp;
  float angle_kd;
  float mechanical_zero;
}angular_speed_angle_pid;

extern angular_speed_angle_pid pitch_pid;
extern angular_speed_angle_pid roll_pid;
extern angular_speed_angle_pid yaw_pid;

void angular_speed_angle_pid_init(angular_speed_angle_pid *set_pid,float set_angular_speed_kp,float set_angular_speed_ki,float set_angular_speed_kd,float set_angle_kp,float set_angle_kd,float set_mechanical_zero);
void angular_speed_pid_calculate(angular_speed_angle_pid *set_pid,float feedbackValue);
void angle_pid_calculate(angular_speed_angle_pid *set_pid,float feedbackValue1,float feedbackValue2);

extern float up_down_uk;
void up_down_control(void);
	
extern float direction_uk;
extern float direction_point_offset;
void direction_control(void);

extern float deepth_pid_point;
extern float deepth_pid_uk;
void deepth_pid(void);
/************************普通运算*************************/
#define adjust_half_16(a) (uint16_t)(a+0.5)//16位四舍五入
#define adjust_half_8(a) (uint8_t)(a+0.5)//8位四舍五入

#endif
