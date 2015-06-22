#include "otameshihistui.h"
#include "ui_otameshihistui.h"



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

        img = cvLoadImage(ui->comboImageName->currentText().toLocal8Bit(), CV_LOAD_IMAGE_COLOR);
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
    cvReleaseImage(&img);
    delete ui;
}

