#include "LineDetect.hpp" 
#include <vector>

using namespace cv;
using namespace std;

LinePointFinder::LinePointFinder() :colorMode(0), lineWidth(20), pointNum(50) {};

void LinePointFinder::setDectParams(int colorInput, int widthInput, int numInput, int segThreshold)//����Ҫ������ɫ���ͣ��߿���Сֵ�����ٵ������ָ���ֵ�Ȳ���

{

	colorMode = colorInput;

	lineWidth = widthInput;

	pointNum = numInput;

	thresholdValue = segThreshold;


}

void LinePointFinder::segmentProcess(Mat &src, Mat &dst) //���ݲ�ɫ�ռ�ģ��ʵ��ͼ��ķָ�
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
			else //red color
			{
				temp = temp_r * 2 - temp_g - temp_b;
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

void LinePointFinder::segmentProcess2(Mat &src, Mat &dst)  //����HSV
{
	Mat imgHSV;
	int hValueMin, hValueMax, sValueMin, sValueMax, vValueMin, vValueMax;

	switch (colorMode)
	{
	case RED:
		hValueMin = 150;
		hValueMax = 360;
		sValueMin = 0;
		sValueMax = 80;
		vValueMin = 230;
		vValueMax = 255;
		break;
	case GREEN:
		hValueMin = 30;
		hValueMax = 90;
		sValueMin = 43;
		sValueMax = 255;
		vValueMin = 46;
		vValueMax = 255;
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
		hValueMin = 150;
		hValueMax = 360;
		sValueMin = 43;
		sValueMax = 255;
		vValueMin = 46;
		vValueMax = 255;
		break;
	}

	cvtColor(src, imgHSV, CV_BGR2HSV);   //��RGB��ʽת��ΪHSV��ʽ
	if (RED == colorMode || 0 == colorMode || 4 == colorMode)
	{
		Mat dst1, dst2;
		inRange(imgHSV, Scalar(hValueMin, sValueMin, vValueMin), Scalar(hValueMax, sValueMax, vValueMax), dst1);   //����HSVɸѡ��һ������
		//inRange(imgHSV, Scalar(0, sValueMin, vValueMin), Scalar(51, sValueMax, vValueMax), dst2);                  //����HSVɸѡ���ڶ�������
		dst = dst1;
		//imshow("dts", dst);
	}
	else
	{
		inRange(imgHSV, Scalar(hValueMin, sValueMin, vValueMin), Scalar(hValueMax, sValueMax, vValueMax), dst);
	}
}

int LinePointFinder::calculatePoint(Mat &src, Point &begin, Point &end) //�Էָ�ͼ�������̬ѧ������ϵõ�ֱ�ߣ������߶γ��ȶ�ֱ�߽��нضϵõ��߶����˵�
{
	vector<Point> findPoints;
	vector<Point> pointsX;
	vector<Point> pointsY;

	int i = 0;
	int j = 0;
	int k = 0;
	int xLeft;
	int xRight;
	int yUp;
	int yDowm;
	Point pointX;
	Point pointY;

	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));  //����ָ����״��ͼ�Σ�������RECT���ں˵ĳߴ磬5��5

	dilate(src, src, element);
	morphologyEx(src, src, MORPH_CLOSE, element);  
	//morphologyEx(src, src, MORPH_OPEN, element);

	vector<vector<Point>> lineContours;
	vector<Vec4i> hierarchy;
	Mat mask1;
	Mat mask2;
	mask1 = Mat::zeros(src.size(), CV_8UC1);
	mask2 = Mat::zeros(src.size(), CV_8UC1);

	findContours(src, lineContours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //CV_RETR_EXTERNAL��Χ������CV_CHAIN_APPROX_SIMPLE ���������յ���Ϣ

	float areaContour = 0;
	int index1 = 0;
	int index2 = 0;
	float area_record[100] = { 0 };
	float temp_record[100] = { 0 };
	for (int i = 0; i < lineContours.size(); i++)
	{
			area_record[i] = contourArea(lineContours[i]);  //��¼���е����
			temp_record[i] = area_record[i];
	}
	//cout << "size"<<lineContours.size() << endl;
	sort(temp_record, temp_record + lineContours.size());   //����������������������
	int areamin = 1000;
	//cout << "���" << temp_record[lineContours.size()-1] << endl;
	//cout << "  " << endl;
	//cout << "  " << endl;
	//cout << "�δ�" << temp_record[lineContours.size() - 2] << endl;
	if (temp_record[lineContours.size() - 1] < areamin && temp_record[lineContours.size() - 2] < areamin)
	{
		cout << "No line" << endl;
		return -1;
	}

	for (int i = 0; i < lineContours.size(); i++)
	{
		if (area_record[i] == temp_record[lineContours.size() - 1]) { index1 = i; }
		if (area_record[i] == temp_record[lineContours.size() - 2]) { index2 = i; }
	}

	if (temp_record[lineContours.size() - 2]<1000) {
		index2 = index1;
	}
	Rect rectContour1 = boundingRect(lineContours[index1]);
	Rect rectContour2 = boundingRect(lineContours[index2]);

	mask1(rectContour1).setTo(255);
	mask2(rectContour2).setTo(255);
	Mat imageMask1, imageMask2;
	Mat src2 = src.clone();
	src.copyTo(imageMask1, mask1);
	src2.copyTo(imageMask2, mask2);
	src = imageMask1.clone();
	src2 = imageMask2.clone();
	//src = src + src2;         //������һ����ʾ
	//imshow("zjz", src+src2);    //������һ����ʾ

	//��X�����������ɨ��
	int sum_x1 = 0;
	int sum_x2 = 0;
	for (i = 0; i < src.rows; i++) {
		xLeft = 0;
		xRight = src.cols - 1;


		for (j = 0; j < src.cols - lineWidth && j >= 0; j++)
		{
			if (src.at<uchar>(i, j) == 255)
			{
				xLeft = j;
				sum_x1 = sum_x1 + xLeft;
				break;
			}
		}
		for (j = 0; j < src.cols - lineWidth && j >= 0; j++)
		{
			if (src2.at<uchar>(i, j) == 255)
			{
				xLeft = j;
				sum_x2 = sum_x2 + xLeft;
				break;
			}
		}


		/*for (j =src.cols-1; j >=lineWidth; j--)
		{
			if (src.at<uchar>(i, j) == 255)
			{
				xLeft = j;
				sum_x1 = sum_x1 + xLeft;
				break;
			}
		}
		for (j = src.cols - 1; j >= lineWidth; j--)
		{
			if (src2.at<uchar>(i, j) == 255)
			{
				xLeft = j;
				sum_x2 = sum_x2 + xLeft;
				break;
			}
		}*/


	}




	//��������ɨ���Ǵ��ڣ���������ɨ����С��
	if (sum_x1 > sum_x2) {
		src = src2;
	}
	//imshow("Left", src);  //��ɸѡ������������ʾ����


	//��X����ɨ��
	for (i = 0; i < src.rows; i++)
	{
		xLeft = 0;
		xRight = src.cols - 1;
		for (j = 0; j < src.cols - lineWidth && j >= 0; j++)
		{
			if (src.at<uchar>(i, j) == 255)
			{
				xLeft = j;
				break;
			}
		}
		for (j = src.cols - 1; j >= lineWidth && j < src.cols; j--)
		{
			if (src.at<uchar>(i, j) == 255)
			{
				xRight = j;
				break;
			}
		}
		if (xRight >= xLeft + lineWidth && xRight <= xLeft + 7 * lineWidth)
		{
			pointX.x = (xLeft + xRight) / 2;
			pointX.y = i;
			pointsX.push_back(pointX);
		}

	}

	//��Y����ɨ��
	for (i = 0; i < src.cols; i++)
	{
		yUp = 0;
		yDowm = src.rows - 1;
		for (j = 0; j < src.rows - lineWidth && j >= 0; j++)
		{
			if (src.at<uchar>(j, i) == 255)
			{
				yUp = j;
				break;
			}
		}
		for (j = src.rows - 1; j >= lineWidth && j < src.rows; j--)
		{
			if (src.at<uchar>(j, i) == 255)
			{
				yDowm = j;
				break;
			}
		}
		if (yDowm >= yUp + lineWidth && yDowm <= yUp + 7 * lineWidth)
		{
			pointY.x = i;
			pointY.y = (yUp + yDowm) / 2;
			pointsY.push_back(pointY);
		}

	}

	if (pointsX.size() < pointNum && pointsY.size() < pointNum)
	{
		cout << "No line find!!!" << endl;
		return -1;
	}
	Vec4f line_detect;

	if (pointsX.size() >= pointsY.size())
		findPoints = pointsX;
	else
		findPoints = pointsY;

	int  len = findPoints.size();

	fitLine(findPoints, line_detect, CV_DIST_L2, 0, 0.01, 0.01);

	for (i = 0; i < len - 1; i++)
		line(src, findPoints[i], findPoints[i + 1], Scalar(128, 0, 0));

	int x0 = line_detect[2];
	int y0 = line_detect[3];

	int pointDisX = findPoints[len - 1].x - findPoints[0].x;
	int pointDisY = findPoints[len - 1].y - findPoints[0].y;

	int pointDis = pow((pointDisX*pointDisX + pointDisY * pointDisY), 0.5);

	pointBegin.x = x0 + pointDis / 2 * line_detect[0];
	pointBegin.y = y0 + pointDis / 2 * line_detect[1];

	pointEnd.x = x0 - pointDis / 2 * line_detect[0];
	pointEnd.y = y0 - pointDis / 2 * line_detect[1];

	int length = 0;
	length = (pointBegin.x-pointEnd.x)*(pointBegin.x-pointEnd.x)+(pointBegin.y-pointEnd.y)*(pointBegin.y-pointEnd.y);
	if (length<6400){return -1;}         //�޶����ֱ�ߵĳ��ȣ�����С��80�������϶�

	if (pointBegin.y > pointEnd.y)
	{
		begin.x = pointBegin.x;
		begin.y = pointBegin.y;

		end.x = pointEnd.x;
		end.y = pointEnd.y;
	}
	else if (pointBegin.y < pointEnd.y)
	{
		begin.x = pointEnd.x;
		begin.y = pointEnd.y;

		end.x = pointBegin.x;
		end.y = pointBegin.y;

	}
	else
	{
		if (pointBegin.x <= pointEnd.x)
		{
			begin.x = pointBegin.x;
			begin.y = pointBegin.y;

			end.x = pointEnd.x;
			end.y = pointEnd.y;
		}
		else
		{
			begin.x = pointEnd.x;
			begin.y = pointEnd.y;

			end.x = pointBegin.x;
			end.y = pointBegin.y;
		}
	}

	return 0;
}



void LinePointFinder::drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 0, 0)) //��ͼ���л�����⵽��ֱ��

{
	cv::line(image, pointBegin, pointEnd, color);

}

void LinePointFinder::calculateResult(Mat &src, Point pointA, Point pointB) //����ͼ�����ĵ���⵽��ֱ�ߵľ����ֱ�߷���ǣ�ֱ�߷���Ϊ pointA->pointB
{
	int width = src.cols;
	int height = src.rows;

	Point center;
	center.x = cvRound(width / 2);
	center.y = cvRound(height / 2);

	int A = 0, B = 0, C = 0;
	A = pointA.y - pointB.y;
	B = pointB.x - pointA.x;
	C = pointA.x*pointB.y - pointA.y*pointB.x;

	distanceResult = -((float)(A*center.x + B * center.y + C)) / ((float)sqrtf(A*A + B * B));

	float temp = (float)(pointA.y - pointB.y) / (float)sqrtf((pointB.x - pointA.x)*(pointB.x - pointA.x) + (pointB.y - pointA.y)*(pointB.y - pointA.y));

	if (pointB.x >= pointA.x)
		angleResult = acosf(temp) * 180 / PI;
	else
		angleResult = -acosf(temp) * 180 / PI;

}



ConfigFileRead::ConfigFileRead(){}

int ConfigFileRead::readConfigFile(string filePath)//��ȡ���ز��������ļ�
{
	configFile.open(filePath.c_str());
	if (configFile.is_open())
	{
		cout << "read config file successed!!!" << endl;
		return 0;
	}
	else
	{
		cout << "read config file failed!!!" << endl;
		return -1;
	}

}

string ConfigFileRead::readValue(string keyWords)//��ȡ���ز��������ļ�����ӦKeyWords��ֵ
{
	if (!configFile.is_open())
	{
		return{};
	}
	streampos pos = configFile.tellg();
	string strLine;
	string value;
	while (!configFile.eof())
	{
		getline(configFile, strLine);

		if ('#' == strLine[0])
		{
			continue;
		}

		size_t pose = strLine.find('=');
		string key = strLine.substr(0, pose);

		if (key == keyWords)
		{
			value = strLine.substr(pose + 1);
			configFile.seekg(pos);
			return value;
		}

	}
	configFile.seekg(pos);
	return{};
}

int ConfigFileRead::closeConfigFile()//�رձ��������ļ�
{
	configFile.close();
	if (configFile.is_open())
		return -1;
	return 0;
}
