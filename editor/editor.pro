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
    aboutdialog.cpp \
    richtextedit.cpp \
    exportdialog.cpp \
    imageinsertiondialog.cpp

HEADERS  += mainwindow.h \
    sectionsform.h \
    sectionwidget.h \
    createsectiondialog.h \
    settings.h \
    sectioneditform.h \
    casepage.h \
    texteditorpage.h \
    section_utils.h \
    aboutdialog.h \
    richtextedit.h \
    exportdialog.h \
    imageinsertiondialog.h

FORMS    += mainwindow.ui \
    sectionsform.ui \
    sectionwidget.ui \
    createsectiondialog.ui \
    sectioneditform.ui \
    casepage.ui \
    texteditorpage.ui \
    aboutdialog.ui \
    exportdialog.ui \
    imageinsertiondialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = appicon.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../omkit-output/release/ -lomkit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../omkit-output/debug/ -lomkit

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../omkit/
