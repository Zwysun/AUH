#include "jy901.h"
/**
  ******************************************************************************
  * @file    jy901.c
  * @author  LiuYang
  * @brief   惯导模块
  *
  *
  ******************************************************************************
  * @attention
  *	JY901 串口1 默认波特率: 9600
	* 
  *	
  *
  ******************************************************************************
  */	

struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;

UsartRec_t JY901_Rec;
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;
	}
}

void JY901_Init(void)
{
//	HAL_UART_Receive_IT(&JY901_USART, (u8 *)aRxBuffer, RXBUFFERSIZE);
		HAL_UART_Receive_DMA(&JY901_USART, JY901_Rec.RX_pData, RX_LEN);  
	__HAL_UART_ENABLE_IT(&JY901_USART, UART_IT_IDLE); 
	//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	
}
static void unpackData(uint8_t *buf,u16 len)
{
	for(int i = 0;i<len;i++)
	{
			CopeSerialData(buf[i]);
	}
	JY901_Rec.RX_flag=0;
}
uint32_t temp;
void HAL_UART1_Receive_IDLE(UART_HandleTypeDef *huart)
{
//	
//  HAL_UART_Receive_IT(huart,aRxBuffer,1);          // Enable the USART5 Interrupt
  if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
    {   
        __HAL_UART_CLEAR_IDLEFLAG(huart);  
        HAL_UART_DMAStop(huart);  
        temp = huart->hdmarx->Instance->CNDTR;  
        JY901_Rec.RX_Size =  RX_LEN - temp;   
        JY901_Rec.RX_flag=1;  
			  unpackData(JY901_Rec.RX_pData,JY901_Rec.RX_Size);
        HAL_UART_Receive_DMA(&huart1,JY901_Rec.RX_pData,RX_LEN);  
    }
}
void JY901_GetData(IMUMsg_t* imu)
{
	imu->roll = stcAngle.Angle[0]/32768.f*180;
	imu->pitch = stcAngle.Angle[1]/32768.f*180;
	imu->heading = stcAngle.Angle[2]/32768.f*180;
}

IMUMsg_t imu_dev;//这个结构体定义了三个姿态

void JY901_Test_Demo(void)
{
	JY901_Init();
	
	while(1)
	{
		JY901_GetData(&imu_dev);
		HAL_Delay(500);
	}
	
}