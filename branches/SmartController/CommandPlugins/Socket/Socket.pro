#-------------------------------------------------
#
# Project created by QtCreator 2017-05-12T15:44:45
#
#-------------------------------------------------

QT       -= gui
QT       += serialport network sql
QT       += widgets
TARGET = Socket
TEMPLATE = lib
CONFIG += plugin
DEFINES += SERVETCP_LIBRARY
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
LIBS += -L../../../SDK/lib -lCommunication
SOURCES += \
    Socket.cpp

HEADERS += \
    Socket.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}