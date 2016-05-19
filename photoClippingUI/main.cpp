#include "photoclipping.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    photoclipping w;
    w.show();

    return a.exec();
}
