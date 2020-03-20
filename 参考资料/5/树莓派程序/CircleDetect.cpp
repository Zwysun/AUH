#include "CircleDetect.hpp"
#define RED 1
#define GREEN 2
#define BLUE 3
#define PI 3.14

extern int circle_num;
extern int B_num;
char str_num[10];
string s2_B = "CB.jpg";
string file_name;

/**************************************************CircleFinder***********************************************/

int bSums(Mat src)
{
	int counter = 0;
	//迭代器访问像素点
	Mat_<uchar>::iterator it = src.begin<uchar>();
	Mat_<uchar>::iterator itend = src.end<uchar>();
	for (; it != itend; ++it)
	{
		if ((*it) > 0) counter += 1;//二值化后，像素点是0或者255
	}
	return counter;
}

CircleFinder::CircleFinder() {}

void CircleFinder::setParams(int colorInput, int minDistValue, int minRadiusValue, int maxRadiusValue, int circleLikeValue, int segThreshold) //设置检测的参数
{
	colorMode = colorInput;   //颜色
	minDist = minDistValue;   //待检测圆最小圆心间距
	minRadius = minRadiusValue;	//待检测圆最小半径
	maxRadius = maxRadiusValue;  //待检测圆最大半径
	circleLike = circleLikeValue;	//待检测圆泛化度 ：数值小检测泛化度好，对噪声和残缺不敏感；数值大则检测结果越接近圆，检测精度越高
	thresholdValue = segThreshold;	//打卡区域大小
}

void CircleFinder::segmentProcess(Mat &src, Mat &dst) //根据彩色空间模型实现图像的分割RGB
{
	int i = 0;
	int j = 0;
	int temp_r = 0;
	int temp_g = 0;
	int temp_b = 0;
	int temp = 0;

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{

			temp_r = (int)src.at<Vec3b>(i, j)[2];
			temp_g = (int)src.at<Vec3b>(i, j)[1];
			temp_b = (int)src.at<Vec3b>(i, j)[0];

			if (RED == colorMode) //red color
			{
				temp = temp_r * 2 - temp_g - temp_b;
			}
			else if (GREEN == colorMode) //green color
			{
				temp = temp_g * 2 - temp_r - temp_b;
			}
			else if (BLUE == colorMode) //blue color
			{
				temp = temp_b * 2 - temp_r - temp_g;
			}

			if (temp <= thresholdValue)
			{
				temp = 0;
			}
			else if (temp > thresholdValue)
			{
				temp = 255;
			}

			dst.at<uchar>(i, j) = temp;



		}

}

void CircleFinder::segmentProcess2(Mat &src, Mat &dst)     //基于HSV的圆形检测
{
	Mat imgHSV;
	int hValueMin, hValueMax, sValueMin, sValueMax, vValueMin, vValueMax;

	switch (colorMode)
	{
	case RED:
		hValueMin = 160;
		hValueMax = 190;
		sValueMin = 150;
		sValueMax = 210;
		vValueMin = 210;
		vValueMax = 260;
		break;
	case GREEN:
		hValueMin = 0;
		hValueMax = 76;
		sValueMin = 50;
		sValueMax = 150;
		vValueMin = 52;
		vValueMax = 200;
		break;
	case BLUE:
		hValueMin = 91;
		hValueMax = 130;
		sValueMin = 43;
		sValueMax = 255;
		vValueMin = 46;
		vValueMax = 255;
		break;
	default:
		hValueMin = 35;
		hValueMax = 90;
		sValueMin = 43;
		sValueMax = 255;
		vValueMin = 46;
		vValueMax = 255;
		break;
	}

	//cvtColor(src, imgHSV, CV_BGR2HSV);    //将输入的RGB图格式转化成HSV
	imgHSV = src;

	if (RED == colorMode)
	{
		Mat dst1, dst2;
		inRange(imgHSV, Scalar(hValueMin, sValueMin, vValueMin), Scalar(hValueMax, sValueMax, vValueMax), dst1);
		inRange(imgHSV, Scalar(0, sValueMin, vValueMin), Scalar(15, sValueMax, vValueMax), dst2);
		dst = dst1 + dst2;
	}
	else
	{
		inRange(imgHSV, Scalar(hValueMin, sValueMin, vValueMin), Scalar(hValueMax, sValueMax, vValueMax), dst);
	}
}





int CircleFinder::circleCalculate(Mat &src) //从分割图像中检测满足条件的最大的圆，赋值给类里的圆心和半径成员变量，并在图像中画出
{

	Mat imgBlur(src.rows, src.cols, CV_8U);    
	radius = 0;
	int index = 0;

	GaussianBlur(src, imgBlur, Size(9, 9), 2, 2); //高斯滤波

//    imshow("circle segment", imgGray);
//	  waitKey(1);

	vector<Vec3f> cir;
	
	HoughCircles(imgBlur, cir, HOUGH_GRADIENT, 1, minDist, 100, circleLike, minRadius, maxRadius);    //霍夫圆检测
	circles = cir;
	//int white_num = bSums(src);//调用函数bSums
	double sum = 0;
	double percent = 0;
	double area = src.rows*src.cols;
	for (int i = 0; i < src.rows - 1; i++)
	{
		for (int j = 0; j < src.cols - 1; j++)
		{
			if (src.at<uchar>(i, j) == 255) { sum = sum + 1; }

		}
	}
	percent = sum / area;

	//cout << "wwwwww"<<circles.size() << endl;

	if (0 != circles.size()||percent>0.06)
	{

		if (0 != circles.size())
		{
			for (int i = 0; i < circles.size(); i++)
			{
				if (circles[i][2] > radius)
				{
					radius = circles[i][2];
					index = i;
				}
			}

			circleCenter.x = cvRound(circles[index][0]);
			circleCenter.y = cvRound(circles[index][1]);

			radius = cvRound(circles[index][2]);
		}

		else {
			circleCenter.x = src.cols/2;
			circleCenter.y = src.rows/2;
			radius = 7;
			sprintf(str_num, "%d", B_num);
			file_name = str_num + s2_B;
			imwrite(file_name, src);
		}
		return 0;
	}
	else {
		cout << "No match circle find!!!" << "****" << endl;   //没有找到圆
		return -1;
	}
}


int CircleFinder::circleCalculate2(Mat &src)
{
	/*radius = 0;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	float areaContour = 0;
	Point2f centerDetect;
	float radiusDetect;
	Point2f centerDetectMax;
	float radiusDetectMax = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		areaContour = contourArea(contours[i]);
		minEnclosingCircle(contours[i], centerDetect, radiusDetect);
		if (areaContour >= 0.95*PI*radiusDetect*radiusDetect && radiusDetect >= minRadius && radiusDetect <= maxRadius)
		{
			if (radiusDetect > radiusDetectMax)
			{
				radiusDetectMax = radiusDetect;
				centerDetectMax.x = centerDetect.x;
				centerDetectMax.y = centerDetect.y;
			}
		}

	}
	if (radiusDetectMax > 0)
	{
		radius = cvRound(radiusDetectMax);
		circleCenter.x = cvRound(centerDetectMax.x);
		circleCenter.y = cvRound(centerDetectMax.y);
		circle(src, circleCenter, radius, Scalar(255, 255, 0), 3, 8, 0);
		return 0;
	}
	else
	{
		cout << "No match circle find!!!" << endl;
		return -1;
	}*/
	int sum = 0;
	double percent = 0;
	for (int i = 0; i < src.rows - 1; i++)
	{
		for (int j = 0; j < src.cols - 1; j++)
		{
			if (src.at<uchar>(i, j) == 255) { sum = sum + 1; }

		}
	}
	percent = sum / (src.rows*src.cols);
	if (percent < 0.05) { return -1; }
	return 0;
}

float CircleFinder::circleDistance(Mat &src)    //返回圆的圆心到图像中心的距离
{
	float distance = 0;
	float distance_x = circleCenter.x - src.cols/2;
	float distance_y = circleCenter.y - src.rows/2;
	distance = sqrtf(distance_x*distance_x + distance_y * distance_y);
	if (distance_y < 0) 
	{
		distance = -distance;
	}
	return distance;
}

