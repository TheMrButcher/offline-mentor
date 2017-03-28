#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T10:19:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = editor
TEMPLATE = app

INCLUDEPATH += ../zlib
LIBS += -L../zlib -lz

INCLUDEPATH += ../quazip/quazip
LIBS += -L../quazip/quazip/release -lquazip

SOURCES += main.cpp\
        mainwindow.cpp \
    sectionsform.cpp \
    sectionwidget.cpp \
    createsectiondialog.cpp \
    settings.cpp \
    sectioneditform.cpp \
    casepage.cpp \
    texteditorpage.cpp \
    section_utils.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    sectionsform.h \
    sectionwidget.h \
    createsectiondialog.h \
    settings.h \
    sectioneditform.h \
    casepage.h \
    texteditorpage.h \
    section_utils.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    sectionsform.ui \
    sectionwidget.ui \
    createsectiondialog.ui \
    sectioneditform.ui \
    casepage.ui \
    texteditorpage.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../omkit-output/release/ -lomkit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../omkit-output/debug/ -lomkit

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../omkit/
