#include "otamesi_hist.h"
//#define ball_size(x,y) ballSize[(((int)y / BALL_SIZE_STEP) * img->width + (int)x)/ BALL_SIZE_STEP]


GetColorHistogram::GetColorHistogram()
	:radius(0)	,img_origin(cvLoadImage("142.png", CV_LOAD_IMAGE_COLOR))
	,img(img_origin)
	,img_ycrcb(cvCreateImage(cvGetSize(img_origin),8,3))
	,histgram(new int[NUM_HISTGRAM])
	,normalizedHistgram(new float[NUM_HISTGRAM])
	,referenceHistgram(new float[NUM_HISTGRAM])
{
	for(int i = 0; i < NUM_HISTGRAM; i ++) referenceHistgram[i] = 0;
	loadReferenceColorHistgram(reference_histgram_filename);
	clearColorHistgram();
	cvCvtColor(img_origin, img_ycrcb, CV_BGR2YCrCb);
	cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);
	cvShowImage("Picture",img_origin);
	mouseEvent();
	center = cvPoint(click_x[0], click_y[0]);
	drawCircle(radius, center);
	getColorHistgram(click_x[0], click_y[0], radius, 100);
	expressHistogram(0);
	cvShowImage("Picture",img);
	cvWaitKey(0);
}

GetColorHistogram::~GetColorHistogram()
{
	//delete[] ballSize;
	delete[] histgram;
	delete[] referenceHistgram;
	delete[] normalizedHistgram;
	cvReleaseImage(&img);
	cvDestroyWindow("Picture");
}

/*
 円を描画
  - radius 半径
  - center 中心座標
*/
void GetColorHistogram::drawCircle(int radius, CvPoint center)
{
	CvScalar color = CV_RGB(255 ,0 ,0);
	int thickness = 1;
	int linetype = 8;
	int shift = 3;
	cvCircle(img ,center, radius, color, thickness, linetype, shift);
}
/*
 クリックで座標を取得するまで待機
*/
void GetColorHistogram::mouseEvent()
{
	cvSetMouseCallback("Picture",mouse);
	cout << "2点をクリックする(LEFT:中心, RIGHT:円弧)" << endl;
	cvWaitKey(0);
	radius = evaluateRadius();
}

/*
 右クリック,左クリックで取得した座標から距離を算出
*/
int GetColorHistogram::evaluateRadius()
{
	int deff[2];
	double result;
	deff[0] = abs(click_x[0] - click_x[1]);
	deff[1] = abs(click_y[0] - click_y[1]);
	//cout << "deff[X] = " << deff[0] << "," << "deff[Y] = " << deff[1] << endl;
	result = pow(deff[0],2) + pow(deff[1],2);
	result = sqrt(result);
	//cout << "r = " << result << endl;
	return (int)result;
}

/*
 ImageProcessing::getColorHistgram
*/
int GetColorHistogram::getColorHistgram(int x, int y, int r, int no_point)
{
		int x0 = max(x - r, 0);
		int y0 = max(y - r, 0);
		int x1 = min(x + r, img_ycrcb->width);
		int y1 = min(y + r, img_ycrcb->height);
		int search_width  = x1 - x0;
		int search_height = y1 - y0;
		int square_r = r * r;
		
		char *image = img_ycrcb->imageData;

		if ((search_width == 0)||(search_height == 0)) return -1;

		for (int i = 0; i < no_point;){
			int xt = x0 + rand() % search_width;
			int yt = y0 + rand() % search_height;
			int square = (x - xt) * (x - xt) + (y - yt) * (y - yt);
			if (square >= square_r){
				continue;
			}
			
			unsigned char *p = (unsigned char *)&image[(yt * img_ycrcb->width + xt) * 3];
			
			int y = p[0];
			int u = p[1];
			int v = p[2];
			
			int hist = ((y >> 3) << 4) + ((u >> 6) << 2) + (v >> 6);
			assert((hist < 64)&&(hist >= 0));
			histgram[hist] ++;
			i++;
			//VisualizeColor((float) histgram[hist],hist, y, u, v);
		}
		float res = 0.0f;

		for(int i = 0; i < NUM_HISTGRAM; i ++){
			normalizedHistgram[i] = (float)histgram[i] / no_point;
			//cout << "[" << i <<"]  " << normalizedHistgram[i] << endl;
			res += min(normalizedHistgram[i], referenceHistgram[i]);
			
			if (!fout){
				cout << "開けないよ"<< endl;
				return 1;
			}
			fout << fixed << setprecision(6) << normalizedHistgram[i] << endl ;

		}
		cout << "score[" << res << "]" << endl;
		return 0;
}

void GetColorHistogram::Outputhistogram()
{
//汎用性の高い書き方模索中
}

/*
 imageProcessing::clearColorHistgram
*/
void GetColorHistogram::clearColorHistgram()
	{
		for(int i = 0; i < NUM_HISTGRAM; i ++){
			histgram[i] = 0;
			normalizedHistgram[i] = 0;
		}
	}

bool GetColorHistogram::loadReferenceColorHistgram(const char *filename)
{
	FILE *fp;
	if (NULL == (fp = fopen(filename, "rt"))) return false;

	for(int i = 0;  i < NUM_HISTGRAM; i ++){
		fscanf(fp, "%f", &referenceHistgram[i]);
	}
	fclose(fp);
	return true;
}

void GetColorHistogram::expressHistogram(int no_point)
{
	Mat hist_img=Mat(Size(330,400),CV_8UC3,Scalar(177,177,177));
	for(int i = 0; i < NUM_HISTGRAM; i ++){
		cv::rectangle(
            hist_img,
            cv::Point(5+(i*5),400),
			//cv::Point(0+(i*5),240-(3*i)),
            cv::Point(5+(i*5),400-(500*normalizedHistgram[i])),
            cv::Scalar::all(0),3,4,0);
		//cv::line(hist_img,Point(0+(i*5),240),Point(0+(i*5),240-(480*normalizedHistgram[i])),CV_RGB(0,51,204),5,4,0);
	}
	//cvSaveImage("histogram.png",hist_img);
	imshow("ヒストグラム",hist_img);
}

#if 0	//ボール半径改変中
/*
 ImageProcessing::initBallSize
 ImageProcessing::calcBallsize
*/
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


void GetColorHistogram::VisualizeColor(float histgram, int hist, int y, int u, int v){
    //Qpen circleColor(QColor(b,g,r),6);
    //circle.drawArc();
    //qDebug() << "ここから関数内"  ;
    //qDebug() << "r=" << r << "g=" << g << "b=" << b;
    cout<< "hist=" << hist << "hisrtgram[hist]=" << histgram << endl;

}

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
