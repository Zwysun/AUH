#include "VideoProcess.hpp"
#include <opencv2/opencv.hpp>
#include <thread>
#include <algorithm> 
#include "LineDetect.hpp"
#include "CircleDetect.hpp"
#include <fstream>
#include <sstream> 
#include <unistd.h>

using namespace cv;

float imuAngle = 0;
float resultDistance = 0;
float resultAngle = 0;
int resultOutFlag = 0;
int markFlag = 0;	//圆检测标志
int circle_num = 0;   //检测到圆的次数
float distance_circle_store[5] = { 0 };

int B_num = 0;

int start_nocircle = 200;

int markLetter = 0;
string Letters;

string filename;
string s1 = "L.jpg";
string s2 = "C.jpg";
string s3 = ".jpg";
int L_num = 0;
int C_num = 0;
char str[20];
char str1[20];



float markResult;

int storeNum = 0;
int store_length = 3;
float storeDistance[3] = { 0 };
float storeAngle[3] = { 0 };
float mean_distance,mean_angle;

int yellow_num = 0;
int run_num = 0;

void rotate_arbitrarily_angle(Mat &jjdsrc, Mat &jjddst, float angle) {
	float radian = (float)(angle / 180.0 * CV_PI);     // 
	int maxBorder =(int) (max(jjdsrc.cols, jjdsrc.rows)* 1.414 ); 
	int dx = (maxBorder - jjdsrc.cols)/2;    int dy = (maxBorder - jjdsrc.rows)/2;  
	copyMakeBorder(jjdsrc, jjddst, dy, dy, dx, dx, BORDER_CONSTANT);       
	Point2f center( (float)(jjddst.cols/2) , (float) (jjddst.rows/2));   
	Mat affine_matrix = getRotationMatrix2D( center, angle, 1.0 );
	warpAffine(jjddst, jjddst, affine_matrix, jjddst.size());       
	float sinVal = abs(sin(radian));    float cosVal = abs(cos(radian)); 
	Size targetSize( (int)(jjdsrc.cols * cosVal +jjdsrc.rows * sinVal),          
		(int)(jjdsrc.cols * sinVal + jjdsrc.rows * cosVal) );     
	int x = (jjddst.cols - targetSize.width) / 2;    int y = (jjddst.rows - targetSize.height) / 2; 
	Rect rect(x, y, targetSize.width, targetSize.height);   
	jjddst = Mat(jjddst,rect);
}


void Pic_Process() 
{
	
	Mat fenge = imread("Letter.jpg");//名为girl.jpg的图片一定要与.cpp文件同路径。
	if (fenge.cols!=0&&fenge.rows!=0){
	Mat fenge1 = imread("Letter.jpg");//名为girl.jpg的图片一定要与.cpp文件同路径
	Mat fenge2 = imread("Letter.jpg");//名为girl.jpg的图片一定要与.cpp文件同路径

	Mat gray_fenge;	Mat gray_fenger;
	cvtColor(fenge, gray_fenge, CV_BGR2GRAY);  //要引用头文件opencv/opencv.hpp
	//namedWindow("image", WINDOW_AUTOSIZE);
	//amedWindow("gray_image", WINDOW_AUTOSIZE);

	//imshow("image", fenge);
	//imshow("gray_image", gray_fenge);

	Mat dimaoImg;
	Mat jjdrot;
	Mat tiduImg;
	Mat erfenge;
	Mat erfenger;
	Mat fenge1r;
	Mat fenge2r;
	Mat tiduImg1;
	Mat  tiduImg1r;
	Mat jjdelement = getStructuringElement(MORPH_RECT, Size(50, 50));
	Mat element12 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat jjdelement1 = getStructuringElement(MORPH_RECT, Size(10, 10));
	morphologyEx(gray_fenge, dimaoImg, MORPH_BLACKHAT, jjdelement);  //黒帽运算
	morphologyEx(dimaoImg, tiduImg, MORPH_GRADIENT, element12);  //形态学梯度运算
	//Sobel(dimaoImg, tiduImg, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	morphologyEx(tiduImg, tiduImg1, MORPH_CLOSE, jjdelement1);
	//imshow("tidu0", tiduImg1);

	threshold(tiduImg, erfenge, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);//二值化

	//imshow("diamo", dimaoImg);
	//imshow("tidu", tiduImg);
	//imshow("erfenge", erfenge);

	//闭运算
	morphologyEx(erfenge, erfenge, MORPH_CLOSE, jjdelement1);
	//imshow("erfenge3", erfenge);

	//去除最小区域
	std::vector<std::vector<cv::Point>> contours12;
	cv::findContours(erfenge, contours12, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	contours12.erase(std::remove_if(contours12.begin(), contours12.end(),
		[](const std::vector<cv::Point>& c) {return cv::contourArea(c) < 100; }), contours12.end());
	erfenge.setTo(0);
	cv::drawContours(erfenge, contours12, -1, cv::Scalar(255), cv::FILLED);
	//imshow("erfenge2",erfenge);

	//霍夫变换
	vector<Vec4i> jjdLines;
	HoughLinesP(erfenge, jjdLines, 1, CV_PI / 360, 170, 30, 200);
	//int jjdlin = (1 + Lines.size()) / 2;
	int jjdlin = 0;
	line(fenge1, Point(jjdLines[jjdlin][0], jjdLines[jjdlin][1]), Point(jjdLines[jjdlin][2], jjdLines[jjdlin][3]), Scalar(0, 0, 255), 2, 8);
	double jjdlinex = jjdLines[jjdlin][2]; double jjdliney = jjdLines[jjdlin][3];	double jjdlinex1 = jjdLines[jjdlin][0]; double jjdliney1 = jjdLines[jjdlin][1];

	double jjdk1 = (jjdliney1 - jjdliney) / (jjdlinex1 - jjdlinex);

	//cout << "!" << jjdk1 << endl << endl;

	//旋转
	rotate_arbitrarily_angle(fenge, jjdrot, atan(jjdk1) * 180 / CV_PI);
	rotate_arbitrarily_angle(erfenge, erfenger, atan(jjdk1) * 180 / CV_PI);
	rotate_arbitrarily_angle(fenge1, fenge1r, atan(jjdk1) * 180 / CV_PI);
	rotate_arbitrarily_angle(fenge2, fenge2r, atan(jjdk1) * 180 / CV_PI);
	rotate_arbitrarily_angle(gray_fenge, gray_fenger, atan(jjdk1) * 180 / CV_PI);
	rotate_arbitrarily_angle(tiduImg1, tiduImg1r, atan(jjdk1) * 180 / CV_PI);
	//imshow("fenge122", jjdrot);
	//imshow("fenge123", gray_fenger);


	Mat dstImg = erfenger.clone();
	vector<vector<Point>> jjdcontours;
	findContours(erfenger, jjdcontours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Rect boundRect12;  //定义外接矩形	    
	vector<vector<Point>>::iterator itc = jjdcontours.begin();
	Point2f rect_point[4];
	int i = 0;  int jjdH[120] = { 0 }; int jjdW[120] = { 0 }; int jjdx[120] = { 0 }; int jjdy[120] = { 0 };

	for (; itc != jjdcontours.end(); itc++)
	{
		//box = minAreaRect(Mat(*itc));  //计算每个轮廓最小外接矩形(旋转)	
		//boundRect = box.boundingRect();				
		boundRect12 = boundingRect(Mat(*itc));
		double jjdrectx = boundRect12.x;	double jjdrecty = boundRect12.y;	double jjdrectw = boundRect12.width;	double jjdrecth = boundRect12.height;
		double jjdk; jjdk = abs((jjdrecty + jjdrecth / 2) - jjdliney) / abs((jjdrectx + jjdrectw / 2) - jjdlinex);

		if (((boundRect12.width / boundRect12.height) < 1.5) && ((boundRect12.height / boundRect12.width) < 10)) {
			if (abs(jjdk)) {
				//circle(dstImg, Point(box.center.x, box.center.y), 5, Scalar(255,0, 0), -1, 8); 
					//绘制最小外接矩形的中心点			
				//rectangle(fenge1r, Point(boundRect12.x, boundRect12.y), Point(boundRect12.x + boundRect12.width, boundRect12.y + boundRect12.height), Scalar(0, 255, 0), 2, 8);
				//rectangle(dstImg, boundRect.tl(), boundRect.br() , Scalar(0, 255, 0), 3, 8);		
				jjdH[i] = boundRect12.height; 		 jjdW[i] = boundRect12.width;		 jjdx[i] = boundRect12.x;		 jjdy[i] = boundRect12.y;
				//cout << "x" << i << " :" << boundRect12.x << endl;
				//cout << "y" << i << " :" << boundRect12.y << endl;
				//cout << "width" << i << " :" << boundRect12.width << endl;
				//cout << "height" << i << " :" << boundRect12.height << endl << endl;
				i++;
			}
		}
	}	int jjdi = i;	int j = 0;
	//imshow("fenge1", fenge1r);
	int jjdvar[120][120] = { 0 };; int jjdsum[120] = { 0 };
	for (i = 0; i < jjdi; i++) {
		for (j = 0; j < jjdi; j++) {
			jjdvar[i][j] = abs(jjdH[i] - jjdH[j]);
			jjdsum[i] = abs(jjdsum[i] + jjdvar[i][j]);
		}
		//cout << "jjdsum" << i << " :" << jjdsum[i] << endl << endl;
	}
	//cout << endl;
	for (i = 0; i < jjdi; i++) {
		for (j = 0; j < jjdi; j++) {
			if (jjdsum[i] - jjdsum[j] < 0) {
				swap(jjdsum[i], jjdsum[j]); swap(jjdH[i], jjdH[j]); swap(jjdW[i], jjdW[j]); swap(jjdx[i], jjdx[j]); swap(jjdy[i], jjdy[j]);
			}
		}
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (jjdx[i] - jjdx[j] < 0) {
				swap(jjdsum[i], jjdsum[j]); swap(jjdH[i], jjdH[j]); swap(jjdW[i], jjdW[j]); swap(jjdx[i], jjdx[j]); swap(jjdy[i], jjdy[j]);
			}
		}
	}

	for (i = 0; i < 4; i++) {
		//cout << "jjdsum" << i << " :" << jjdsum[i] << endl << endl;
		rectangle(fenge2r, Point(jjdx[i], jjdy[i]), Point(jjdx[i] + jjdW[i], jjdy[i] + jjdH[i]), Scalar(0, 255, 0), 2, 8);
	}
	//imshow("fenge2", fenge2r);
	if (jjdx[0] == 0 && jjdy[0] == 0 && jjdW[0] == 0 && jjdH[0] == 0) {
		Mat p1 = Mat::zeros(28, 28, CV_8UC3); //
		resize(erfenge, p1, p1.size());
		imwrite("p1.jpg", p1);
	}
	else {
		Rect jjdrect1(jjdx[0], jjdy[0], jjdW[0], jjdH[0]);
		Mat image_roi1 = erfenger(jjdrect1);
		//rotate_arbitrarily_angle(image_roi1, image_roi1, 180);
		Mat p1 = Mat::zeros(28, 28, CV_8UC3); 
		resize(image_roi1, p1, p1.size());
		imwrite("p1.jpg", p1);
	}
	//2
	if (jjdx[1] == 0 && jjdy[1] == 0 && jjdW[1] == 0 && jjdH[1] == 0) {
		Mat p2 = Mat::zeros(28, 28, CV_8UC3); //
		resize(erfenge, p2, p2.size());
		imwrite("p2.jpg", p2);
	}
	else {
		Rect jjdrect2(jjdx[1], jjdy[1], jjdW[1], jjdH[1]);
		Mat image_roi2 = erfenger(jjdrect2);
		//rotate_arbitrarily_angle(image_roi2, image_roi2, 180);
		Mat p2 = Mat::zeros(28, 28, CV_8UC3); //
		resize(image_roi2, p2, p2.size());
		imwrite("p2.jpg", p2);
	}
	//3
	if (jjdx[2] == 0 && jjdy[2] == 0 && jjdW[2] == 0 && jjdH[2] == 0) {
		Mat p3 = Mat::zeros(28, 28, CV_8UC3); //
		resize(erfenge, p3, p3.size());
		imwrite("p3.jpg", p3);
	}
	else {
		Rect jjdrect3(jjdx[2], jjdy[2], jjdW[2], jjdH[2]);
		Mat image_roi3 = erfenger(jjdrect3);
		//rotate_arbitrarily_angle(image_roi3, image_roi3, 180);
		Mat p3 = Mat::zeros(28, 28, CV_8UC3); //
		resize(image_roi3, p3, p3.size());
		imwrite("p3.jpg", p3);
	}
	//4
	if (jjdx[3] == 0 && jjdy[3] == 0 && jjdW[3] == 0 && jjdH[3] == 0) {
		Mat p4 = Mat::zeros(28, 28, CV_8UC3); //
		resize(erfenge, p4, p4.size());
		imwrite("p4.jpg", p4);
	}
	else {
		Rect jjdrect4(jjdx[3], jjdy[3], jjdW[3], jjdH[3]);
		Mat image_roi4 = erfenger(jjdrect4);
		//rotate_arbitrarily_angle(image_roi4, image_roi4, 180);
		Mat p4 = Mat::zeros(28, 28, CV_8UC3); //
		resize(image_roi4, p4, p4.size());
		imwrite("p4.jpg", p4);
	}
}
}


/*统计传入的src灰度图的白色像素的数目，大于等于4，返回0，小于4，返回-1，用来检查图像中是否有字母存在*/
int markCheck(Mat &src)
{
	int sum = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (src.at<uchar>(i, j) == 255)
				sum++;
		}
	}
	if (sum >= 4)
		return 0;
	else
		return -1;
}

/*构造函数，和字母识别相关，没有返回值*/
VideoProcess::VideoProcess() {
	result_link_type *result_link = new result_link_type;
	result_link->head = result_link->end = NULL;
	result_link->result_num = 0;
}

/*frame的捕捉函数*/
void VideoProcess::frame_fetch(VideoProcess *videoProcess) {
	result_link_type* result_link = videoProcess->result_link;
	while (1) {
		videoProcess->cap >> videoProcess->frame;    //在获取的图像中右移一个frame
		cout << "get one frame!!!" << endl;      //得到一个frame
		cout << "frame size is :" << videoProcess->frame.rows << " " << videoProcess->frame.cols << endl;    //输出frame的宽和高
		struct result_node_datatype *
		result_node = new struct result_node_datatype;
		result_node->result = videoProcess->frame;
		//                imshow("frame",videoProcess->frame);   //显示捕捉到的frame
		//                waitKey(30);
		result_push(result_link, result_node);   //？？？？？？？？
		cout << "finish push!!!" << endl;
	}
}

void VideoProcess::run() {
	cap.open(0);      //打开上面的摄像头设备
//	cap1.open(1);
//	std::thread  t2(frame_fetch, this);     //线程，捕捉frame
	std::thread  t1(frame_process, this);   //线程，处理frame

	t1.detach();           //快速回收线程资源
//        t2.detach();

}

void VideoProcess::frame_process(VideoProcess *videoProcess)
{
	result_link_type* result_link = videoProcess->result_link;
	struct result_node_datatype *result_node2 = NULL;

	/// add by wwb 20190318
	ConfigFileRead configFileRead;

	int ret = configFileRead.readConfigFile("config.cfg");   //读取config.cfg文件

	if (ret < 0)
	{
		cout << "config file read failed, program shutdown!" << endl;     //读取失败
		return;
	}
	
	/*
	#检测最小线宽
	lineWidth=30

	#线段最小点数
	pointNum=60

	#超彩色分割阈值
	segThresholdValue=50

	#待检测圆最小圆心间距
	minDistValue=50

	#待检测圆最小半径
	minRadiusValue=25

	#待检测圆最大半径
	maxRadiusValue=100

	#待检测圆泛化度 ：数值小检测泛化度好，对噪声和残缺不敏感；数值大则检测结果越接近圆，检测精度越高
	circleLikeValue=55

	#打卡区域大小
	markAreaSize=150
	*/

	//atoi：将字符串str转换成一个整数并返回结果
	int segThresholdValue = atoi(configFileRead.readValue("segThresholdValue").c_str());
	int lineWidthValue = atoi(configFileRead.readValue("lineWidth").c_str());
	int pointNumValue = atoi(configFileRead.readValue("pointNum").c_str());

	int minDistValue = atoi(configFileRead.readValue("minDistValue").c_str());
	int minRadiusValue = atoi(configFileRead.readValue("minRadiusValue").c_str());
	int maxRadiusValue = atoi(configFileRead.readValue("maxRadiusValue").c_str());
	int circleLikeValue = atoi(configFileRead.readValue("circleLikeValue").c_str());

	int markAreaSize = atoi(configFileRead.readValue("markAreaSize").c_str());

	ret = configFileRead.closeConfigFile();        //关闭config.cfg文件
	if (ret != 0)
	{
		cout << "config file close failed, program shutdown!" << endl;
		return;
	}


	if (lineWidthValue <= 0 || pointNumValue <= 0)
	{
		cout << "params in config file are not in right range, program shutdown!" << endl;
		return;
	}

	cout << "read and close configFile success!!!" << endl;

	int lineFindState = 1;//导航状态，0为初始状态，1为 1点->2点状态，2为 2点->3点状态，3为 3点->1点状态（比赛的等腰直角的三个位置），但这里貌似是因为三段路线等待颜色不一样所以要区分

	LinePointFinder pointFinder;          //水下巡线
	CircleFinder circleFinder;            //水下圆形

	Mat img; //原始图像
	Mat UP;
	Mat imgResize; //下采样图像

	int width = 0; //图像宽度
	int height = 0;//图像高度

	Point pointA; //拟合后的直线截断端点A
	Point pointB; //拟合后的直线截断端点B

	Point centerFind;  //圆心
	int radiusFind;    //半径


//	double imuAngel = 0;  //输入的惯导累计航向角，范围【-PI，PI】
	double frontAngle = 0;//目标航向角，范围【-PI，PI】
	double diffAngle = 0; //目标航向角与累计航向角的偏差角，用以计算导航直线的前后方向

	int findLetterFlag = 0;

	//	VideoCapture cap(0);	
	//VideoCapture cap1(1);   //下方的摄像头
	//Mat imgUp;
	//cap1 >> imgUp;    //获取上方摄像头的图像
	//usleep(1000*500);        //使程序暂时停止，单位是微秒，只在Linux系统中使用

	while (1)
	{
		/*
				result_node2 = result_pop(result_link);
						cout<<"image size is :"<<result_node2->result.rows<<"  "<<result_node2->result.cols<<endl;
				if (result_node2 == NULL)
				{
					std::this_thread::sleep_for(std::chrono::microseconds(30));
					continue;
				}
				imshow("frame", result_node2->result);
				if (videoProcess->first_flag == 0) {
					videoProcess->init_base_contour(videoProcess->basic_model, result_node2->result);
					videoProcess->first_flag = 1;
				}
		*/
		double time0 = static_cast<double>(getTickCount());

		videoProcess->cap >> img;
		//videoProcess->cap1 >> UP;

		//cap1.set(CV_CAP_PROP_FRAME_WIDTH,640);
		//cap1.set(CV_CAP_PROP_FRAME_HEIGHT,480);

		cout<<"bbbbb"<<endl;
		//imshow("UP",UP);

		//cout << "lineFindState is :" << lineFindState << endl;    //导航状态
/*
		if(0 == videoProcess->first_flag)
		{
			videoProcess->init_base_contour(videoProcess->basic_model,img);
			videoProcess->first_flag = 1;
		}
*/
		width = img.cols;
		height = img.rows;

		resize(img, imgResize, Size(width / 2, height / 2)); //下采样为原尺寸的1/2
		//cout << "resize image size is :" << imgResize.rows << "  " << imgResize.cols << endl;         //输出重建的图片的尺寸

		Mat imgSegment(imgResize.rows, imgResize.cols, CV_8U); 	//分割图像，CV_8U - 8-bit unsigned integers ( 0..255 )3

		pointFinder.setDectParams(lineFindState, lineWidthValue, pointNumValue, segThresholdValue);  ////设置要检测的颜色类型，线宽最小值，最少点数，分割阈值等参数

		pointFinder.segmentProcess2(imgResize, imgSegment);   //检测特定颜色的直线，颜色是由lineFindState决定的

		ret = pointFinder.calculatePoint(imgSegment, pointA, pointB);  //寻找直线的起始点与终止点

		if (0 == ret)  //返回值是0，说明找到了直线
		{
			line(imgResize, pointA, pointB, Scalar(255, 0, 0), 2, 8, 0);    //在imgResize上画出这条直线

//			line(img, Point(pointA.x * 2, pointA.y * 2), Point(pointB.x * 2, pointB.y * 2), Scalar(255, 0, 0), 2, 8, 0);
			switch (lineFindState)
			{
			case 1:
				frontAngle = 0;
				break;
			case 2:
				frontAngle = PI / 2;
				break;
			case 3:
				frontAngle = -3 * PI / 4;
				break;
			case 4:
				frontAngle = -PI;
				break;
			default:
				frontAngle = 0;
				break;
			}

			diffAngle = frontAngle - imuAngle;
			if (diffAngle > PI)
				diffAngle = 2 * PI - diffAngle;
			if (diffAngle < -PI)
				diffAngle = 2 * PI + diffAngle;
			diffAngle = abs(diffAngle);
			//cout << "diffAngle is : " << diffAngle << endl;
			//cout << pointA.x << " " << pointA.y << ", " << pointB.x << " " << pointB.y << endl;     //输出直线两个端点的坐标
			if (diffAngle <= PI / 2)
			{
				pointFinder.calculateResult(imgResize, pointA, pointB);
			}
			else
			{
				pointFinder.calculateResult(imgResize, pointB, pointA);
			}
			resultDistance = pointFinder.distanceResult;
			resultAngle = pointFinder.angleResult;

			if (storeNum != store_length) {
				storeDistance[storeNum] = resultDistance;
				storeAngle[storeNum] = resultAngle;
				storeNum = storeNum + 1;
			}
			else
			{
				if (L_num%5==0){
					sprintf(str,"%d",L_num);
					filename = str+s1;
					imwrite(filename,imgResize);
					L_num = L_num+1;
					
				}
				else{L_num = L_num+1;}
				storeNum = 0;
				ofstream OutFile("Result.txt"); //利用构造函数创建txt文本，并且打开该文本
				char floatchar[15];
				for (int k = 0; k < store_length; k++) {
					sprintf(floatchar, "%.2f", storeDistance[k]);
					OutFile << floatchar<<"    ";  //把字符串内容写入Test.txt文件
					sprintf(floatchar, "%.2f", storeAngle[k]);
					OutFile << floatchar << "\n";  //把字符串内容写入Test.txt文件
				}
				OutFile.close();            //关闭Test.txt文件
				sort(storeDistance, storeDistance + store_length);
				sort(storeAngle, storeAngle + store_length);
				//mean_distance = (storeDistance[1] + storeDistance[2] + storeDistance[3]) / 3;
				//mean_angle = (storeAngle[1] + storeAngle[2] + storeAngle[3]) / 3;
				mean_distance = storeDistance[1];
				mean_angle = storeAngle[1];
				cout << "Mean Distance and Angle: " << mean_distance << "  ,  " << mean_angle << endl;
				resultOutFlag = 1;
			}

			//cout << "distance and angle is : " << pointFinder.distanceResult << " , " << pointFinder.angleResult << endl;
		}

		int circleColor = 2;   //黄色的球/红色的瓶盖

		if (lineFindState < 4)
		{
			/*switch (lineFindState)
			{
			case 0:
				circleColor = GREEN;
				break;
			case 1:
				circleColor = BLUE;
				break;
			case 2:
				circleColor = RED;
				break;
			case 3:
				circleColor = GREEN;
				break;
			default:
				circleColor = GREEN;
				break;
			}*/

			Mat imageSegmentCircle(imgResize.rows, imgResize.cols, CV_8U);

			Mat imgResize2;

			resize(img, imgResize2, Size(width / 2, height / 2)); //下采样为原尺寸的1/2

			circleFinder.setParams(circleColor, minDistValue, minRadiusValue, maxRadiusValue, circleLikeValue, segThresholdValue);

			circleFinder.segmentProcess2(imgResize2, imageSegmentCircle);

			//imshow("circlebefore", imgResize2);

			ret = circleFinder.circleCalculate(imageSegmentCircle);

			//imshow("circleSegment", imageSegmentCircle);

			if (0 == ret)	//检测到了圆
			{
				if (run_num==0){
					yellow_num = yellow_num+1;
					cout << "Yellow-------------------------" << yellow_num << endl;
					run_num = 500;
				}

				if (circle_num != 1) {
					distance_circle_store[circle_num] = circleFinder.circleDistance(imgResize2);
					circle_num = circle_num + 1;
				}
				else {
					circle_num = 0;
					cout << "Radius:" << circleFinder.radius << endl;
					circle(imgResize, circleFinder.circleCenter, circleFinder.radius, Scalar(255, 255, 0), 3, 8, 0);

					if (C_num % 5 == 0) {
						sprintf(str, "%d", C_num);
						filename = str + s2;
						imwrite(filename, imgResize);
						C_num = C_num + 1;
					}
					else { C_num = C_num + 1; }

					sort(distance_circle_store, distance_circle_store + 1);
					markResult = distance_circle_store[0] ;
					markFlag = 1;
				}
				
			}

		}
		//imshow("edge image", contours);
		imshow("resize", imgResize);
		//imshow("picture", img);
		//imshow("segment", imgSegment);

		
		//识别到一次圆之后，接下来500次while循环不能对识别圆进行计数
		if (yellow_num!=0){
			run_num = run_num-1;
			if (run_num<0){run_num = 0;}
		}

		//cout << "process one frame using time :" << ((double)getTickCount() - time0) / getTickFrequency() * 1000 << endl;  //每一次检测直线使用的时间

		if (yellow_num >=1&&yellow_num<=6&&start_nocircle==0)  //表示到达了识别字母的地方
		{   
			yellow_num += 1;
			//拍照保存
			//cap1.open(1);   
			//Mat imgUp;

			//imshow("4",imgUp);
			//while(1){cap1>>imgUp; imshow("3",imgUp);}
			//cap1>>imgUp;
			cout<<"ggg"<<endl;
			//UP.convertTo(UP,CV_8UC3,255);
			sprintf(str1,"%d",yellow_num);
			
			//imwrite(str1+s3, UP);
			cout<<"latter"<<endl;
			
			/*加上蒋金达的代码，输入img，输出四张28×28的图片,一个调用函数*/
			
			//Pic_Process();
			cout<<"jjd"<<endl;
			//写一个空的txt文件，用于Python的通信
			
			ofstream OutFile("zjz.txt");
			OutFile.close();
		}
		ifstream file;
		file.open("Recognition.txt", ios::in);
		
		/*if (file)
		{
			int n = 0;
			markLetter = 0;
			while (getline(file, Letters, '\n'))
			{
				n++;
			}
			file.close();
		}

		if (start_nocircle>0)
		{
			start_nocircle = start_nocircle-1;
		}*/
		char c = waitKey(1);
		
	}
	
	
	
}




void VideoProcess::init_base_contour(vector<vector<Point>> & vec, const Mat &frame) {

	Mat src1 = Mat::zeros(frame.size(), frame.type());
	rectangle(src1, Rect(Point(100, 100), Point(frame.rows / 2, frame.rows / 2)), Scalar(255, 255, 255), -1, 8);
	Mat gray1;
	cvtColor(src1, gray1, CV_RGB2GRAY);
	vector<Vec4i> hierarchy1;
	vector<vector<Point>> Basecontour1;
	findContours(gray1, Basecontour1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	vec.push_back(Basecontour1[0]);
}
