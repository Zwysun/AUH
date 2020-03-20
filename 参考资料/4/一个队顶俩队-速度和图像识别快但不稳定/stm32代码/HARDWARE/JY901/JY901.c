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
	
	USART1_RX_BUF[USART1_Rx_CNT++]=ucData;	//将收到的数据存入缓冲区中，每调用一次自动移位
	if (USART1_RX_BUF[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		USART1_Rx_CNT=0;
		return;
	}
	if (USART1_Rx_CNT<11) {return;}//数据不满11个，则返回。直到存了11个byte的数据后，再处理数据
	else
	{
		switch(USART1_RX_BUF[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
			case 0x51:	memcpy(&stcAcc,&USART1_RX_BUF[2],8);  //memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
									
									break;
			case 0x52:	memcpy(&stcGyro,&USART1_RX_BUF[2],8);	//角速度,°/s
									yaw_speed=(stcGyro.w[2]/32768.0*2000);yaw_speed*=(-1);
									break;
			case 0x53:	memcpy(&stcAngle,&USART1_RX_BUF[2],8);	//角度
									//printf("angle reflashed \n");
									roll=(int)(stcAngle.Angle[1]/32768.0*180);roll*=(-1);
									pitch=(int)(stcAngle.Angle[0]/32768.0*180);pitch*=(-1);
									yaw=(int)(stcAngle.Angle[2]/32768.0*180);yaw*=(-1);
									break;
		}
		USART1_Rx_CNT=0;//清空缓存区，以便处理下一波数据
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
