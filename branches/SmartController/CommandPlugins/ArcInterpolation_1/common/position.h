#ifndef POSITION_H
#define POSITION_H

#include "homogeneous_transformation.h"

namespace route_planning_typedef {

// 表示点的坐标位置
class Position : virtual public HomogeneousTransformationMatrix
{
public:
    Position(double x = 0.0, double y = 0.0, double z = 0.0);

    virtual mat get_homogeneous_transformation_matrix();

    void set_position(double x, double y, double z);

    void set_x(double x);
    double get_x() const;

    void set_y(double y);
    double get_y() const;

    void set_z(double z);
    double get_z() const;

public:
    double x, y, z;
};
}   // end namespace route_planning_typedef

#endif // POSITION_H
