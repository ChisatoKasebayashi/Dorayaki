#include "photoclipping.h"
#include "ui_photoclipping.h"

photoclipping::photoclipping(QWidget *parent) :
    QWidget(parent),
    settings(QDir::homePath()+"/photoClipping.ini", QSettings::IniFormat),
    ui(new Ui::photoclipping)
{
    ui->setupUi(this);
    connectSignals();
    ui->graphicsImage->setScene(&scene);
    count = settings.value("SAVECOUNT",0).toInt();
    if(settings.value("IMAGEDIR").toString().size())
    {
        QString imagedir = settings.value("IMAGEDIR").toString();
        setFileList(imagedir);
    }
    if(settings.value("SAVEDIR",QDir::homePath() + "/Pictures").toString().size())
    {
        QString saveto = settings.value("SAVEDIR",QDir::homePath() + "/Pictures").toString();
        settings.remove("SAVEDIR");
        ui->comboSaveto->insertItem(0, saveto);
        ui->comboSaveto->setCurrentIndex(0);
        myq.makeDirectory(saveto, "Annotations");
        myq.makeDirectory(saveto, "YOLO_Annotations");
    }
    currentIndexChangedLabel();
}

photoclipping::~photoclipping()
{
    outputtxt();
    settings.setValue("SAVEDIR",ui->comboSaveto->currentText());
    settings.setValue("IMAGEDIR",ui->lineSelectFolder->text());
    settings.setValue("SAVECOUNT",count);
    delete ui;
}

void photoclipping::connectSignals()
{
    connect(ui->pushSelectFolder, SIGNAL(clicked()), this, SLOT(onPushSelectFolder()));
    connect(ui->graphicsImage, SIGNAL(mouseMoved(int,int ,Qt::MouseButton)), this, SLOT(onMouseMovedGraphicsImage(int,int ,Qt::MouseButton)));
    connect(ui->graphicsImage, SIGNAL(mouseReleased(int,int, Qt::MouseButton)), this, SLOT(onMouseReleasedGraphicImage(int,int, Qt::MouseButton)));
    connect(ui->pushSkip, SIGNAL(clicked()), this, SLOT(onPushSkip()));
    connect(ui->pushRevert, SIGNAL(clicked()), this, SLOT(onPushRevert()));
    connect(ui->pushSaveto, SIGNAL(clicked()), this, SLOT(onPushSaveto()));
    connect(ui->comboLabel, SIGNAL(currentIndexChanged(int)),this, SLOT(currentIndexChangedLabel()));
}

void photoclipping::onPushSelectFolder()
{
    count = 0;
    QDir dir = myq.selectDir();
    if(dir.exists())
    {
        setFileList(dir.path());
    }
}

void photoclipping::onPushSaveto()
{
    QDir dir = myq.selectDir();
    ui->comboSaveto->insertItem(0,dir.path());
    ui->comboSaveto->setCurrentIndex(0);
    currentIndexChangedLabel();
    myq.makeDirectory(dir.path(), "Annotations");
    myq.makeDirectory(dir.path(), "YOLO_Annotations");
}

void photoclipping::setFileList(QString dirpath)
{
    ui->lineSelectFolder->setText(dirpath);
    working_directory.setCurrent(dirpath);
    imglist = myq.scanFiles(dirpath,"*.png");
    ui->labelImageNum->setText(QString("%1 / %2").arg(count).arg(imglist.size()));
    if(count < imglist.size())
    {
        drawImage(imglist[count].filePath());
    }
    RecentImg.resize(imglist.size());
}

void photoclipping::onMouseMovedGraphicsImage(int x,int y ,Qt::MouseButton button)
{
    if(count < imglist.size())
    {
        CorrectCoordinatesOfOutside(x,y);
        c_point.x = x;
        c_point.y = y;
        ui->label_axis->setText(QString("x:%1 y:%2").arg(x).arg(y));
        updatescene();
    }
}

void photoclipping::onMouseReleasedGraphicImage(int x, int y ,Qt::MouseButton button)
{
    image_tmp tmp;
    object_tmp tmp2;
    yolo_tmp tmp3;
    if(count < imglist.size() && button == Qt::LeftButton)
    {
        CorrectCoordinatesOfOutside(x,y);
        scene.addRect(x - ui->spinSize->value()/2
                      ,y - ui->spinSize->value()/2
                      ,ui->spinSize->value(),ui->spinSize->value()
                      ,QPen(QColor(255,0,0)));
        points.push_back(QString("%1 %2 %3").arg(imglist[count].fileName()).arg(x).arg(y));
        ui->labelPreview->setPixmap(myq.MatBGR2pixmap(img_now).copy(x - ui->spinSize->value()/2
                                                                    ,y - ui->spinSize->value()/2
                                                                    ,ui->spinSize->value(),ui->spinSize->value()));
        cv::Mat clp(img_now,cv::Rect(x - ui->spinSize->value()/2, y - ui->spinSize->value()/2, ui->spinSize->value(),ui->spinSize->value()));

        tmp.fn          = imglist[count].fileName();
        tmp.fn.chop(4);
        tmp.filename    = QString("Image filename : \"%1\"").arg(imglist[count].fileName());
        tmp.size        = QString("Image size (X x Y x C) : %1 x %2 x %3").arg(img_now.cols).arg(img_now.rows).arg(img_now.channels());
        tmp.database    = QString("Database : \"Ball detection network by CITBrains\"");
        tmp.groundtruth = QString("Objects with ground truth : 1 { \"Ball\"}");

        tmp2.label      = QString("Original label for object 1 \"Ball\" : \"2016Ball\"");
        tmp2.center     = QString("Center point on object 1 \"Ball\" (X, Y) : (%1, %2)").arg(x).arg(y);
        tmp2.box        = QString("Bounding box for object 1 \"Ball\" (Xmin, Ymin) - (Xmax, Ymax) : (%1, %2) - (%3, %4)")
                .arg(x - ui->spinSize->value()/2)
                .arg(y - ui->spinSize->value()/2)
                .arg(x + ui->spinSize->value()/2)
                .arg(y + ui->spinSize->value()/2);
        tmp.obj_tmp.push_back(tmp2);
        img_tmp.push_back(tmp);
        tmp3.fn = imglist[count].fileName();
        tmp3.fn.chop(4);
        tmp3.x = (float)x/img_now.cols;
        tmp3.y = (float)y/img_now.rows;
        tmp3.w = (float)ui->spinSize->value()/img_now.cols;
        tmp3.h = (float)ui->spinSize->value()/img_now.rows;
        YOLOimg_tmp.push_back(tmp3);


        photoSaveImage(clp);
    }
    else if(button == Qt::RightButton)
    {
        onPushSkip();
    }
}

void photoclipping::updatescene()
{
    scene.clear();
    drawImage(imglist[count].filePath());
    scene.addRect(c_point.x - ui->spinSize->value()/2
                  ,c_point.y - ui->spinSize->value()/2
                  ,ui->spinSize->value(),ui->spinSize->value()
                  ,QPen(QColor(255,0,0)));
    scene.addLine(c_point.x-5, c_point.y, c_point.x+5, c_point.y,QPen(QColor(255,0,0)));
    scene.addLine(c_point.x,c_point.y-5, c_point.x, c_point.y+5, QPen(QColor(255,0,0)));
    scene.addEllipse(c_point.x-ui->spinSize->value()/2,c_point.y-ui->spinSize->value()/2,ui->spinSize->value(),ui->spinSize->value(),QPen(QColor(100,150,250),2));
    if(ui->spinSize->value()==2)
    {
        scene.addLine(c_point.x-80, c_point.y, c_point.x+80, c_point.y,QPen(QColor(100,150,250)));
        scene.addLine(c_point.x,c_point.y-80, c_point.x, c_point.y+80, QPen(QColor(100,150,250)));
        scene.addLine(c_point.x-80, c_point.y-80, c_point.x+80, c_point.y+80,QPen(QColor(100,150,250)));
        scene.addLine(c_point.x-80,c_point.y+80, c_point.x+80, c_point.y-80, QPen(QColor(100,150,250)));
    }
    ui->labelPreview->setPixmap(myq.MatBGR2pixmap(img_now).copy(c_point.x - ui->spinSize->value()/2
                                                                ,c_point.y - ui->spinSize->value()/2
                                                                ,ui->spinSize->value(),ui->spinSize->value()));
}

void photoclipping::onPushSkip()
{
    count++;
    if(count < imglist.size())
    {
        RecentImg.clear();
        drawImage(imglist[count].filePath());
        ui->labelImageNum->setText(QString("%1 / %2").arg(imglist.size()-count).arg(imglist.size()));
        points.push_back(QString("%1 %2 %3").arg(imglist[count-1].fileName()).arg(-1).arg(-1));
    }
    else
    {
        scene.clear();
        ui->labelPreview->setText("exit");
        ui->labelImageNum->setText(QString("%1 / %2").arg(imglist.size()-count).arg(imglist.size()));
    }
    ui->pushRevert->setEnabled(TRUE);
}

void photoclipping::onPushRevert()
{
    count--;
    if(count <= 0)
    {
        ui->pushRevert->setDisabled(TRUE);
        count=0;
    }
    else if(count > imglist.size()+1)
    {
        ui->pushRevert->setDisabled(TRUE);
    }
    else
    {
        QFile::remove(RecentImg[count]);
        points.pop_back();
        drawImage(imglist[count].filePath());
    }
}

void photoclipping::currentIndexChangedLabel()
{
    QDir dir = myq.makeDirectory(ui->comboSaveto->currentText(),ui->comboLabel->currentText());
    save_count = myq.scanFiles(dir.path(), "*.png").size();
}

int photoclipping::drawImage(QString filepath)
{
    scene.clear();
    img_now = cv::imread(filepath.toStdString());
    if(img_now.empty())
        return 1;
    scene.addPixmap(myq.MatBGR2pixmap(img_now));
    ui->graphicsImage->setMinimumSize(img_now.cols+200, img_now.rows+200);
    ui->graphicsImage->setMaximumSize(img_now.cols+200, img_now.rows+200);
    ui->spinSize->setMaximum(img_now.cols<img_now.rows?img_now.cols:img_now.rows);
    ui->labelFilename->setText(imglist[count].fileName());
    return 0;
}


void photoclipping::CorrectCoordinatesOfOutside(int &x, int &y)
{
    x = (x >= ui->spinSize->value()/2) ?x:ui->spinSize->value()/2;
    y = (y >= ui->spinSize->value()/2) ?y:ui->spinSize->value()/2;
    x = (x < img_now.cols - ui->spinSize->value()/2) ?x:img_now.cols - ui->spinSize->value()/2-1;
    y = (y < img_now.rows - ui->spinSize->value()/2) ?y:img_now.rows - ui->spinSize->value()/2-1;
/*
    x = (x >= 0) ?x:0;
    y = (y >= 0) ?y:0;
    x = (x < img_now.cols) ?x:img_now.cols-1;
    y = (y < img_now.rows) ?y:img_now.rows-1;
*/
}

void photoclipping::photoSaveImage(cv::Mat src)
{
    QDir dir = myq.makeDirectory(ui->comboSaveto->currentText(),ui->comboLabel->currentText());
    RecentImg[count] = dir.path() + "/" + myq.filenameGen(ui->comboLabel->currentText(),save_count);
    cv::imwrite(RecentImg[count].toStdString(),src);
    qDebug() << "Saved :" << RecentImg[count];
    count++;
    save_count++;
    ui->pushRevert->setEnabled(TRUE);
    if(count < imglist.size())
    {
        drawImage(imglist[count].filePath());
        ui->labelImageNum->setText(QString("%1 / %2").arg(imglist.size()-count).arg(imglist.size()));
    }
    else
    {
        scene.clear();
        ui->labelPreview->setText("exit");
        ui->labelImageNum->setText(QString("%1 / %2").arg(imglist.size()-count).arg(imglist.size()));
    }
}

void photoclipping::outputtxt()
{
    std::ofstream ofs;
    ofs.open(QString(ui->comboSaveto->currentText() + "/points.txt").toLocal8Bit(), std::ios_base::app);
    for(int i=0; i<points.size(); i++)
    {
        ofs << points[i].toStdString() << std::endl;
    }
    ofs.close();

    qDebug() << " --- Step Annotations ---(" << img_tmp.size() << ")";
    for(int i=0; i<img_tmp.size();i++)
    {
        std::ofstream ofs;
        ofs.open(QString(ui->comboSaveto->currentText() + "/Annotations/" + img_tmp[i].fn +".txt").toLocal8Bit(), std::ios_base::app);
        ofs << img_tmp[i].filename.toStdString() << std::endl;
        ofs << img_tmp[i].size.toStdString() << std::endl;
        ofs << img_tmp[i].database.toStdString() << std::endl;
        ofs << img_tmp[i].groundtruth.toStdString() << std::endl;
        for(int j=0; j<img_tmp[i].obj_tmp.size();j++)
        {
            ofs << img_tmp[i].obj_tmp[j].label.toStdString() << std::endl;
            ofs << img_tmp[i].obj_tmp[j].center.toStdString() << std::endl;
            ofs << img_tmp[i].obj_tmp[j].box.toStdString() << std::endl;
        }
        ofs.close();
    }
    qDebug() << " --- Step YOLO_Annotations ---(" << YOLOimg_tmp.size() << ")";
    for(int i=0; i<YOLOimg_tmp.size();i++)
    {
        std::ofstream ofs;
        ofs.open(QString(ui->comboSaveto->currentText() + "/YOLO_Annotations/" + YOLOimg_tmp[i].fn +".txt").toLocal8Bit(), std::ios_base::app);
        QString str = QString("1 %1 %2 %3 %4").arg(YOLOimg_tmp[i].x).arg(YOLOimg_tmp[i].y).arg(YOLOimg_tmp[i].w).arg(YOLOimg_tmp[i].h);
        ofs << str.toStdString() << std::endl;
        ofs.close();
    }
}

void photoclipping::wheelEvent(QWheelEvent *pEvent)
{
    double dSteps = (double)pEvent->delta() / 120.0 * 4;
    ui->spinSize->setValue(ui->spinSize->value()+(int)dSteps);
    updatescene();
}
