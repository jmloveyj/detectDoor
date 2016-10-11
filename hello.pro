#-------------------------------------------------
#
# Project created by QtCreator 2016-09-29T10:13:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hello
TEMPLATE = app


SOURCES += main.cpp\
        hello.cpp

HEADERS  += hello.h

FORMS    += hello.ui

INCLUDEPATH +=/usr/local/include

LIBS += -L/usr/local/lib -L/System/Library/Frameworks/ImageIO.framework/Versions/A/Resources -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core


