#include <opencv2\opencv_lib.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

const char *filename="142.png";

void main(void)
  {
    IplImage *img = 0;
	img = cvLoadImage(filename,CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);
    cvShowImage("Picture",img);
    cvWaitKey(0);
  }