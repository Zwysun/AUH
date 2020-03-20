#include "sx1278.h"
#include "pwm.h"
////摄像头使用
//void PWM_ServoInit(void)
//{
////	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
////	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,SERVO_INIT);
//}
//int GetAngle = 0;
//void SetCameraAngle(int value)
//{
////	GetAngle = value;
////	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,(GetAngle-50)*6+SERVO_INIT);
//}

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

extern void LED_SW(u8 num,u8 sta);
extern void SX1278_GetWorkingParam(void);

//void PWM_Init(void)
//{
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1490);
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1490);
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1490);//  水平  右推进器
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1490);
//	
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1490);
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1490);
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1490);//  水平  左推进器
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1490);
//	
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//	
//	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
//	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
//	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
//}


/**
  ******************************************************************************
  * @file    sx1278.c
  * @author  LiuYang
  * @brief   Lora模块
  *
  *
  ******************************************************************************
  * @attention
  *	Lora模块 串口3 默认波特率: 9600
	* E32-170T30D 170MHz 0x0000 0x28 2.4kbps 9600 8N1 1W
  *	模块有四种工作模式，由引脚 M0、 M1 设置；详细情况如下表所示：
	*	模式（0-3）       M0      M1                  模式介绍                                               备注
	*	0 一般模式        0       0            串口打开，无线打开，透明传输                           接收方必须是模式 0、 1
	*
	*	1 唤醒模式        1       0						 串口打开，无线打开；                                  接收方可以是模式 0
	*	                                       和模式 0 唯一区别：数据包发射前，自动增加唤醒码，       接收方可以是模式 1
	*                                        这样才能唤醒工作在模式 2 的接收方
  *
	*	2 省电模式        0       1            串口接收关闭，无线处于空中唤醒模式，收到无线数          发射方必须模式 1
	* 																			 据后，打开串口发出数据。                               该模式下不能发射
	* 
	* 
	* 3 休眠模式        1        1           模块进入休眠，可以接收参数设置命令                     
	详见工作参数详解
  *
  ******************************************************************************
  */
SX1278_Parse_State_t sx1278_parse;
SX1278_Info_t        sx1278_info;
UsartRec_t SX1278_Rec;

void SX1278_Init(void)
{
	SX1278_GetWorkingParam();
//	PWM_Init();
	
//	HAL_Delay(3000);
	
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   上下
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   前后
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);//
//		
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   上下
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);// 
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   前后
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);//
		
	HAL_Delay(500);
		
	
  HAL_UART_Receive_DMA(&SX1278_USART, SX1278_Rec.RX_pData, sizeof(SX1278_Rec.RX_pData));//开启dma传输  
	__HAL_UART_ENABLE_IT(&SX1278_USART, UART_IT_IDLE); //启动中断
}

static void unpackData(uint8_t *buf,u16 len)
{
	switch(sx1278_parse)
	{
		case SX1278_GET_WORK_STA:
			if(buf[0]==0xC0){
				sx1278_info.address = buf[1]<<8|buf[2];
				sx1278_info.baud = buf[1]<<8|buf[2];
			}
			break;
		case SX1278_GET_VERSION:break;
		case SX1278_RESET:break;
		default:break;
	}
}


static uint32_t temp;
void HAL_UART3_Receive_IDLE(UART_HandleTypeDef *huart)//IDLE空闲中断
{
		char string[20];
		uint8_t len_str;
	
//		pwm_right =1445;
//		pwm_left =1447;

////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   上下，前
////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   上下，后
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   前后，右
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   前后，左
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
	
	//高速发送会阻断硬件接受，软件上无法改善
//	static bool flag0 = 1;
//  HAL_UART_Receive_IT(huart,aRxBuffer,1);          // Enable the USART5 Interrupt
	
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  
        SX1278_Rec.RX_Size =  RX_LEN - temp;   
        SX1278_Rec.RX_flag=1;  
 			  SX1278_unpackData(SX1278_Rec.RX_pData, SX1278_Rec.RX_Size);//！！！在这里处理接收到的lora数据
		   //	memset(SX1278_Rec.RX_pData,'\0',RX_LEN);
        HAL_UART_Receive_DMA(huart, SX1278_Rec.RX_pData, sizeof(SX1278_Rec.RX_pData));  
    }
		
//		if(flag0){LED_SW(4,flag0);LED_SW(5,flag0);flag0=0;}
//		else{LED_SW(4,flag0);LED_SW(5,flag0);flag0=1;}
//					len_str=sprintf(string,"PWMinIDLE%d\r\n",test_pwm1);
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)string, len_str, 0x100);
}

void SX1278_GetWorkingParam(void)
{
	u8 cmd[6] = {0xC0, 0x00, 0x17, 0X1B, 0X2a, 0X47};
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_SET);//休眠模式
	delay_ms(500);
	HAL_UART_Transmit(&SX1278_USART, cmd, 6, 0x100);//模块会返回当前配置参数
	sx1278_parse = SX1278_GET_WORK_STA;
}
void SX1278_GetData()
{
	 
}

void SX1278_SendData(uint8_t buf[],u16 len)
{
	HAL_UART_Transmit(&SX1278_USART, buf, len ,0x100);
}

void SX1278_Test_Demo(void)
{
	SX1278_Init();
	HAL_Delay(500);
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);//一般模式
	HAL_Delay(500);
	
	while(1)
	{

		HAL_Delay(100);

	}

}