#-------------------------------------------------
#
# Project created by QtCreator 2018-04-10T09:56:37
#
#-------------------------------------------------

QT       += widgets
QT       += core gui network xml sql
TARGET = Hardwarefrom
TEMPLATE = lib

DEFINES += HARDWAREFROM_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../SDK/lib -lXmlManager
LIBS += -L../../SDK/lib -lDataBaseBatchManager
LIBS += -L../../SDK/lib -lDBInstanceManager
INCLUDEPATH += ../../SDK/include/
SOURCES += \
    addarm.cpp \
    addaxisfrom.cpp \
    addcoordinate.cpp \
    addcoordinateui.cpp \
    addiofrom.cpp \
    addserialport.cpp \
    addtcpfrom.cpp \
    xmlinfosave.cpp \
    HardwareFrom.cpp


HEADERS +=\
        hardwarefrom_global.h \
    addarm.h \
    addaxisfrom.h \
    addcoordinate.h \
    addcoordinateui.h \
    addiofrom.h \
    addserialport.h \
    addtcpfrom.h \
   # Comm.h \
    xmlinfosave.h \
    HardwareFrom.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

SUBDIRS += \
    ../TestUI/TestUI.pro

RESOURCES += \
    qss.qrc
