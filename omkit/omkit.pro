#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T14:18:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = omkit
TEMPLATE = lib

INCLUDEPATH += ../zlib
LIBS += -L../zlib -lz

INCLUDEPATH += ../quazip/quazip
LIBS += -L../quazip/quazip/release -lquazip

DEFINES += OMKIT_LIBRARY

SOURCES += omkit.cpp \
    utils.cpp \
    case.cpp \
    section.cpp \
    json_utils.cpp \
    html_utils.cpp \
    answer.cpp \
    solution.cpp \
    ui_utils.cpp \
    trainingsettings.cpp \
    zip_utils.cpp \
    string_utils.cpp \
    caseimage.cpp

HEADERS += omkit.h\
        omkit_global.h \
    utils.h \
    case.h \
    section.h \
    json_utils.h \
    html_utils.h \
    answer.h \
    solution.h \
    ui_utils.h \
    trainingsettings.h \
    zip_utils.h \
    string_utils.h \
    caseimage.h \
    smallbimap.h

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
