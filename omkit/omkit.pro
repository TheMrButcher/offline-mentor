#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T14:18:56
#
#-------------------------------------------------

QT       -= gui

TARGET = omkit
TEMPLATE = lib

DEFINES += OMKIT_LIBRARY

SOURCES += omkit.cpp \
    utils.cpp

HEADERS += omkit.h\
        omkit_global.h \
    utils.h

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
