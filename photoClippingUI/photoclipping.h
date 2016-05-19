#ifndef PHOTOCLIPPING_H
#define PHOTOCLIPPING_H

#include <QWidget>
#include <MyQclass.h>
#include <MyQView.h>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QSettings>

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
    void onMouseMovedGraphicsImage(int x, int y ,Qt::MouseButton button);
    void onMouseReleasedGraphicImage(int x, int y ,Qt::MouseButton button);
    void onPushSkip();
    void onPushRevart();

private:
    Ui::photoclipping *ui;
    void connectSignals();
    MyQclass myq;
    int count;
    cv::Mat img_now;
    QFileInfoList imglist;
    QGraphicsScene scene;
    QString RecentImg;
    QSettings settings;
};

#endif // PHOTOCLIPPING_H
