#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <QMap>
#include <QVector>
#include <QString>
#include <QDebug>
#include <float.h>
#include <VelocityCurve.h>

namespace MP {

class Position
{
public:
    Position(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z)
    {}

    double x, y, z;
};

class Posture
{
public:
    Posture(double yaw = 0, double pitch = 0, double roll = 0) :
        yaw(yaw), pitch(pitch), roll(roll)
    {}

    double yaw, pitch, roll;
};

class Pose : public Position, public Posture
{
public:
    Pose() : Position(), Posture()
    {}

    Pose(double x, double y, double z, double yaw, double pitch, double roll):
        Position(x, y, z),
        Posture(yaw, pitch, roll)
    {}

    void scale(const double unit)
    {
        this->x *= unit;
        this->y *= unit;
        this->z *= unit;
        this->yaw *= unit;
        this->pitch *= unit;
        this->roll *= unit;
    }
};

class AxisInfo
{
public:
    bool isOutOfLimit(double value) const
    {
        if (value < lower_limit || value > upper_limit)
            return true;
        return false;
    }

public:
    QString id;
    double lower_limit;
    double upper_limit;
};

class PathInfo
{
public:
    QString axis_group;
    QString type;
    QString mode;
    QString data_file;
    QString offset_file;
    QString rotate_R;
    double rotation_angle_R;
    QVector<QString> teach_points;
    QVector<QString> sub_pathes;
    QString IO_port;
    double ahead_of_start;
    double ahead_of_end;
    double x_scale_coefficient;
    double y_scale_coefficient;

    void clear()
    {
        axis_group = "";
        type = "";
        mode = "";
        data_file = "";
        offset_file = "";
        rotate_R = "";
        rotation_angle_R = 0;
        teach_points.clear();
        sub_pathes.clear();
        IO_port = "";
        ahead_of_start = 0;
        ahead_of_end = 0;
        x_scale_coefficient = 1;
        y_scale_coefficient = 1;
    }
};

class TrajectoryInfo {
public:
    QString id;
    PathInfo path_info;
    VelocityCurveInfo velocity_curve_info;

    void clear()
    {
        id = "";
        path_info.clear();
    }
};

class MotionInfo
{
public:
    QMap<QString, QMap<QString, AxisInfo>> axises_info;
    QMap<QString, TrajectoryInfo> trajectories_info;
    QString tool_param_file;
    double tool_projection_length;
    double tool_degree_offset;
};

}   // end namespace MP

#endif // TYPEDEF_H
