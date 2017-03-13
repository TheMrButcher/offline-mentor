#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T10:19:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sectionsform.cpp \
    sectionwidget.cpp \
    section.cpp

HEADERS  += mainwindow.h \
    version.h \
    sectionsform.h \
    sectionwidget.h \
    section.h

FORMS    += mainwindow.ui \
    sectionsform.ui \
    sectionwidget.ui

RESOURCES += \
    resources.qrc
