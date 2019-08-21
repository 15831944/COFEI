#-------------------------------------------------
# Project created by QtCreator 2017-09-02T10:35:10
#此动态库是为直线插补开发，其中包含２轴插补，３轴插补...折线段插补等插补范围!
#包含轴插补指令中的IO触发功能！
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = LCSPLib
TEMPLATE = lib

DEFINES += LCSPLIB_LIBRARY
VERSION = 2.0.0
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../../SDK/include/
SOURCES += lcsplib.cpp

HEADERS += lcsplib.h\
        lcsplib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
