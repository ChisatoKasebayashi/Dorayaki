#include "otameshihistui.h"
#include "ui_otameshihistui.h"

#define reference_histgram_filename "reference_histgram.txt"

int click_x[2], click_y[2];
void mouse(int event ,int x ,int y ,int flags ,void *param);
ofstream fout("histogram_log.txt");


OtameshiHistUI::OtameshiHistUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OtameshiHistUI)
{
    ui->setupUi(this);
    connectSignals();
}

void OtameshiHistUI::connectSignals(){
    int ret;
    ret = connect(ui->checkCVImage, SIGNAL(stateChanged(int)),this ,SLOT(oncheckCVImage(int)));
    assert(ret);
    ret = connect(ui->pushStar, SIGNAL(clicked()),this, SLOT(onpushStar()));
    assert(ret);
}

void OtameshiHistUI::oncheckCVImage(int state){
    if(state){
        ui->labeldebug->setText("checked");
        ui->checkCVImage->setEnabled(false);

        initHistogram();
        cvShowImage("Picture",img_origin);
        mouseEvent();
        cvWaitKey(0);
        drawCircle(radius, center);
        cvDestroyWindow("Picture");
        cvShowImage("Picture",img);
        cvWaitKey(0);
        cvDestroyWindow("Picture");

        ui->checkCVImage->setEnabled(true);
        ui->checkCVImage->setChecked(Qt::Unchecked);
    }
    else{
        ui->labeldebug->setText("unchecked");
    }
}

void OtameshiHistUI::onpushStar(){
    ui->labeldebug->setText("pushStar!");
}

OtameshiHistUI::~OtameshiHistUI()
{
    destHistogram();
    delete ui;
}

void OtameshiHistUI::initHistogram(){
    radius = 0;
    img_origin = cvLoadImage(ui->comboImageName->currentText().toLocal8Bit(), CV_LOAD_IMAGE_COLOR);
    img = img_origin;
    if(img_origin){
        img_ycrcb = cvCreateImage(cvGetSize(img_origin),8,3);
        cvCvtColor(img_origin, img_ycrcb, CV_BGR2YCrCb);
    }
    histgram = new int[NUM_HISTGRAM];
    normalizedHistgram = new float[NUM_HISTGRAM];
    referenceHistgram = new float[NUM_HISTGRAM];
    clearColorHistgram();
    for(int i = 0; i < NUM_HISTGRAM; i ++) referenceHistgram[i] = 0;
    loadReferenceColorHistgram(reference_histgram_filename);

}

void OtameshiHistUI::destHistogram(){
    delete[] histgram;
    delete[] referenceHistgram;
    delete[] normalizedHistgram;
    if(img_origin){
        cvReleaseImage(&img_ycrcb);
    }
}

/*
* imageProcessing::clearColorHistgram
*/
void OtameshiHistUI::clearColorHistgram()
    {
        for(int i = 0; i < NUM_HISTGRAM; i ++){
            histgram[i] = 0;
            normalizedHistgram[i] = 0;
        }
    }

bool OtameshiHistUI::loadReferenceColorHistgram(const char *filename)
{
    FILE *fp;
    if (NULL == (fp = fopen(filename, "rt"))) return false;

    for(int i = 0;  i < NUM_HISTGRAM; i ++){
        fscanf(fp, "%f", &referenceHistgram[i]);
    }
    fclose(fp);
    return true;
}

/*
* クリックで座標を取得するまで待機
*/
void OtameshiHistUI::mouseEvent()
{
    cvSetMouseCallback("Picture",mouse);
    cvWaitKey(0);
    radius = evaluateRadius();
}

/*
* 右クリック,左クリックで取得した座標から距離を算出
*/
int OtameshiHistUI::evaluateRadius()
{
    int deff[2];
    double result;
    deff[0] = abs(click_x[0] - click_x[1]);
    deff[1] = abs(click_y[0] - click_y[1]);
    result = pow(deff[0],2) + pow(deff[1],2);
    result = sqrt(result);
    return (int)result;
}

/*
* 円を描画
*  - radius 半径
*  - center 中心座標
*/
void OtameshiHistUI::drawCircle(int radius, CvPoint center)
{
    CvScalar color = CV_RGB(255 ,0 ,0);
    int thickness = 1;
    int linetype = 8;
    int shift = 3;
    cvCircle(img ,center, radius, color, thickness, linetype, shift);
}

void mouse(int event,int x, int y,int flags,void *param=NULL)
{
    switch(event)
    {
        case CV_EVENT_LBUTTONDOWN:
            click_x[0] = x;
            click_y[0] = y;
            break;

        case CV_EVENT_RBUTTONDOWN:
            click_x[1] = x;
            click_y[1] = y;
            break;

        default:
            break;
    }
}

