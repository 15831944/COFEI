#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T10:02:44
#
#-------------------------------------------------

QT       -= gui
QT       += script
QT       += sql
TARGET = ScriptManager
TEMPLATE = lib
VERSION = 1.0.0
DEFINES += SCRIPTMANAGER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../../SDK/lib -lDataBaseBatchManager -lDBInstanceManager
INCLUDEPATH += ../../../SDK/include/
SOURCES += scriptmanager.cpp \
    converter.cpp

HEADERS += scriptmanager.h\
        scriptmanager_global.h \
    converter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
