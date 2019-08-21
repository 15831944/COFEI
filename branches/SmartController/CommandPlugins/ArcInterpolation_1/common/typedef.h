#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <QMap>
#include <QVector>
#include <QString>
#include "posture.h"
#include "pose.h"
#include <QDebug>

namespace route_planning_typedef {

// 段的类型
// 直线，外圆弧，内圆弧
enum SegmentType {STRAIGHTLINE, ARC, UNDEFINED};

// 圆弧的类型
// 未定义，顺时针圆弧，逆时针圆弧
enum ArcType {Undefined, ClockWise, AntiClockWise};

// 表示坐标系变换时的位移
class Trans{
public:
    Trans(double x_trans = 0.0, double y_trans = 0.0, double z_trans = 0.0)
    {
        this->x_trans = x_trans;
        this->y_trans = y_trans;
        this->z_trans = z_trans;
    }

public:
    double x_trans, y_trans, z_trans;
};

// 表示坐标系变换时的旋转
class Rot
{
public:
    Rot(double x_rot = 0.0, double y_rot = 0.0, double z_rot = 0.0)
    {
        this->x_rot = x_rot;
        this->y_rot = y_rot;
        this->z_rot = z_rot;
    }

public:
    double x_rot, y_rot, z_rot;
};

// 表示坐标系变换时的位移和旋转
class TransRot : public Trans, public Rot
{
public:
    TransRot(double x_trans = 0.0, double y_trans = 0.0, double z_trans = 0.0, double x_rot = 0.0, double y_rot = 0.0, double z_rot = 0.0) :
        Trans(x_trans, y_trans, z_trans), Rot(x_rot, y_rot, z_rot)
    {}
};

// 一段
class SegmentInfo
{
public:
    int Id;
    int mode;   // 段的类型，CAD数据导入，点示教
    SegmentType type;   // 段类型
    QString file;
    QString posture_teach_point;

    // to do...
    // 直线/圆弧示教点，使用固定的名称，作为Map的key
    QMap<int ,QString> teach_points;
};

// 一条路径
class ProfileInfo
{
public:
    QString id; // 该条路径的Id
    int mode;   // CAD数据导入，已知形状点示教，点示教拟合
    int num;    // 该条路径中段的数量或点的数量
    QMap<int, SegmentInfo> segments;   // 路径中段的信息
    QMap<int, QString> points;
};

class AxisInfo
{
public:
    bool is_out_of_soft_limit(const double & value) const   // 判断是否超出软极限限制
    {
        if (value < soft_limit_start || value > soft_limit_end)
            return true;
        return false;
    }

public:
    QString id; // 每个方向对应的轴ID
    double soft_limit_start;    // 软极限起点
    double soft_limit_end; // 软极限终点
};


class MotionInfo
{
public:
    QString Id;
    QMap<QString, AxisInfo> axises;   // 每个方向对应的轴ID
    TransRot trans_rot_1;   // 基坐标系到工作台坐标系的变换
    TransRot trans_rot_2;   // 工作台坐标系到工件坐标系的变换
    QMap<QString, ProfileInfo> all_profile_info;
    double tool_projection_length;   // 胶筒在平面的投影长度
    double tool_init_degree;    // 工具起始状态下和x轴正方向的角度（以角度为单位）
    QString camera_calibration_file;
};

}   // end namespace route_planning_typedef

#endif // TYPEDEF_H
