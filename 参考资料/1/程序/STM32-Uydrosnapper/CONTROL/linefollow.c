#include "pid_for_posture.h"
#include "motor.h"
#include "command_from_pi.h"
#include "linefollow.h"
#include "delay.h"
#include "usart.h"
#include "JY901.h"
#include "led.h"
int islineloopclosed =0;
int isyawloopclosed=0;
int avr_leftright=6;
int mov_lagtime=0,mov_lagpro=0;
const int nav_gain=4, nav_pro=47;
const float dis2angle=-0.05;//这个比例和当前的悬停深度有很大的关系
const int distance_threshold=100;
const int yaw_threshold=10;
const int depth_threshold=1;
const int pitch_threshold=8;
const int localization_threshold=50;
const int left_couple=-85;const int right_couple=-84;

//目前能识别的深度和水灯带的深度差是23cm


//基本思路1：	根据与红线质心的偏距来修正巡线角的给定值。
//						偏距为0时，巡线角的给定值为0
//						线在左侧时，巡线角的给定值>0
//						线在右侧时，巡线角的给定值<0
//						巡线角修正和步进交替进行。步进只要设定avr_leftright即可，巡线角修正每隔一段时间进行，修正一个控制周期后
//							速度回到avr_leftright。巡线修正时不前进
//						缺点是巡线角的数据周期很大意义上限制了前进速度
//第一个巡线版本不要想太复杂，实现了再说

//设定一个安全阈值，以像素数为单位，在安全阈值内的时候不需要停下来调节转角
//					
//前进方向与色带角度,指向右侧为正
//中心到中线的像素数，机器人偏右侧时distance为正
//JY901的roll角度在跨过180°后会跳到-180°

PIDtypedef PID_line;

void PID_line_Init(float P,float I,float D)
{
	PID_line.proportion=P;
	PID_line.integral=I;
	PID_line.derivative=D;
	PID_line.setpoint=0;//目标角度是0
	reset_PID_line();
}

void reset_PID_line(void)
{
	PID_line.prev_error=0;
	PID_line.last_error=0;
}

//这个程序的建立在巡线角没有太大误差的基础上的
//在这之前要执行刹车，之后要执行slightmove forward
void Turn_leftright(int target_angle,int test_lag_cnt,int test_speed)
{
	//加入预刹车后，cnt=10(7没准也行),speed=1，效果不错，缺点是质心漂移,响应较慢
	static int error=0;
	int safeindex=0;
	yaw_Reset();

//目前的思路是通过JY901的yaw角度来判断转弯停止

	
//这个延时时间里面刚睡醒的JY901发送最新的角度到USART1
//如果没有下面那个whileloop，错误的yaw传入，会跳过前两个loop
	while(yaw!=0);
	LED_Modify(SHUTDOWN_ALL);

	while(1)
	{
		error=target_angle-yaw;
		left=-4*test_speed*(error<0);
		right=-4*test_speed*(error>0);
		Setpwm_LR();
		if(error*error<target_angle*target_angle*0.04)//
			break;
	}
	LED_Modify(LIGHTEN_ALL);
	while(1)
	{
		error=target_angle-yaw;
		if(yaw_speed*yaw_speed<30||error*error<2)
			break;										//无论如何都不能在这个循环里面越过target angle
		left=-test_speed*(error>0);
		right=-test_speed*(error<0);//右转时，如果执行到这里的时候就已经转过头了
		Setpwm_LR();		
	}	
	LED_Modify(LIGHTEN_EVEN_PORT);
	while(1)
	{
		error=target_angle-yaw;//error为正的时候还需要右转
		if(error>yaw_threshold||error<-yaw_threshold)
			safeindex=0;
		else
			safeindex++;
		
		left=-test_speed*(error<0);//其实原地转弯只转1个电机也是OK的，力度需要的其实非常小
		right=-test_speed*(error>0);//所以选择力度小的喷射方向
		Setpwm_LR();
		
		if(safeindex==test_lag_cnt)//200ms的运算周期或者是7的计数值比起实际下水的情况，可能需要变化
		{
			left=0,right=0;
			Setpwm_LR();
			break;
		}			
		delay_ms(200);//这个延时的作用相当于是运算周期
	}
	LED_Modify(LIGHTEN_ODD_PORT);
}

void Slight_MoveForward(int lag_time,int mov_lagpro)//前进的mov_lagpro要小于后退的mov_lagpro
{
//目标应当是前进一段距离x,阻尼使得v为0，a=0
//反向推动的话，1可能根本推不动	
		TIM_SetCompare3(TIM1,1455);
		TIM_SetCompare3(TIM3,1555);
		delay_ms(lag_time*10);

		left=-18;
		right=-17;
		Setpwm_LR();
		delay_ms(lag_time*23);//配凑力的话不好配，还是配延时方便，但是延时时间超过输入 
													//因此单次调用的话，lagtime不要超过70，如果lagtime过小，则偏转力会相对放大，前进不明显
													//目前测试下来，lagtime 取 50的效果比较好
		left=0;
		right=0;
		Setpwm_LR();
}

void Slight_MoveBehind(int lag_time,int mov_lagpro)
{
//目标应当是前进一段距离x,阻尼使得v为0，a=0
		left=-18;
		right=-17;
		Setpwm_LR();
		delay_ms(lag_time*27);//同样地，lagtime取50合适，但是缺点是会产生转矩，和前进是一样的

		TIM_SetCompare3(TIM1,1455);
		TIM_SetCompare3(TIM3,1555);

		delay_ms(lag_time*10);
		
		left=0;
		right=0;
		Setpwm_LR();
}

//这个程序运行之前也建议关闭巡线闭环，理想的情况是黄球占用第三个线程
void Localization(void)
{
	//x_error暂时不在控制范围内
	int safeindex=0;
	while(1)
	{
		if(y_error<localization_threshold&&y_error>-localization_threshold)
		{		
			safeindex++;
			delay_ms(100);
		}
		else
		{
			safeindex=0;
			if(y_error>0)
				Slight_MoveForward(50,15);
			else if(y_error<0)
				Slight_MoveBehind(50,20);
		}
	}
}

void Macroturn_leftright(int target_angle,int test_lag_cnt)
{
	//10的话在静水里面有些太慢了，可以尝试用7试试
	static int error=0;
	yaw_Reset();
	while(yaw!=0);
//	LED_Modify(SHUTDOWN_ALL);

	while(1)
	{
		error=target_angle-yaw;
		left=left_couple*(error<0)+1*(error>0);
		right=right_couple*(error>0)+1*(error<0);
		Setpwm_LR();	
		if(error*error<target_angle*target_angle*0.15)
			break;
	}
//	LED_Modify(LIGHTEN_ALL);
	while(1)
	{
		error=target_angle-yaw;//这里刹车的时候，往往角速度
		if(yaw_speed*yaw_speed<9||error*error<17)			
//		if(error*error<17)//水阻大的时候，上面的停止逻辑会提前终止，提前很久

			break;										
		left=left_couple*(error>0)+1*(error<0);
		right=right_couple*(error<0)+1*(error>0);
		Setpwm_LR();	
	}	
//以下测试用	
	left=0,right=0;
	Setpwm_LR();
//以上测试用	
//	LED_Modify(LIGHTEN_EVEN_PORT);


/*	while(1)
	{
		error=target_angle-yaw;//error为正的时候还需要右转
		if(error>yaw_threshold||error<-yaw_threshold)
			safeindex=0;
		else
			safeindex++;
		
		left=-test_speed*(error<0);//其实原地转弯只转1个电机也是OK的，力度需要的其实非常小
		right=-test_speed*(error>0);//所以选择力度小的喷射方向
		Setpwm_LR();
		
		if(safeindex==test_lag_cnt)//200ms的运算周期或者是7的计数值比起实际下水的情况，可能需要变化
		{
			left=0,right=0;
			Setpwm_LR();
			break;
		}			
		delay_ms(200);
	}
*/
//	LED_Modify(LIGHTEN_ODD_PORT);
}

void Brake(int brake_pitch,int lag_time)
{
//执行这个函数之前要关闭巡线的闭环
//可以通过逐步改变target pitch，类似外骨骼关节转角，来实现更平滑的刹车	
//如果是在巡线角度没修正的时候刹车+转弯，那会出现不好的事情	
	static int i=0;
	printf("A pitch-brake is happening\n");
	left=0,right=0;
	Setpwm_LR();
	
//目前的初步思路是改变PITCH的给定值，通过增大迎水面面积来刹车
	Target_pitch=brake_pitch;//给定pitch为负的话，前后电机的水平分力有助于刹车
//角度越大，过渡过程会有更大的扰动，这是和深度环耦合的
	for(i=0;i<lag_time;i++)
	{
		USART_SendData(USART2,CMD_LOCALIZATION_ID);//就不知道发这么快有没有事情
		delay_ms(200);
		if(y_error!=1000) pre_y_error=y_error;
	}
	Target_pitch=0;
	//pitch会有2下扰动，因此刹车后要后退两步，通过调节brake pitch的大小来使得后退距离和刹车触发的延迟抵消
	//随后就靠定位来修正那一点点误差
}

void Brake_pitch_straight(int brake_pitch,int lag_time)
{
//执行这个函数之前要关闭巡线的闭环

	static int i=0;
	printf("A straight pitch-brake is happening\n");
	left=left_couple,right=right_couple;
	Setpwm_LR();
	
//目前的初步思路是改变PITCH的给定值，通过增大迎水面面积来刹车
	Target_pitch=brake_pitch;//给定pitch为负的话，前后电机的水平分力有助于刹车
//角度越大，过渡过程会有更大的扰动，这是和深度环耦合的
	for(i=0;i<lag_time;i++)
	{
		USART_SendData(USART2,CMD_LOCALIZATION_ID);//就不知道发这么快有没有事情
		if(pitch<brake_pitch/3)
		{
			left=0,right=0;
			Setpwm_LR();
		}	
		delay_ms(200);
		if(y_error!=1000) pre_y_error=y_error;
	}
	
	Target_pitch=0;
	//pitch会有2下扰动，因此刹车后要后退两步，通过调节brake pitch的大小来使得后退距离和刹车触发的延迟抵消
	//随后就靠定位来修正那一点点误差
}

void Brake_straight(int lag_time)
{
//执行这个函数之前要关闭巡线的闭环

	static int i=0;
	printf("A straight brake is happening\n");
	left=left_couple,right=right_couple;
	Setpwm_LR();

	for(i=0;i<lag_time;i++)
	{	
		USART_SendData(USART2,CMD_LOCALIZATION_ID);//就不知道发这么快有没有事情
		delay_ms(200);//这个延迟时间很关键,目前建议1000ms
		if(y_error!=1000) pre_y_error=y_error;
	}
	left=0,right=0;
	Setpwm_LR();
	//随后就靠定位来修正那一点点误差
}
