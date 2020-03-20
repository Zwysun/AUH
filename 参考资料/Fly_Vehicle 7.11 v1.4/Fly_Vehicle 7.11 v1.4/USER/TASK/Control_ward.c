#include "Control_ward.h"
#include "control.h"

enum WORK_STATE{FORWARD=0,ROTATE,STOP}work;
extern ControlMsg_t control_data;
extern enum MODE_CHOOSE{MODE0=0,MODEt}mode;
extern uint16_t test_pwm;//����Ĭ��ֵ����1500
extern uint16_t *test_pwm1;
extern uint16_t test_pwm2;
extern uint16_t *test_pwm3;
extern uint16_t test_pwm4;
extern uint16_t *test_pwm5;
extern uint16_t test_pwm6;
extern uint16_t *test_pwm7;
extern uint16_t test_pwm8;
extern uint16_t pwm_left;
extern uint16_t pwm_right;
extern uint16_t pwm_front;
extern uint16_t pwm_back;
extern void Control_0(void);
extern void Control_t(void);
extern void ForwardStop(void);
extern void LED_SW(u8 num,u8 sta);
extern void Cruise(void);
extern float front_back_rocker;                        //��� - ǰ��ҡ��λ��
extern float left_right_rocker;                        //��� - ����ҡ��λ��
extern float up_down_rocker;                           //�ұ� - ����ҡ��λ��
extern float left_right_adjust_rock;                   //�ұ� - ����ҡ��λ��
extern bool osStart;
extern bool stop_startflag;

extern int16_t test_motor;
extern float heading_pid_tar;
extern float heading_pid_out;
extern float pitching_pid_out;
extern float deepth_pid_out;
extern float deep_tar;
extern float navi_position;
extern char abcstring[4];
extern uint8_t enter[];
//����ʹ��
extern u8 pitch_en;
extern u8 deep_en;
extern u8 roll_en;

void Rotate(int angle);
void GoBackward(int delay);
	
void control_Ward_Function(void const * argument)
{
 
  portTickType tick = xTaskGetTickCount();

	int count = 5;
	int count_stop = 1;//�����޸ĳ�ֵ
	int count_IT = 0;//�����ε���ݮ���жϴ���
	char string[20];
	uint8_t len_str;
	work = FORWARD;
	static bool in = 1;
	static bool inn = 1;
	
	while(1)
	{
		#ifdef DEBUG
		
		#else
		if(osStart){//if(osStart)
			if(xQueueReceive(Control_Message_Queue,&(control_data),portMAX_DELAY))
			{
				//ˮƽ PID ����
	//			count--;
	//			if(count==0){
	//				len_str=sprintf(string,"CONTROL_ward\r\n");
	//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
	//			}
				
				
				switch(work){//switch(work)
					case FORWARD:{//
						Cruise();
						Control_t();
						break;
					}
					case STOP:{//case STOP:
						LED_SW(6,in);LED_SW(7,in);
						in=!in;
						if(stop_startflag){
							GoBackward(2100);//����2s
							while(work != ROTATE){
								ForwardStop();
							}
							__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
							Rotate(-90+control_data.navi_angle);//������תΪ��
							work = FORWARD;
							//osDelay(1600);
							__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
						}//if(stop_startflag)
						break;
					}//case STOP:
					default:break;
				}//switch(work)
				
				
//				switch(work){//switch(work)
//					case FORWARD:{//
//						Cruise();
//						Control_t();
//						break;
//					}
//					case ROTATE:{//case STOP:
//						if(stop_startflag){
//							LED_SW(6,in);LED_SW(7,in);
//							in=!in;
//							GoBackward(2100);//����2s
//							osDelay(900);//!!!!!ʶ����ĸ��ʱ��
//							__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
//							Rotate(-90+control_data.navi_angle);//������תΪ��
//							work = FORWARD;
//							__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
//							LED_SW(8, inn);
//							inn=!inn;
//						}//if(stop_startflag)
//						break;
//					}//case STOP:
//					default:break;
//				}//switch(work)
				
				
				
				
//				switch(work){//switch(work)
//					case FORWARD:{//
//						Cruise();
//						Control_t();
//						break;
//					}
//					case STOP:{//case STOP:
//						if(stop_startflag){
//							GoBackward(2100);//����2s
//							switch(count_stop){
//								case 1:{
//									__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
//									count_stop++;
//									work = FORWARD;
//									osDelay(200);
//									break;
//								}
//								case 2:{
//									__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
//									Rotate(-90+control_data.navi_angle);//������תΪ��
//									count_stop++;
//									work = FORWARD;
//									ForwardStop();//��rotate�Ĳ���
//									osDelay(5000);
//									break;
//								}
//								case 3:{
//									while(work != ROTATE){
//										ForwardStop();
//									}
//									__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
//									Rotate(-120+control_data.navi_angle);//������תΪ��
//									count_stop++;
//									work = FORWARD;
//									//osDelay(1600);
//									break;
//								}
//								case 4:{
//									deep_tar = 20;
//									osDelay(10000);
//									HAL_UART_Transmit(&SX1278_USART, (uint8_t*)abcstring,4, 0x100);
//									HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
//									HAL_UART_Transmit(&SX1278_USART, (uint8_t*)abcstring,4, 0x100);
//									HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
//									break;
//								}
//								default:break;
//							}
//							__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
//							break;
//						}//if(stop_startflag)
//					}//case STOP:
//					default:break;
//				}//switch(work)


			}//if(xQueueReceive(Control_Message_Queue,&(control_data),portMAX_DELAY))
		}//if(osStart)
		#endif
	}//while(1)
}

void Rotate(int angle)
{
	int t = angle*63/3;  //ԭֵ69
	static bool i=1;
	if(t>0){
		//��ת
		pwm_left = 1548;
		pwm_right = 1600;
//		pwm_left = 1452;
//		pwm_right = 1452;
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
		osDelay(t);
	}
	else{
		pwm_left = 1420;
		pwm_right = 1451;
//		pwm_left = 1452;
//		pwm_right = 1452;
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
		osDelay(-t);
	}
	ForwardStop();
	LED_SW(4, i);
	i=!i;
}

void GoBackward(int delay)
{
	pwm_left = 1332;
	pwm_right = 1615;
//	pwm_left = 1452;
//	pwm_right = 1452;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
	osDelay(delay);
}

