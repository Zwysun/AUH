/*高精度增量式PID控制*/

#include "pid_for_posture.h"
#include "motor.h"
#include "delay.h"
#include "linefollow.h"
#include "usart.h"
#include "HP206C.h"
#include "led.h"
#include "JY901.h"
//水下机器人准备起跑时的深度，单位为cm

int Target_depth = 0;
int Target_pitch = 0;
PIDtypedef PID_pitch;
PIDtypedef PID_depth;

int isposeloopclosed =0;

int incPIDcalc(PIDtypedef *PIDx,int nextpoint)  //PID计算
{
	int iError,iincpid;
	iError=PIDx->setpoint-nextpoint;  //当前误差
	iincpid=                                               //增量计算
		(int)(PIDx->proportion*iError                //e[k]项
		-PIDx->integral*PIDx->last_error          //e[k-1]
		+PIDx->derivative*PIDx->prev_error);//e[k-2]
	PIDx->prev_error=PIDx->last_error; //存储误差，便于下次计算
	PIDx->last_error=iError;
	return iincpid;
}				

void PID_setpoint(PIDtypedef*PIDx,int setvalue)  //设定 PID预期值
{
	PIDx->setpoint =setvalue;
}

void incPIDinit(PIDtypedef*PIDx)//初始化，参数清零
{
	PIDx->last_error=0;
	PIDx->prev_error=0;
}

void PID_setgain(PIDtypedef*PIDx,float pp,float ii,float dd)//设定PID  kp ki kd三个参数
{
	PIDx->proportion=pp;
	PIDx->integral=ii;
	PIDx->derivative=dd;
}

void PID_pos_init(void)//pid初始化
{	
	 PID_setpoint(&PID_pitch,Target_pitch);//说明了最佳的pitch角度应该是0°
	 PID_setpoint(&PID_depth,Target_depth);
	 PID_setgain(&PID_pitch,P_pitch,I_pitch,D_pitch);//如果用宏定义来表示PID的3个增益的话，注意传参时的数据类型和括号
	 PID_setgain(&PID_depth,P_depth,I_depth,D_depth);//如果用宏定义来表示PID的3个增益的话，注意传参时的数据类型和括号
	 incPIDinit(&PID_pitch);
	 incPIDinit(&PID_depth);
}
//该函数只能在main进程里面调用，这样比较合适
void Depth_Disturb_Suppress(void)
{
	static int error=0;
	int safeindex=0;
	while(1)
	{
		error=Target_depth-(int)Hydrosnapper_hp206c.depth;//目前depth threshold为1，相当严格了
		if(error>depth_threshold||error<-depth_threshold)
			safeindex=0;
		else
			safeindex++;
		
		if(safeindex==5)
			break;	
		delay_ms(200);//这个延时的作用相当于是运算周期
	}
}

void Pitch_Disturb_Suppress(void)
{
	static int error=0;
	int safeindex=0;
	while(1)
	{
		error=Target_pitch-pitch;//目前pitch threshold为3，相当严格了
		if(error>pitch_threshold||error<-pitch_threshold)
			safeindex=0;
		else
			safeindex++;
		
		if(safeindex==3)
			break;	
		delay_ms(200);//这个延时的作用相当于是运算周期
	}
}
