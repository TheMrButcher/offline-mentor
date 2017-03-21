#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T14:18:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = omkit
TEMPLATE = lib

DEFINES += OMKIT_LIBRARY

SOURCES += omkit.cpp \
    utils.cpp \
    case.cpp \
    section.cpp \
    json_utils.cpp \
    html_utils.cpp \
    answer.cpp \
    solution.cpp \
    ui_utils.cpp

HEADERS += omkit.h\
        omkit_global.h \
    utils.h \
    case.h \
    section.h \
    json_utils.h \
    html_utils.h \
    answer.h \
    solution.h \
    ui_utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    CONFIG(debug, debug|release) {
        target.path = $${PWD}/../omkit-output/debug
    }
    CONFIG(release, debug|release) {
        target.path = $${PWD}/../omkit-output/release
    }
    INSTALLS += target
}

RESOURCES += \
    resources.qrc
