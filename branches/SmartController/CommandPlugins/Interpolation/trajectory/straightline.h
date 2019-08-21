#ifndef STRAIGHTLINE_H
#define STRAIGHTLINE_H

#include "BaseTrajectory.h"
#include <frames.hpp>
#include <frames_io.hpp>
#include <path_line.hpp>
#include <trajectory.hpp>
#include <trajectory_segment.hpp>
#include <trajectory_composite.hpp>
#include <trajectory_composite.hpp>
#include <trajectory_composite.hpp>
#include <trajectory_stationary.hpp>
#include <rotational_interpolation_sa.hpp>
#include <string>

namespace MP {

class StraightLine : public BaseTrajectory
{
public:
    StraightLine(const QVector<Pose> *samples,
                 const VelocityCurveInfo *velocity_curve_info);
    virtual ~StraightLine();

    virtual bool IsSamplesAvailable() const;
    virtual void GetTrajectory(double cycle_in_millsecond,
                               QVector<Pose> *pose_per_cycle,
                               bool enable_log = false,
                               std::__cxx11::string log_file = "",
                               bool re_calculate = true);
    virtual void GetIncrease(double cycle_in_millsecond,
                             QVector<Pose> *increase_per_cycle,
                             bool enable_log = false,
                             std::__cxx11::string log_file = "",
                             bool re_calculate = true);
    int GetNumberOfPos(double t, double cycle_in_millsecond);

private:
    void buildPath();
    void buildVelocityProfile();
    void buildTrajectory();

    void logTrajectory(std::__cxx11::string log_file, const QVector<KDL::Frame> &pose);

private:
    KDL::Path *path;
    KDL::Trajectory *trajectory;
    const int kNumOfSamples = 2;
};

}   // end of namespace MP

#endif // STRAIGHTLINE_H
