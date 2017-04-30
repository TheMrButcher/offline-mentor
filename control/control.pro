#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T00:41:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = control
TEMPLATE = app

INCLUDEPATH += ../zlib
LIBS += -L../zlib -lz

INCLUDEPATH += ../quazip/quazip
LIBS += -L../quazip/quazip/release -lquazip

SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    settingsdialog.cpp \
    solutionsform.cpp \
    section_utils.cpp \
    solution_utils.cpp \
    solutionexplorer.cpp \
    answerpage.cpp \
    textexplorer.cpp \
    trainingcreationwizard.cpp \
    settingswizard.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    settings.h \
    settingsdialog.h \
    solutionsform.h \
    section_utils.h \
    solution_utils.h \
    solutionexplorer.h \
    answerpage.h \
    textexplorer.h \
    trainingcreationwizard.h \
    settingswizard.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    solutionsform.ui \
    solutionexplorer.ui \
    answerpage.ui \
    textexplorer.ui \
    trainingcreationwizard.ui \
    settingswizard.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../omkit-output/release/ -lomkit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../omkit-output/debug/ -lomkit

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../omkit/
