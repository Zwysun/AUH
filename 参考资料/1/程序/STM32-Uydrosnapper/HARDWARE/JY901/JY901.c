#include "JY901.h"
#include "usart.h"
#include <string.h>
#include "delay.h"
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;

int roll=0, pitch=0, yaw=0;
float yaw_speed=0;
u8 IMU_Sleep_Buf[5]={0xff,0xaa,0x22,0x01,0x00};
void CopeSerial1Data(unsigned char ucData)
{
	
	USART1_RX_BUF[USART1_Rx_CNT++]=ucData;	//���յ������ݴ��뻺�����У�ÿ����һ���Զ���λ
	if (USART1_RX_BUF[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		USART1_Rx_CNT=0;
		return;
	}
	if (USART1_Rx_CNT<11) {return;}//���ݲ���11�����򷵻ء�ֱ������11��byte�����ݺ��ٴ�������
	else
	{
		switch(USART1_RX_BUF[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
			case 0x51:	memcpy(&stcAcc,&USART1_RX_BUF[2],8);  //memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
									
									break;
			case 0x52:	memcpy(&stcGyro,&USART1_RX_BUF[2],8);	//���ٶ�,��/s
									yaw_speed=(stcGyro.w[2]/32768.0*2000);yaw_speed*=(-1);
									break;
			case 0x53:	memcpy(&stcAngle,&USART1_RX_BUF[2],8);	//�Ƕ�
									//printf("angle reflashed \n");
									roll=(int)(stcAngle.Angle[1]/32768.0*180);roll*=(-1);
									pitch=(int)(stcAngle.Angle[0]/32768.0*180);pitch*=(-1);
									yaw=(int)(stcAngle.Angle[2]/32768.0*180);yaw*=(-1);
									break;
		}
		USART1_Rx_CNT=0;//��ջ��������Ա㴦����һ������
	}
}

void yaw_Reset(void)
{
	static int buf_cnt;
	for(buf_cnt=0;buf_cnt<5;buf_cnt++)
	{
		USART_SendData(USART1,IMU_Sleep_Buf[buf_cnt]);
		delay_ms(1);
	}
	delay_ms(300);
	for(buf_cnt=0;buf_cnt<5;buf_cnt++)
	{
		USART_SendData(USART1,IMU_Sleep_Buf[buf_cnt]);
		delay_ms(1);
	}

}
