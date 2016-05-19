#include "MyQView.h"

MyQView::MyQView(QWidget *parent)
{
    this->setMouseTracking(true);
}

void MyQView::mouseReleaseEvent(QMouseEvent *eve)
{
    int x = eve->x();
    int y = eve->y();
    if(0 < x && x < this->width())
        if(0 < y && y < this->height())
            emit mouseReleased(x,y,eve->button());
}
void MyQView::mouseMoveEvent(QMouseEvent *eve)
{
    int x = eve->x();
    int y = eve->y();
    if(0 < x && x < this->width())
        if(0 < y && y < this->height())
            emit mouseMoved(x,y,eve->button());
}
