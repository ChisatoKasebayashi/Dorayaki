#include "otameshihistui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OtameshiHistUI w;
    w.show();

    return a.exec();
}

/*
void mouse(int event,int x, int y,int flags,void *param=NULL)
{
    switch(event)
    {
        case CV_EVENT_LBUTTONDOWN:
            std::cout<<"click[0]  "<<x<<","<<y<<"\n";
            click_x[0] = x;
            click_y[0] = y;
            break;

        case CV_EVENT_RBUTTONDOWN:
            std::cout<<"click[1]  "<<x<<","<<y<<"\n";
            click_x[1] = x;
            click_y[1] = y;
            break;

        default:
            break;
    }
}
*/
