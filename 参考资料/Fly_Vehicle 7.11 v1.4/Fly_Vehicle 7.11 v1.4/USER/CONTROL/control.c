#include "control.h"

extern uint16_t pwm_left;
extern uint16_t pwm_right;
extern uint16_t pwm_front;
extern uint16_t pwm_back;

extern PID_t T_A_pid;//ˮƽת��
extern PID_t T_D_pid;//ˮƽת��

extern ControlMsg_t control_data;
extern uint16_t test_pwm;//����Ĭ��ֵ����1500
extern uint16_t *test_pwm1;
extern uint16_t test_pwm2;
extern uint16_t *test_pwm3;
extern uint16_t test_pwm4;
extern uint16_t *test_pwm5;
extern uint16_t test_pwm6;
extern uint16_t *test_pwm7;
extern uint16_t test_pwm8;
extern uint8_t enter[];
extern PID_t T_pid;//ˮƽת��

extern void LED_SW(u8 num,u8 sta);
extern void Cruise(void);
extern void ForwardStop(void);

//��ֱ������ �ٶ�����
void SetUpDowm(float front_val,float back_val)//min--max �����˶�
{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1502-front_val); //1
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1502+back_val);//3 // ��������߷���
//	 static 	uint16_t	last_value_front = mid;
//	 static 	uint16_t	last_value_back = mid;
// 
//    uint16_t front = mid + front_val;
//    uint16_t back = mid - back_val;  // ��������߷���
//    
//    front = CONSTRAIN(front, last_value_front+max_speed_delta, last_value_front-max_speed_delta);//#define CONSTRAIN(x,max,min) (x>max?max:(x<min?min:x))
//    back = CONSTRAIN(back, last_value_back+max_speed_delta, last_value_back-max_speed_delta);

//    front = CONSTRAIN(front, max_speed_value, min_speed_value);
//    back = CONSTRAIN(back, max_speed_value, min_speed_value);
//	
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,front); //1
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,back);//3
//	
//	
//    last_value_front = front;
//    last_value_back = back;
} 

float temp_l = 0;
float temp_r =0;
//ˮƽ������ �ٶ�����
void SetHorizontal(float f_b, float rotate)//ˮƽ�˶�
{
	temp_l = f_b+left_mid+rotate;
	temp_r = f_b+right_mid-rotate;
	//���Ʒ���һ
  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,temp_r);//2
 	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,temp_l);//4
	
	
	//���Ʒ�����
//	static uint16_t last_value_left = left_mid;
//  static uint16_t last_value_right = right_mid;
//	
//	float right = right_mid + f_b - rotate;//��
//	float left = left_mid + f_b + rotate;//��
// 
//	right = CONSTRAIN(right, last_value_right+max_speed_delta, last_value_right-max_speed_delta);
//	left = CONSTRAIN(left, last_value_left+max_speed_delta, last_value_left-max_speed_delta);
//    
//  left = CONSTRAIN(left, max_speed_value, min_speed_value);
//  right = CONSTRAIN(right, max_speed_value, min_speed_value);

//  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,right);//2
// 	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,left);//4

//	last_value_left =  left;
//	last_value_right = right;

}

u16 xCounterPi_To_Stm32 = 5;
void Clear_Control_Disable_Counter(void)
{
	xCounterPi_To_Stm32 = 0;
}

bool Get_Control_Disable_Counter_State(void)
{
	bool ret;
	if(xCounterPi_To_Stm32 >=5){
		ret = false;
	}else{
		ret = true;
	}
	return ret;
}

void Adder_Control_Disable_Counter(void)
{
	if(xCounterPi_To_Stm32>=5){
		xCounterPi_To_Stm32 = 5;
	}else{
		xCounterPi_To_Stm32++;
	}
}

//void Control_0(void)
//{
//	char string[20];
//	uint8_t len_str;
////	static int count1 = 0;
////	static int count2 = 0;
//	
//	len_str=sprintf(string,"pwm_left=%d\r\n",pwm_left);
//	HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//	len_str=sprintf(string,"pwm_right=%d\r\n",pwm_right);
//	HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//	
//			if(control_data.navi_angle<10 && control_data.navi_distance<1000)//���Ĳ����Ƿ�����
//			{
//				if(control_data.navi_distance>40 || control_data.navi_distance<-40)//���볬��
//				{
////					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"a", 1, 0x100);
//					
//					if(control_data.navi_angle>0 && pwm_right>1422 && pwm_right<1459)//��ת
//					{
////						//����٣��Ҽ���
//						pwm_left += 3;
//						pwm_right += 3;
////						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ���£�ǰ
////						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
////						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ���£���
////						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
////						count1++;
////						if(count1==1)
////						{
////							len_str=sprintf(string,"pwm1=%d\r\n",test_pwm1);
////							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
////							len_str=sprintf(string,"pwm5=%d\r\n",test_pwm5);
////							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
////							count1=0;
////						}
//						return;
//					}
//					if(control_data.navi_angle<0 && pwm_left>1425 && pwm_left<1461)//��ת
//					{
//						//�Ҽ��٣������
//						pwm_left -= 3;
//						pwm_right -= 3;
////						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ���£�ǰ
////						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
////						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ���£���
////						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
////						count2++;
////						if(count2==1)
////						{
////							len_str=sprintf(string,"pwm1=%d\r\n",test_pwm1);
////							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
////							len_str=sprintf(string,"pwm5=%d\r\n",test_pwm5);
////							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
////							count2=0;
////						}
//						return;
//					}
//				}
//				else
//				{
//						pwm_right =1439;
//						pwm_left =1561;
////						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ���£�ǰ
////						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
////						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ���£���
////						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"90", 2, 0x100);
//					HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
//					return;
//				}
//			}
//			return;
//}

void Control_t(void)
{
	char string[20];
	uint8_t len_str;
	static int count1 = 0;
	static int count2 = 0;
	static int count3 = 0;
	static bool i = 1;
	
			if(control_data.navi_angle<580 && control_data.navi_distance<1000)//���Ĳ����Ƿ�����
			{
				if(control_data.navi_distance>5 || control_data.navi_distance<-5)//���볬��
				{
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"a", 1, 0x100);
					
					if(control_data.navi_angle>0)// && pwm_right>1422 && pwm_right<1459)//��ת
					{
						int t1 = Position_PID_Cal(0,control_data.navi_angle,&T_A_pid);
						int t2 = Position_PID_Cal(0,-control_data.navi_distance,&T_D_pid);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						int t = t1 + t2;
						if(t>T0)t=190;
						if(t<0)t=0;
						count1++;
						if(count1==5)
						{
							len_str=sprintf(string,"left_t1=%d\r\n",t1);
							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
							len_str=sprintf(string,"left_t2=%d\r\n",t2);
							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//							len_str=sprintf(string,"pwm1=%d\r\n",test_pwm1);
//							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//							len_str=sprintf(string,"pwm5=%d\r\n",test_pwm5);
//							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
							count1=0;
						}
//						//��ӽ�ͣת����//����
						uint16_t temp_pwm = pwm_left;
						pwm_left = 1500;
//						pwm_right += 3;
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ���£�ǰ
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ���£���
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
						delay_ms(t);
						pwm_left = temp_pwm;
						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
						delay_ms(T0-t);
						return;
					}
					else if(control_data.navi_angle<0)// && pwm_left>1425 && pwm_left<1461)//��ת
					{
						int t1 = Position_PID_Cal(0,-control_data.navi_angle,&T_A_pid);
						int t2 = Position_PID_Cal(0,control_data.navi_distance,&T_D_pid);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						int t = t1 + t2;
						if(t>T0)t=190;
						if(t<0)t=0;
						count2++;
						if(count2==5)
						{
//							len_str=sprintf(string,"pwm1=%d\r\n",test_pwm1);
//							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
//							len_str=sprintf(string,"pwm5=%d\r\n",test_pwm5);
//							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
							len_str=sprintf(string,"right_t1=%d\r\n",t1);
							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
							len_str=sprintf(string,"right_t2=%d\r\n",t2);
							HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
							count2=0;
						}
						//�ҽӽ�ͣת����//����
//						pwm_left += 3;
						uint16_t temp_pwm = pwm_right;
						pwm_right = 1500;
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ���£�ǰ
//						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ���£���
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
//						__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
						delay_ms(t);
						pwm_right = temp_pwm;
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
						delay_ms(T0-t);
						return;
					}
				}
				else
				{
					Cruise();
					count3++;
					if(count3==5)
					{
						HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"90", 2, 0x100);
						HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
						count3=0;
					}
					return;
				}
			}
			else{
				LED_SW(5, i);
				i=!i;
				ForwardStop();
				return;
			}
			return;
}