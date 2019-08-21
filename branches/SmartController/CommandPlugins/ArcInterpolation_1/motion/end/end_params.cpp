#include "end_params.h"
#include <QDebug>

EndParams::EndParams(double x, double y, double z, double yaw, double pitch, double roll)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
}

QVector<Pose> * EndParams::get(const QVector<mat> * const pose, QString & errMsg)
{
    QVector<Pose> * end_pose = new QVector<Pose>;
    if (!end_pose) {
        errMsg = "EndParams::get(), memory allocate fail.";
        return NULL;
    }

    Pose p;
    for (auto it = pose->begin(); it != pose->end(); it++) {
        p.x = (*it)(0, 3) - length * (*it)(2, 2) * (*it)(1, 1);
        p.y = (*it)(1, 3) - length * (*it)(1, 0);
        p.z = (*it)(2, 3) + length * (*it)(0, 2) * (*it)(1 ,1);
        p.roll = asin((*it)(1, 0)) * 180 / 3.1415926;

        end_pose->push_back(p);
    }

    return end_pose;
}

void EndParams::set_length(double length)
{
    this->length = length;
}

double EndParams::get_length() const
{
    return this->length;
}

void EndParams::set_yaw(double yaw)
{
    this->yaw = yaw;
}

double EndParams::get_yaw() const
{
    return this->yaw;
}

void EndParams::set_pitch(double pitch)
{
    this->pitch = pitch;
}

double EndParams::get_pitch() const
{
    this->pitch;
}

void EndParams::set_roll(double roll)
{
    this->roll = roll;
}

double EndParams::get_roll() const
{
    return this->roll;
}
