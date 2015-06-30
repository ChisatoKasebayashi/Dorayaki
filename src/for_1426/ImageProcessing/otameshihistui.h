#ifndef OTAMESHIHISTUI_H
#define OTAMESHIHISTUI_H
#define _USE_MATH_DEFINES

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <fstream>
#include <iomanip>
using namespace std;
using namespace cv;

namespace Ui {
class OtameshiHistUI;
}

class OtameshiHistUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit OtameshiHistUI(QWidget *parent = 0);
    ~OtameshiHistUI();

public slots:
    void oncheckCVImage(int state);
    void onpushStar();

private:
    Ui::OtameshiHistUI *ui;
    void connectSignals();
    static const int NUM_HISTGRAM = 67;
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
    QGraphicsScene scene;
    QGraphicsScene sceneRef;
    QGraphicsScene sceneCol;
    QGraphicsScene sceneEuc;
    QGraphicsScene sceneCos;
    QGraphicsScene sceneNor;

    void processingGetColorHistogram();
    void initHistogram();
    void destHistogram();
    void clearColorHistgram();
    bool loadReferenceColorHistgram(const char *filename);
    void mouseEvent();
    int evaluateRadius();
    void drawCircle(int radius, CvPoint center);
    int getColorHistgram(int x, int y, int r, int no_point);
    double InnerProduct(float vec1[], float vec2[],int n);
    void plotBallHistogram(float hist,int cnt);
    void plotReferenceHistogram(float hist,int cnt);
    void plotColorHistgram(float ref, float nor, int cnt);
    void plotEuclid(float ref, float nor, int cnt);
    void plotCosine(float ref, float nor, int cnt);
    void plotNormalize(float ref, float nor, int cnt);
    void RGB2YCbCr(int b, int g, int r, int *y, int *cb,int *cr);
    int polarCoordinatesHistogram(int y,int cb,int cr);

};

#endif // OTAMESHIHISTUI_H
