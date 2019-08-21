#include "motion.h"
#include <unistd.h>
#include <fstream>
#include <cmath>

namespace MP {

Motion::Motion()
{
    unit_converter = nullptr;
    servo_controller = nullptr;

    config = config->GetInstance();
}

Motion::~Motion()
{
    if (unit_converter)
        delete unit_converter;
    if (servo_controller)
        delete servo_controller;
}

void Motion::Init()
{
    if (!unit_converter)
        unit_converter = new ServoInstructionUnitConverter;

    if (!servo_controller)
        servo_controller = new ServoControl;

//    servo_controller->set_axises_id(config->motion_info->axises_info);
}

void Motion::SetTrajectory(Trajectory &trajectory)
{
    this->trajectory = &trajectory;
}

void Motion::SetAxisesID(QMap<QString, AxisInfo> axises_id)
{
    servo_controller->set_axises_id(axises_id);
}

void Motion::SetStopVar(const bool *stop)
{
    this->stop = stop;
}

void Motion::Run()
{
    servo_controller->init();

    QMap<QString, int> current_position;
    servo_controller->getAxisCurrentPos(servo_controller->axises_["J1"].id, current_position["J1"]);
    servo_controller->getAxisCurrentPos(servo_controller->axises_["J2"].id, current_position["J2"]);
    servo_controller->getAxisCurrentPos(servo_controller->axises_["J3"].id, current_position["J3"]);
    servo_controller->getAxisCurrentPos(servo_controller->axises_["J4"].id, current_position["J4"]);
    QMap<QString, int> &target_position = current_position;


    QThread::msleep(trajectory->GetDelayBeforeStart());
    StartDispense(trajectory->GetIOPort());

    Pose increase;
    int count = trajectory->GetInterpolationCount();
    for (int i = 0; i < count; ++i) {
        if (*stop)
            return;

        increase = trajectory->GetJointIncrease(i);
        unit_converter->convert(increase);
        servo_controller->run(increase, target_position);

//        qDebug() << i << "/" << trajectory->GetInterpolationCount()<< ":\n"
//                 << "x_increase: " << increase.x
//                 << "y_increase: " << increase.y
//                 << "z_increase: " << increase.z
//                 << "roll_increase: " << increase.roll << "\n"
//                 << target_position["J1"] << " " << target_position["J2"] << " "
//                 << target_position["J3"] << " " << target_position["J4"] << endl;

        if (i == trajectory->pos_to_set_io_disable) {
            StopDispense(trajectory->GetIOPort());
//            qDebug() << "stop dispense, i = " << i << endl;
        }
    }
}

int Motion::GetAxisCurrentPos(QString axis_id)
{
    servo_controller->init();
    int pos;
    servo_controller->getAxisCurrentPos(servo_controller->axises_[axis_id].id, pos);

    return pos;
}

void Motion::StartDispense(QString io)
{
    uchar value = 1;
    CoreGlobal::BaseCalcFun::AnalysisIOString(io, value);
}

void Motion::StopDispense(QString io)
{
    uchar value = 0;
    CoreGlobal::BaseCalcFun::AnalysisIOString(io, value);
}

} // end of namespace MP
