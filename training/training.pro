#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T23:04:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = training
TEMPLATE = app

INCLUDEPATH += ../zlib
LIBS += -L../zlib -lz

INCLUDEPATH += ../quazip/quazip
LIBS += -L../quazip/quazip/release -lquazip

SOURCES += main.cpp\
        mainwindow.cpp \
    loginform.cpp \
    settings.cpp \
    sectionsform.cpp \
    sectionwidget.cpp \
    trainingform.cpp \
    questionpage.cpp \
    mentoranswerpage.cpp \
    user_utils.cpp \
    solution_utils.cpp \
    totalpage.cpp \
    group_utils.cpp

HEADERS  += mainwindow.h \
    loginform.h \
    settings.h \
    sectionsform.h \
    sectionwidget.h \
    trainingform.h \
    questionpage.h \
    mentoranswerpage.h \
    user_utils.h \
    solution_utils.h \
    totalpage.h \
    group_utils.h

FORMS    += mainwindow.ui \
    loginform.ui \
    sectionsform.ui \
    sectionwidget.ui \
    trainingform.ui \
    questionpage.ui \
    mentoranswerpage.ui \
    totalpage.ui

RESOURCES += \
    resources.qrc

RC_FILE = appicon.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../omkit-output/release/ -lomkit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../omkit-output/debug/ -lomkit

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../omkit/
