#ifndef END_PARAMS_H
#define END_PARAMS_H

#include "homogeneous_transformation.h"
#include "typedef.h"
#include "QVector"
#include "pose.h"

using namespace route_planning_typedef;

// 末端参数
class EndParams
{
public:
    EndParams(double x = 0, double y = 0, double z = 0, double yaw = 0, double pitch = 0, double roll = 0);

    QVector<Pose> * get(const QVector<mat> * const pose, QString & errMsg); // 根据位姿矩阵反推参数

    void set_length(double length);  // 设置胶筒长度
    double get_length() const;

    void set_yaw(double yaw);
    double get_yaw() const;

    void set_pitch(double pitch);
    double get_pitch() const;

    void set_roll(double roll);
    double get_roll() const;

private:
    double x, y, z; // 机械臂末端的坐标位置
    double length;  // 胶筒的长度
    double yaw;  // 偏航角
    double pitch;  // 俯仰角
    double roll;  // 滚动角
};

#endif // END_PARAMS_H
