#include "pid.h"
PID angle_x_pid;
PID angle_y_pid;
PID angle_z_pid;
PID high_pid;

/*************************************************************************
*                             浙江海洋大学
*
*  函数名称：PID_Parameter_Init
*  功能说明：设置指定结构体的参数
*  参数说明：*pp：      指向了PID结构体的地址
*            setkp：    要设定的比例常数
*            setki：    要设定的积分常数
*            setkd：    要设定的微分常数
*  函数返回：无
*  修改时间：2012-2-14    已测试
*  备    注：
*************************************************************************/
void PID_Parameter_Init(PID *pp,float setpoint,float setkp, float setki, float setkd)
{
   pp->point = setpoint;   //设定的目标值
   pp->kp = setkp;         //设定的比例常数 Proportional Const
   pp->ki = setki;         //设定的积分常数 Integral Const
   pp->kd = setkd;         //设定的微分常数 Derivative Const
   pp->ek = 0;             //e[k]   本次偏差
   pp->ek_1 = 0;           //e[k-1] 上次偏差
   pp->ek_2 = 0;           //e[k-2] 上上次偏差
   pp->uk = 0;;            //本次PID输出值
   pp->uk_1 = 0;           //上次PID输出值
   pp->ek_sum = 0;         //偏差总和
}

/*************************************************************************
*                             浙江海洋大学
*
*  函数名称：Position_PID
*  功能说明：位置式PID算法
*  参数说明：*pp 控制结构体地址指针;c实际测量值(对于舵机来说为图像error)
*  函数返回：无
*  修改时间：2015-11-1   
*  备    注：用于舵机
*************************************************************************/
void Position_PID(PID *pp,float error)
{   
   (pp->ek)=(pp->point)+error;
   (pp->ek_sum)+=(pp->ek);   
   (pp->uk)=(pp->kp)*(pp->ek)+(pp->ki)*(pp->ek_sum)+(pp->kd)*((pp->ek)-(pp->ek_1));  
   if((pp->uk)<-1000)
   {
      pp->uk=-1000;
   }
   else if((pp->uk)>1000)
   {
      pp->uk=1000;
   }
   pp->uk_1 = pp->uk;  
   pp->ek_1 = pp->ek;
}

/*************************************************************************
*                             浙江海洋大学
*
*  函数名称：Increase_PID
*  功能说明：增量式PID算法
*  参数说明：*pp 控制结构体地址指针;c实际测量值(对于电机来说为速度测量值)
*  函数返回：无
*  修改时间：2015-11-1 
*  备    注：用于电机
*************************************************************************/
void Increase_PID(PID *pp, float feedbackValue)
{   
  pp->ek = pp->point - feedbackValue;
  pp->uk = pp->uk_1 + (pp->kp)*((pp->ek)-(pp->ek_1))+(pp->ki)*(pp->ek)+(pp->kd)*((pp->ek)-2*(pp->ek_1)+(pp->ek_2));   
  //输出限幅
  if (pp->uk < -1000)
  {
    pp->uk = -1000;
  }
  else if (pp->uk > 1000)
  {
    pp->uk = 1000;
  }   
  pp->uk_1 = pp->uk;	
  pp->ek_2 = pp->ek_1;
  pp->ek_1 = pp->ek;
}

void pid_INIT(void)
{
  PID_Parameter_Init(&angle_x_pid,0,0.1,0.0,1);
  PID_Parameter_Init(&angle_y_pid,0,0.1,0.0,1);
  PID_Parameter_Init(&angle_z_pid,0,0.15,0.01,1);
  PID_Parameter_Init(&high_pid,0,0.1,0.005,0);
}
