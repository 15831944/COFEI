#ifndef XYZRFOURAXISFK_H
#define XYZRFOURAXISFK_H

#include "typedef.h"
#include "unit.h"

namespace MP {

class XyzrFourAxisFK
{
public:
    XyzrFourAxisFK(double tool_projective_length, double tool_origin_angle);
    ~XyzrFourAxisFK();

    Pose JntToCart(double x, double y, double z, double roll);

private:
    double tool_projective_length;  // unit: mm
    double tool_origin_angle;       // unit: radian
};

}   // end of namespace MP

#endif // XYZRFOURAXISFK_H
