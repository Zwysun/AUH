#include"usbl.h"
#include"stm32f10x_usart.h" //stm32串口函数库

//逆超短基线

unsigned char yaw_high,yaw_low,pitch_high,pitch_low,roll_high,roll_low;

unsigned char distance_high,distance_low,bearing_high,bearing_low,depth_high,depth_low;

float usbl_roll,usbl_pitch,usbl_yaw; 

int usbl_distance,usbl_bearing,usbl_depth;

unsigned char CRC16_High,CRC16_Low;

 unsigned char message_txd[13]={start_frame,opp_id,test,0,0,0,0,0,0,0,0,0,0}; 
 unsigned char message_rxd[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};

int X_angle=79,Y_angle=79;
float x_angle_f=79,y_angle_f=79;

 extern  float traget_yaw;   
 extern float yaw; 
 

/********************************************/
//命令发送程序，需要调用本地串口发送代码
//该函数可拓展含参数命令，故予以保留
/********************************************/
void USBL_command_send(unsigned char command_num) // 
{   
	
}

//需要自己补充底层代码

int message_send(void) //信息发送函数 
{
	

}


/********************************************/
//命令解析程序
//调用该函数前需要把接收缓冲区装填完毕
/********************************************/
unsigned char rxd_command_handle(void)
{
	//unsigned int CRC_result=0;
	unsigned char CRC_high;
	unsigned char CRC_low;  //分别存放CRC校检的高8位低8位
	int16_t temp=0;
  unsigned char i;
	
	int x_ball;
	
	static unsigned int ball_get_count=0;
	
	unsigned char ball_get_flag=0;//是否找到球的标志
	
	/*
	// 发送一个字节数据到USART1//		
	USART_SendData(USART3, local_id);			
	///等待发送完毕 /
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
	
	//发送一个字节数据到USART1 //		
	USART_SendData(USART3, message_rxd[1]);			
	// 等待发送完毕 // 
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	 */
	x_ball=message_rxd[2];
	
	if(message_rxd[1])
		ball_get_flag=1;
	
	if(ball_get_flag)
		{
				traget_yaw=yaw-(1.125*x_ball-45);
				ball_get_count++;
				if(ball_get_count>21)
					ball_get_count=21;
		}
	else
		{
			

		}
	
	


}


/********************************************/
//16位CRC校检
//缓冲区数组地址，数组数量，校检码
/********************************************/

unsigned char aucCRCHi[] = {      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
                                  0x00, 0xC1, 0x81, 0x40
                                  };

unsigned char aucCRCLo[] =      { 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
                                  0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
                                  0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
                                  0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
                                  0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
                                  0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
                                  0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
                                  0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
                                  0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
                                  0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
                                  0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
                                  0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
                                  0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
                                  0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
                                  0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
                                  0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
                                  0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
                                  0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
                                  0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
                                  0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
                                  0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
                                  0x41, 0x81, 0x80, 0x40
                                };





void CRC16Table( unsigned char *p, int usLen)
{
    unsigned char ucCRCHi = 0xFF;
    unsigned char ucCRCLo = 0xFF;
    int iIndex;
    while( usLen-- )
    {
        iIndex  = ucCRCLo ^ *(p++);
        ucCRCLo = ( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    
		CRC16_High=ucCRCHi; //CRC校检输出
		CRC16_Low=ucCRCLo;//全局变量形式输出
    

    ucCRCHi = 0xFF;
    ucCRCLo = 0xFF;
}



