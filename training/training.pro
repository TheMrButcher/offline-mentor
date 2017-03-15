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
    loginform.cpp \
    settings.cpp \
    sectionsform.cpp \
    sectionwidget.cpp \
    section.cpp

HEADERS  += mainwindow.h \
    loginform.h \
    settings.h \
    sectionsform.h \
    sectionwidget.h \
    section.h

FORMS    += mainwindow.ui \
    loginform.ui \
    sectionsform.ui \
    sectionwidget.ui

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../omkit-output/release/ -lomkit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../omkit-output/debug/ -lomkit

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../omkit/
