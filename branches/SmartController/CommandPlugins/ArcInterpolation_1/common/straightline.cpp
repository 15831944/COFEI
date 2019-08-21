#include "straightline.h"
#include "MyGlobal.h"
#include <QDebug>

namespace route_planning_typedef {

StraightLine::StraightLine(SegmentInfo & segment_info) : Segment(segment_info)
{
    increase_per_interpolation_cycle = new QVector<Pose>;
}

StraightLine::~StraightLine()
{
    delete increase_per_interpolation_cycle;
}

bool StraightLine::init(QString & errMsg)
{
    if (!set_teach_points(errMsg))
        return false;

    if (!set_length(errMsg))
        return false;

    if (!set_interpolation_num(errMsg))
        return false;

    calculate_increase();

    return true;
}

// 直线插补，每次的增量是一样的
Pose StraightLine::get_increase(int i)
{
    if (increase_per_interpolation_cycle->isEmpty())
        calculate_increase();

    return (*increase_per_interpolation_cycle)[0];
}

Pose StraightLine::get_start_teach_point() const
{
    return start_teach_point;
}

Pose StraightLine::get_end_teach_point() const
{
    return end_teach_point;
}

int StraightLine::get_interpolation_num() const
{
    return interpolation_num;
}

void StraightLine::calculate_increase()
{
    Pose increase;
    increase.yaw = 0;
    increase.pitch = 0;
    increase.roll = 0;

    // 每次插补，x y和z方向的增量
    increase.x = (end_teach_point.x - start_teach_point.x) / interpolation_num;
    increase.y = (end_teach_point.y - start_teach_point.y) / interpolation_num;
    increase.z = (end_teach_point.z - start_teach_point.z) / interpolation_num;

    increase_per_interpolation_cycle->push_back(increase);
}

bool StraightLine::set_interpolation_num(QString & errMsg)
{
    if (displacement_per_interpolation_cycle <= 0) {
        errMsg = "错误的插补步长";
        qDebug() << "错误的插补步长";
        return false;
    }

    if ((interpolation_num = length / displacement_per_interpolation_cycle) <= 0) { // 插补次数
        errMsg = "错误的插补次数";
        qDebug() << "错误的插补次数";
        return false;
    }
    return true;
}

bool StraightLine::set_teach_points(QString &errMsg)
{
    qDebug() << "set teach points" << endl;
    if (2 != teach_points.size()) {
        errMsg = "Error: 直线段有且仅能有两个示教点";
        return false;
    }

    // 各个方向对应的轴Id
    QString x_axis = config->motion_info->axises["X"].id;
    QString y_axis = config->motion_info->axises["Y"].id;
    QString z_axis = config->motion_info->axises["Z"].id;
    QString yaw_axis = config->motion_info->axises["Yaw"].id;
    QString pitch_axis = config->motion_info->axises["Pitch"].id;
    QString roll_axis = config->motion_info->axises["Roll"].id;

    // 快速定位中定义的示教点的Id
    QString & start_teach_point_id = teach_points[1];
    QString & end_teach_point_id = teach_points[2];

    qDebug() << "start_teach_point_id: " << start_teach_point_id << "\n"
             << "end_teach_point_id: " << end_teach_point_id << endl;

    auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
    if (!item.contains(start_teach_point_id)) {
        qDebug() << "Error: 没有在快速定位中找到指定的起点示教点";
        errMsg = "Error: 没有在快速定位中找到指定的起点示教点";
        return false;
    }
    if (!item.contains(end_teach_point_id)) {
        qDebug() << "Error: 没有在快速定位中找到指定的终点示教点";
        errMsg = "Error: 没有在快速定位中找到指定的终点示教点";
        return false;
    }

    for (auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.begin();
         item != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.end(); item++)
    {
        //从快速定位部分获取校准点信息
        if (start_teach_point_id == item.key()) {
            start_teach_point.x = item.value().axisMap[x_axis].pos;
            start_teach_point.y = item.value().axisMap[y_axis].pos;
            start_teach_point.z = item.value().axisMap[z_axis].pos;
            start_teach_point.yaw = item.value().axisMap[yaw_axis].pos;
            start_teach_point.pitch = item.value().axisMap[pitch_axis].pos;
            start_teach_point.roll = item.value().axisMap[roll_axis].pos;
        }

        if (end_teach_point_id == item.key()) {
            end_teach_point.x = item.value().axisMap[x_axis].pos;
            end_teach_point.y = item.value().axisMap[y_axis].pos;
            end_teach_point.z = item.value().axisMap[z_axis].pos;
            end_teach_point.yaw = item.value().axisMap[yaw_axis].pos;
            end_teach_point.pitch = item.value().axisMap[pitch_axis].pos;
            end_teach_point.roll = item.value().axisMap[roll_axis].pos;
        }
    }
    start_teach_point.scale(0.1);
    end_teach_point.scale(0.1);

    qDebug() << "start_teach_point:"
             << "\n\tx: " << start_teach_point.x
             << "\n\ty: " << start_teach_point.y
             << "\n\tz: " << start_teach_point.z
             << "\n\tyaw: " << start_teach_point.yaw
             << "\n\tpitch: " << start_teach_point.pitch
             << "\b\troll: " << start_teach_point.roll << endl;
    qDebug() << "end_teach_point:"
             << "\n\tx: " << end_teach_point.x
             << "\n\ty: " << end_teach_point.y
             << "\n\tz: " << end_teach_point.z
             << "\n\tyaw: " << end_teach_point.yaw
             << "\n\tpitch: " << end_teach_point.pitch
             << "\b\troll: " << end_teach_point.roll << endl;

    if (!is_equal(start_teach_point.roll, end_teach_point.roll)) {
        qDebug() << "示教直线段时，起点和终点需要保持相同的旋转角度．";
        errMsg = "示教直线段时，起点和终点需要保持相同的旋转角度．";
        return false;
    }

    return true;
}

bool StraightLine::set_length(QString & errMsg)
{
    length = sqrt((start_teach_point.x - end_teach_point.x) * (start_teach_point.x - end_teach_point.x) +
                  (start_teach_point.y - end_teach_point.y) * (start_teach_point.y - end_teach_point.y) +
                  (start_teach_point.z - end_teach_point.z) * (start_teach_point.z - end_teach_point.z));
    if (0 == length) {
        errMsg = "示教的直线长度为０．";
        return false;
    }

    qDebug() << "straight length: " << length << endl;
    return true;
}

QVector<mat> * StraightLine::get_pose(QString & errMsg)
{

}

QVector<mat> * StraightLine::re_get_pose(QString & errMsg)
{

}

}   // end namespace route_planning_tyepdef


