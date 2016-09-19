#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T23:58:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photoClippingUI
TEMPLATE = app


SOURCES += main.cpp\
        photoclipping.cpp \
    MyQclass.cpp \
    MyQView.cpp

HEADERS  += photoclipping.h \
    MyQclass.h \
    MyQView.h

FORMS    += photoclipping.ui

# OpenCV settings
unix:
{
    INCLUDEPATH += /usr/include/opencv
    LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video
}

#win32:
#{
#    DEPENDPATH  += "C:/OpenCV249/include"
#    INCLUDEPATH += "C:/OpenCV249/include"
#    LIBS += -L"C:/OpenCV249/x86/vc11/lib"
#    LIBS += -lopencv_core249 -lopencv_highgui249 -lopencv_imgproc249
#}
