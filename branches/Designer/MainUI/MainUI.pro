#-------------------------------------------------
#
# Project created by QtCreator 2018-05-29T08:57:24
#
#-------------------------------------------------

QT       += core gui xml script sql serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../SDK/lib -lCoWidgetBox -lCoGraphics -lCoAction -lCSingleApplication
LIBS += -L../../SDK/lib -lCoGraphicsItems -lScriptManager -lStackManager -lMqttManager
LIBS += -L../../SDK/lib -lDBInstanceManager -lDataBaseBatchManager
LIBS += -L../../SDK/lib -lMainFrame -lec_control -lLogManager -lXmlManager -lPackLib -lCommunication -lLoadFileManager -lTeachDeviceRunLib
LIBS += -L../../third-party/alglib/lib/ -lalg
LIBS += -L../../third-party/kdl/lib -lorocos-kdl -lorocos-kdl-models
INCLUDEPATH += ../../SDK/include/

SOURCES += main.cpp\
        mainwindow.cpp \
    ThreadObject.cpp \
    checkabbitthread.cpp \
    checkdirection.cpp
HEADERS  += mainwindow.h \
    ThreadObject.h \
    checkabbitthread.h \
    checkdirection.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
