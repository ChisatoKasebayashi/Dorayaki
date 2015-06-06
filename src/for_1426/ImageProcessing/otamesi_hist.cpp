#include "otamesi_hist.h"
//#define ball_size(x,y) ballSize[(((int)y / BALL_SIZE_STEP) * img->width + (int)x)/ BALL_SIZE_STEP]


GetColorHistogram::GetColorHistogram()
	:radius(0)
	,img_origin(cvLoadImage("142.png", CV_LOAD_IMAGE_COLOR))
	,img(img_origin)
{
	cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);
	cvShowImage("Picture",img);
	mouseEvent();
	center = cvPoint(click_x[0], click_y[0]);
	drawCircle(radius, center);
	cvShowImage("Picture",img);
	cvWaitKey(0);
}

GetColorHistogram::~GetColorHistogram()
{
	cvReleaseImage(&img);
	cvDestroyWindow("Picture");
}

void GetColorHistogram::drawCircle(int radius, CvPoint center)
{
	CvScalar color = CV_RGB(255 ,0 ,0);
	int thickness = 1;
	int linetype = 8;
	int shift = 3;
	cvCircle(img ,center, radius, color, thickness, linetype, shift);
}

void GetColorHistogram::mouseEvent()
{
	cvSetMouseCallback("Picture",mouse);
	cout << "2点をクリックする" << endl;
	cvWaitKey(0);
	radius = evaluateRadius();
}


int GetColorHistogram::evaluateRadius()
{
	int deff[1];
	double result;
	deff[0] = abs(click_x[0] - click_x[1]);
	deff[1] = abs(click_y[0] - click_y[1]);
	cout << "deff[X] = " << deff[0] << "," << "deff[Y] = " << deff[1] << endl;
	result = pow(deff[0],2) + pow(deff[1],2);
	result = sqrt(result);
	cout << "r = " << result << endl;
	return (int)result;
}

#if 0	//ボール半径改変中

void GetColorHistogram::initBallSize(){
	int num = (img->width * img->height) / (BALL_SIZE_STEP * BALL_SIZE_STEP);
	ballSize = new int[num];
	for(int i = 0; i < num; i ++) ballSize[i] = 0;
}

void GetColorHistogram::calcBallSize()
{
		for(int y = 0; y < img->height; y += BALL_SIZE_STEP){
			for(int x = 0; x < img->width; x += BALL_SIZE_STEP){
				int fx, fy, fz = 500, ball_diameter = 200;
				float angle2dot = 5.1f;
				if (detectObject[0]->convertFieldPos(x/2, (img->height-y)/2, &fx, &fy) == 0){
					float length = sqrt((float)(fx * fx + fy * fy + fz * fz));
					float angle = atan2(ball_diameter / 2, length) * 2.0f * 180.0f / 3.14159f;
					float ball_diameter_dot = angle * angle2dot;
					ball_size(x, y) = (int)ball_diameter_dot;
				} else {
					ball_size(x, y) = 0;
				}
			}
		}
#if 0			
		FILE * fp = fopen("ball_size.csv", "wt");
		for(int y = 0; y < cap_height; y += BALL_SIZE_STEP){
			for(int x = 0; x < img->width; x += BALL_SIZE_STEP){
				fprintf(fp, "%d,", ball_size(x, y));
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
#endif
	}

#endif

int main(){
	cout << "start" << endl;
	GetColorHistogram BallHistogram;
	cout << "end" << endl;
	return 0;
}

void mouse(int event,int x, int y,int flags,void *param=NULL)
{
	switch(event)
	{
		case CV_EVENT_LBUTTONDOWN:
			std::cout<<"click[0]  "<<x<<","<<y<<"\n";
			click_x[0] = x;
			click_y[0] = y;
			break;
		
		case CV_EVENT_RBUTTONDOWN:
			std::cout<<"click[1]  "<<x<<","<<y<<"\n";
			click_x[1] = x;
			click_y[1] = y;
			break;

		default:
			break;
	}
}