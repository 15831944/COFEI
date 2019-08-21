
#-------------------------------------------------
#
# Project created 2017/11/11 上午10:25 
#
#-------------------------------------------------

QT -= gui
#QT += core
QT += widgets

TARGET = Pallet
TEMPLATE = lib
CONFIG += plugin
DEFINES += PALLET_LIBRARY
VERSION = 1.0.01


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../../SDK/include/

SOURCES += pallet.cpp \
    palleinfo.cpp \
    myarrary.cpp \
    msg.cpp \
    signal.cpp \
    positionstore.cpp \
    motioncontrol.cpp

HEADERS += pallet.h \
    palletinfo.h \
    axisinfo.h \
    myarray.h \
    positionstore.h \
    motioncontrol.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

