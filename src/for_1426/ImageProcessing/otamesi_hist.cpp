#include <opencv2\opencv_lib.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

const char *filename="142.png";

void mouse(int event,int x,int y,int flags,void*param);


int main(){
	IplImage *img =0;
	img=cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);
	cvShowImage("Picture",img);
	cvSetMouseCallback("Picture",mouse);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Picture");

	return 0;
}
void mouse (int event,int x, int y,int flags,void *param=NULL)
{
	switch(event)
	{
		case CV_EVENT_LBUTTONDOWN:
			std::cout<<x<<","<<y<<"\n";
			break;
		
		case CV_EVENT_RBUTTONDOWN:
			std::cout<<x<<","<<y<<"\n";
			break;

		default:
			break;
	}
}