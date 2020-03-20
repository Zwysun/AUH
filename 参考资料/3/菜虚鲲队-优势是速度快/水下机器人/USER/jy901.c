#include "jy901.h"
/**
  ******************************************************************************
  * @file    jy901.c
  * @author  LiuYang
  * @brief   �ߵ�ģ��
  *
  *
  ******************************************************************************
  * @attention
  *	JY901 ����1 Ĭ�ϲ�����: 9600
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
	
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
//CopeSerialDataΪ�����жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������

void CopeSerialData(unsigned char ucData)
{
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);
			break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����
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
