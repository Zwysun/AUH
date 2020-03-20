#include <opencv2/opencv.hpp>
#include <iostream>

#define RED 1
#define GREEN 2
#define BLUE 3
#define PI 3.14
using namespace cv;
using namespace std;

class LinePointFinder

{

private:
	//��⵽���߶ζ˵�
	Point pointBegin;
	Point pointEnd;

	//������
	int colorMode; //�����ɫģʽ
	int lineWidth; //������С�߿�
	int pointNum; //�����ֱ�ߵ���������
	int thresholdValue; //����ɫ��ֵ�ָ���ֵ

public:

	//��⵽�Ľ��
	float distanceResult;
	float angleResult;

	LinePointFinder();
	void setDectParams(int colorInput, int widthInput, int numInput, int segThreshold);
	void segmentProcess(Mat &src, Mat &dst);
	void segmentProcess2(Mat &src, Mat &dst);
	int calculatePoint(Mat &src, Point &begin, Point &end);
	void drawDetectedLines(cv::Mat& image, cv::Scalar color);
	void calculateResult(Mat &src, Point pointA, Point pointB);
};

class ConfigFileRead
{
private:

	ifstream configFile;

public:

	ConfigFileRead();
	int readConfigFile(string filePath);
	string readValue(string keyWords);
	int closeConfigFile();
};

