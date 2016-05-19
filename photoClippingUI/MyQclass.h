#ifndef MYQCLASS_H
#define MYQCLASS_H

#include <iostream>
#include <QObject>
#include <QWidget>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTime>

class MyQclass :public QWidget
{
    Q_OBJECT
public:
    QFileInfo selectFile(QString filter);
    QDir selectDir();
    QFileInfoList scanFiles(QString targetpath ,QString filter);
    QFileInfoList scanDirs(QString targetpath ,QString filter);
    QPixmap MatBGR2pixmap(cv::Mat src);
    QDir makeDirectory(QString targetPath, QString dirname);
    QString filenameGen(QString objectname, int num, QString extension=".png");
private:
};

#endif // MYQCLASS_H
