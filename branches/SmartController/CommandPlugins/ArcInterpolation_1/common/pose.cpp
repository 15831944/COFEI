#include "pose.h"

namespace route_planning_typedef {

Pose::Pose(double x, double y, double z, double yaw, double pitch, double roll) : Position(x, y, z), Posture(yaw, pitch, roll)
{}

Pose::Pose(Position position, Posture posture) : Position(position), Posture(posture)
{}

Pose::Pose() : HomogeneousTransformationMatrix(), Position(), Posture()
{}

void Pose::set_position(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Pose::set_posture(double yaw, double pitch, double roll)
{
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
}

void Pose::scale(const double unit)
{
    this->x *= unit;
    this->y *= unit;
    this->z *= unit;
    this->yaw *= unit;
    this->pitch *= unit;
    this->roll *= unit;
}

mat Pose::get_homogeneous_transformation_matrix()
{
    get_trans_rot_matrix(x, y, z, yaw, pitch, roll);
}
}
