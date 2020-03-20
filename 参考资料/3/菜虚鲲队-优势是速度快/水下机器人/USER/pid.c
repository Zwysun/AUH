#include "pid.h"
PID angle_x_pid;
PID angle_y_pid;
PID angle_z_pid;
PID high_pid;

/*************************************************************************
*                             �㽭�����ѧ
*
*  �������ƣ�PID_Parameter_Init
*  ����˵��������ָ���ṹ��Ĳ���
*  ����˵����*pp��      ָ����PID�ṹ��ĵ�ַ
*            setkp��    Ҫ�趨�ı�������
*            setki��    Ҫ�趨�Ļ��ֳ���
*            setkd��    Ҫ�趨��΢�ֳ���
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*  ��    ע��
*************************************************************************/
void PID_Parameter_Init(PID *pp,float setpoint,float setkp, float setki, float setkd)
{
   pp->point = setpoint;   //�趨��Ŀ��ֵ
   pp->kp = setkp;         //�趨�ı������� Proportional Const
   pp->ki = setki;         //�趨�Ļ��ֳ��� Integral Const
   pp->kd = setkd;         //�趨��΢�ֳ��� Derivative Const
   pp->ek = 0;             //e[k]   ����ƫ��
   pp->ek_1 = 0;           //e[k-1] �ϴ�ƫ��
   pp->ek_2 = 0;           //e[k-2] ���ϴ�ƫ��
   pp->uk = 0;;            //����PID���ֵ
   pp->uk_1 = 0;           //�ϴ�PID���ֵ
   pp->ek_sum = 0;         //ƫ���ܺ�
}

/*************************************************************************
*                             �㽭�����ѧ
*
*  �������ƣ�Position_PID
*  ����˵����λ��ʽPID�㷨
*  ����˵����*pp ���ƽṹ���ַָ��;cʵ�ʲ���ֵ(���ڶ����˵Ϊͼ��error)
*  �������أ���
*  �޸�ʱ�䣺2015-11-1   
*  ��    ע�����ڶ��
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
*                             �㽭�����ѧ
*
*  �������ƣ�Increase_PID
*  ����˵��������ʽPID�㷨
*  ����˵����*pp ���ƽṹ���ַָ��;cʵ�ʲ���ֵ(���ڵ����˵Ϊ�ٶȲ���ֵ)
*  �������أ���
*  �޸�ʱ�䣺2015-11-1 
*  ��    ע�����ڵ��
*************************************************************************/
void Increase_PID(PID *pp, float feedbackValue)
{   
  pp->ek = pp->point - feedbackValue;
  pp->uk = pp->uk_1 + (pp->kp)*((pp->ek)-(pp->ek_1))+(pp->ki)*(pp->ek)+(pp->kd)*((pp->ek)-2*(pp->ek_1)+(pp->ek_2));   
  //����޷�
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
