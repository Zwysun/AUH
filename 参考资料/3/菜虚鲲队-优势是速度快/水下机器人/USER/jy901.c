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

unsigned char ucRxBuffer[15];
unsigned char ucRxCnt = 0;	
	
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。

void CopeSerialData(unsigned char ucData)
{
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
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);
			break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);
			break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);
			break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);
			break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);
			break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);
			break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);
			break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);
			break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);
			break;
		}
		ucRxCnt=0;
	}
}

void JY901_GetData(IMUMsg_t* imu)
{
	imu->roll = stcAngle.Angle[0]/32768.f*180;
	imu->pitch = stcAngle.Angle[1]/32768.f*180;
	imu->heading = stcAngle.Angle[2]/32768.f*180;
	imu->x_angular_velocity = stcGyro.w[0]/32768.f*2000;
	imu->y_angular_velocity = stcGyro.w[1]/32768.f*2000;
	imu->z_angular_velocity = stcGyro.w[2]/32768.f*2000;
}
IMUMsg_t imu_dev;
