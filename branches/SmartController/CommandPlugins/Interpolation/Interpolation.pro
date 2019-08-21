
#-------------------------------------------------
#
# Project created 2018/1/4 下午3:07 
#
#-------------------------------------------------

QT += core
QT -= gui
QT += widgets
TARGET = interpolation
TEMPLATE = lib
CONFIG += plugin
CONFIG += link_pkgconfig
PKGCONFIG += serial

DEFINES += INTERPOLATION_LIBRARY

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
        ../../../third-party/alglib/include/ \
        ../../../third-party/kdl/include/ \
        /usr/include/eigen3 \
        generic \
        motion \
        trajectory \
        config \
        kinematics

LIBS += -L../../../third-party/alglib/lib/ -lalg
LIBS += -L../../../third-party/kdl/lib -lorocos-kdl -lorocos-kdl-models

SOURCES += \
    generic/conf.cpp \
    motion/ServoControl.cpp \
    motion/converter.cpp \
    motion/motion.cpp \
#    trajectory/arc.cpp \
    trajectory/straightline.cpp \
    trajectory/curve.cpp \
    trajectory/Spline3D.cpp \
#    trajectory/homogeneous_transformation.cpp \
    trajectory/VelocityCurve.cpp \
    trajectory/BaseTrajectory.cpp \
    Interpolation.cpp \
    trajectory/trajectories.cpp \
    kinematics/xyzr_fouraxis_fk.cpp \
    kinematics/xyzr_fouraxis_ik.cpp

HEADERS += \
    generic/conf.h \
    motion/ServoControl.h \
    motion/converter.h \
    motion/motion.h \
#    trajectory/arc.h \
    trajectory/straightline.h \
    trajectory/curve.h \
    trajectory/Spline3D.h \
#    trajectory/homogeneous_transformation.h \
    trajectory/typedef.h \
    trajectory/VelocityCurve.h \
    trajectory/BaseTrajectory.h \
    Interpolation.h \
    generic/unit.h \
    trajectory/trajectories.h \
    kinematics/xyzr_fouraxis_fk.h \
    kinematics/xyzr_fouraxis_ik.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
