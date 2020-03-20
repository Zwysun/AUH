#ifndef __LINEFOLLOW_H
#define __LINEFOLLOW_H
#include "sys.h"
#include "pid_for_posture.h"

extern PIDtypedef PID_line;

extern float Kp_line , Ki_line , Kd_line ;
extern int prerror_line;
extern  int avr_leftright;
extern const int nav_gain, nav_pro;
extern const float dis2angle;
extern const int distance_threshold;
extern const int yaw_threshold;
extern const int depth_threshold;
extern const int pitch_threshold;
extern const int localization_threshold;
extern int  mov_lagtime,mov_lagpro;
extern int isyawloopclosed;
extern const int left_couple,right_couple;
extern int brake_lagtime,brake_angle;
void reset_PID_line(void);
void PID_line_Init(float P,float I,float D);
void Brake(int brake_pitch,int lag_time);
void Brake_pitch_straight(int brake_angle,int lag_time);
void Brake_straight(int lag_time);

void Turn_leftright(int target_angle,int test_lag_cnt,int test_speed);
void Macroturn_leftright(int target_angle,int test_lag_cnt);
void Slight_MoveForward(int lag_time,int mov_lagpro);
void Slight_MoveBehind(int lag_time,int mov_lagpro);

#endif



