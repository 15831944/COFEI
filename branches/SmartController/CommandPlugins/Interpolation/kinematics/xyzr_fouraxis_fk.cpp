#include "xyzr_fouraxis_fk.h"
#include "unit.h"

namespace MP {

XyzrFourAxisFK::XyzrFourAxisFK(double tool_projective_length, double tool_origin_angle)
{
    this->tool_projective_length = tool_projective_length;
    this->tool_origin_angle = tool_origin_angle;
}

XyzrFourAxisFK::~XyzrFourAxisFK()
{

}

Pose XyzrFourAxisFK::JntToCart(double x, double y, double z, double roll)
{
    Pose p;
    p.x = x + tool_projective_length * cos(degree2radian(roll + tool_origin_angle));
    p.y = y + tool_projective_length * sin(degree2radian(roll + tool_origin_angle));
    p.z = z;

    return p;
}

}   // end of namespace MP

