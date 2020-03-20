#include "queue.h"

using namespace std;
class VideoProcess {
private:
	//dnn::Net net;
	result_link_type *result_link;
	Mat frame;
	VideoCapture cap;
	VideoCapture cap1;
	int first_flag;
	int frame_count;
	vector<vector<Point>>basic_model;
public:
	VideoProcess();
	void run();
private:
	static void frame_fetch(VideoProcess *videoProcess);
	static void frame_process(VideoProcess *videoProcess);
	void init_base_contour(vector<vector<Point>> & vec, const Mat &frame);
	//void data_process(Mat &frame, VideoProcess *videoProcess);
	//int  otsu(IplImage* src);
	//void getMaxClass(const Mat &probBlob, int *classId, double *classProb);
};
