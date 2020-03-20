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
	//检测到的线段端点
	Point pointBegin;
	Point pointEnd;

	//检测参数
	int colorMode; //检测颜色模式
	int lineWidth; //检测的最小线宽
	int pointNum; //待拟合直线的最少数量
	int thresholdValue; //超彩色阈值分割阈值

public:

	//检测到的结果
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

