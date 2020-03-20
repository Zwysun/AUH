#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class CircleFinder
{
private:
	int colorMode;
	int minDist;	//Ҫ���Բ�ε���СԲ�ļ��
	int minRadius;  //���Բ����С�뾶
	int maxRadius;  //���Բ�����뾶
	int circleLike; //����Բ�̶�
	int thresholdValue; //����ɫ��ɫ�ָ���ֵ

public:
	vector<Vec3f> circles; //������õ���Բ����
	Point circleCenter;    //��⵽Բ��Բ��
	int radius;            //��⵽Բ�İ뾶

	CircleFinder();
	void setParams(int colorInput, int minDistValue, int minRadiusValue, int maxRadiusValue, int circleLikeValue, int segThreshold);
	void segmentProcess(Mat &src, Mat &dst);
	void segmentProcess2(Mat &src, Mat &dst);
	int circleCalculate(Mat &src);
	int circleCalculate2(Mat &src);
	float circleDistance(Mat &src);
};