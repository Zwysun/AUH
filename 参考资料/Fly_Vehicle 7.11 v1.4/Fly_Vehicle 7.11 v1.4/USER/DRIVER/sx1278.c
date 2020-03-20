#include "sx1278.h"
#include "pwm.h"
////����ͷʹ��
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

extern uint16_t test_pwm;//����Ĭ��ֵ����1500
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
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,1490);//  ˮƽ  ���ƽ���
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,1490);
//	
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,1490);
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,1490);
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1490);//  ˮƽ  ���ƽ���
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
  * @brief   Loraģ��
  *
  *
  ******************************************************************************
  * @attention
  *	Loraģ�� ����3 Ĭ�ϲ�����: 9600
	* E32-170T30D 170MHz 0x0000 0x28 2.4kbps 9600 8N1 1W
  *	ģ�������ֹ���ģʽ�������� M0�� M1 ���ã���ϸ������±���ʾ��
	*	ģʽ��0-3��       M0      M1                  ģʽ����                                               ��ע
	*	0 һ��ģʽ        0       0            ���ڴ򿪣����ߴ򿪣�͸������                           ���շ�������ģʽ 0�� 1
	*
	*	1 ����ģʽ        1       0						 ���ڴ򿪣����ߴ򿪣�                                  ���շ�������ģʽ 0
	*	                                       ��ģʽ 0 Ψһ�������ݰ�����ǰ���Զ����ӻ����룬       ���շ�������ģʽ 1
	*                                        �������ܻ��ѹ�����ģʽ 2 �Ľ��շ�
  *
	*	2 ʡ��ģʽ        0       1            ���ڽ��չرգ����ߴ��ڿ��л���ģʽ���յ�������          ���䷽����ģʽ 1
	* 																			 �ݺ󣬴򿪴��ڷ������ݡ�                               ��ģʽ�²��ܷ���
	* 
	* 
	* 3 ����ģʽ        1        1           ģ��������ߣ����Խ��ղ�����������                     
	��������������
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
	
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ����
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,test_pwm3);//2   ǰ��
//	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,test_pwm4);//
//		
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ����
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);// 
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,test_pwm7);//4   ǰ��
//	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,test_pwm8);//
		
	HAL_Delay(500);
		
	
  HAL_UART_Receive_DMA(&SX1278_USART, SX1278_Rec.RX_pData, sizeof(SX1278_Rec.RX_pData));//����dma����  
	__HAL_UART_ENABLE_IT(&SX1278_USART, UART_IT_IDLE); //�����ж�
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
void HAL_UART3_Receive_IDLE(UART_HandleTypeDef *huart)//IDLE�����ж�
{
		char string[20];
		uint8_t len_str;
	
//		pwm_right =1445;
//		pwm_left =1447;

////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,test_pwm1);//1   ���£�ǰ
////		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,test_pwm2);// 
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,test_pwm5);//3   ���£���
////		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,test_pwm6);//
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,*test_pwm3);//2   ǰ����
//		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,*test_pwm3);//
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,*test_pwm7);//4   ǰ����
//		__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_4,*test_pwm7);//
	
	//���ٷ��ͻ����Ӳ�����ܣ�������޷�����
//	static bool flag0 = 1;
//  HAL_UART_Receive_IT(huart,aRxBuffer,1);          // Enable the USART5 Interrupt
	
		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  
        SX1278_Rec.RX_Size =  RX_LEN - temp;   
        SX1278_Rec.RX_flag=1;  
 			  SX1278_unpackData(SX1278_Rec.RX_pData, SX1278_Rec.RX_Size);//�����������ﴦ����յ���lora����
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
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_SET);//����ģʽ
	delay_ms(500);
	HAL_UART_Transmit(&SX1278_USART, cmd, 6, 0x100);//ģ��᷵�ص�ǰ���ò���
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
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port,LORA_M1_Pin,GPIO_PIN_RESET);//һ��ģʽ
	HAL_Delay(500);
	
	while(1)
	{

		HAL_Delay(100);

	}

}