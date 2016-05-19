#ifndef MYQVIEW_H
#define MYQVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QDebug>

class MyQView :public QGraphicsView
{
    Q_OBJECT
public:
    MyQView(QWidget *parent = 0);
signals:
    void mouseMoved(int x, int y, Qt::MouseButton button);
    void mouseReleased(int x, int y ,Qt::MouseButton button);
protected:
    void mouseMoveEvent(QMouseEvent *eve);
    void mouseReleaseEvent(QMouseEvent *eve);
};

#endif // MYQVIEW_H
