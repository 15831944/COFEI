#include "BaseTrajectory.h"
#include <QString>

namespace MP {

BaseTrajectory::BaseTrajectory(const QVector<Pose> *samples,
               const VelocityCurveInfo *velocity_curve_info)
{
    this->samples = samples;
    this->velocity_curve_info = velocity_curve_info;
    this->velocity_curve = nullptr;

    //    setToolDegreeOffset(Config::GetInstance()->motion_info->tool_degree_offset);
    //    setToolProjectionLeneth(Config::GetInstance()->motion_info->tool_projection_length);
}

//BaseTrajectory::BaseTrajectory(const TrajectoryInfo *trajectory_info) : BaseTrajectory()
//{
//    samples = new QVector<Position>;
//    this->trajectory_info = trajectory_info;

//    setTeachPoints();
//}

BaseTrajectory::~BaseTrajectory()
{}

//Pose BaseTrajectory::get_increase(int i)
//{
//    if (increase_per_interpolation_cycle->isEmpty())
//        throw std::logic_error((trajectory_info->id + ": increase is empty.").toStdString());
//    if (i >= increase_per_interpolation_cycle->size())
//        throw std::logic_error((trajectory_info->id + ": increase index out of range.").toStdString());

//    return (*increase_per_interpolation_cycle)[i];
//}

//void BaseTrajectory::SetTrajectoryInfo(const TrajectoryInfo *trajectory_info)
//{
//    this->trajectory_info = trajectory_info;
//}

//void BaseTrajectory::setToolProjectionLeneth(double tool_projection_length)
//{
//    this->tool_projection_length = tool_projection_length;
//}

//void BaseTrajectory::setToolDegreeOffset(double tool_degree_offsett)
//{
//    this->tool_degree_offsett = tool_degree_offsett;
//}

//void BaseTrajectory::setTeachPoints()
//{
//    QString J1 = Config::GetInstance()->motion_info->axises_info["J1"].id;
//    QString J2 = Config::GetInstance()->motion_info->axises_info["J2"].id;
//    QString J3 = Config::GetInstance()->motion_info->axises_info["J3"].id;
//    QString J4 = Config::GetInstance()->motion_info->axises_info["J4"].id;

//    const auto &Teach_Points_Map = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
//    for (auto teach_point : trajectory_info->path_info.teach_points) {
//        if (!Teach_Points_Map.contains(teach_point))
//            throw std::logic_error("setTeachPoints(): can not find teach point " + teach_point.toStdString());

//        Pose p;
//        if (!J1.isEmpty() && Teach_Points_Map[teach_point].axisMap.contains(J1))
//            p.x = Teach_Points_Map[teach_point].axisMap[J1].pos;
//        if (!J2.isEmpty() && Teach_Points_Map[teach_point].axisMap.contains(J2))
//            p.y = Teach_Points_Map[teach_point].axisMap[J2].pos;
//        if (!J3.isEmpty() && Teach_Points_Map[teach_point].axisMap.contains(J3))
//            p.z = Teach_Points_Map[teach_point].axisMap[J3].pos;
//        if (!J4.isEmpty() && Teach_Points_Map[teach_point].axisMap.contains(J4))
//            p.roll = Teach_Points_Map[teach_point].axisMap[J4].pos;

//        teach_points.push_back(p);
//    }
//}



}   // end of namespace MP
