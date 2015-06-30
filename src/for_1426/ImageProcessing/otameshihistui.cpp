#include "otameshihistui.h"
#include "ui_otameshihistui.h"

#define reference_histgram_filename "reference_histgram.txt"

int click_x[2], click_y[2];
void mouse(int event ,int x ,int y ,int flags ,void *param);
ofstream fout("histogram_log.txt");
ifstream fin;


OtameshiHistUI::OtameshiHistUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OtameshiHistUI)
{
    ui->setupUi(this);
    connectSignals();
    ui->graphicsBallHist->setScene(&scene);
    ui->graphicsColorHist->setScene(&sceneCol);
    ui->graphicsReference->setScene(&sceneRef);
    ui->visualizeColor->setScene(&sceneVisual);
    //ui->graphicsEuclid->setScene(&sceneEuc);
    //ui->graphicsCosine->setScene(&sceneCos);
    //ui->graphicsNormalize->setScene(&sceneNor);
    QPen pgray(QColor(200, 200, 200));
    for(int y=-4; y<=4; y++) {
        scene.addLine(-200, y*20, 200, y*20, pgray);
        sceneRef.addLine(-200, y*20, 200, y*20, pgray);
        sceneCol.addLine(-200, y*20, 200, y*20, pgray);
        sceneEuc.addLine(-200, y*20, 200, y*20, pgray);
        sceneCos.addLine(-200, y*20, 200, y*20, pgray);
        sceneNor.addLine(-200, y*20, 200, y*20, pgray);
    }
    for(int x=-32; x<=32; x++) {
        scene.addLine(x*6, -90, x*6, 90, pgray);
        sceneRef.addLine(x*6, -90, x*6, 90, pgray);
        sceneCol.addLine(x*6, -90, x*6, 90, pgray);
        sceneEuc.addLine(x*6, -90, x*6, 90, pgray);
        sceneCos.addLine(x*6, -90, x*6, 90, pgray);
        sceneNor.addLine(x*6, -90, x*6, 90, pgray);
    }
    for(int x1=0 ; x1<=64 ; x1++ ){
        sceneVisual.addLine(x1*20,-300,x1*20,300,pgray);
    }
}

void OtameshiHistUI::connectSignals(){
    int ret;
    ret = connect(ui->checkCVImage, SIGNAL(stateChanged(int)),this ,SLOT(oncheckCVImage(int)));
    assert(ret);
    ret = connect(ui->pushStar, SIGNAL(clicked()),this, SLOT(onpushStar()));
    assert(ret);
    ret = connect(ui->Chisato,SIGNAL(clicked(bool)),this,SLOT(Chisato()));
}

void OtameshiHistUI::oncheckCVImage(int state){
    if(state){
        ui->labeldebug->setText("checked");
        ui->checkCVImage->setEnabled(false);
        processingGetColorHistogram();
        ui->checkCVImage->setEnabled(true);
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
    delete ui;
}

void OtameshiHistUI::processingGetColorHistogram(){
    initHistogram();
    cvShowImage("Picture",img_origin);
    mouseEvent();
    center = cvPoint(click_x[0], click_y[0]);
    drawCircle(radius, center);
    getColorHistgram(click_x[0], click_y[0], radius, 100);
    cvDestroyWindow("Picture");
    cvShowImage("Picture",img);
    cvWaitKey(0);
    cvDestroyWindow("Picture");
    destHistogram();
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
    cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);

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

int OtameshiHistUI::loadReferenceColorHistgram(const char *filename)
{
    fin.open(filename);
    if(fin.fail()){
        QMessageBox::critical(this, tr("Error"), tr("reference_histgram.txt cannot find."));
    }
    for(int i = 0; i< NUM_HISTGRAM; i++){
        fin>> referenceHistgram[i];
    }
    return 0;
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

/*
* ImageProcessing::getColorHistgram
*/
int OtameshiHistUI::getColorHistgram(int x, int y, int r, int no_point)
{
        int x0 = max(x - r, 0);
        int y0 = max(y - r, 0);
        int x1 = min(x + r, img_ycrcb->width);
        int y1 = min(y + r, img_ycrcb->height);
        int search_width  = x1 - x0;
        int search_height = y1 - y0;
        int square_r = r * r;

        char *image = img_ycrcb->imageData;
        char *img = img_origin->imageData;

        if ((search_width == 0)||(search_height == 0)) return -1;

        for (int i = 0; i < no_point;){
            int xt = x0 + rand() % search_width;
            int yt = y0 + rand() % search_height;
            int square = (x - xt) * (x - xt) + (y - yt) * (y - yt);
            if (square >= square_r){
                continue;
            }

            unsigned char *p = (unsigned char *)&image[(yt * img_ycrcb->width + xt) * 3];
            unsigned char *t = (unsigned char *)&img[(yt * img_origin->width + xt) *3];

            int y = p[0];
            int u = p[1];
            int v = p[2];

            int b = t[0];
            int g = t[1];
            int r = t[2];

            //qDebug() << "r=" << r << "g=" << g << "b=" << b;

            int hist = ((y >> 6) << 4) + ((u >> 6) << 2) + (v >> 6);
            assert((hist < 64)&&(hist >= 0));
            histgram[hist] ++;
            i++;
            //qDebug() << "hist=" << hist << "hisrtgram[hist]=" << histgram[hist];
            VisualizeColor((float) histgram[hist],hist, b, g, r);
        }

        float res = 0.0f;
        double euc_res = 0.0f;

        for(int i = 0; i < NUM_HISTGRAM; i ++){
            normalizedHistgram[i] = (float)histgram[i] / no_point;
            //qDebug() << histgram[i] ;
            res += min(normalizedHistgram[i], referenceHistgram[i]);
            euc_res += sqrt(pow(normalizedHistgram[i]-referenceHistgram[i],2));

            if (!fout){
                QMessageBox::critical(this, tr("Error"), tr("File cannot open."));
                return 1;
            }
            fout << fixed << setprecision(6) << normalizedHistgram[i] << endl ;



            plotBallHistogram(normalizedHistgram[i],i);
            plotReferenceHistogram(referenceHistgram[i],i);
            plotColorHistgram(referenceHistgram[i],normalizedHistgram[i],i);
            //plotEuclid(referenceHistgram[i],normalizedHistgram[i],i);
            //plotCosine(referenceHistgram[i],normalizedHistgram[i],i);
            plotNormalize(referenceHistgram[i],normalizedHistgram[i],i);
        }
        double theta = (InnerProduct(normalizedHistgram,referenceHistgram,64)/
                            sqrt(InnerProduct(normalizedHistgram,normalizedHistgram,64)*
                                        InnerProduct(referenceHistgram,referenceHistgram,64)));
        ui->lcdScoreColorHist->display(res);
        ui->lcdScoreEuclid->display(euc_res);
        ui->lcdScoreCosine->display(theta);
        return 0;
}

double OtameshiHistUI::InnerProduct(float vec1[], float vec2[],int n){
    double s=0.0f;
    for(int i=0;i<n;i++){
        s +=vec1[i]*vec2[i];
    }
    return s;

}

void OtameshiHistUI::plotBallHistogram(float hist, int cnt){
    cnt = cnt - (NUM_HISTGRAM/2);
    QPen pHist(QColor(255,0,0),6);
    scene.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*hist), pHist);
}
void OtameshiHistUI::plotReferenceHistogram(float hist, int cnt){
    cnt = cnt - (NUM_HISTGRAM/2);
    QPen pHist(QColor(0,0,255),6);
    sceneRef.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*hist), pHist);
    sceneCol.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*hist), pHist);
    sceneEuc.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*hist), pHist);
    sceneCos.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*hist), pHist);
    sceneNor.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*hist), pHist);
}

void OtameshiHistUI::plotColorHistgram(float ref, float nor, int cnt){
    float res = 0.0f;
    cnt = cnt - (NUM_HISTGRAM/2);
    QPen pHist(QColor(255,100,100),6);
    res = min(ref, nor);
    sceneCol.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*res), pHist);
}

void OtameshiHistUI::plotEuclid(float ref, float nor, int cnt){
    double res = 0.0f;
    cnt = cnt - (NUM_HISTGRAM/2);
    QPen pHist(QColor(100,255,100),6);
    res = sqrt( pow((ref-nor),2));
    sceneEuc.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*res), pHist);
}

void OtameshiHistUI::plotCosine(float ref, float nor, int cnt){


}

void OtameshiHistUI::plotNormalize(float ref, float nor, int cnt){
}

void OtameshiHistUI::Chisato(){
    QMessageBox::information(this,"seki","seki");
}
void OtameshiHistUI::VisualizeColor(float histgram, int hist, int b, int g, int r){
    QPen pcolor(QColor(r,g,b),20);
    float a=histgram-1;
    if(a==-1){
        sceneVisual.addLine(20*hist,290-20*(histgram),20*hist,290-20*histgram,pcolor);
    }
    else{
       sceneVisual.addLine(20*hist,290-20*(histgram-1),20*hist,290-20*histgram,pcolor);
    }
    //Qpen circleColor(QColor(b,g,r),16);
    //circle.drawArc();
    //qDebug() << "ここから関数内"  ;
    //qDebug() << "r=" << r << "g=" << g << "b=" << b;
    qDebug() <<"hist=" << hist << "hisrtgram[hist]=" << histgram;

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


