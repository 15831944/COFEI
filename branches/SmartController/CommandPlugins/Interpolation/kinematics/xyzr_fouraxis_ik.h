#ifndef XYZRFOURAXISIK_H
#define XYZRFOURAXISIK_H


#include "typedef.h"

namespace MP {

class XyzrFourAxisIk
{
public:
    XyzrFourAxisIk(double tool_projective_length, double tool_origin_angle);
    ~XyzrFourAxisIk();

    Pose CartToJnt(const Pose &increase, double current_angle, double next_angle);

private:
    double tool_projective_length;  // unit: mm
    double tool_origin_angle;       // unit: radian
};

}   // end of namespace MP

#endif // XYZRFOURAXISIK_H
