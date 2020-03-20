/*�߾�������ʽPID����*/

#include "pid_for_posture.h"
#include "motor.h"
#include "delay.h"
#include "linefollow.h"
#include "usart.h"
#include "HP206C.h"
#include "led.h"
#include "JY901.h"
//ˮ�»�����׼������ʱ����ȣ���λΪcm

int Target_depth = 0;
int Target_pitch = 0;
PIDtypedef PID_pitch;
PIDtypedef PID_depth;

int isposeloopclosed =0;

int incPIDcalc(PIDtypedef *PIDx,int nextpoint)  //PID����
{
	int iError,iincpid;
	iError=PIDx->setpoint-nextpoint;  //��ǰ���
	iincpid=                                               //��������
		(int)(PIDx->proportion*iError                //e[k]��
		-PIDx->integral*PIDx->last_error          //e[k-1]
		+PIDx->derivative*PIDx->prev_error);//e[k-2]
	PIDx->prev_error=PIDx->last_error; //�洢�������´μ���
	PIDx->last_error=iError;
	return iincpid;
}				

void PID_setpoint(PIDtypedef*PIDx,int setvalue)  //�趨 PIDԤ��ֵ
{
	PIDx->setpoint =setvalue;
}

void incPIDinit(PIDtypedef*PIDx)//��ʼ������������
{
	PIDx->last_error=0;
	PIDx->prev_error=0;
}

void PID_setgain(PIDtypedef*PIDx,float pp,float ii,float dd)//�趨PID  kp ki kd��������
{
	PIDx->proportion=pp;
	PIDx->integral=ii;
	PIDx->derivative=dd;
}

void PID_pos_init(void)//pid��ʼ��
{	
	 PID_setpoint(&PID_pitch,Target_pitch);//˵������ѵ�pitch�Ƕ�Ӧ����0��
	 PID_setpoint(&PID_depth,Target_depth);
	 PID_setgain(&PID_pitch,P_pitch,I_pitch,D_pitch);//����ú궨������ʾPID��3������Ļ���ע�⴫��ʱ���������ͺ�����
	 PID_setgain(&PID_depth,P_depth,I_depth,D_depth);//����ú궨������ʾPID��3������Ļ���ע�⴫��ʱ���������ͺ�����
	 incPIDinit(&PID_pitch);
	 incPIDinit(&PID_depth);
}
//�ú���ֻ����main����������ã������ȽϺ���
void Depth_Disturb_Suppress(void)
{
	static int error=0;
	int safeindex=0;
	while(1)
	{
		error=Target_depth-(int)Hydrosnapper_hp206c.depth;//Ŀǰdepth thresholdΪ1���൱�ϸ���
		if(error>depth_threshold||error<-depth_threshold)
			safeindex=0;
		else
			safeindex++;
		
		if(safeindex==5)
			break;	
		delay_ms(200);//�����ʱ�������൱������������
	}
}

void Pitch_Disturb_Suppress(void)
{
	static int error=0;
	int safeindex=0;
	while(1)
	{
		error=Target_pitch-pitch;//Ŀǰpitch thresholdΪ3���൱�ϸ���
		if(error>pitch_threshold||error<-pitch_threshold)
			safeindex=0;
		else
			safeindex++;
		
		if(safeindex==3)
			break;	
		delay_ms(200);//�����ʱ�������൱������������
	}
}
