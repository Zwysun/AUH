#include <opencv2/opencv.hpp>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

using namespace std;
using namespace cv;

class Serial{
	private:
	int fd_up,fd_down;

	public:
	Serial();
	int run ();
	private:
//	static void send_message_up(void *ptr,void *ptr1);
//	static void recv_message_down(void *ptr);
//	static void recv_message_up(void *ptr);
	static void send_message_down(void *ptr);
};
