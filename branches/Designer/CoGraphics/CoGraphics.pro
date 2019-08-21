#-------------------------------------------------
#
# Project created by QtCreator 2018-03-22T13:00:25
#
#-------------------------------------------------

QT       -= gui
QT       += sql xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CoGraphics
TEMPLATE = lib

DEFINES += COGRAPHICS_LIBRARY

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
LIBS += -L../../SDK/lib -lPackLib -lXmlManager
LIBS += -L../../SDK/lib -lDBInstanceManager -lDataBaseBatchManager
SOURCES += Graphics.cpp\
    ItemFactory.cpp \
    Base/Base.cpp \
    Undo/Undo.cpp \
    Undo/UndoAddDel.cpp \
    Undo/UndoMoveDrag.cpp \
    Item/Item.cpp \
    Item/ItemEx.cpp \
    Scene/Scene.cpp \
    Viewer/Viewer.cpp \
    Arrow.cpp \
    Js/Js.cpp \
    Js/CoJs.cpp


HEADERS += Graphics.h\
        cographics_global.h \
    ItemFactory.h \
    Base/Base.h \
    Undo/Undo.h \
    Undo/UndoAddDel.h \
    Undo/UndoMoveDrag.h \
    Item/Item.h \
    Item/ItemEx.h \
    Scene/Scene.h \
    Viewer/Viewer.h \
    Arrow.h \
    Js/Js.h \
    Js/CoJs.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    CoGraphics.qrc
