#include "path.h"
#include <iostream>

using route_planning_typedef::Pose;

Path::Path()
{
    path = new QMap<QString, Profile>;
    coordinate_transformation = new CoordinateTransformation;

    QString errMsg;
    init(errMsg);
}

Path::~Path()
{
    delete coordinate_transformation;
    delete path;
}

bool Path::init(QString & errMsg)
{
    if (!(conf = conf->GetInstance())) {
        errMsg = "Error --> Path::init() : get config instance fail";
        return false;
    }

    path_info = &(conf->motion_info->all_profile_info);

    if (!path) {
        if (!(path = new QMap<QString, Profile>)) {
            errMsg = "Error --> Path::init() : memory allocate fail.";
            return false;
        }
    }

    for (auto item = path_info->begin(); item != path_info->end(); item++) {
        Profile profile(item.value());
        path->insert(item.key(), profile);
    }

    if (!coordinate_transformation) {
        if (!(coordinate_transformation = new CoordinateTransformation)) {
            errMsg = "Error --> Path::init() : memory allocae fail.";
            return false;
        }
    }
    coordinate_transformation->init(errMsg);

    return true;
}

QVector<Segment *> * Path::mode_2_get(QString profile_id, QString &errMsg)
{
    return (*path)[profile_id].mode_2_get(errMsg);
}

QVector<QVector<mat>> * Path::get(QString profile_id, QString & errMsg)
{
    QVector<QVector<mat>>  * pose_in_workobject = (*path)[profile_id].get(errMsg);
    if (!pose_in_workobject)
        return NULL;

    QVector<QVector<mat>> * pose_in_base = new QVector<QVector<mat>>;
    if (!pose_in_base) {
        errMsg = "memeoy allocate fail.";
        return NULL;
    }

    mat coordinate_transformation_matrix = coordinate_transformation->get_transformation_matrix(errMsg);

    for (auto item = pose_in_workobject->begin(); item != pose_in_workobject->end(); item++) {
        int size = item->size();
        QVector<mat> pose;
        for (int i = 0; i < size; i++)
            pose.push_back(coordinate_transformation_matrix * (*item)[i]);
        pose_in_base->push_back(pose);
    }

//    qDebug() << "show mat:" << endl;
//    for (auto it = pose_in_base->begin(); it != pose_in_base->end(); it++)
//        for (auto item = it->begin(); item != it->end(); item++)
//            std::cout << *item << std::endl;

    return pose_in_base;
}
