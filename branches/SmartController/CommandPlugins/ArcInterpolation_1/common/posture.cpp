#include "posture.h"

namespace route_planning_typedef {

Posture::Posture(double yaw, double pitch, double roll)
{
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
}

mat Posture::get_homogeneous_transformation_matrix()
{
    return get_rot_matrix(yaw, pitch, roll);
}

void Posture::set_posture(double yaw, double pitch, double roll)
{
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
}

void Posture::set_yaw(double yaw)
{
    this->yaw = yaw;
}

double Posture::get_yaw() const
{
    return this->yaw;
}

void Posture::set_pitch(double pitch)
{
    this->pitch = pitch;
}

double Posture::get_pitch() const
{
    return this->pitch;
}

void Posture::set_roll(double roll)
{
    this->roll = roll;
}

double Posture::get_roll() const
{
    return this->roll;
}
}
