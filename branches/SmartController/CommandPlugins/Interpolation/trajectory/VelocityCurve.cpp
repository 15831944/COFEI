#include "VelocityCurve.h"

namespace MP {

VelocityCurve::VelocityCurve(double start_pos, double end_pos,
                             const VelocityCurveInfo *velocity_curve_info)
{
    this->start_pos = start_pos;
    this->end_pos = end_pos;
    this->velocity_curve_info = velocity_curve_info;
    velocity_curve = nullptr;

    createVelocityCurve();
}

VelocityCurve::~VelocityCurve()
{
    if (velocity_curve)
        delete velocity_curve;
}

double VelocityCurve::Duration()
{
    return velocity_curve->Duration();
}

double VelocityCurve::Pos(double t)
{
    return velocity_curve->Pos(t);
}

KDL::VelocityProfile *VelocityCurve::GetVelocityProfile() const
{
    return velocity_curve;
}

void VelocityCurve::createVelocityCurve()
{
    switch (velocity_curve_info->type) {
    case VelocityCurveInfo::VelocityCurveType::RECTANGLE:
        velocity_curve = createRectangleVelocityCurve();
        break;
    case VelocityCurveInfo::VelocityCurveType::TRAPHALF:
        velocity_curve = createTrapHalfVelocityCurve();
        break;
    case VelocityCurveInfo::VelocityCurveType::TRAP:
        velocity_curve = createTrapVelocityCurve();
        break;
    case VelocityCurveInfo::VelocityCurveType::SPLINE:
        velocity_curve = createSplineVelocityCurve();
        break;
    default:
        throw std::logic_error("VelocityProfile::init(), unknown velocity profile type");
    }
}

KDL::VelocityProfile *VelocityCurve::createRectangleVelocityCurve()
{
    KDL::VelocityProfile_Rectangular *vel_curve = new KDL::VelocityProfile_Rectangular();

    double max_vel = millmeter2meter(velocity_curve_info->max_vel);
    vel_curve->SetMax(max_vel);
    vel_curve->SetProfile(start_pos, end_pos);

    return vel_curve;
}

KDL::VelocityProfile *VelocityCurve::createTrapHalfVelocityCurve()
{
    KDL::VelocityProfile_TrapHalf *vel_curve = new KDL::VelocityProfile_TrapHalf();

    double max_vel = millmeter2meter(velocity_curve_info->max_vel);
    double max_acc = millmeter2meter(velocity_curve_info->max_acc);
    vel_curve->SetMax(max_vel, max_acc, true);
    vel_curve->SetProfile(start_pos, end_pos);

    return vel_curve;
}

KDL::VelocityProfile *VelocityCurve::createTrapVelocityCurve()
{
    KDL::VelocityProfile_Trap *vel_curve = new KDL::VelocityProfile_Trap();

    double max_vel = millmeter2meter(velocity_curve_info->max_vel);
    double max_acc = millmeter2meter(velocity_curve_info->max_acc);
    vel_curve->SetMax(max_vel, max_acc);
    vel_curve->SetProfile(start_pos, end_pos);

    return vel_curve;
}

KDL::VelocityProfile *VelocityCurve::createSplineVelocityCurve()
{
    KDL::VelocityProfile_Spline *vel_curve = new KDL::VelocityProfile_Spline();

    double vel_at_start = millmeter2meter(velocity_curve_info->vel_at_start);
    double acc_at_start = millmeter2meter(velocity_curve_info->acc_at_start);
    double vel_at_end = millmeter2meter(velocity_curve_info->vel_at_end);
    double acc_at_end = millmeter2meter(velocity_curve_info->acc_at_end);
    double duration = velocity_curve_info->duration;
    vel_curve->SetProfileDuration(start_pos, vel_at_start, acc_at_start,
                                  end_pos, vel_at_end, acc_at_end, duration);

    return vel_curve;
}

double VelocityCurve::millmeter2meter(double i)
{
    return i * 0.001;
}

}   // endf of namespace MP
