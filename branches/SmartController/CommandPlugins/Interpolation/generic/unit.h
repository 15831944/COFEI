#ifndef UNIT_H
#define UNIT_H

#include "typedef.h"
#include <cmath>

namespace MP {

inline double degree2radian(double degree)
{
    return degree * M_PI / 180.0;
}

inline double radian2degree(double radian)
{
    return radian * 180.0 / M_PI;
}

inline double millmeter2meter(double mill_second)
{
    return mill_second * 0.001;
}

inline double meter2millmeter(double second)
{
    return second * 1000;
}

inline bool is_equal(double a, double b)
{
    double c = a - b;
    if (c < 0)
        c = -c;

    if (c < 0.01)
        return true;
    return false;
}

inline bool is_equal(const Position &p1, const Position &p2)
{
    double x = p1.x - p2.x;
    if (x < 0)
        x = -x;
    double y = p1.y - p2.y;
    if (y < 0)
        y = -y;
    double z = p1.z - p2.z;
    if (z < 0)
        z = -z;

    if (x < 0.01 && y < 0.01 && z < 0.01)
        return true;
    return false;
}

}   // end of namespace MP

#endif  // UNIT_H
