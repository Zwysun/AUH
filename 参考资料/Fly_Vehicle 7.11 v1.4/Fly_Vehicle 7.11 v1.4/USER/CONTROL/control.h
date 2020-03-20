#ifndef __CONTROL_H
#define __CONTROL_H

#include "includes.h"

#define CONSTRAIN(x,max,min) (x>max?max:(x<min?min:x))//取三个参数的中位数
#define MAX(a,b)             (a>b?a:b)
#define MIN(a,b)             (a<b?a:b)

#define  T0                 200
#define  MAX_T_A            190
#define  MAX_T_D            170
#define  MAX_SPEED          100
#define  max_speed_delta    5
#define  mid                1500

#define  front_mid          1500
#define  back_mid           1500

// 左电机   -27    73       mid + (73-27)/2   23  
// 右电机   -13    87       mid + (87-13)/2   37
#define  left_mid           1493
#define  right_mid          1507
#define  max_speed_value    (mid + MAX_SPEED)
#define  min_speed_value    (mid - MAX_SPEED)

typedef enum 
{
    MANUAL = 0,
    AUTO
}ControlState_t;

typedef struct{
//	int16_t channel[4];
	float temperture;
	float depth;
	float heading;                															//航向
	float pitching;               															//前后偏角
	float rolling;                															//左右偏角
	ControlState_t depth_state;
	ControlState_t heading_state;
	bool  Control_Disable;
	float navi_angle;
	float navi_distance;
}ControlMsg_t;

void SetUpDowm(float front_val,float back_val); //垂直运动
void SetHorizontal(float f_b,float rotate);//水平运动

//指令发送超时
void Clear_Control_Disable_Counter(void);
bool Get_Control_Disable_Counter_State(void);
void Adder_Control_Disable_Counter(void);
#endif

