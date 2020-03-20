#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include "VideoProcess.hpp"
#include "serial.hpp"

using namespace std;
using namespace cv;

int main() {
	VideoProcess *videoProcess = new VideoProcess();
	videoProcess->run();
	Serial *serial = new Serial();
	serial->run();
	while (1) {}
	return 0;
}
