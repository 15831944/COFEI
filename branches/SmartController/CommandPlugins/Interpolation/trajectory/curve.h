#ifndef CURVE_H
#define CURVE_H

#include <string>
#include "Spline3D.h"
#include "BaseTrajectory.h"
#include "VelocityCurve.h"

namespace MP {

using std::string;

class Curve : public BaseTrajectory
{
public:
    Curve(const QVector<Pose> *samples,
          const VelocityCurveInfo *velocity_curve_info);
    ~Curve();

    virtual bool IsSamplesAvailable() const;
    virtual void GetTrajectory(double cycle_in_millsecond,
                               QVector<Pose> * pose_per_cycle,
                               bool log_trajectory = false,
                               std::__cxx11::string log_file = "",
                               bool re_calculate = true);
    virtual void GetIncrease(double cycle_in_millsecond,
                             QVector<Pose> *increase_per_cycle,
                             bool enable_log = false,
                             std::__cxx11::string log_file = "",
                             bool re_calculate = true);

    void GetTotalDisaplacement(double cycle_in_millsecond);
    int GetNumberOfPos(double t, double cycle_in_millsecond);

private:
    void init();
    void buildPath();
    void buildVelocityProfile();

    void logTrajectory(std::__cxx11::string log_file, const QVector<Pose> &pose);

private:
    Spline3D *path;

    double angle_per_interpolation_cycle;
    double total_displacement;

    const int kMinSamplesNumber = 5;
};


}   // end of namespace MP

#endif // CURVE_H
