#ifndef POSE_H
#define POSE_H

#include "posture.h"
#include "position.h"

namespace route_planning_typedef {
// 表示位态 = 位置 + 姿态
class Pose : public Position, public Posture
{
public:
    Pose(double x, double y, double z, double yaw, double pitch, double roll);
    Pose(Position position, Posture posture);
    Pose();

    void set_position(double x, double y, double z);
    void set_posture(double yaw, double pitch, double roll);
    void scale(const double unit);   // 对单位进行缩放

    virtual mat get_homogeneous_transformation_matrix();
};
}   // end namespace route_planning_typedef

#endif // POSE_H
