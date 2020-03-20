#include "board.h"
QueueHandle_t IMU_Message_Queue = NULL;
QueueHandle_t Control_Message_Queue = NULL;  
QueueHandle_t Command_Message_Queue = NULL;  
QueueHandle_t Pressure_Message_Queue = NULL; 
QueueHandle_t TempHum_Message_Queue = NULL;
QueueHandle_t Battery_Message_Queue = NULL; 

uint8_t RxBuffer2[8] = "Test";
__IO uint8_t RxCounter2 = 0x00; 
uint8_t packed_data[20];
extern uint16_t test_pwm;//����Ĭ��ֵ����1500
extern uint16_t test_pwm1;
extern uint16_t test_pwm2;
extern uint16_t test_pwm3;
extern uint16_t test_pwm4;
extern uint16_t test_pwm5;
extern uint16_t test_pwm6;
extern uint16_t test_pwm7;
extern uint16_t test_pwm8;
extern uint8_t lora_ch;
extern void SX1278_GetWorkingParam(void);
void CopeSerial2Data(unsigned char ucData);

/**
  * @brief  
  * @param  
  * @note  
  */
void delay_ms(int nms)   
{
	if(xTaskGetSchedulerState()==taskSCHEDULER_NOT_STARTED)
	{
		HAL_Delay(nms);
	}else
	{
		osDelay(nms);
	}
}
/**
  * @brief  ������Ϣ���С��ź�������ʱ������ 
  * @param  
  * @note   1. �ڸ�������ִ��ǰ���� 2. ע���ֹ�жϴ�ϴ���
  */	
void Task_Queue_Semaphore_Timers_Create(void)
{
	do{
		Pressure_Message_Queue = xQueueCreate(1, sizeof(PressureMsg_t));
	}while(Pressure_Message_Queue==NULL);
	
	do{
		IMU_Message_Queue = xQueueCreate(1, sizeof(IMUMsg_t));
	}while(IMU_Message_Queue==NULL);
	
	do{
		Control_Message_Queue = xQueueCreate(3, sizeof(ControlMsg_t));
	}while(Control_Message_Queue==NULL);
	
	do{
		TempHum_Message_Queue = xQueueCreate(1, sizeof(TempHumiMsg_t));
	}while(TempHum_Message_Queue==NULL);
	
	do{
			Battery_Message_Queue = xQueueCreate(1, sizeof(PowerMsg_t));	
	}while(Battery_Message_Queue==NULL);
	
}
/**
  * @brief  
  * @param  
  * @note  
  */	
 
void BSP_Init(void)
{	
	delay_ms(100);
  POWER_KEY(1);//���� - ����
	PWM_Init();//һ������Ҫ��CCR=1490������������������˺������ֵΪ1500.
	
	Batt_Init();
	
//	POWER_KEY(1);//���� - ����
	
	SX1278_Init();
	
	JY901_Init();
	HP206C_Init();
	
	SHT35_Init();
	PID_Init();
  Stm32_Pi_Init();
	
	SX1278_GetWorkingParam();
	
	HAL_Delay(500);
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port,LORA_M0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);//һ��ģʽ
	HAL_Delay(500);
	
	//����ʹ��
//	HP206C_Test_Demo();  //ѹ������demo
//	SW_Test_Demo();      //��������demo
//  LED_Test_Demo();     //led ��demo  OK
//	PWM_Test_Demo();     // Tim1 ch2 �и���
//	SX1278_Test_Demo();  //Lora ģ��
//	Batt_Test_Demo();    //����demo
//  SHT35_TestDemo();    //SHT35��ʪ��demo  
//	JY901_Test_Demo();   //��������demo
//	PI_STM32_Test_Demo();//��ݮ�ɴ���demo

}
/**
  * @brief  
  * @param  
  * @note  
  */
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)//JY901
	{
		HAL_UART1_Receive_IDLE(huart);
	}
	if(huart->Instance == USART2)//��ݮ��
	{
		HAL_PI_To_UART_Receive_IDLE(huart);
	}
	if(huart->Instance == USART3)//Loraģ�� SX1278
	{
	  HAL_UART3_Receive_IDLE(huart);
//		__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE);
//		if(__HAL_UART_GET_FLAG(huart, USART_IT_TXE) != RESET)	   
//		{	
//			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
//		}
//		if(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE) != RESET)                   
//		{ 
//			CopeSerial2Data(lora_ch); //��������	     
//		}
	}
	if(huart->Instance == UART4)//
	{
		
	}
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//  RXNE�Ļص����������������������
//{
//  /* Prevent unused argument(s) compilation warning */
//  
//		int i=0;
//		uint8_t enter[]={0x0d,0x0a};
//		HAL_UART_Transmit(&SX1278_USART, RxBuffer2, 7 ,0x100);//huart->RxXferSize
//		switch(RxBuffer2[0])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
//		{
//			case 'a'://11
//			{
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"11", 2, 0x100);
//				__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
//				break;
//			}  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����

//			case 'b'://99
//			{
//				__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1490);//1   ����
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1490);// 
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1490);//2   ǰ��
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1490);//
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1490);//3   ����
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1490);// 
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1490);//4   ǰ��
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1490);//
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"99", 2, 0x100);
//				break;
//			}
//			case 'c'://13
//			{
//				test_pwm1=0;
//				for(i=1;i<5;i++)
//				{
//					test_pwm1=10*test_pwm1+(RxBuffer2[i]-'0');
//				}
//				test_pwm2=test_pwm1;
//				test_pwm5=test_pwm1;
//				test_pwm6=test_pwm1;
//				if(test_pwm1>1390 && test_pwm1<1590){
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ����
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ����
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"13", 2, 0x100);
//				}
//				break;
//			}
//			case 'd'://2
//			{
//				test_pwm3=0;
//				for(i=1;i<5;i++)
//				{
//					test_pwm3=10*test_pwm3+(RxBuffer2[i]-'0');
//				}
//				test_pwm4=test_pwm3;
//				if(test_pwm3>1390 && test_pwm3<1590){
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"2", 1, 0x100);
//				}
//				break;
//			}
//			case 'e'://24
//			{
//				test_pwm3=0;
//				for(i=1;i<5;i++)
//				{
//					test_pwm3=10*test_pwm3+(RxBuffer2[i]-'0');
//				}
//				test_pwm4=test_pwm3;
//				test_pwm7=test_pwm3;
//				test_pwm8=test_pwm3;
//				if(test_pwm3>1390 && test_pwm3<1590){
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);// 
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"24", 2, 0x100);
//				}
//				break;
//			}
//			case 'f'://4
//			{
//				test_pwm7=0;
//				for(i=1;i<5;i++)//RxCounter2-2
//				{
//					test_pwm7=10*test_pwm7+(RxBuffer2[i]-'0');
//				}
//				test_pwm8=test_pwm7;
//				if(test_pwm7>1390 && test_pwm7<1590){
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"4", 1, 0x100);
//				}
//				break;
//			}
//		}
//	
//	HAL_UART_Receive_IT(&huart2, (uint8_t *)RxBuffer2, 7);/* NOTE: This function Should not be modified, when the callback is needed,
//           the HAL_UART_RxCpltCallback could be implemented in the user file   */
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static u8 sw = 1;
	if(HAL_GPIO_ReadPin(POWER_KEY_STATE_GPIO_Port,POWER_KEY_STATE_Pin) == GPIO_PIN_RESET)
	{
		if(sw == 0)
		{
			POWER_KEY(1);//���� - ����
			sw = 1;
		}else{
			 POWER_KEY(0);//�ػ�	
			sw = 0;			
		}
		
	}
	
}



 //CopeSerialDataΪ����3�жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
//void CopeSerial2Data(unsigned char ucData)
//{
//	
//	RxBuffer2[RxCounter2++]=ucData;	//���յ������ݴ��뻺������
//	if (RxBuffer2[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
//	{
//		RxCounter2=0;
//		return;
//	}
//	if (RxCounter2<7) {return;}//���ݲ���11�����򷵻�
//	else
//	{
//		int i=0;
//		uint8_t enter[]={0x0d,0x0a};
//		HAL_UART_Transmit(&SX1278_USART, RxBuffer2, RxCounter2 ,0x100);
//		switch(RxBuffer2[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
//		{
//			case 0x51://11
//			{
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"11", 2, 0x100);
//				__HAL_UART_ENABLE_IT(&PI_USART, UART_IT_IDLE);
//				break;
//			}  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����

//			case 0x52://99
//			{
//				__HAL_UART_DISABLE_IT(&PI_USART, UART_IT_IDLE);
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1490);//1   ����
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,1490);// 
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1490);//2   ǰ��
//				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1490);//
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1490);//3   ����
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1490);// 
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1490);//4   ǰ��
//				__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,1490);//
//				HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"99", 2, 0x100);
//				break;
//			}
//			case 0x53://13
//			{
//				test_pwm1=0;
//				for(i=2;i<RxCounter2-2;i++)
//				{
//					test_pwm1=10*test_pwm1+(RxBuffer2[i]-'0');
//				}
//				test_pwm2=test_pwm1;
//				test_pwm5=test_pwm1;
//				test_pwm6=test_pwm1;
//				if(test_pwm1>1390 && test_pwm1<1590){
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ����
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ����
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"13", 2, 0x100);
//				}
//				break;
//			}
//			case 0x54://2
//			{
//				test_pwm3=0;
//				for(i=2;i<RxCounter2-2;i++)
//				{
//					test_pwm3=10*test_pwm3+(RxBuffer2[i]-'0');
//				}
//				test_pwm4=test_pwm3;
//				if(test_pwm3>1390 && test_pwm3<1590){
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"2", 1, 0x100);
//				}
//				break;
//			}
//			case 0x55://24
//			{
//				test_pwm3=0;
//				for(i=2;i<RxCounter2-2;i++)
//				{
//					test_pwm3=10*test_pwm3+(RxBuffer2[i]-'0');
//				}
//				test_pwm4=test_pwm3;
//				test_pwm7=test_pwm3;
//				test_pwm8=test_pwm3;
//				if(test_pwm3>1390 && test_pwm3<1590){
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);// 
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"24", 2, 0x100);
//				}
//				break;
//			}
//			case 0x56://4
//			{
//				test_pwm7=0;
//				for(i=2;i<RxCounter2-2;i++)
//				{
//					test_pwm7=10*test_pwm7+(RxBuffer2[i]-'0');
//				}
//				test_pwm8=test_pwm7;
//				if(test_pwm7>1390 && test_pwm7<1590){
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   ǰ��
//					__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);// 
//					HAL_UART_Transmit(&SX1278_USART, (uint8_t*)"4", 1, 0x100);
//				}
//				break;
//			}
//		}
//		RxCounter2=0;//��ջ�����
//	}
//}