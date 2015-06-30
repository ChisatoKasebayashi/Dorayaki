#include "otameshihistui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OtameshiHistUI w;
    w.show();
    return a.exec();
}
