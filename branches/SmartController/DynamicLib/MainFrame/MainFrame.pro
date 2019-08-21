#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T10:46:13
#
#-------------------------------------------------

QT       -= gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = MainFrame
TEMPLATE = lib
VERSION = 3.1.0
DEFINES += MAINFRAME_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../../SDK/lib/ -lLogManager -lScriptManager -lStackManager
INCLUDEPATH += ../../../SDK/include/

SOURCES += mainframe.cpp \
    #handlexml.cpp \
    #loadPlugin.cpp \
    #copyfiles.cpp \
    #mythread.cpp \
    #childflows.cpp \
    #operateflows.cpp \
    safethread.cpp \
    #orgflow.cpp \
    #resetflow.cpp \
    #mainflows.cpp \
    #mychlidflowsignal.cpp \
    FlowDispatcher.cpp \
    FlowOperater.cpp \
    OperateThread.cpp \
    OperateSinThread.cpp \
    RunPlugin.cpp \
    autoflowoperater.cpp \
    autoflowframe.cpp
    #loadfilemanager.cpp

HEADERS += mainframe.h\
        mainframe_global.h \
    #handlexml.h \
    #loadPlugin.h \
    #copyfiles.h \
    #mythread.h \
    #childflows.h \
    customstruct.h \
    #operateflows.h \
    safethread.h \
    #orgflow.h \
    #resetflow.h \
    #mainflows.h \
    #mychlidflowsignal.h \
    FlowDispatcher.h \
    FlowOperater.h \
    OperateThread.h \
    OperateSinThread.h \
    RunPlugin.h \
    autoflowoperater.h \
    autoflowframe.h
    #loadfilemanager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
