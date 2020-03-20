#include "PID.h"

PID_t Deep_pid;
PID_t Heading_pid;//ˮƽת��
PID_t T_A_pid;//ˮƽת��
PID_t T_D_pid;//ˮƽת��
PID_t Pitch_pid;//ǰ��ʱͷ������
//��һ����ǰ��ʱ������б(��Ž�rolling)����һ��������û�����ƣ���Ҫ��������

//����ʽPID
//	-15.8    -0.076   ��ѹС��15.6V
//  -13.8    -0.05    ��ѹ����15.6V

//λ��ʽPID
//   -12     -0.35
                             //kp,ki,kd,     p_max,      p_min,     i_max,     i_min,    d_max,     d_min,  out_max,  out_min   ,����
float Heading_pid_param[12]={ 0  ,  0,   0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float     T_A_pid_param[12]={ -3.7,  0,  0,  MAX_T_A,    0,   MAX_T_A,    0,   MAX_T_A,    0,   MAX_T_A,   0,   0};
float     T_D_pid_param[12]={ -0.3, 0,   0,  MAX_T_D,-160,MAX_T_D,-160,MAX_T_D,-160,MAX_T_D,-160,   0};
float    Deep_pid_param[12]={ -100, 0,   0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
float   Pitch_pid_param[12]={ -10 , 0,0,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,MAX_SPEED,-MAX_SPEED,0};
//��ki���������ki�ɵ��³���
//kd������ǰ����

void PID_Init(void)//PID  - ��ʼ��
{
  PID_Clear(&Deep_pid);//pid�ۻ�����գ�����max��min��������k����
	PID_Clear(&Pitch_pid);
//	PID_Clear(&Heading_pid);
	PID_Clear(&T_A_pid);
	PID_Clear(&T_D_pid);
//	PID_Param_Init(&Heading_pid, Heading_pid_param);//����  PID//������ʼ��
	PID_Param_Init(&T_A_pid, T_A_pid_param);//����  PID//T������ʼ��
	PID_Param_Init(&T_D_pid, T_D_pid_param);//����  PID//T������ʼ��
	PID_Param_Init(&Pitch_pid, Pitch_pid_param);//������ʼ�� 
	PID_Param_Init(&Deep_pid, Deep_pid_param);//���� - �ƽ���//������ʼ��
}
void PID_KP_ADJUST(PID_t *pid, float kp) //PID ����kp���ú���
{
	pid->Kp = kp;//����
	return;
}
void PID_KI_ADJUST(PID_t *pid, float ki) //PID ����ki���ú���
{
	pid->Ki = ki;//����
	return;
}
void PID_KD_ADJUST(PID_t *pid, float kd) //PID ����kd���ú���
{
	pid->Kd = kd;//΢��
	return;
}
void PID_Param_Init(PID_t *pid, float *pid_param) //PID �������ú���
{
	pid->Kp = pid_param[0];//����
	pid->Ki = pid_param[1];//����
	pid->Kd = pid_param[2];//΢��
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
void PID_Clear(PID_t *pid)                   //PID �ۼ������
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
������2���������ص���ʵ��ֵ������˵��Ҫ��pwm��ֵ��ֵ
���������ȣ�inputӦ���ǵ�ǰѹ����������ֵ��tar��Ŀ�����ʱѹ����������ֵ
����ֵoutput��һ�����ֵ��������һ��������Ҫ�ж�����ȣ�Ҫ�����pwmֵ����ֵ�����·���ĵ��

��һ��������λ��ʽpid�����ݸ�����tar�������input������ɢpid��ʽ����output��Ȼ��ֱ�ӷ���output
��һ�ε���ʱ������һ��input������һ��output��Ȼ��ڶ��ε���ʱӦ�ðѵ�һ�η��ص�output����input����

�ڶ�������������ʽpid�����ݸ�����tar�������input������ɢpid��ʽ��������increment
Ȼ��increment������output�ϣ������
��һ�ε���ʱ������һ��input�������input�Ļ����ϼ�increment���õ�output��Ȼ�󷵻�output
�ڶ��ε���ʱ������һ�η��ص�output����input���롣
���������ʽpid��ʵ�ֺ���Incremental_PID_Cal�У���һ�ε���ʱ,pid->outputһ��ʼӦ�õ���input��������0

*/
float Position_PID_Cal(float tar,float input,PID_t *pid)//λ��ʽPID
{
	pid->Bias = tar -input;
	
	pid->p_Out = pid->Kp*pid->Bias;
	pid->p_Out = CONSTRAIN(pid->p_Out, pid->p_OutMAX, pid->p_OutMIN);
	
	pid->i_Out+= pid->Ki*pid->Bias;//�������ֵ��ע��������+=
	pid->i_Out = CONSTRAIN(pid->i_Out, pid->i_OutMAX, pid->i_OutMIN);
	
	pid->d_Out = pid->Kd*(pid->Bias - pid->Last_Bias);
  pid->d_Out = CONSTRAIN(pid->d_Out, pid->d_OutMAX, pid->d_OutMIN);
	
	pid->OutPut  = pid->p_Out + pid->i_Out + pid->d_Out;
	pid->OutPut  = CONSTRAIN(pid->OutPut,pid->OutPutMAX,pid->OutPutMIN);//ȡ������������λ��
	
	pid->Last_Bias = pid->Bias;						
  return pid->OutPut;
}

float Incremental_PID_Cal(float tar,float input,PID_t *pid)//����ʽPID����
{
	float increment = 0;
	pid->OutPut = input;//�Լ��ӵ����
	pid->Bias = tar - input;

	pid->p_Out = pid->Kp*(pid->Bias-pid->Last_Bias);                                      
	pid->p_Out = CONSTRAIN(pid->p_Out, pid->p_OutMAX, pid->p_OutMIN);
	
	pid->i_Out = pid->Ki*pid->Bias;                                                          
	pid->i_Out = CONSTRAIN(pid->i_Out, pid->i_OutMAX, pid->i_OutMIN);
	
	pid->d_Out = pid->Kd*(pid->Bias - 2*pid->Last_Bias + pid->Last_Last_Bias);//���ײ��
  pid->d_Out = CONSTRAIN(pid->d_Out, pid->d_OutMAX, pid->d_OutMIN);
		
	increment = pid->p_Out + pid->i_Out + pid->d_Out; //����
	
	if(fabs(pid->Bias) < pid->dead_zone){
		
		increment = 0;
	}
	pid->OutPut += increment;
	pid->OutPut  = CONSTRAIN(pid->OutPut,pid->OutPutMAX,pid->OutPutMIN);
	
	pid->Last_Last_Bias = pid->Last_Bias;
	pid->Last_Bias = pid->Bias;		
	
  return pid->OutPut;
}
 
