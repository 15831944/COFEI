#ifndef VELOCITYCURVE_H
#define VELOCITYCURVE_H

#include <velocityprofile_rect.hpp>
#include <velocityprofile_trap.hpp>
#include <velocityprofile_traphalf.hpp>
#include <velocityprofile_spline.hpp>

namespace MP {

class VelocityCurveInfo
{
public:
    enum VelocityCurveType {
        RECTANGLE,
        TRAPHALF,
        TRAP,
        SPLINE,
        UnDefined
    };

    VelocityCurveType type;
    double max_vel;
    double max_acc;
    double vel_at_start, acc_at_start;
    double vel_at_end, acc_at_end;
    double duration;
};

class VelocityCurve final
{
public:
    VelocityCurve(double start_pos, double end_pos,
                  const VelocityCurveInfo *velocity_curve_info);
    ~VelocityCurve();

    double Duration();
    double Pos(double t);
    KDL::VelocityProfile *GetVelocityProfile() const;

private:
    void createVelocityCurve();
    KDL::VelocityProfile *createRectangleVelocityCurve();
    KDL::VelocityProfile *createTrapHalfVelocityCurve();
    KDL::VelocityProfile *createTrapVelocityCurve();
    KDL::VelocityProfile *createSplineVelocityCurve();

    double millmeter2meter(double i);

private:
    const VelocityCurveInfo *velocity_curve_info;
    KDL::VelocityProfile *velocity_curve;
    double start_pos, end_pos;
};

}   // end of namespace MP

#endif // VELOCITYCURVE_H
