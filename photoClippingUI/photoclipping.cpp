#include "photoclipping.h"
#include "ui_photoclipping.h"

photoclipping::photoclipping(QWidget *parent) :
    QWidget(parent),
    count(0),
    settings(QDir::homePath()+"/photoClipping.ini", QSettings::IniFormat),
    ui(new Ui::photoclipping)
{
    ui->setupUi(this);
    ui->graphicsImage->setScene(&scene);
    connectSignals();
    if(settings.value("IMAGEDIR").toString().size())
    {
        QString imagedir = settings.value("IMAGEDIR").toString();
        setFileList(imagedir);
    }
    if(settings.value("SAVEDIR",QDir::homePath() + "/Pictures").toString().size())
    {
        settings.remove("SAVEDIR");
        QString saveto = settings.value("SAVEDIR",QDir::homePath() + "/Pictures").toString();
        ui->comboSaveto->insertItem(0, saveto);
        ui->comboSaveto->setCurrentIndex(0);
    }
}

photoclipping::~photoclipping()
{
    settings.setValue("SAVEDIR",ui->comboSaveto->currentText());
    settings.setValue("IMAGEDIR",ui->lineSelectFolder->text());
    delete ui;
}

void photoclipping::connectSignals()
{
    connect(ui->pushSelectFolder, SIGNAL(clicked()), this, SLOT(onPushSelectFolder()));
    connect(ui->graphicsImage, SIGNAL(mouseMoved(int,int ,Qt::MouseButton)), this, SLOT(onMouseMovedGraphicsImage(int,int ,Qt::MouseButton)));
    connect(ui->graphicsImage, SIGNAL(mouseReleased(int,int, Qt::MouseButton)), this, SLOT(onMouseReleasedGraphicImage(int,int, Qt::MouseButton)));
    connect(ui->pushSkip, SIGNAL(clicked()), this, SLOT(onPushSkip()));
    connect(ui->pushRevart, SIGNAL(clicked()), this, SLOT(onPushRevart()));
    connect(ui->pushSaveto, SIGNAL(clicked()), this, SLOT(onPushSaveto()));
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
}

void photoclipping::setFileList(QString dirpath)
{
    ui->lineSelectFolder->setText(dirpath);
    imglist = myq.scanFiles(dirpath,"*.png");
    ui->labelImageNum->setText(QString("%1 / %1").arg(imglist.size()));
    if(imglist.size())
    {
        drawImage(imglist[count].filePath());
    }
}

void photoclipping::onMouseMovedGraphicsImage(int x,int y ,Qt::MouseButton button)
{
    if(imglist.size())
    {
        scene.clear();
        drawImage(imglist[count].filePath());
        CorrectCoordinatesOfOutside(x,y);
        scene.addRect(x - ui->spinSize->value()/2
                      ,y - ui->spinSize->value()/2
                      ,ui->spinSize->value(),ui->spinSize->value()
                      ,QPen(QColor(255,0,0)));
        ui->labelPreview->setPixmap(myq.MatBGR2pixmap(img_now).copy(x - ui->spinSize->value()/2
                                                                    ,y - ui->spinSize->value()/2
                                                                    ,ui->spinSize->value(),ui->spinSize->value()));
    }
}

void photoclipping::onMouseReleasedGraphicImage(int x, int y ,Qt::MouseButton button)
{
    if(imglist.size() && button == Qt::LeftButton)
    {
        CorrectCoordinatesOfOutside(x,y);
        scene.addRect(x - ui->spinSize->value()/2
                      ,y - ui->spinSize->value()/2
                      ,ui->spinSize->value(),ui->spinSize->value()
                      ,QPen(QColor(255,0,0)));
        ui->labelPreview->setPixmap(myq.MatBGR2pixmap(img_now).copy(x - ui->spinSize->value()/2
                                                                    ,y - ui->spinSize->value()/2
                                                                    ,ui->spinSize->value(),ui->spinSize->value()));
        cv::Mat clp(img_now,cv::Rect(x - ui->spinSize->value()/2, y - ui->spinSize->value()/2, ui->spinSize->value(),ui->spinSize->value()));
        photoSaveImage(clp);
    }
    else if(button == Qt::RightButton)
    {
        onPushSkip();
    }
}

void photoclipping::onPushSkip()
{
    count++;
    if(count < imglist.size())
        drawImage(imglist[count].filePath());
}

void photoclipping::onPushRevart()
{
    count--;
    QFile::remove(RecentImg);
    drawImage(imglist[count].filePath());
    ui->pushRevart->setDisabled(TRUE);
}

int photoclipping::drawImage(QString filepath)
{
    scene.clear();
    img_now = cv::imread(filepath.toStdString());
    if(img_now.empty())
        return 1;
    scene.addPixmap(myq.MatBGR2pixmap(img_now));
    ui->graphicsImage->setMinimumSize(img_now.cols, img_now.rows);
    ui->graphicsImage->setMaximumSize(img_now.cols+2, img_now.rows+2);
    ui->labelImageNum->setText(QString("%1 / %2").arg(imglist.size()-count).arg(imglist.size()));
    return 0;
}


void photoclipping::CorrectCoordinatesOfOutside(int &x, int &y)
{
    x = (x > ui->spinSize->value()/2) ?x:ui->spinSize->value()/2;
    y = (y > ui->spinSize->value()/2) ?y:ui->spinSize->value()/2;
    x = (x < img_now.cols - ui->spinSize->value()/2) ?x:img_now.cols - ui->spinSize->value()/2;
    y = (y < img_now.rows - ui->spinSize->value()/2) ?y:img_now.rows - ui->spinSize->value()/2;
}

void photoclipping::photoSaveImage(cv::Mat src)
{
    QDir dir = myq.makeDirectory(ui->comboSaveto->currentText(),ui->comboLabel->currentText());
    int cnt = myq.scanFiles(dir.path(),"*.png").size();
    RecentImg = dir.path() + "/" + myq.filenameGen(ui->comboLabel->currentText(),cnt);
    cv::imwrite(RecentImg.toStdString(),src);
    qDebug() << "Saved :" << RecentImg;
    count++;
    ui->pushRevart->setEnabled(TRUE);
    if(count < imglist.size())
    {
        drawImage(imglist[count].filePath());
    }
    else
    {
        scene.clear();
        ui->labelPreview->setText("exit");
    }
}
