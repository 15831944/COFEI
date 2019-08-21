#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T09:59:10
#
#-------------------------------------------------
QT       += widgets
QT       += sql
QT       += core gui xml network

TARGET = DataManagerForm
TEMPLATE = lib
DEFINES += DATAMANAGERFORM_LIBRARY
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -L../../../SDK/lib -lDBInstanceManager
LIBS += -L../../../SDK/lib -lHardwarefrom
LIBS += -L../../../SDK/lib -lDataBaseBatchManager
#LIBS += -L../../SDK/lib -lMulThreaCopyFile
#LIBS += -L../../SDK/lib -lDataBaseManager
INCLUDEPATH += ../../../SDK/include/

SOURCES += \
    CountAttriWidget.cpp \
    TableModel.cpp \
    ComboBoxDelegate.cpp \
    TableView.cpp \
    StringRegisterAttriWidget.cpp \
    NumberRegisterAttriWidget.cpp \
    VirtualIOAttriWidget.cpp \
    TimingAttriWidget.cpp \
    CylinderInfoAttriWidget.cpp \
    DataManagerForm.cpp \
    VariableInforWidget.cpp \
    motionctrattriwidget.cpp \
    cspmotionctrwidget.cpp

HEADERS  += \
    CountAttriWidget.h \
    TableModel.h \
    ComboBoxDelegate.h \
    TableView.h \
    StringRegisterAttriWidget.h \
    NumberRegisterAttriWidget.h \
    VirtualIOAttriWidget.h \
    TimingAttriWidget.h \
    CylinderInfoAttriWidget.h \
    DataManagerForm.h \
    VariableInforWidget.h \
    datamanagerform_global.h \
    motionctrattriwidget.h \
    cspmotionctrwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
