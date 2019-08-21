#ifndef PROFILE_H
#define PROFILE_H

#include <QDebug>
#include <QFile>
#include <fstream>
#include <iostream>
#include <sstream>
#include "arc.h"
#include "config.h"

using namespace route_planning_typedef;

// 表示一段连续的轮廓
// 位置
class Profile
{
public:
    Profile();
    Profile(ProfileInfo & profile_info);
    Profile(const Profile & profile);   // copy constructor

    virtual ~Profile();

    QVector<QVector<mat>> * get(QString & errMsg);

    QVector<Segment *> * mode_2_get(QString & errMsg);

    const Profile & operator =(const Profile & p);  // assign operator overload

private:
    ProfileInfo profile_info;
    QVector<QVector<mat>> * pose_in_workojbect_coordinate_system;

    QVector<Segment *> profile_segments;
};

#endif // PROFILE_H
