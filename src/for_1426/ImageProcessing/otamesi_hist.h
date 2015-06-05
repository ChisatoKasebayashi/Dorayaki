#include <opencv2\opencv_lib.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

int click_x, click_y;
void mouse(int event ,int x ,int y ,int flags ,void *param);

class GetColorHistogram{

private:
//	static const int BALL_SIZE_STEP = 10;
//	int *ballSize;
	int x;
	int y;
	int radius;
	CvPoint center;

public:
	IplImage *img;
	GetColorHistogram();
	~GetColorHistogram();
	void drawCircle(int radius, CvPoint center);
//	void calcBallSize();
//	void initBallSize();
};