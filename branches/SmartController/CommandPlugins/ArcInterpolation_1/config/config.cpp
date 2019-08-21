#include "config.h"
#include <iostream>
using std::cout;

namespace route_planning_typedef {

Config::Config()
{
    motion_info = new MotionInfo;

    auto info = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_MotionInfo;

    motion_info->Id = info.id;

    for (auto it = info.axises_id.begin(); it != info.axises_id.end(); it++) {
        AxisInfo axis_info;
        axis_info.id = it.value();

        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
        QString str;
        if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(it.value()))
            return;
        str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[it.value()].swLimitStart;
        axis_info.soft_limit_start =  str.toDouble() * 1000;

        str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[it.value()].swLimitEnd;
        axis_info.soft_limit_end = str.toDouble() * 1000;

        motion_info->axises.insert(it.key(), axis_info);
    }

    motion_info->trans_rot_1.x_trans = info.trans_rot_1.x_trans;
    motion_info->trans_rot_1.y_trans = info.trans_rot_1.y_trans;
    motion_info->trans_rot_1.z_trans = info.trans_rot_1.z_trans;
    motion_info->trans_rot_1.x_rot = info.trans_rot_1.x_rot;
    motion_info->trans_rot_1.y_rot = info.trans_rot_1.y_rot;
    motion_info->trans_rot_1.z_rot = info.trans_rot_1.z_rot;

    motion_info->trans_rot_2.x_trans = info.trans_rot_2.x_trans;
    motion_info->trans_rot_2.y_trans = info.trans_rot_2.y_trans;
    motion_info->trans_rot_2.z_trans = info.trans_rot_2.z_trans;
    motion_info->trans_rot_2.x_rot = info.trans_rot_2.x_rot;
    motion_info->trans_rot_2.y_rot = info.trans_rot_2.y_rot;
    motion_info->trans_rot_2.z_rot = info.trans_rot_2.z_rot;

    for (auto item = info.all_profile_info.begin(); item != info.all_profile_info.end(); item++) {
        QString id = item.key();
        ProfileInfo one_profile_info;
        one_profile_info.id = item.value().id;
        one_profile_info.mode = item.value().mode;
        one_profile_info.num = item.value().num;

        if (1 == item.value().mode) {
            for (auto it = item.value().segments.begin(); it != item.value().segments.end(); it++) {
                SegmentInfo segment_info;
                segment_info.Id =  it->id;
                segment_info.mode = it->mode;
                if ("直线" == it->type)
                    segment_info.type = STRAIGHTLINE;
                else if ("圆弧" == it->type)
                    segment_info.type = ARC;
                else segment_info.type = UNDEFINED;
                segment_info.file = it->segment_mode_1.file;
                segment_info.posture_teach_point = it->segment_mode_1.start_teach_point;
                segment_info.teach_points.clear();

                one_profile_info.segments.insert(segment_info.Id, segment_info);
            }
        }
        else if (2 == item.value().mode) {
            for (auto it = item.value().segments.begin(); it != item.value().segments.end(); it++) {
                SegmentInfo segment_info;
                segment_info.Id = it->id;
                segment_info.mode = it->mode;
                if ("直线" == it->type)
                    segment_info.type = STRAIGHTLINE;
                else if ("圆弧" == it->type)
                    segment_info.type = ARC;
                else segment_info.type = UNDEFINED;
                segment_info.teach_points = it->segment_mode_2.teach_points;
                segment_info.file = "";
                segment_info.posture_teach_point = "";

                one_profile_info.segments.insert(segment_info.Id, segment_info);
            }
        }
        else if (3 == item.value().mode) {
            for (auto it = item.value().points.begin(); it != item.value().points.end(); it++) {
                one_profile_info.points.insert(it.key(), it.value());
            }
        }

        motion_info->all_profile_info.insert(id, one_profile_info);
    }
    motion_info->camera_calibration_file = info.camera_calibration_file;
    motion_info->tool_projection_length = info.tool_projection_length;
    motion_info->tool_init_degree = info.tool_init_degree;

    std::cout << *this;
}

Config::~Config()
{
    delete motion_info;
}

ProfileInfo * Config::get_one_profile_info(QString profile_id) const
{
    return &(motion_info->all_profile_info[profile_id]);
}

std::ostream & operator <<(std::ostream & os, const Config & c)
{
    os << "motion id: " << c.motion_info->Id.toStdString() << "\n";
    os << "\taxises id:\n";
    for (auto item = c.motion_info->axises.begin(); item != c.motion_info->axises.end(); item++) {
        os << "\t\t"
           << item.key().toStdString() << ": " << item.value().id.toStdString() << "\n";
    }

    os << "\t坐标系变换_1:\n";
    os << "\t\tX轴位移: " << c.motion_info->trans_rot_1.x_trans << "\n"
       << "\t\tY轴位移: " << c.motion_info->trans_rot_1.y_trans << "\n"
       << "\t\tZ轴位移: " << c.motion_info->trans_rot_1.z_trans << "\n"
       << "\t\tX轴旋转: " << c.motion_info->trans_rot_1.x_rot << "\n"
       << "\t\tY轴旋转: " << c.motion_info->trans_rot_1.y_rot << "\n"
       << "\t\tZ轴旋转: " << c.motion_info->trans_rot_1.z_rot << "\n";

    os << "\t坐标系变换_2:\n";
    os << "\t\tX轴位移: " << c.motion_info->trans_rot_2.x_trans << "\n"
       << "\t\tY轴位移: " << c.motion_info->trans_rot_2.y_trans << "\n"
       << "\t\tZ轴位移: " << c.motion_info->trans_rot_2.z_trans << "\n"
       << "\t\tX轴旋转: " << c.motion_info->trans_rot_2.x_rot << "\n"
       << "\t\tY轴旋转: " << c.motion_info->trans_rot_2.y_rot << "\n"
       << "\t\tZ轴旋转: " << c.motion_info->trans_rot_2.z_rot << "\n";


    os << "\t路径信息:\n";

    for (auto item = c.motion_info->all_profile_info.begin(); item != c.motion_info->all_profile_info.end(); item++) {
        os << "\t\t路径: " << item.key().toStdString() << "(Id: " << item.value().id.toStdString()
           << ", Mode: " << item.value().mode << ", Num: " << item.value().num << ")\n";

        if (1 == item.value().mode) {
            for(auto it = item.value().segments.begin(); it != item.value().segments.end(); it++) {
                os << "\t\t\tsegments:\n";
                os << "\t\t\t\tsegment " << it.value().Id << ":\n";
                os << "\t\t\t\t\t模式: " << it.value().mode << "\n";
                if (STRAIGHTLINE == it.value().type )
                    os << "\t\t\t\t\t类型: 直线\n";
                else if (ARC == it.value().type)
                    os << "\t\t\t\t\t类型: 圆弧\n";
                else os << "\t\t\t\t\t类型: 未定义的类型\n";

                os << "\t\t\t\t\t文件: " << it.value().file.toStdString() << "\n\t\t\t\t起点示教点: " << it.value().posture_teach_point.toStdString() << "\n";
            }
        }
        else if (2 == item.value().mode) {
            for (auto it = item.value().segments.begin(); it != item.value().segments.end(); it++) {
                os << "\t\t\tsegments:\n";
                os << "\t\t\t\tsegment " << it.value().Id << ":\n";
                os << "\t\t\t\t\t模式: " << it.value().mode << "\n";
                if (STRAIGHTLINE == it.value().type )
                    os << "\t\t\t\t\t类型: 直线\n";
                else if (ARC == it.value().type)
                    os << "\t\t\t\t\t类型: 圆弧\n";
                else os << "\t\t\t\t\t类型: 未定义的类型\n";

                os << "\t\t\t\t\t示教点:\n";
                for (auto i = it.value().teach_points.begin(); i != it.value().teach_points.end(); i++)
                    os << "\t\t\t\t\t\t" << i.key() << ": " << i.value().toStdString() << "\n";
            }
        }
        else if (3 == item.value().mode) {
            os << "\t\t\t示教点:\n";
            for (auto it = item.value().points.begin(); it != item.value().points.end(); it++)
                os << "\t\t\t\t"  << it.key() << ": " << it.value().toStdString() << "\n";
        }

    }
    os << "\t相机校正信息:\n"
       << "\t\t" << c.motion_info->camera_calibration_file.toStdString() << "\n";
    os << "\t胶筒投影长度:\n"
       << "\t\t" << c.motion_info->tool_projection_length << "\n";
    os << "\t工具起始角度:\n"
       << "\t\t " << c.motion_info->tool_init_degree << std::endl;

    return os;
}

}   // end namespace route_planning_typedef
