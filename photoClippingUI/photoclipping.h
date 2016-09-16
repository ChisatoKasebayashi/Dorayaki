#ifndef PHOTOCLIPPING_H
#define PHOTOCLIPPING_H

#include <QWidget>
#include <MyQclass.h>
#include <MyQView.h>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QSettings>
#include <fstream>

namespace Ui {
class photoclipping;
}

class photoclipping : public QWidget
{
    Q_OBJECT

public:
    explicit photoclipping(QWidget *parent = 0);
    ~photoclipping();
    int drawImage(QString filepath);
    void CorrectCoordinatesOfOutside(int &x, int &y);
    void photoSaveImage(cv::Mat src);
    void setFileList(QString dirpath);

private slots:
    void onPushSelectFolder();
    void onPushSaveto();
    void onMouseMovedGraphicsImage(int x, int y ,Qt::MouseButton button);
    void onMouseReleasedGraphicImage(int x, int y ,Qt::MouseButton button);
    void onPushSkip();
    void onPushRevert();
    void currentIndexChangedLabel();

private:
    Ui::photoclipping *ui;
    void connectSignals();
    void outputtxt();
    MyQclass myq;
    int count;
    int save_count;
    cv::Mat img_now;
    QFileInfoList imglist;
    QGraphicsScene scene;
    std::vector<QString> RecentImg;
    QSettings settings;
    std::vector<QString> points;
    struct object_tmp
    {
        QString label;
        QString center;
        QString box;
    };
    struct image_tmp
    {
        QString fn;
        QString filename;
        QString size;
        QString database;
        QString groundtruth;
        std::vector<object_tmp> obj_tmp;
    };
    std::vector<image_tmp> img_tmp;
    void wheelEvent(QWheelEvent *pEvent);
    struct cursur_point
    {
        int x;
        int y;
    }c_point;
    QDir working_directory;
    void updatescene();
};

#endif // PHOTOCLIPPING_H
