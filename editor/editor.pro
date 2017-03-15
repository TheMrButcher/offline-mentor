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
    section.cpp \
    createsectiondialog.cpp \
    settings.cpp \
    sectioneditform.cpp \
    case.cpp \
    casepage.cpp \
    texteditorpage.cpp

HEADERS  += mainwindow.h \
    sectionsform.h \
    sectionwidget.h \
    section.h \
    createsectiondialog.h \
    settings.h \
    sectioneditform.h \
    case.h \
    casepage.h \
    texteditorpage.h

FORMS    += mainwindow.ui \
    sectionsform.ui \
    sectionwidget.ui \
    createsectiondialog.ui \
    sectioneditform.ui \
    casepage.ui \
    texteditorpage.ui

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../omkit-output/release/ -lomkit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../omkit-output/debug/ -lomkit

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../omkit/
