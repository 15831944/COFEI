#-------------------------------------------------
#
# Project created by QtCreator 2017-03-06T15:11:05
#
#-------------------------------------------------

QT       -= gui

TARGET = EventManager
TEMPLATE = lib
VERSION = 1.0.0
DEFINES += EVENTMANAGER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += eventmanager.cpp

HEADERS += eventmanager.h\
        eventmanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
