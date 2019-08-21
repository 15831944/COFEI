#include "conf.h"
#include <float.h>
#include <iostream>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>

namespace MP {

using std::cout;
using std::endl;

Config::Config()
{
    motion_info = new MotionInfo;
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/参数配置/软件参数配置.xml";
    ReadMotionInfo(path, "运动", *motion_info);

    std::cout << *this;
}

Config::~Config()
{
    if (motion_info)
        delete motion_info;
}

void Config::ReadMotionInfo(QString config_file, const QString &node, MotionInfo &info)
{
    QFile file(config_file);
    if (!file.exists())
        throw std::runtime_error("config file not exists.");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader reader(&file);

        while (!reader.atEnd()) {
            reader.readNext();
            if (reader.isStartElement() && reader.name() == node) {
                while (!reader.atEnd()) {
                    reader.readNext();
                    if (reader.isStartElement()) {
                        QString node = reader.name().toString();
                        if ("轴信息" == node)
                            getAxisId(reader, node, info);
                        else if ("路径信息" == node)
                            getPathes(reader, node, info);
                        else if ("工具参数文件" == node)
                            getToolParamFile(reader, node, info);
//                        else if  ("工具投影长度" == node)
//                            getToolProjectionLength(reader, node, info);
//                        else if ("工具角度偏移" == node)
//                            getToolDegreeOffset(reader, node, info);
                    }
                    else if (reader.isEndElement() && (reader.name()==node))
                        break;
                }
            }
        }
    }
    if (file.isOpen())
        file.close();
}

void Config::getAxisId(QXmlStreamReader &reader, const QString &node, MotionInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        while (reader.isStartElement() && reader.name() == "分组")
        {
            QString group_id;
            QXmlStreamAttributes attributes = reader.attributes();
            if (attributes.hasAttribute("Id"))
                group_id = attributes.value("Id").toString();

            QMap<QString, AxisInfo> axis_group_info;

            while (!reader.atEnd())
            {
                reader.readNext();
                if (reader.isStartElement())
                {
                    QString node, id;
                    node = reader.name().toString();
                    if ("J1" == node || "J2" == node || "J3" == node ||
                        "J4" == node || "J5" == node || "J6" == node)
                    {
                        id = reader.readElementText();
                        AxisInfo axis_info;
                        axis_info.id = id;
                        GetAxisLimit(axis_info);
                        axis_group_info.insert(node, axis_info);
                    }
                }
                else if (reader.isEndElement() && reader.name()=="分组")
                        break;
            }
            info.axises_info.insert(group_id, axis_group_info);
        }

        if (reader.isEndElement() && reader.name()==node)
            break;
    }


//    while (!reader.atEnd()) {
//        reader.readNext();
//        if (reader.isStartElement()) {
//            QString node, id;
//            node = reader.name().toString();
//            if ("J1" == node || "J2" == node || "J3" == node ||
//                    "J4" == node || "J5" == node || "J6" == node) {
//                id = reader.readElementText();
//                AxisInfo axis_info;
//                axis_info.id = id;
//                GetAxisLimit(axis_info);
//                info.axises_info.insert(node, axis_info);
//            }
//        }
//        else if (reader.isEndElement() && reader.name()==node)
//            break;
//    }
}

void Config::getPathes(QXmlStreamReader &reader, const QString &node, MotionInfo &info)
{
//    while (!reader.atEnd()) {
//        reader.readNext();
//        if (reader.isStartElement() && reader.name() == "路径") {
//            QString node = reader.name().toString();
//            TrajectoryInfo trajectory_info;

//            getPathAttribute(reader, trajectory_info);
//            getPathContent(reader, node, trajectory_info);

//            info.trajectories_info.insert(trajectory_info.id, trajectory_info);
//        }
//        else if (reader.isEndElement() && (reader.name() == node))
//            break;
//    }
    while (!reader.atEnd())
    {
        reader.readNext();
        while (reader.isStartElement() && reader.name() == "路径")
        {
            QString node = reader.name().toString();
            TrajectoryInfo trajectory_info;
            trajectory_info.clear();
            getPathAttribute(reader, trajectory_info);
            getPathContent(reader, node, trajectory_info);
            info.trajectories_info.insert(trajectory_info.id, trajectory_info);
        }

        if (reader.isEndElement() && reader.name()==node)
            break;
    }
}


void Config::getPathAttribute(QXmlStreamReader &reader, TrajectoryInfo &trajectory_info)
{
    QXmlStreamAttributes attributes = reader.attributes();

    trajectory_info.id = "";
    if(attributes.hasAttribute("Id"))
        trajectory_info.id = attributes.value("Id").toString();

    trajectory_info.path_info.type = "";
    if (attributes.hasAttribute("Type"))
        trajectory_info.path_info.type = attributes.value("Type").toString();

    trajectory_info.path_info.mode = "";
    if (attributes.hasAttribute("Mode"))
        trajectory_info.path_info.mode = attributes.value("Mode").toString();

    trajectory_info.velocity_curve_info.type = VelocityCurveInfo::UnDefined;
    if (attributes.hasAttribute("VelocityProfile"))
        trajectory_info.velocity_curve_info.type = GetVelocityCurveType(attributes.value("VelocityProfile").toString());

    trajectory_info.velocity_curve_info.max_vel = DBL_MIN;
    if (attributes.hasAttribute("MaxVel"))
        trajectory_info.velocity_curve_info.max_vel = attributes.value("MaxVel").toDouble();

    trajectory_info.velocity_curve_info.max_acc = DBL_MIN;
    if (attributes.hasAttribute("MaxAcc"))
        trajectory_info.velocity_curve_info.max_acc = attributes.value("MaxAcc").toDouble();

    trajectory_info.velocity_curve_info.vel_at_start = DBL_MIN;
    if (attributes.hasAttribute("StartVel"))
        trajectory_info.velocity_curve_info.vel_at_start = attributes.value("StartVel").toDouble();

    trajectory_info.velocity_curve_info.acc_at_start = DBL_MIN;
    if (attributes.hasAttribute("StartAcc"))
        trajectory_info.velocity_curve_info.acc_at_start = attributes.value("StartAcc").toDouble();

    trajectory_info.velocity_curve_info.vel_at_end = DBL_MIN;
    if (attributes.hasAttribute("EndVel"))
        trajectory_info.velocity_curve_info.vel_at_end = attributes.value("EndVel").toDouble();

    trajectory_info.velocity_curve_info.acc_at_end = DBL_MIN;
    if (attributes.hasAttribute("EndAcc"))
        trajectory_info.velocity_curve_info.acc_at_end = attributes.value("EndAcc").toDouble();

    trajectory_info.velocity_curve_info.duration = DBL_MIN;
    if (attributes.hasAttribute("Duration"))
        trajectory_info.velocity_curve_info.duration = attributes.value("Duration").toDouble();
}

void Config::getPathContent(QXmlStreamReader &reader, const QString &end_node, TrajectoryInfo &trajectory_info)
{
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.isStartElement()) {
            if ("轴信息" == reader.name()) {
                QString axis_group = reader.readElementText();
                trajectory_info.path_info.axis_group = axis_group;
                std::cout << "get axis info" << std::endl;
            }
            else if ("示教点" == reader.name()) {
                QString point = reader.readElementText();
                trajectory_info.path_info.teach_points.push_back(point);
            }
            else if ("子路径" == reader.name()) {
                QString sub_path = reader.readElementText();
                trajectory_info.path_info.sub_pathes.push_back(sub_path);
            }
            else if ("数据文件" == reader.name()) {
                QString data_file = reader.readElementText();
                trajectory_info.path_info.data_file = data_file;
            }
            else if ("偏移文件" == reader.name()) {
                QString offset_file = reader.readElementText();
                trajectory_info.path_info.offset_file = offset_file;
            }
            else if ("R轴旋转" == reader.name()) {
                QString rotate_R = reader.readElementText();
                trajectory_info.path_info.rotate_R = rotate_R;
            }
            else if ("R轴旋转角度" == reader.name()) {
                double rotation_angle_R = reader.readElementText().toDouble();
                trajectory_info.path_info.rotation_angle_R = rotation_angle_R;
            }
            else if ("IO" == reader.name()) {
                QString io = reader.readElementText();
                trajectory_info.path_info.IO_port = io;
            }
            else if ("出胶提前时间" == reader.name()) {
                double ahead_of_start = reader.readElementText().toDouble();
                trajectory_info.path_info.ahead_of_start = ahead_of_start;
            }
            else if ("停止出胶提前时间" == reader.name()) {
                double ahead_of_end = reader.readElementText().toDouble();
                trajectory_info.path_info.ahead_of_end = ahead_of_end;
            }
            else if ("x缩放系数" == reader.name()) {
                double x_scale_coefficient = reader.readElementText().toDouble();
                trajectory_info.path_info.x_scale_coefficient = x_scale_coefficient;
            }
            else if ("y缩放系数" == reader.name()) {
                double y_scale_coefficient = reader.readElementText().toDouble();
                trajectory_info.path_info.y_scale_coefficient = y_scale_coefficient;
            }
        }
        else if (reader.isEndElement() && (end_node == reader.name()))
            break;
    }
}

void Config::getToolParamFile(QXmlStreamReader &reader, const QString &node, MotionInfo &info)
{
    info.tool_param_file = reader.readElementText();
    while (!reader.atEnd()) {
        if (reader.isEndElement() && reader.name() == node)
            break;
        reader.readNext();
    }
}

void Config::GetAxisLimit(AxisInfo &axis_info)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if (!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(axis_info.id))
        return;
    QString str;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[axis_info.id].swLimitStart;
    axis_info.lower_limit = str.toDouble() * 1000;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[axis_info.id].swLimitEnd;
    axis_info.upper_limit = str.toDouble() * 1000;
}

VelocityCurveInfo::VelocityCurveType Config::GetVelocityCurveType(const QString &str) const
{
    VelocityCurveInfo::VelocityCurveType type = VelocityCurveInfo::UnDefined;
    if (str == "矩形")
        type = VelocityCurveInfo::RECTANGLE;
    else if (str == "半梯形")
        type = VelocityCurveInfo::TRAPHALF;
    else if (str == "梯形")
        type = VelocityCurveInfo::TRAP;
    else if (str == "S形")
        type = VelocityCurveInfo::SPLINE;

    return type;
}

std::ostream &operator <<(std::ostream &out, const Config &config)
{
    out << "Axises Info:\n";
    for (auto item = config.motion_info->axises_info.begin();
         item != config.motion_info->axises_info.end();
         ++item)
    {
        out << "\t" << item.key().toStdString() << ":\n";
        for (auto it = item.value().begin(); it != item.value().end(); ++it)
            out << "\t\t" << it.key().toStdString() << ": "
                << it.value().id.toStdString() << "\n";
    }
//        out << "\t" << item.key().toStdString() << ": "
//            << item.value().id.toStdString() << "\n";

    out << "路径信息:\n";
    for (auto item = config.motion_info->trajectories_info.begin();
         item != config.motion_info->trajectories_info.end();
         ++item) {
        out << "\t路径: "
            << "(Id: " << item.value().id.toStdString() << ", "
            << "Type: " << item.value().path_info.type.toStdString() << ", "
            << "Mode: " << item.value().path_info.mode.toStdString() << ", "
            << "VelocityProfile: " << QString::number(item.value().velocity_curve_info.type, 10, 6).toStdString() << ", "
            << "Max Vel: " << (item.value().velocity_curve_info.max_vel == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.max_vel, 10, 6)).toStdString() << ", "
            << "Max Acc: " << (item.value().velocity_curve_info.max_acc == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.max_acc, 10, 6)).toStdString() << ", "
            << "Start Vel: " << (item.value().velocity_curve_info.vel_at_start == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.vel_at_start, 10, 6)).toStdString() << ", "
            << "Start Acc: " << (item.value().velocity_curve_info.acc_at_start == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.acc_at_start, 10, 6)).toStdString() << ", "
            << "End Vel: " << (item.value().velocity_curve_info.vel_at_end == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.vel_at_end, 10, 6)).toStdString() << ", "
            << "End Acc: " << (item.value().velocity_curve_info.acc_at_end == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.acc_at_end, 10, 6)).toStdString() << ", "
            << "Duration: " << (item.value().velocity_curve_info.duration == DBL_MIN ? QString("NULL") : QString::number(item.value().velocity_curve_info.duration, 10, 6)).toStdString() << ")\n ";

        out << "\t\t轴信息: " << item.value().path_info.axis_group.toStdString() << "\n"
            << "\t\tData file: \n\t\t\t"
            << item.value().path_info.data_file.toStdString() << "\n"
            << "\t\tOffset file: \n\t\t\t"
            << item.value().path_info.offset_file.toStdString() << "\n";

        out << "\t\tR轴旋转: \n\t\t\t"
            << item.value().path_info.rotate_R.toStdString() << "\n";
        out << "\t\tR轴旋转角度: \n\t\t\t"
            << item.value().path_info.rotation_angle_R << "\n";
        out << "\t\tIO: \n\t\t\t" << item.value().path_info.IO_port.toStdString() << "\n"
            << "\t\tahead of start: \n\t\t\t" << item.value().path_info.ahead_of_start << "\n"
            << "\t\tahead of end: \n\t\t\t" << item.value().path_info.ahead_of_end << "\n"
            << "\t\tscale coefficient: \n\t\t\t" << item.value().path_info.x_scale_coefficient << "\n"
            << "\t\tscale coefficient: \n\t\t\t" << item.value().path_info.y_scale_coefficient << "\n";

        out << "\t\tPoint: ";
        for (auto point : item.value().path_info.teach_points)
            out << " " << point.toStdString();
        out << "\n";

        out << "\t\tSub Pathes: ";
        for (auto sub_path : item.value().path_info.sub_pathes)
            out << " " << sub_path.toStdString();
        out << "\n";
    }

    out << "工具参数文件: " << config.motion_info->tool_param_file.toStdString() << endl;

    return out;
}

}   // end of namespace MP
