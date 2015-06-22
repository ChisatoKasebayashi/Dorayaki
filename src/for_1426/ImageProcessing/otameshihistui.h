#ifndef OTAMESHIHISTUI_H
#define OTAMESHIHISTUI_H

#include <QMainWindow>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

//int click_x[2], click_y[2];
//void mouse(int event ,int x ,int y ,int flags ,void *param);
//ofstream fout("histogram_log.txt");

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
};

#endif // OTAMESHIHISTUI_H
