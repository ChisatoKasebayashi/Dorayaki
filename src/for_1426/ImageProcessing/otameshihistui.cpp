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
    ui->graphicsBallHist->setScene(&scene);
    ui->graphicsColorHist->setScene(&sceneCol);
    ui->graphicsReference->setScene(&sceneRef);
}

void OtameshiHistUI::connectSignals(){
    int ret;
    ret = connect(ui->checkCVImage, SIGNAL(stateChanged(int)),this ,SLOT(oncheckCVImage(int)));
    assert(ret);
    ret = connect(ui->pushStar, SIGNAL(clicked()),this, SLOT(onpushStar()));
    assert(ret);
    ret = connect(ui->comboImageName, SIGNAL(currentIndexChanged(int)),this ,SLOT(onComboImageChangedIndex()));
    assert(ret);
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
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString home = QDir::homePath();
    QString strDir = QFileDialog::getExistingDirectory(
        this,
        tr("select dir"),
        home, options);
    if ( !strDir.isEmpty() )
    {
        QDir dir( strDir );
            QStringList strlFilter;
            strlFilter << "*.png";
            QFileInfoList list = dir.entryInfoList( strlFilter, QDir::Files );
            for ( int i = 0; i < list.size(); i++ )
            {
                ui->comboImageName->addItem(strDir +"/" +list[i].fileName());
            }
    }
}

void OtameshiHistUI::onComboImageChangedIndex(){
    QImage VImage;
    VImage.load(ui->comboImageName->currentText());
    ui->labelImage->setPixmap(QPixmap::fromImage(VImage));
}

OtameshiHistUI::~OtameshiHistUI()
{
    delete ui;
}

void OtameshiHistUI::processingGetColorHistogram(){
    setScene();
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

void OtameshiHistUI::setScene(){
    scene.clear();
    sceneRef.clear();
    sceneCol.clear();

    QPen pgray(QColor(200, 200, 200));
    for(int y=-4; y<=4; y++) {
        scene.addLine(-200, y*20, 200, y*20, pgray);
        sceneRef.addLine(-200, y*20, 200, y*20, pgray);
        sceneCol.addLine(-200, y*20, 200, y*20, pgray);
    }
    for(int x=-32; x<=32; x++) {
        scene.addLine(x*6, -90, x*6, 90, pgray);
        sceneRef.addLine(x*6, -90, x*6, 90, pgray);
        sceneCol.addLine(x*6, -90, x*6, 90, pgray);
    }
}

void OtameshiHistUI::initHistogram(){
    radius = 0;
    img_origin = cvLoadImage(ui->comboImageName->currentText().toLocal8Bit(), CV_LOAD_IMAGE_COLOR);
    img = img_origin;
    histgram = new int[NUM_HISTGRAM];
    normalizedHistgram = new float[NUM_HISTGRAM];
    referenceHistgram = new float[NUM_HISTGRAM];
    clearColorHistgram();
    for(int i = 0; i < NUM_HISTGRAM; i ++){
        referenceHistgram[i] = 0;
        histgram[i] = 0;
    }
    loadReferenceColorHistgram(reference_histgram_filename);
    cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);

}

void OtameshiHistUI::destHistogram(){
    delete[] histgram;
    delete[] referenceHistgram;
    delete[] normalizedHistgram;
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

/*
* ImageProcessing::getColorHistgram
*/
int OtameshiHistUI::getColorHistgram(int x, int y, int r, int no_point)
{
        int x0 = max(x - r, 0);
        int y0 = max(y - r, 0);
        int x1 = min(x + r, img_origin->width);
        int y1 = min(y + r, img_origin->height);
        int search_width  = x1 - x0;
        int search_height = y1 - y0;
        int square_r = r * r;

        char *image = img_origin->imageData;

        if ((search_width == 0)||(search_height == 0)) return -1;

        for (int i = 0; i < no_point;){
            int xt = x0 + rand() % search_width;
            int yt = y0 + rand() % search_height;
            int square = (x - xt) * (x - xt) + (y - yt) * (y - yt);
            if (square >= square_r){
                continue;
            }

            unsigned char *p = (unsigned char *)&image[(yt * img_origin->width + xt) * 3];

            int b = p[0];
            int g = p[1];
            int r = p[2];
            int y,cb,cr;
            RGB2YCbCr(b,g,r,&y,&cb,&cr);

            //qDebug() << '['<<i<<']';
            //qDebug() << "BGR  ["<<b <<',' << g <<','<< r << ']';
            //qDebug() << "YCbCr["<<y <<',' << cb <<','<< cr << ']';

            //int hist = ((b >> 6) << 4) + ((g >> 6) << 2) + (r >> 6);
            int hist = polarCoordinatesHistogram(y,cb,cr);
            assert((hist < NUM_HISTGRAM)&&(hist >= 0));
            histgram[hist] ++;
            i++;
        }
        float res = 0.0f;
        double euc_res = 0.0f;

        for(int i = 0; i < NUM_HISTGRAM; i ++){
            normalizedHistgram[i] = (float)histgram[i] / no_point;
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
        }
        double theta = (InnerProduct(normalizedHistgram,referenceHistgram,NUM_HISTGRAM)/
                            sqrt(InnerProduct(normalizedHistgram,normalizedHistgram,NUM_HISTGRAM)*
                                        InnerProduct(referenceHistgram,referenceHistgram,NUM_HISTGRAM)));
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

void OtameshiHistUI::RGB2YCbCr(int b, int g, int r, int *y, int *cb,int *cr){
    *y = (77*r+150*g+29*b)>>8;
    *cb= ((b-*y)*144+32768)>>8;
    *cr= ((r-*y)*182+32768)>>8;
    *cb -= 128;
    *cr -= 128;

    *y = std::max(0, std::min(*y, 255));
    *cb = std::max(-128, std::min(*cb, 127));
    *cr = std::max(-128, std::min(*cr, 127));
}

int OtameshiHistUI::polarCoordinatesHistogram(int y, int cb, int cr){
    double r = sqrt(cb*cb + cr*cr);
    double rad = atan2(cr, cb);
    int theta = ((rad*180) / M_PI);
    if(theta<0)
        theta = 360+theta;
    //qDebug() << 'y' <<y<<'r'<<r;
    //qDebug() << "Cb"<<cb << "Cr"<<cr<< "rad"<<rad << "theta"<<theta;
    if(r < 30){
        if(y<50)
            return NUM_HISTGRAM-1;
        else if(150<y)
            return NUM_HISTGRAM-3;
        else
            return NUM_HISTGRAM-2;
    }
    else{
        theta = theta>>3;
        //qDebug() << "theta=" << theta;
        return theta;
    }
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
}

void OtameshiHistUI::plotColorHistgram(float ref, float nor, int cnt){
    float res = 0.0f;
    cnt = cnt - (NUM_HISTGRAM/2);
    QPen pHist(QColor(255,100,100),6);
    res = min(ref, nor);
    sceneCol.addLine(cnt*6+3, 100, cnt*6+3, 100-(int)(200*res), pHist);
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

