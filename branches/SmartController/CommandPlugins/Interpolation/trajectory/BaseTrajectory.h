#ifndef BASETRAJECTORY_H
#define BASETRAJECTORY_H

#include "conf.h"
#include "typedef.h"
#include "VelocityCurve.h"
#include <QVector>
#include <string>
#include <fstream>

namespace MP {

class BaseTrajectory
{
public:
    BaseTrajectory(const QVector<Pose> *samples,
                   const VelocityCurveInfo *velocity_curve_info);
    virtual ~BaseTrajectory();

    virtual bool IsSamplesAvailable() const = 0;
    virtual void GetTrajectory(double cycle_in_millsecond,
                               QVector<Pose> *pose_per_cycle,
                               bool enable_log = false,
                               std::__cxx11::string log_file = "",
                               bool re_calculate = true) = 0;
    virtual void GetIncrease(double cycle_in_millsecond,
                             QVector<Pose> *increase_per_cycle,
                             bool enable_log = false,
                             std::__cxx11::string log_file = "",
                             bool re_calculate = true) = 0;
    virtual int GetNumberOfPos(double t, double cycle_in_millsecond) = 0;

protected:
    const QVector<Pose> *samples;

    const VelocityCurveInfo *velocity_curve_info;
    VelocityCurve *velocity_curve;
};

}   // end of namespace MP

#endif // BASETRAJECTORY_H
