#include <MyQclass.h>

QFileInfo MyQclass::selectFile(QString filter)
{
    QFileDialog::Options options;
    QString strSelectedFilter;
    QString strFName = QFileDialog::getOpenFileName(
                this,
                tr( "select file" ),
                QDir::homePath(),
                filter,
                &strSelectedFilter,
                options );

    if ( !strFName.isEmpty() )
    {
        QFileInfo fileinfo(strFName);
        return fileinfo;
    }
    QMessageBox::warning(this,"Error","File not found.");
    QFileInfo none;
    return none;
}

QDir MyQclass::selectDir()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

    QString strDir = QFileDialog::getExistingDirectory(
                this,
                tr("select Dir"),
                QDir::homePath()
                ,options);

    if ( !strDir.isEmpty() )
    {
        QDir dir(strDir);
        return dir;
    }
    QMessageBox::warning(this,"Error","Directly not found.");
    QDir none;
    return none;
}


QFileInfoList MyQclass::scanDirs(QString targetpath,QString filter)
{
    QDir dir( targetpath );
    QStringList strlFilter;
    strlFilter << filter;
    QFileInfoList list = dir.entryInfoList( strlFilter, QDir::Dirs );

    return list;
}

QFileInfoList MyQclass::scanFiles(QString targetpath ,QString filter)
{
    QDir dir( targetpath );
    QStringList strlFilter;
    strlFilter << filter;
    QFileInfoList list = dir.entryInfoList( strlFilter, QDir::Files );

    return list;
}

QPixmap MyQclass::MatBGR2pixmap(cv::Mat src)
{
    cv::cvtColor(src, src,CV_BGR2BGRA);
    QPixmap pixmap = QPixmap::fromImage(QImage(src.data, src.cols, src.rows, QImage::Format_ARGB32));
    return pixmap;
}

QDir MyQclass::makeDirectory(QString targetPath, QString dirname)
{
    QDir dir(targetPath);
    if(!dir.cd(dirname))
    {
        dir.mkdir(dirname);
        dir.cd(dirname);
    }
    return dir;
}

QString MyQclass::filenameGen(QString objectname,int num,QString extension)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString fname = QString("%1%2%3%4%5")
            .arg(objectname)
            .arg(dateTime.date().toString("MMdd"))
            .arg(dateTime.time().toString("hhmm"))
            .arg(num+1,5,10,QChar('0'))
            .arg(extension);
    return fname;
}
