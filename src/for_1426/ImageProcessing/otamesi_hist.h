#include <opencv2\opencv_lib.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

int click_x[2], click_y[2];
void mouse(int event ,int x ,int y ,int flags ,void *param);

class GetColorHistogram{

private:
//	static const int BALL_SIZE_STEP = 10;
//	int *ballSize;
	static const int NUM_HISTGRAM = 64;
	IplImage *img_origin;
	IplImage *img;
	IplImage *img_ycrcb;
	int x;
	int y;
	int radius;
	CvPoint center;
	float *normalizedHistgram;
	int *histgram;

public:
	GetColorHistogram();
	~GetColorHistogram();
	void drawCircle(int radius, CvPoint center);
	void mouseEvent();
	int evaluateRadius();
//	void calcBallSize();
//	void initBallSize();
	int getColorHistgram(int x, int y, int r, int no_point);
	void clearColorHistgram();
};