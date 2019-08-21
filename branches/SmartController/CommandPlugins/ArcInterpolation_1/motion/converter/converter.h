#ifndef CONVERTER_H
#define CONVERTER_H

#include <QMap>
#include <QVector>
#include <QString>
#include "config.h"
#include "typedef.h"
#include "pose.h"
#include "segment.h"

using namespace route_planning_typedef;

class MotionParams
{
public:
    QMap<QString, int> increase;

    int interpolation_num;
};

// 将坐标系中的位姿信息，转换为给伺服的指令信息
class Converter
{
public:
    Converter(QVector<Pose> * pose_in_base_coordinate_ = NULL);
    virtual ~Converter();

    void set_pose_in_base_coordinate(QVector<Pose> * pose_in_base_coordinate_);
    QMap<QString, QVector<int>> * convert(QString & errMsg);
    void clear();

    bool convert(Pose & pose);

private:
    QVector<Pose> *  pose_in_base_coordinate_;    // 目标位姿的坐标表示
    QMap<QString, QVector<int>> * pose_in_user_unit_;    // 用户单位表示的目标位姿
public:
    double kXDisplacementPerUserUnit;   // X轴伺服电机,一个指令单位对应的位移量，单位: mm
    double kYDisplacementPerUserUnit;   // Y轴伺服电机,一个指令单位对应的位移量，单位: mm
    double kZDisplacementPerUserUnit;   // Z轴伺服电机,一个指令单位对应的位移量，单位: mm
    double kYawDegreePerUserUnit;   // 控制偏航角的伺服电机,一个指令单位对应的旋转角度, 单位: degree
    double kPitchDegreePerUserUnit;    // 控制仰角的伺服电机, 一个指令单位对应的旋转角度, 单位: degree
    double kRollDegreePerUserUnit; // 控制滚动角的伺服电机, 一个指令单位对应的旋转角度, 单位: degree
};

#endif // CONVERTER_H
