#include "algorithm.h"


float x_angular_speed,y_angular_speed,z_angular_speed;
float yaw=0,roll=0,pitch=0;//欧拉角

angular_speed_angle_pid pitch_pid;
angular_speed_angle_pid roll_pid;
angular_speed_angle_pid yaw_pid;

void angular_speed_angle_pid_init(angular_speed_angle_pid *set_pid,float set_angular_speed_kp,float set_angular_speed_ki,float set_angular_speed_kd,float set_angle_kp,float set_angle_kd,float set_mechanical_zero)
{
  set_pid->angular_speed_point=0;
  set_pid->angular_speed_kp=set_angular_speed_kp;
  set_pid->angular_speed_ki=set_angular_speed_ki;
  set_pid->angular_speed_kd=set_angular_speed_kd;
  set_pid->angular_speed_ek_1=0;
  set_pid->angular_speed_ek_sum=0;
  set_pid->angular_speed_ek_k_1=0;
  set_pid->angular_speed_uk=0;
  set_pid->angle_point=0;
  set_pid->angle_kp=set_angle_kp;
  set_pid->angle_kd=set_angle_kd;
  set_pid->mechanical_zero=set_mechanical_zero;
}
void angular_speed_pid_calculate(angular_speed_angle_pid *set_pid,float feedbackValue)
{
  float out=0;
  float angular_speed_ek=0;
  float angular_speed_ek_k=0;
  u8 ki_vaild=0;
  //计算误差
  angular_speed_ek=set_pid->angular_speed_point-feedbackValue;
  //积分
  if(fabs(angular_speed_ek)<70) set_pid->angular_speed_ek_sum+=angular_speed_ek;
	else set_pid->angular_speed_ek_sum=0;
  //积分限幅
  if(set_pid->angular_speed_ek_sum>7000) set_pid->angular_speed_ek_sum=7000;
  else if(set_pid->angular_speed_ek_sum<-7000) set_pid->angular_speed_ek_sum=-7000;
  //积分分离
  if(fabs(angular_speed_ek)<70) ki_vaild=1;
  //微分滤波
  angular_speed_ek_k=angular_speed_ek-set_pid->angular_speed_ek_1;
  angular_speed_ek_k=angular_speed_ek_k*0.2+set_pid->angular_speed_ek_k_1*0.8;
  //计算输出值
  out=set_pid->angular_speed_kp*angular_speed_ek+(float)ki_vaild*set_pid->angular_speed_ki*set_pid->angular_speed_ek_sum+set_pid->angular_speed_kd*angular_speed_ek_k; 
  //输出限幅
  if(out<-300) out=-300;
  else if(out>300) out=300;
  //输出
  set_pid->angular_speed_uk=out;
  //迭代参数
  set_pid->angular_speed_ek_1=angular_speed_ek;
  set_pid->angular_speed_ek_k_1=angular_speed_ek_k;
}

void angle_pid_calculate(angular_speed_angle_pid *set_pid,float feedbackValue1,float feedbackValue2)
{
  float out=0;
  float angle_ek=0;
  //计算误差
  angle_ek=set_pid->angle_point-(feedbackValue1-set_pid->mechanical_zero);
  //计算输出值
  out=set_pid->angle_kp*angle_ek+set_pid->angle_kd*feedbackValue2;
  //输出限幅
  if(out<-100) out=-100;
  else if(out>100) out=100;
  //串级pid
  set_pid->angular_speed_point=out;
}

float up_down_point=0;
float up_down_uk=0;
void up_down_control(void)
{
	float error;
	float out;
	error=up_down_point-imu_dev.roll;
	out=error*8-imu_dev.x_angular_velocity*1.0;
	if(out<-150) out=-150;
  else if(out>150) out=150;
	up_down_uk=-out;
}

float direction_point=0;
float direction_uk=0;
float direction_point_offset=0;
void direction_control(void)
{
	float error=0;
	float out;
//	if(navi_center_to_line_distance*navi_center_to_line_angle>0)
//	{
//		direction_point=0;
//	}
//	else
	direction_point=navi_center_to_line_distance*0.1-direction_point_offset;
	if(direction_point>30) direction_point=30;
	else if(direction_point<-30) direction_point=-30;
	error=direction_point-navi_center_to_line_angle;
//	speed=70-error*error/5;
	out=error*7-imu_dev.z_angular_velocity*3;
	if(out<-90) out=-90;
  else if(out>90) out=90;
	direction_uk=-out;
}
float deepth_pid_point=37;
float deepth_pid_uk=0;
void deepth_pid(void)
{
	float error;
	float out;
	error=deepth_pid_point-hp206c.depth;
	out=error*5;
	if(out<-300) out=-300;
  else if(out>300) out=300;
	deepth_pid_uk=out;
}
