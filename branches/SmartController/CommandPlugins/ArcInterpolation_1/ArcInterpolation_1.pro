
#-------------------------------------------------
#
# Project created 2018/1/4 下午3:07 
#
#-------------------------------------------------

QT += core
QT -= gui
QT += widgets
TARGET = ArcInterpolation_1
TEMPLATE = lib
CONFIG += plugin
DEFINES += ARCINTERPOLATION_1_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../../SDK/include/ \
        ../../../third-party/armadillo/include/ \
        common \
        motion \
        path \
        config  \

SOURCES += arcinterpolation_1.cpp \
    common/arc.cpp \
    common/pose.cpp \
    common/segment.cpp \
    common/posture.cpp \
    common/position.cpp \
    common/homogeneous_transformation.cpp \
    config/config.cpp \
    motion/end/end_params.cpp \
    motion/converter/converter.cpp \
    motion/motioncontrol/motioncontrol.cpp \
    path/path.cpp \
    path/profile/profile.cpp \
    path/coordinate_transformation/coordinate_transformation.cpp \
    motion/motion.cpp \
    common/straightline.cpp


HEADERS += arcinterpolation_1.h \
    common/typedef.h \
    common/arc.h \
    common/pose.h \
    common/segment.h \
    common/posture.h \
    common/position.h \
    common/homogeneous_transformation.h \
    config/config.h \
    motion/end/end_params.h \
    motion/converter/converter.h \
    motion/motioncontrol/motioncontrol.h \
    path/path.h \
    path/profile/profile.h \
    path/coordinate_transformation/coordinate_transformation.h \
    motion/motion.h \
    common/straightline.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
