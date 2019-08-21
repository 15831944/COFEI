#ifndef POSTURE_H
#define POSTURE_H

#include "homogeneous_transformation.h"

namespace route_planning_typedef {

// 表示姿态, 即关于主坐标轴x, y, z旋转的角度
// 用x轴表示目标点的位姿,
// 绕x轴的旋转角为偏航角(yaw); 绕y轴的旋转角为俯仰角(pitch)； 绕z轴的旋转角为滚动角(roll)
class Posture : virtual public HomogeneousTransformationMatrix
{
public:
    Posture(double yaw = 0, double pitch = 0, double roll = 0);

    virtual arma::mat get_homogeneous_transformation_matrix();

    void set_posture(double yaw, double pitch, double roll);

    void set_yaw(double yaw);
    double get_yaw() const;

    void set_pitch(double pitch);
    double get_pitch() const;

    void set_roll(double roll);
    double get_roll() const;

public:
    double yaw, pitch, roll;
};

}   // end namespace route_planning_typedef

#endif // POSTURE_H
