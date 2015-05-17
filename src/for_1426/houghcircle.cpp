#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\opencv_lib.hpp>

int main(int argc, char* argv[] )
{
	int i;
	float *p;
	double w=320,h=240;

	IplImage *src_img=0, *gray_img=0;
	CvMemStorage *storage;
	CvSeq *circles=0;
	CvCapture *capture=0;


	capture=cvCreateCameraCapture(0);

	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,w);
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,h);


	cvNamedWindow("circles",CV_WINDOW_AUTOSIZE);

	while(1){
		src_img=cvQueryFrame(capture);

		CvSize sizeOfImage=cvGetSize(src_img);

		gray_img=cvCreateImage(cvGetSize(src_img),IPL_DEPTH_8U,1);

		cvCvtColor(src_img,gray_img,CV_BGR2GRAY);

		cvSmooth(gray_img,gray_img,CV_GAUSSIAN,11,11,0,0);
		storage=cvCreateMemStorage(0);
		

		//HoughCircle(グレイスケール現画像、結果円ベクトル、　、解像度比の逆数、中心円の最少距離、閾値、中心円の閾値、最少半径、最大半径);
		circles=cvHoughCircles(gray_img,storage,CV_HOUGH_GRADIENT,1,50,100,100,0,0);

		for(i=0;i<circles->total;i++){
			p=(float*)cvGetSeqElem(circles,i);
			cvCircle(gray_img,cvPoint(cvRound(p[0]),cvRound(p[1])),3,CV_RGB(0,255,0),3,8,0);
			cvCircle(gray_img,cvPoint(cvRound(p[0]),cvRound(p[1])),cvRound(p[2]),CV_RGB(255,0,0),3,8,0);
		}


		cvShowImage("circles",gray_img);
		
		cvWaitKey(10);

	}
	cvDestroyWindow("circles");
	cvReleaseImage(&src_img);
	cvReleaseImage(&gray_img);
	cvReleaseMemStorage(&storage);

	return 0;
}



