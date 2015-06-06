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
	IplImage *img_origin;
	IplImage *img;
	int x;
	int y;
	int radius;
	CvPoint center;

public:
	GetColorHistogram();
	~GetColorHistogram();
	void drawCircle(int radius, CvPoint center);
	void mouseEvent();
	int evaluateRadius();
//	void calcBallSize();
//	void initBallSize();
};