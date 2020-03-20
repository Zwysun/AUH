#include "serial.hpp"
#include "crc16.hpp"
#include <iostream>
using namespace std;
extern float imuAngle;
extern float resultDistance;
extern float resultAngle;
extern int resultOutFlag;
extern int markFlag;
extern float markResult;
//extern int detectFlag;
//extern char detectResult[8];
extern float mean_distance;
extern float mean_angle;

extern int markLetter;
extern string Letters;


void Serial::send_message_down(void *ptr)   //void指针，不指定特定类型
{
	int fd  = *(int*)(ptr);
	unsigned int nextTime = millis() + 10 ;
	unsigned char sendFrameCnt = 0;
	unsigned char sendFrameId;
	unsigned char frameHead = 0xFE;
	unsigned char frameLen;
	unsigned char frameTail = 0X0A;
	unsigned short checkValue;
    	
	while(1)
	{
		//char palindrome[11] ={'a','b','c','d','e','f','e','d','c','b','a'};
		//char palindrome[11] ={'a','b','c','d','e','f','e','d','c','b','a'};
		uint8_t sendFrame[15] ={0};
		char* tempDistance;
		char* tempAngle;
		char* tempmarkresult;
		char* tempLetters;


//		cout<<"!!!!!!!!!!!!resultOutFlag is  :"<<resultOutFlag<<endl;
//		cout<<"!!!!!!!!!!!!markFlag is :"<<markFlag<<endl;
//		cout<<"!!!!!!!!!!!!detectFlag is :"<<detectFlag<<endl;

		if (1 == resultOutFlag)
		{   
			sendFrameId = 0X23;
		    frameLen = 0X0F;    //发送的字节数
			tempDistance = (char*)(&mean_distance);      //距离
			tempAngle = (char*)(&mean_angle);            //角度
			sendFrame[0] = frameHead;	//信息头 
			sendFrame[1] = frameLen;	//字节数             
			sendFrame[2] = sendFrameCnt;	//记录发送信息的次数，超过255就回到0
			sendFrame[3] = sendFrameId;     
			memcpy(sendFrame+4,tempAngle,4);	//float数据占4个字节
			memcpy(sendFrame+8,tempDistance,4);
			checkValue = crc_check(sendFrame, 12);
			sendFrame[12] = checkValue>>8;
			sendFrame[13] = checkValue&0X00FF;
			sendFrame[14] = frameTail;	//结束标志

			cout<<"send result is :"<<resultDistance<<" "<<resultAngle<<endl;

			for(int i=0;i<frameLen;i++)
			{
			//serialPutchar (fd, palindrome[i]) ;
			    serialPutchar (fd, sendFrame[i]) ;  //树莓派串口发送，fd：文件描述符
//				usleep(1000);
		    }
			if(255 == sendFrameCnt)
			    sendFrameCnt = 0;
			else
			    sendFrameCnt++;
			resultOutFlag = 0;	//发送完之后将检测标志去除
		}

		/*检测到圆发送*/
		if (1 == markFlag)      //检测到圆
		{
			tempmarkresult = (char*)(&markResult);
			sendFrameId = 0X24;
			frameLen = 0X0B;
			sendFrame[0] = frameHead;
			sendFrame[1] = frameLen;
			sendFrame[2] = sendFrameCnt;
			sendFrame[3] = sendFrameId;
			memcpy(sendFrame+4,tempmarkresult,4);
			checkValue = crc_check(sendFrame,8);
			sendFrame[8] = checkValue>>8;
			sendFrame[9] = checkValue&0X00FF;
			sendFrame[10] = frameTail;

			for(int i=0;i<frameLen;i++)
			{
				serialPutchar (fd, sendFrame[i]) ;
//				usleep(1000);
			}
			if(255 == sendFrameCnt)
				sendFrameCnt = 0;
			else
				sendFrameCnt++;
			markFlag = 0;
		}

		/*字母发送*/
		if (1 == markLetter)
		{
			sendFrameId = 0X25;
			frameLen = 0X0B;    //发送的字节数
			tempLetters = (char*)(&Letters);
			sendFrame[0] = frameHead;
			sendFrame[1] = frameLen;
			sendFrame[2] = sendFrameCnt;
			sendFrame[3] = sendFrameId;
			memcpy(sendFrame + 4, tempLetters, 4);
			checkValue = crc_check(sendFrame, 8);  //对前面的8个进行校验
			sendFrame[8] = checkValue >> 8;
			sendFrame[9] = checkValue & 0X00FF;
			sendFrame[10] = frameTail;
			for (int i = 0; i < frameLen; i++)
			{
				serialPutchar(fd, sendFrame[i]);
				//usleep(1000);
			}
			if (255 == sendFrameCnt)
				sendFrameCnt = 0;
			else
				sendFrameCnt++;
			markLetter = 0;
		}

	usleep(1000*30);
	}
}      

/*void Serial::recv_message_down(void *ptr){
	int fd  = *(int*)(ptr);
	unsigned char recvBuffer[200]={0};
	int bufferUsedNUm = 0;
	unsigned short checkValue;
	int ipAddress1, ipAddress2, ipAddress3, ipAddress4;
	char* cmdBuffer;
	while(1){
			while (serialDataAvail (fd))
			{
				recvBuffer[bufferUsedNUm] = serialGetchar(fd);
				bufferUsedNUm++;
				if(bufferUsedNUm >=200)
				{
					bufferUsedNUm = 0;
				}
			}
			int i = 0;
			for(i=0; i<bufferUsedNUm; i++)
			{
				if(i + 10 >= bufferUsedNUm )
				    break;
				if(recvBuffer[i]==0XFE  && recvBuffer[i+10] == 0X0A)
				{

			        checkValue = crc_check(recvBuffer+i,8);

					if(recvBuffer[i+8] ==(checkValue>>8) && recvBuffer[i+9] == (checkValue&0X00FF))
					{
						if(recvBuffer[i+3] == 0X02)
							imuAngle = *((float *)(recvBuffer + 4));
						else if(recvBuffer[i+3] == 0X26)
						{
					        ipAddress1 = recvBuffer[4];
							ipAddress2 = recvBuffer[5];
							ipAddress3 = recvBuffer[6];
							ipAddress4 = recvBuffer[7];
							sprintf(cmdBuffer,"ifconfig wlan0 %d.%d.%d.%d",ipAddress1,ipAddress2,ipAddress3,ipAddress4);
							system("ifconfig wlan0 down");
							system(cmdBuffer);
							system("ifconfig wlan0 up");
						}
					}
				    bufferUsedNUm = bufferUsedNUm - i -11;
					memmove(recvBuffer,recvBuffer+i+11,bufferUsedNUm);
					i=0;
				}
			}
			bufferUsedNUm = bufferUsedNUm - i;
			memmove(recvBuffer,recvBuffer+i,bufferUsedNUm);

			usleep(1000*10);
	}
}*/




// void Serial::send_message_up(void *ptr,void *ptr1){
// 	printf("up ttyUSB0");
// 	int fd  = *(int*)(ptr);
// 	VideoCapture capture = *(VideoCapture*)ptr1;
// 	Mat src;
// 	unsigned int nextTime = millis () + 10 ;
// 	const int width = 40;
// 	const int height = 30;
// 	while(1){
// 		capture >>src;
// 		cvtColor(src,src,CV_BGR2GRAY);
// 		resize(src,src,Size(width,height));
// 		char palindrome[11] ={'a','b','c','d','e','f','e','d','c','b','a'};
// 		for(int i=0;i<11;i++){
// 				serialPutchar (fd, palindrome[i]) ;
// 		}
// 		for(int y=0;y<height;y++){
// 			for(int x=0;x<width;x++){
// 				unsigned char value =src.at<uchar>(y,x);
// 				//fflush (stdout) ;
// 				serialPutchar (fd, value) ;
// 			}
// 		}
// 		//usleep(1000*100);
// 		imshow("src",src);
// 		waitKey(10);
// 	}
// }
// void Serial::recv_message_up(void *ptr){
// 	int fd  = *(int*)(ptr);
// 	while(1){
// 			while (serialDataAvail (fd))
// 			{
// 				printf (" -> %3d", serialGetchar (fd)) ;
// 				fflush (stdout) ;
// 			}
// 	}
// }
// void Serial::send_message_down(void *ptr){
// 	int fd  = *(int*)(ptr);
// 	unsigned int nextTime = millis () + 10 ;
// 	while(1){
// 		//char palindrome[11] ={'a','b','c','d','e','f','e','d','c','b','a'};
// 		char palindrome[11] ={'a','b','c','d','e','f','e','d','c','b','a'};
// 		for(int i=0;i<100;i++){
// 				//serialPutchar (fd, palindrome[i]) ;
// 				serialPutchar (fd, i) ;
// 				usleep(1000*100);
// 		}
// 	}
// }
// void Serial::recv_message_down(void *ptr){
// 	int fd  = *(int*)(ptr);
// 	while(1){
// 			while (serialDataAvail (fd))
// 			{
// 				printf (" -> %3d", serialGetchar (fd)) ;
// 				fflush (stdout) ;
// 			}
// 	}
// }

Serial::Serial(){

}
int Serial::run ()
{


/*
	if ((fd_up = serialOpen ("/dev/ttyUSB0", 115200)) < 0)
	//if ((fd = serialOpen ("/dev/ttyS0", 115200)) < 0)
	{fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;return 1 ;}
	if (wiringPiSetup () == -1)
	{fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;return 1 ;}
*/

	if ((fd_down = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
	//if ((fd = serialOpen ("/dev/ttyS0", 115200)) < 0)
	{fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;return 1 ;}
	if (wiringPiSetup () == -1)
	{fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;return 1 ;}

/*
	std::thread t1(send_message_up,&fd_up,&capture);
	std::thread t2(recv_message_up,&fd_up);
*/
	std::thread t3(send_message_down,&fd_down);
//std::thread t4(recv_message_down,&fd_down);
//	t1.detach();
//	t2.detach();
	t3.detach();
	//t4.detach();
	cout<<"serial run"<<endl;
	while(1);
	return 0 ;
}
