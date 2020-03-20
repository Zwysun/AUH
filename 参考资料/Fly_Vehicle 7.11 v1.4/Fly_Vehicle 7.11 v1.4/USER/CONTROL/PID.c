#include "PID.h"

PID_t Deep_pid;
PID_t Heading_pid;//水平转弯
PID_t T_A_pid;//水平转弯
PID_t T_D_pid;//水平转弯
PID_t Pitch_pid;//前进时头翘起来
//另一个是前进时左右倾斜(大概叫rolling)，这一点螺旋桨没法控制，需要调整配重

//增量式PID
//	-15.8    -0.076   电压小于15.6V
//  -13.8    -0.05    电压大于15.6V

//位置式PID
//   -12     -0.35
                             //kp,ki,kd,     p_max,      p_min,     i_max,     i_min,    d_max,     d_min,  out_max,  out_min   ,死区
float Heading_pid_param[12]={ 0  ,  0,   0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float     T_A_pid_param[12]={ -3.7,  0,  0,  MAX_T_A,    0,   MAX_T_A,    0,   MAX_T_A,    0,   MAX_T_A,   0,   0};
float     T_D_pid_param[12]={ -0.3, 0,   0,  MAX_T_D,-160,MAX_T_D,-160,MAX_T_D,-160,MAX_T_D,-160,   0};
float    Deep_pid_param[12]={ -100, 0,   0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float   Pitch_pid_param[12]={ -10 , 0,0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
//用ki消除静差，但ki可导致超调
//kd用于提前调整

void PID_Init(void)//PID  - 初始化
{
  PID_Clear(&Deep_pid);//pid累积量清空，参数max、min、死区、k不变
	PID_Clear(&Pitch_pid);
//	PID_Clear(&Heading_pid);
	PID_Clear(&T_A_pid);
	PID_Clear(&T_D_pid);
//	PID_Param_Init(&Heading_pid, Heading_pid_param);//定航  PID//参数初始化
	PID_Param_Init(&T_A_pid, T_A_pid_param);//定航  PID//T参数初始化
	PID_Param_Init(&T_D_pid, T_D_pid_param);//定航  PID//T参数初始化
	PID_Param_Init(&Pitch_pid, Pitch_pid_param);//参数初始化 
	PID_Param_Init(&Deep_pid, Deep_pid_param);//定深 - 推进器//参数初始化
}
void PID_KP_ADJUST(PID_t *pid, float kp) //PID 参数kp设置函数
{
	pid->Kp = kp;//比例
	return;
}
void PID_KI_ADJUST(PID_t *pid, float ki) //PID 参数ki设置函数
{
	pid->Ki = ki;//积分
	return;
}
void PID_KD_ADJUST(PID_t *pid, float kd) //PID 参数kd设置函数
{
	pid->Kd = kd;//微分
	return;
}
void PID_Param_Init(PID_t *pid, float *pid_param) //PID 参数设置函数
{
	pid->Kp = pid_param[0];//比例
	pid->Ki = pid_param[1];//积分
	pid->Kd = pid_param[2];//微分
	pid->p_OutMAX = pid_param[3];
	pid->p_OutMIN = pid_param[4];
	pid->i_OutMAX = pid_param[5];
	pid->i_OutMIN = pid_param[6];
	pid->d_OutMAX = pid_param[7];
	pid->d_OutMIN = pid_param[8];
	pid->OutPutMAX = pid_param[9];
	pid->OutPutMIN = pid_param[10];
	pid->dead_zone = pid_param[11];
}
void PID_Clear(PID_t *pid)                   //PID 累计量清空
{
  pid->Bias = 0;
	pid->Last_Bias = 0;
	pid->Last_Last_Bias = 0;
	pid->p_Out = 0;
	pid->i_Out = 0;
	pid->d_Out = 0;
	pid->OutPut = 0;
} 

/*
下面这2个函数返回的是实际值，或者说是要给pwm赋值的值
例如控制深度，input应该是当前压力传感器的值，tar是目标深度时压力传感器的值
返回值output是一个深度值，代表这一步调整后要有多少深度，要换算成pwm值，赋值给上下方向的电机

第一个函数是位置式pid，根据给定的tar和输入的input，用离散pid公式计算output；然后直接返回output
第一次调用时，输入一个input，返回一个output，然后第二次调用时应该把第一次返回的output当作input输入

第二个函数是增量式pid，根据给定的tar和输入的input，用离散pid公式计算增量increment
然后将increment叠加在output上，再输出
第一次调用时，输入一个input，在这个input的基础上加increment，得到output，然后返回output
第二次调用时，将第一次返回的output当作input输入。
因此在增量式pid的实现函数Incremental_PID_Cal中，第一次调用时,pid->output一开始应该等于input，而不是0

*/
float Position_PID_Cal(float tar,float input,PID_t *pid)//位置式PID
{
	pid->Bias = tar -input;
	
	pid->p_Out = pid->Kp*pid->Bias;
	pid->p_Out = CONSTRAIN(pid->p_Out, pid->p_OutMAX, pid->p_OutMIN);
	
	pid->i_Out+= pid->Ki*pid->Bias;//计算积分值，注意这里是+=
	pid->i_Out = CONSTRAIN(pid->i_Out, pid->i_OutMAX, pid->i_OutMIN);
	
	pid->d_Out = pid->Kd*(pid->Bias - pid->Last_Bias);
  pid->d_Out = CONSTRAIN(pid->d_Out, pid->d_OutMAX, pid->d_OutMIN);
	
	pid->OutPut  = pid->p_Out + pid->i_Out + pid->d_Out;
	pid->OutPut  = CONSTRAIN(pid->OutPut,pid->OutPutMAX,pid->OutPutMIN);//取三个参数的中位数
	
	pid->Last_Bias = pid->Bias;						
  return pid->OutPut;
}

float Incremental_PID_Cal(float tar,float input,PID_t *pid)//增量式PID计算
{
	float increment = 0;
	pid->OutPut = input;//自己加的这句
	pid->Bias = tar - input;

	pid->p_Out = pid->Kp*(pid->Bias-pid->Last_Bias);                                      
	pid->p_Out = CONSTRAIN(pid->p_Out, pid->p_OutMAX, pid->p_OutMIN);
	
	pid->i_Out = pid->Ki*pid->Bias;                                                          
	pid->i_Out = CONSTRAIN(pid->i_Out, pid->i_OutMAX, pid->i_OutMIN);
	
	pid->d_Out = pid->Kd*(pid->Bias - 2*pid->Last_Bias + pid->Last_Last_Bias);//二阶差分
  pid->d_Out = CONSTRAIN(pid->d_Out, pid->d_OutMAX, pid->d_OutMIN);
		
	increment = pid->p_Out + pid->i_Out + pid->d_Out; //增量
	
	if(fabs(pid->Bias) < pid->dead_zone){
		
		increment = 0;
	}
	pid->OutPut += increment;
	pid->OutPut  = CONSTRAIN(pid->OutPut,pid->OutPutMAX,pid->OutPutMIN);
	
	pid->Last_Last_Bias = pid->Last_Bias;
	pid->Last_Bias = pid->Bias;		
	
  return pid->OutPut;
}
 
