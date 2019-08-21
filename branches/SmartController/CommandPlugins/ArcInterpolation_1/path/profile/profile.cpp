#include "profile.h"
#include "straightline.h"

Profile::Profile()
{
    pose_in_workojbect_coordinate_system = new QVector<QVector<mat>>;
}

Profile::Profile(ProfileInfo & profile_info)
{
    this->profile_info = profile_info;
    pose_in_workojbect_coordinate_system = new QVector<QVector<mat>>;
}

Profile::Profile(const Profile & profile)
{
    profile_info = profile.profile_info;
    pose_in_workojbect_coordinate_system = new QVector<QVector<mat>>;
    *pose_in_workojbect_coordinate_system = *(profile.pose_in_workojbect_coordinate_system);
}

const Profile & Profile::operator =(const Profile & p)
{
    profile_info = p.profile_info;
    pose_in_workojbect_coordinate_system = new QVector<QVector<mat>>;
    *pose_in_workojbect_coordinate_system = *(p.pose_in_workojbect_coordinate_system);

    return *this;
}

Profile::~Profile()
{
    delete pose_in_workojbect_coordinate_system;
}

// 路径模式２，示教模式
QVector<Segment *> * Profile::mode_2_get(QString &errMsg)
{
    if (!profile_segments.isEmpty())
        return &profile_segments;

    if (2 != profile_info.mode) {
        errMsg += " 路径模式设置错误";
        return nullptr;
    }

    if (profile_info.num < 0) {
        errMsg += " 路径段数目非法";
        return nullptr;
    }

    if ( profile_info.segments.size() != profile_info.num) {
        errMsg += " 路径段数目与设置不符合";
        return nullptr;
    }

    for (auto item = profile_info.segments.begin(); item != profile_info.segments.end(); item++) {
        if (2 != item.value().mode) {
            errMsg += ("段: " + QString::number(item.value().Id, 10) + " 段模式设置错误");
            return nullptr;
        }
        if (STRAIGHTLINE == item.value().type) {
            Segment * ptr = new StraightLine(item.value());
            if (!ptr) {
                errMsg += ("段: " + QString::number(item.value().Id, 10) + " 内存分配错误");
                return nullptr;
            }
            if (!ptr->init(errMsg)) {
                errMsg += ("段: " + QString::number(item.value().Id, 10) + " 初始化失败");
                return nullptr;
            }
            profile_segments.push_back(ptr);
        }
        else if (ARC == item.value().type) {
            Segment * ptr = new Arc(item.value());
            if (!ptr) {
                errMsg += ("段: " + QString::number(item.value().Id, 10) + " 内存分配错误");
                return nullptr;
            }
            if (!ptr->init(errMsg)) {
                errMsg += ("段: " + QString::number(item.value().Id, 10) + " 初始化失败");
                return nullptr;
            }
            profile_segments.push_back(ptr);
        }
    }

    return &profile_segments;
}

// 路径模式３，示教拟合模式

// 路径模式１，CAD数据导入模式
QVector<QVector<mat>> * Profile::get(QString & errMsg)
{
    qDebug() << "路径模式１，CAD数据导入模式:" << endl;

    if (!pose_in_workojbect_coordinate_system) {
        pose_in_workojbect_coordinate_system = new QVector<QVector<mat>>;
        if (!pose_in_workojbect_coordinate_system) {
            errMsg = "Error --> ";
            return NULL;
        }
    }

    if (!pose_in_workojbect_coordinate_system->isEmpty())
        return pose_in_workojbect_coordinate_system;

    if (profile_info.num != profile_info.segments.size()) {
        errMsg = "Error --> ";
        return NULL;
    }

    if (profile_info.num <= 0) {
        errMsg = "Error --> ";
        return NULL;
    }

    int size = profile_info.segments.size();
    if (size <= 0) {
        errMsg = "Error --> ";
        return NULL;
    }

    // to do...
    // 添加不同类型的段
    for (int i = 0; i < size; i++) {
        SegmentInfo & segment_info = (profile_info.segments)[i];
        if (ARC == segment_info.type) {
            Arc arc(segment_info);
            arc.Init(errMsg);
            QVector<mat> * p = arc.get_pose(errMsg);
            if (!p)
                return NULL;
            pose_in_workojbect_coordinate_system->push_back(*p);

        }
        else if (STRAIGHTLINE == segment_info.type) {
            errMsg = "type is straight line, to do...";
            return NULL;
        }
        else {
            errMsg = "Unkown segment type.";
            return NULL;
        }
    }

    return pose_in_workojbect_coordinate_system;
}
