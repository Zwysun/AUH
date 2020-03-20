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
const float dis2angle=-0.05;//��������͵�ǰ����ͣ����кܴ�Ĺ�ϵ
const int distance_threshold=100;
const int yaw_threshold=10;
const int depth_threshold=1;
const int pitch_threshold=8;
const int localization_threshold=50;
const int left_couple=-85;const int right_couple=-84;

//Ŀǰ��ʶ�����Ⱥ�ˮ�ƴ�����Ȳ���23cm


//����˼·1��	������������ĵ�ƫ��������Ѳ�߽ǵĸ���ֵ��
//						ƫ��Ϊ0ʱ��Ѳ�߽ǵĸ���ֵΪ0
//						�������ʱ��Ѳ�߽ǵĸ���ֵ>0
//						�����Ҳ�ʱ��Ѳ�߽ǵĸ���ֵ<0
//						Ѳ�߽������Ͳ���������С�����ֻҪ�趨avr_leftright���ɣ�Ѳ�߽�����ÿ��һ��ʱ����У�����һ���������ں�
//							�ٶȻص�avr_leftright��Ѳ������ʱ��ǰ��
//						ȱ����Ѳ�߽ǵ��������ںܴ�������������ǰ���ٶ�
//��һ��Ѳ�߰汾��Ҫ��̫���ӣ�ʵ������˵

//�趨һ����ȫ��ֵ����������Ϊ��λ���ڰ�ȫ��ֵ�ڵ�ʱ����Ҫͣ��������ת��
//					
//ǰ��������ɫ���Ƕ�,ָ���Ҳ�Ϊ��
//���ĵ����ߵ���������������ƫ�Ҳ�ʱdistanceΪ��
//JY901��roll�Ƕ��ڿ��180��������-180��

PIDtypedef PID_line;

void PID_line_Init(float P,float I,float D)
{
	PID_line.proportion=P;
	PID_line.integral=I;
	PID_line.derivative=D;
	PID_line.setpoint=0;//Ŀ��Ƕ���0
	reset_PID_line();
}

void reset_PID_line(void)
{
	PID_line.prev_error=0;
	PID_line.last_error=0;
}

//�������Ľ�����Ѳ�߽�û��̫�����Ļ����ϵ�
//����֮ǰҪִ��ɲ����֮��Ҫִ��slightmove forward
void Turn_leftright(int target_angle,int test_lag_cnt,int test_speed)
{
	//����Ԥɲ����cnt=10(7û׼Ҳ��),speed=1��Ч������ȱ��������Ư��,��Ӧ����
	static int error=0;
	int safeindex=0;
	yaw_Reset();

//Ŀǰ��˼·��ͨ��JY901��yaw�Ƕ����ж�ת��ֹͣ

	
//�����ʱʱ�������˯�ѵ�JY901�������µĽǶȵ�USART1
//���û�������Ǹ�whileloop�������yaw���룬������ǰ����loop
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
			break;										//������ζ����������ѭ������Խ��target angle
		left=-test_speed*(error>0);
		right=-test_speed*(error<0);//��תʱ�����ִ�е������ʱ����Ѿ�ת��ͷ��
		Setpwm_LR();		
	}	
	LED_Modify(LIGHTEN_EVEN_PORT);
	while(1)
	{
		error=target_angle-yaw;//errorΪ����ʱ����Ҫ��ת
		if(error>yaw_threshold||error<-yaw_threshold)
			safeindex=0;
		else
			safeindex++;
		
		left=-test_speed*(error<0);//��ʵԭ��ת��ֻת1�����Ҳ��OK�ģ�������Ҫ����ʵ�ǳ�С
		right=-test_speed*(error>0);//����ѡ������С�����䷽��
		Setpwm_LR();
		
		if(safeindex==test_lag_cnt)//200ms���������ڻ�����7�ļ���ֵ����ʵ����ˮ�������������Ҫ�仯
		{
			left=0,right=0;
			Setpwm_LR();
			break;
		}			
		delay_ms(200);//�����ʱ�������൱������������
	}
	LED_Modify(LIGHTEN_ODD_PORT);
}

void Slight_MoveForward(int lag_time,int mov_lagpro)//ǰ����mov_lagproҪС�ں��˵�mov_lagpro
{
//Ŀ��Ӧ����ǰ��һ�ξ���x,����ʹ��vΪ0��a=0
//�����ƶ��Ļ���1���ܸ����Ʋ���	
		TIM_SetCompare3(TIM1,1455);
		TIM_SetCompare3(TIM3,1555);
		delay_ms(lag_time*10);

		left=-18;
		right=-17;
		Setpwm_LR();
		delay_ms(lag_time*23);//������Ļ������䣬��������ʱ���㣬������ʱʱ�䳬������ 
													//��˵��ε��õĻ���lagtime��Ҫ����70�����lagtime��С����ƫת������ԷŴ�ǰ��������
													//Ŀǰ����������lagtime ȡ 50��Ч���ȽϺ�
		left=0;
		right=0;
		Setpwm_LR();
}

void Slight_MoveBehind(int lag_time,int mov_lagpro)
{
//Ŀ��Ӧ����ǰ��һ�ξ���x,����ʹ��vΪ0��a=0
		left=-18;
		right=-17;
		Setpwm_LR();
		delay_ms(lag_time*27);//ͬ���أ�lagtimeȡ50���ʣ�����ȱ���ǻ����ת�أ���ǰ����һ����

		TIM_SetCompare3(TIM1,1455);
		TIM_SetCompare3(TIM3,1555);

		delay_ms(lag_time*10);
		
		left=0;
		right=0;
		Setpwm_LR();
}

//�����������֮ǰҲ����ر�Ѳ�߱ջ������������ǻ���ռ�õ������߳�
void Localization(void)
{
	//x_error��ʱ���ڿ��Ʒ�Χ��
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
	//10�Ļ��ھ�ˮ������Щ̫���ˣ����Գ�����7����
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
		error=target_angle-yaw;//����ɲ����ʱ���������ٶ�
		if(yaw_speed*yaw_speed<9||error*error<17)			
//		if(error*error<17)//ˮ����ʱ�������ֹͣ�߼�����ǰ��ֹ����ǰ�ܾ�

			break;										
		left=left_couple*(error>0)+1*(error<0);
		right=right_couple*(error<0)+1*(error>0);
		Setpwm_LR();	
	}	
//���²�����	
	left=0,right=0;
	Setpwm_LR();
//���ϲ�����	
//	LED_Modify(LIGHTEN_EVEN_PORT);


/*	while(1)
	{
		error=target_angle-yaw;//errorΪ����ʱ����Ҫ��ת
		if(error>yaw_threshold||error<-yaw_threshold)
			safeindex=0;
		else
			safeindex++;
		
		left=-test_speed*(error<0);//��ʵԭ��ת��ֻת1�����Ҳ��OK�ģ�������Ҫ����ʵ�ǳ�С
		right=-test_speed*(error>0);//����ѡ������С�����䷽��
		Setpwm_LR();
		
		if(safeindex==test_lag_cnt)//200ms���������ڻ�����7�ļ���ֵ����ʵ����ˮ�������������Ҫ�仯
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
//ִ���������֮ǰҪ�ر�Ѳ�ߵıջ�
//����ͨ���𲽸ı�target pitch������������ؽ�ת�ǣ���ʵ�ָ�ƽ����ɲ��	
//�������Ѳ�߽Ƕ�û������ʱ��ɲ��+ת�䣬�ǻ���ֲ��õ�����	
	static int i=0;
	printf("A pitch-brake is happening\n");
	left=0,right=0;
	Setpwm_LR();
	
//Ŀǰ�ĳ���˼·�Ǹı�PITCH�ĸ���ֵ��ͨ������ӭˮ�������ɲ��
	Target_pitch=brake_pitch;//����pitchΪ���Ļ���ǰ������ˮƽ����������ɲ��
//�Ƕ�Խ�󣬹��ɹ��̻��и�����Ŷ������Ǻ���Ȼ���ϵ�
	for(i=0;i<lag_time;i++)
	{
		USART_SendData(USART2,CMD_LOCALIZATION_ID);//�Ͳ�֪������ô����û������
		delay_ms(200);
		if(y_error!=1000) pre_y_error=y_error;
	}
	Target_pitch=0;
	//pitch����2���Ŷ������ɲ����Ҫ����������ͨ������brake pitch�Ĵ�С��ʹ�ú��˾����ɲ���������ӳٵ���
	//���Ϳ���λ��������һ������
}

void Brake_pitch_straight(int brake_pitch,int lag_time)
{
//ִ���������֮ǰҪ�ر�Ѳ�ߵıջ�

	static int i=0;
	printf("A straight pitch-brake is happening\n");
	left=left_couple,right=right_couple;
	Setpwm_LR();
	
//Ŀǰ�ĳ���˼·�Ǹı�PITCH�ĸ���ֵ��ͨ������ӭˮ�������ɲ��
	Target_pitch=brake_pitch;//����pitchΪ���Ļ���ǰ������ˮƽ����������ɲ��
//�Ƕ�Խ�󣬹��ɹ��̻��и�����Ŷ������Ǻ���Ȼ���ϵ�
	for(i=0;i<lag_time;i++)
	{
		USART_SendData(USART2,CMD_LOCALIZATION_ID);//�Ͳ�֪������ô����û������
		if(pitch<brake_pitch/3)
		{
			left=0,right=0;
			Setpwm_LR();
		}	
		delay_ms(200);
		if(y_error!=1000) pre_y_error=y_error;
	}
	
	Target_pitch=0;
	//pitch����2���Ŷ������ɲ����Ҫ����������ͨ������brake pitch�Ĵ�С��ʹ�ú��˾����ɲ���������ӳٵ���
	//���Ϳ���λ��������һ������
}

void Brake_straight(int lag_time)
{
//ִ���������֮ǰҪ�ر�Ѳ�ߵıջ�

	static int i=0;
	printf("A straight brake is happening\n");
	left=left_couple,right=right_couple;
	Setpwm_LR();

	for(i=0;i<lag_time;i++)
	{	
		USART_SendData(USART2,CMD_LOCALIZATION_ID);//�Ͳ�֪������ô����û������
		delay_ms(200);//����ӳ�ʱ��ܹؼ�,Ŀǰ����1000ms
		if(y_error!=1000) pre_y_error=y_error;
	}
	left=0,right=0;
	Setpwm_LR();
	//���Ϳ���λ��������һ������
}
