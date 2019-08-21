#include "position.h"

namespace route_planning_typedef {

Position::Position(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

mat Position::get_homogeneous_transformation_matrix()
{
    return get_trans_matrix(x, y, z);
}

void Position::set_position(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Position::set_x(double x)
{
    this->x = x;
}

double Position::get_x() const
{
    return this->x;
}

void Position::set_y(double y)
{
    this->y = y;
}

double Position::get_y() const
{
    return this->y;
}

void Position::set_z(double z)
{
    this->z = z;
}

double Position::get_z() const
{
    return this->z;
}
}
