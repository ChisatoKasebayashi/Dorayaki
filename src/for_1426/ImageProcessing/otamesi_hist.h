#include <opencv2\opencv_lib.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define reference_histgram_filename "reference_histgram.txt"

int click_x[2], click_y[2];
void mouse(int event ,int x ,int y ,int flags ,void *param);
ofstream fout("histogram_log.txt");

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
	float *referenceHistgram;
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
	void Outputhistogram();
	void clearColorHistgram();
	bool loadReferenceColorHistgram(const char *filename);
	void expressHistogram(int no_point);
	void VisualizeColor(float histgram, int hist, int  y, int u, int v);
};
