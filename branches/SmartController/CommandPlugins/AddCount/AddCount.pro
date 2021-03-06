
#-------------------------------------------------
#
# Project created 2017/8/21 上午8:28 
#
#-------------------------------------------------

QT += core sql widgets
QT -= gui

TARGET = AddCount
TEMPLATE = lib
CONFIG += plugin
DEFINES += ADDCOUNT_LIBRARY

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
LIBS += -L../../../SDK/lib -lDataBaseBatchManager -lDBInstanceManager

SOURCES += addcount.cpp
HEADERS += addcount.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

