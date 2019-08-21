#ifndef MOTION_H
#define MOTION_H

#include <QString>
#include <QVector>
#include "trajectories.h"
#include "converter.h"
#include "ServoControl.h"

namespace MP {

class Motion
{
public:
    Motion();
    virtual ~Motion();

    void Init();
    void SetTrajectory(Trajectory &trajectory);
    void SetAxisesID(QMap<QString, AxisInfo> axises_id);
    void SetStopVar(const bool *stop);
    void Run();

    int GetAxisCurrentPos(QString aixs_id);

private:
    void StartDispense(QString io);
    void StopDispense(QString io);

private:
    const bool *stop;

    ServoInstructionUnitConverter *unit_converter;
    ServoControl *servo_controller;

    Trajectory *trajectory;
    Config *config;
};

} // end of namespace MP

#endif // MOTION_H
