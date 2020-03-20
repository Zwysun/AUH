#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class CircleFinder
{
private:
	int colorMode;
	int minDist;	//要检测圆形的最小圆心间隔
	int minRadius;  //检测圆的最小半径
	int maxRadius;  //检测圆的最大半径
	int circleLike; //泛化圆程度
	int thresholdValue; //超彩色颜色分割阈值

public:
	vector<Vec3f> circles; //霍夫检测得到的圆集合
	Point circleCenter;    //检测到圆的圆心
	int radius;            //检测到圆的半径

	CircleFinder();
	void setParams(int colorInput, int minDistValue, int minRadiusValue, int maxRadiusValue, int circleLikeValue, int segThreshold);
	void segmentProcess(Mat &src, Mat &dst);
	void segmentProcess2(Mat &src, Mat &dst);
	int circleCalculate(Mat &src);
	int circleCalculate2(Mat &src);
	float circleDistance(Mat &src);
};