#-------------------------------------------------
#
# Project created by QtCreator 2018-07-11T09:33:13
#
#-------------------------------------------------

QT       -= gui
QT       += xml serialport network widgets sql
TARGET = LoadFileManager
TEMPLATE = lib

DEFINES += LOADFILEMANAGER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../../SDK/lib/ -lXmlManager -lCommunication -lMqttManager
LIBS += -L../../../third-party/alglib/lib/ -lalg
LIBS += -L../../../third-party/kdl/lib -lorocos-kdl -lorocos-kdl-models

INCLUDEPATH += ../../../SDK/include/

SOURCES += LoadFileManager.cpp \
    HandleXml.cpp \
    LoadPlugin.cpp

HEADERS += LoadFileManager.h\
        loadfilemanager_global.h \
    HandleXml.h \
    LoadPlugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
