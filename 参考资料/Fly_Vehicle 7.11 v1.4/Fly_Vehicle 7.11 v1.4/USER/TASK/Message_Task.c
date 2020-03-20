#include "Message_Task.h"
#include "stdlib.h"

#define PWM_END 7
#define K_END 8

ControlMsg_t control_data;
static PowerMsg_t power_data;
static IMUMsg_t imu_data;
static TempHumiMsg_t temp_humi_data;
PressureMsg_t pressure_data;
uint8_t abcFlag = 0;
extern bool osStart;
enum PID_CHOOSE{DEEP=1,HEAD,PITCH,T}PID_CHOICE;
enum MODE_CHOOSE{MODE0=0,MODEt}mode = 0;
extern enum WORK_STATE{FORWARD=0,ROTATE,STOP}work;

extern uint16_t pwm_left;
extern uint16_t pwm_right;
extern uint16_t pwm_front;
extern uint16_t pwm_back;

extern uint16_t test_pwm;//所有默认值都是1500
extern uint16_t *test_pwm1;
extern uint16_t test_pwm2;
extern uint16_t *test_pwm3;
extern uint16_t test_pwm4;
extern uint16_t *test_pwm5;
extern uint16_t test_pwm6;
extern uint16_t *test_pwm7;
extern uint16_t test_pwm8;
extern int len_string_depth;
extern char string_depth[20];
extern PI_UsartRec_t stm32_pi_Rec;
bool stop_startflag;

extern void PID_KP_ADJUST(PID_t *pid, float kp); //PID 参数kp设置函数
extern void PID_KI_ADJUST(PID_t *pid, float ki); //PID 参数ki设置函数
extern void PID_KD_ADJUST(PID_t *pid, float kd); //PID 参数kd设置函数
extern void MX_FREERTOS_Init(void);

uint8_t enter[]={0x0d,0x0a};
float navi_center_to_line_angle = 0.0;   //中心到色带角度
float navi_center_to_line_distance = 0.0;//中心到色带距离
char abcstring[4];


int16_t front_speed = 0;
char temp_buf[30]={0};
u32 pack_num = 0;
void message_Task_Function(void const * argument)
{
  BaseType_t err;
	portTickType tick = xTaskGetTickCount();
	
	int count = 0;
	char string[20];
	uint8_t len_str;
  /* Infinite loop */
  for(;;)
  {
		#ifdef DEBUG
		//HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin);		 
		#else
 
		xQueuePeek(TempHum_Message_Queue,&temp_humi_data,1);//温湿度
		xQueuePeek(IMU_Message_Queue,&imu_data,1);          //电子罗盘
		xQueuePeek(Pressure_Message_Queue,&pressure_data,1);//压传
		xQueuePeek(Battery_Message_Queue,&power_data,1);    //压传
		
		control_data.heading = imu_data.heading;
		control_data.pitching = imu_data.pitch;
		control_data.rolling = imu_data.roll-1.2;
		control_data.depth = pressure_data.depth;
	  control_data.temperture = pressure_data.Temperature;

 
		Adder_Control_Disable_Counter(); 		
		control_data.Control_Disable = Get_Control_Disable_Counter_State();

		if(Control_Message_Queue!=NULL)//发给控制任务 
		{   
				err=xQueueSend(Control_Message_Queue,&control_data,1);	// 手柄控制
				if(err==errQUEUE_FULL)   	 
				{
						;//printf("队列Key_Queue已满，数据发送失败!\r\n");
				}
		}
		
		if(stm32_pi_Rec.RX_flag){
			if(abcFlag){
				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"word:",5, 0x100);
				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)abcstring,4, 0x100);
				HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)abcstring,4, 0x100);
//				HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
				abcFlag--;
			}
			count++;
			if(count==8){//曾用参数12
				//将control_data.navi_angle转为字符串，然后用串口分别发给树莓派和lora电脑端
				len_str=sprintf(string,"dis=%3.3f\r\n",control_data.navi_distance);//有用！！！！！
				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
				len_str=sprintf(string,"ang=%3.3f\r\n",control_data.navi_angle);
				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
				count = 0;
			}
			stm32_pi_Rec.RX_flag=0;
		}
		
//		static int time = 0;//深度数据返回
//		if(time>10){time = 0;
//			//sprintf(temp_buf,"dis=%.2f,ang = %.2f\r\n",control_data.navi_distance,control_data.navi_angle);
//			sprintf(temp_buf,"rolling=%.2f,depth = %.2f\r\n",control_data.rolling,control_data.depth);
//			SX1278_SendData((uint8_t*)temp_buf,strlen(temp_buf));
//		}else{time ++;}
		#endif
 		vTaskDelayUntil(&tick,100);
	}
}

float p_val = 0.0;
float i_val = 0.0;
float d_val = 0.0;

void SX1278_unpackData(uint8_t *buf,u16 len)
{
	char string[20];
	uint8_t len_str;
	
//	void *p=0;
	int i=0;
//	HAL_UART_Transmit(&SX1278_USART, buf, len ,0x100);
	if(strstr((char*)buf,"in")){//初始化运动//例如in0
//		len_str=sprintf(string,"PWMinUNPACK_11_0=%d\r\n",test_pwm1);
//		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
		
		pwm_right =1451;
		pwm_left =1552;

//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   上下，前
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   上下，后
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
		
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"in", 2, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		
		__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
		stop_startflag=1;
		osStart = 1;
		
		return;
//			len_str=sprintf(string,"PWMinUNPACK_11_1=%d\r\n",test_pwm1);
//			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
	}
	
	if(strstr((char*)buf,"it")){//开树莓派中断
//					len_str=sprintf(string,"PWMinUNPACK_11_0=%d\r\n",test_pwm1);
//			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
		
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"it", 2, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
		return;
//			len_str=sprintf(string,"PWMinUNPACK_11_1=%d\r\n",test_pwm1);
//			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
	}
	
	if(strstr((char*)buf,"pid")){//格式例如pid1
		if(strstr((char*)buf,"1"))PID_CHOICE = DEEP;
		if(strstr((char*)buf,"2"))PID_CHOICE = HEAD;
		if(strstr((char*)buf,"3"))PID_CHOICE = PITCH;
		if(strstr((char*)buf,"4"))PID_CHOICE = T;
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"pid", 3, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		
		return;
	}
	
	if(strstr((char*)buf,"kp")){//格式例如kp111.11
		float kp=0;
		char str[10];
		memcpy(str, buf+2*sizeof(char), 6*sizeof(char));
		kp = atof(str);
		switch (PID_CHOICE)
		{
			case DEEP:PID_KP_ADJUST(&Deep_pid, kp);break;
			case HEAD:PID_KP_ADJUST(&Heading_pid, kp);break;
			case PITCH:PID_KP_ADJUST(&Pitch_pid, kp);break;
			default:break;
		}
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"kp", 2, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		
		return;
	}
	
	if(strstr((char*)buf,"ki")){//格式例如ki111.11
		float ki=0;
		char str[10];
		memcpy(str, buf+2*sizeof(char), 6*sizeof(char));
		ki = atof(str);
		switch (PID_CHOICE)
		{
			case DEEP:PID_KI_ADJUST(&Deep_pid, ki);break;
			case HEAD:PID_KI_ADJUST(&Heading_pid, ki);break;
			case PITCH:PID_KI_ADJUST(&Pitch_pid, ki);break;
			default:break;
		}
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"ki", 2, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		
		return;
	}
		
	if(strstr((char*)buf,"kd")){//格式例如kd111.11
		float kd=0;
		char str[10];
		memcpy(str, buf+2*sizeof(char), 6*sizeof(char));
		kd = atof(str);
		switch (PID_CHOICE)
		{
			case DEEP:PID_KD_ADJUST(&Deep_pid, kd);break;
			case HEAD:PID_KD_ADJUST(&Heading_pid, kd);break;
			case PITCH:PID_KD_ADJUST(&Pitch_pid, kd);break;
			default:break;
		}
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"kd", 2, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		
		return;
	}
	
	if(strstr((char*)buf,"off")){//终止运动
		__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);//关中断
		
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1500);//1   上下，前
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1500);// 
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1500);//2   前后，右
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1500);//
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1500);//3   上下，后
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1500);// 
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1500);//4   前后，左
		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1500);//
		HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"off", 3, 0x100);
		HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		return;
	}
	
	if(strstr((char*)buf,"ver")){//垂向控制
		pwm_front=0;
		for(i=3;i<7;i++)
		{
			pwm_front=10*pwm_front+(buf[i]-'0');
		}
		pwm_back=pwm_front;
		if(pwm_front>1300 && pwm_front<1700){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,*test_pwm1);//1   上下，前
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,*test_pwm1);// 
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,*test_pwm5);//3   上下，后
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,*test_pwm5);// 
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"ver", 3, 0x100);
			HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
			
//			len_str=sprintf(string,"PWMinUNPACK%d\r\n",*test_pwm1);
//			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
		}
		return;
	}
	
	if(strstr((char*)buf,"front")){//前电机控制
		pwm_front=0;
		for(i=5;i<9;i++)
		{
			pwm_front=10*pwm_front+(buf[i]-'0');
		}
		if(pwm_front>1300 && pwm_front<1700){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,*test_pwm1);//1   上下，前
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,*test_pwm1);// 
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"front", 5, 0x100);
			HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		}
		return;
	}
	
	if(strstr((char*)buf,"right")){//右电机控制
		pwm_right=0;
		for(i=5;i<9;i++)
		{
			pwm_right=10*pwm_right+(buf[i]-'0');
		}
		if(pwm_right>1300 && pwm_right<1700){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);// 
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"right", 5, 0x100);
			HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		}
		return;
	}
	
	if(strstr((char*)buf,"back")){//后电机控制
		pwm_back=0;
		for(i=4;i<8;i++)
		{
			pwm_back=10*pwm_back+(buf[i]-'0');
		}
		if(pwm_back>1300 && pwm_back<1700){
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,*test_pwm5);//3   上下，后
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,*test_pwm5);// 
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"back", 4, 0x100);
			HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		}
		return;
	}
	
  if(strstr((char*)buf,"ward")){//前后控制
		pwm_right=0;
		for(i=4;i<8;i++)
		{
			pwm_right=10*pwm_right+(buf[i]-'0');
		}
		pwm_left=pwm_right;
		if(pwm_right>1300 && pwm_right<1700){
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
			__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"ward", 4, 0x100);
			HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		}
		return;
	}
	
	if(strstr((char*)buf,"left")){//左电机控制
		pwm_left=0;
		for(i=4;i<8;i++)
		{
			pwm_left=10*pwm_left+(buf[i]-'0');
		}
		if(pwm_left>1300 && pwm_left<1700){
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
			HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"left", 4, 0x100);
			HAL_UART_Transmit(&SX1278_USART, enter, 2, 0x100);
		}
		return;
	}
	
	return;
}

void Pi_Stm32_unpackData(uint8_t *buf,u16 len)
{
	u8 step = 0;
	for(int i =0;i<len;i++)
	{
		switch(step)
		{
			case 0:                      //检测包头
				if(buf[i]==0xFE){
					step = 1;
				}
				break;
			case 1:
				if(buf[i]<=(len-i+1)){
					if(buf[buf[i]-1+(i-1)] == 0x0a){//检测包长
						step = 2;
					}					
				}else{
           step = 0;
				}
				break;
			case 2:i++;
		   switch(buf[i])//0x25是字母，0x26是STOP,0x27是ROTATE
        {
            case MSG_NAVI_ID://用于识别树莓派发来的帧的数据的数据头
								memcpy((void*)&navi_center_to_line_angle,(void*)&(buf[i+1]),4);//
						    memcpy((void*)&navi_center_to_line_distance,(void*)&(buf[i+5]),4);
//								memcpy((void*)&navi_center_to_line_angle2,(void*)&(buf[i+9]),4);//
//						    memcpy((void*)&navi_center_to_line_distance2,(void*)&(buf[i+13]),4);
//								if(fabs(navi_center_to_line_angle1)<fabs(navi_center_to_line_angle2)){
//									control_data.navi_distance = navi_center_to_line_distance1;
//									control_data.navi_angle    = navi_center_to_line_angle1;
//								}
//								else{
									control_data.navi_distance = navi_center_to_line_distance;
									control_data.navi_angle    = navi_center_to_line_angle*180/3.1416;
//								}
                Clear_Control_Disable_Counter();
                break;
						case 0x25:
								memcpy((void*)abcstring,(void*)&(buf[i+1]),4);
//								__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);//一收到字母就关中断
								abcFlag = 10;
								work = ROTATE;
								break;
						case 0x26:
								work = STOP;
								break;
            default:break;
        }
				step = 0;
				break;
			 	
		}
		
	}
     
}
//注释
//typedef struct{
////	int16_t channel[4];
//	float temperture;//一个float4个字节
//	float depth;
//	float heading;              //航向,z
//	float pitching;             //前后偏角,y
//	float rolling;              //左右偏角,x
//	ControlState_t depth_state;
//	ControlState_t heading_state;
//	bool  Control_Disable;
//	float navi_angle;
//	float navi_distance;
//}ControlMsg_t;
