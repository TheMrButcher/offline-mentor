#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T23:04:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = training
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginform.cpp

HEADERS  += mainwindow.h \
    version.h \
    loginform.h

FORMS    += mainwindow.ui \
    loginform.ui

RESOURCES += \
    resources.qrc
