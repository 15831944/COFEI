#include "xyzr_fouraxis_ik.h"
#include "unit.h"

namespace MP {

XyzrFourAxisIk::XyzrFourAxisIk(double tool_projective_length, double tool_origin_angle)
{
    this->tool_projective_length = tool_projective_length;
    this->tool_origin_angle = tool_origin_angle;
}

XyzrFourAxisIk::~XyzrFourAxisIk()
{
}

Pose XyzrFourAxisIk::CartToJnt(const Pose &increase, double current_angle, double next_angle)
{
    Pose increase_;
    increase_.x = increase.x - tool_projective_length * (cos(degree2radian(next_angle + tool_origin_angle)) - cos(degree2radian(current_angle + tool_origin_angle)));
    increase_.y = increase.y - tool_projective_length * (sin(degree2radian(next_angle + tool_origin_angle)) - sin(degree2radian(current_angle + tool_origin_angle)));
    increase_.z = increase.z;
    increase_.roll = next_angle - current_angle;

    return increase_;
}

}   // end of namespace MP
