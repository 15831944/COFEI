#-------------------------------------------------
#
# Project created by QtCreator 2018-04-08T14:54:59
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = CoWidgetBox
TEMPLATE = lib

DEFINES += COWIDGETBOX_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../SDK/include/
LIBS += -L../../SDK/lib -lCoAction

SOURCES += WidgetBox.cpp \
    TitleBar.cpp \
    AttributeBox/AttributeBox.cpp \
    TaskBar/Iconlabel.cpp \
    TaskBar/Rect.cpp \
    TaskBar/Taskbar.cpp \
    TaskBar/TaskbarScheme.cpp \
    TaskBar/Taskbox.cpp \
    TaskBar/Taskgroup.cpp \
    TaskBar/Taskheader.cpp

HEADERS += WidgetBox.h \
    CoWidgetBox_global.h \
    TitleBar.h \
    AttributeBox/AttributeBox.h \
    TaskBar/Iconlabel.h \
    TaskBar/Rect.h \
    TaskBar/Taskbar.h \
    TaskBar/TaskbarScheme.h \
    TaskBar/Taskbox.h \
    TaskBar/Taskgroup.h \
    TaskBar/Taskheader.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
RESOURCES += \
    WidgetBox.qrc
