
#-------------------------------------------------
#
# Project created 2017/6/26 上午10:59 
#
#-------------------------------------------------

QT += core
QT -= gui
QT += widgets
TARGET = RectCooInter
TEMPLATE = lib
CONFIG += plugin
DEFINES += RECTCOOINTER_LIBRARY
VERSION = 1.0.0
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
LIBS += -L../../../SDK/lib -lLCSPLib
LIBS += -L../../../SDK/lib -lCoordinate

SOURCES += rectcoointer.cpp
HEADERS += rectcoointer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

