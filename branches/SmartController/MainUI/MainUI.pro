#-------------------------------------------------
#
# Project created by QtCreator 2017-02-23T00:42:18
#
#-------------------------------------------------
QT       += charts sql
QT       += serialport network
QT       += opengl
QT       += core gui\
                    xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainUI
TEMPLATE = app
VERSION = 2.3.8
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
LIBS += -L../../SDK/lib -lLogManager
LIBS += -L../../SDK/lib -lPackLib
LIBS += -L../../SDK/lib -lMainFrame -lLoadFileManager
LIBS += -L../../SDK/lib -lec_control
LIBS += -L../../SDK/lib -lSecretManager
LIBS += -L../../SDK/lib -lTorqueChart
LIBS += -L../../SDK/lib -lCSingleApplication
LIBS += -L../../SDK/lib -lDataBaseManager
LIBS += -L../../SDK/lib -lCommunication
LIBS += -L../../SDK/lib -lLCSPLib
LIBS += -L../../SDK/lib -lStackManager
LIBS += -L../../SDK/lib -lCoordinate
LIBS += -lGLU
INCLUDEPATH += ../../SDK/include/

SOURCES += main.cpp\
        mainwindow.cpp \
    flywidget.cpp \
    debugform.cpp \
    pushbutton.cpp \
    qcwindicatorlamp.cpp \
    lampclick.cpp \
    monitorform.cpp \
    customtabwidget.cpp \
    checkdlg.cpp \
    calibrationui.cpp \
    axisinfoui.cpp \
    analysishwconfig.cpp \
    configform.cpp \
    login.cpp \
    showphotoform.cpp \
    userlogin.cpp \
    lineedit.cpp \
    myglwidget.cpp\
    my_talbview.cpp \
    cspinfoui.cpp \
    csptablewidget.cpp \
    csptritable.cpp \
    addtabel.cpp \
    axisrunpos.cpp \
    cbasetabwidget.cpp \
    prescripttabwidget.cpp \
    cylinderlamp.cpp \
    customlansiui.cpp

HEADERS  += mainwindow.h \
    flywidget.h \
    debugform.h \
    pushbutton.h \
    qcwindicatorlamp.h \
    lampclick.h \
    monitorform.h \
    customtabwidget.h \
    checkdlg.h \
    calibrationui.h \
    axisinfoui.h \
    analysishwconfig.h \
    configform.h \
    login.h \
    userlogin.h \
    showphotoform.h \
    lineedit.h \
    myglwidget.h\
    my_talbview.h \
    cspinfoui.h \
    csptablewidget.h \
    csptritable.h \
    addtabel.h \
    axisrunpos.h \
    cbasetabwidget.h \
    prescripttabwidget.h \
    cylinderlamp.h \
    customlansiui.h
     

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

