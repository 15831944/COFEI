#include "converter.h"


Converter::Converter(QVector<Pose> * pose_in_base_coordinate)
{
    kXDisplacementPerUserUnit = 0.0001;
    kYDisplacementPerUserUnit = 0.0001;
    kZDisplacementPerUserUnit = 0.0001;
    kYawDegreePerUserUnit = 0.0001;
    kPitchDegreePerUserUnit = 0.0001;
    kRollDegreePerUserUnit = 0.0001;

    this->pose_in_base_coordinate_ = pose_in_base_coordinate;
    this->pose_in_user_unit_ = NULL;
}

Converter::~Converter()
{
    delete pose_in_user_unit_;
}

void Converter::set_pose_in_base_coordinate(QVector<Pose> * pose)
{
    this->pose_in_base_coordinate_ = pose;
}

QMap<QString, QVector<int>> * Converter::convert(QString & errMsg)
{
    if (!pose_in_user_unit_) {
        pose_in_user_unit_ = new QMap<QString, QVector<int>>;
        if (!pose_in_user_unit_) {
            errMsg = "Error --> Converter::converter(), memeory allocate fail.";
            return NULL;
        }
    }
    pose_in_user_unit_->clear();

    int size = pose_in_base_coordinate_->length();
    if (size <= 0) {
        errMsg = "Error --> Converter::converter(), pose_in_base_coordinate->length() is " + size;
        return NULL;
    }

    int x, y, z, yaw, pitch, roll;
    for (int i = 0; i < size; i++) {
        x = (*pose_in_base_coordinate_)[i].x / kXDisplacementPerUserUnit;
        y = (*pose_in_base_coordinate_)[i].y / kYDisplacementPerUserUnit;
        z = (*pose_in_base_coordinate_)[i].z / kZDisplacementPerUserUnit;
        yaw = (*pose_in_base_coordinate_)[i].yaw / kYawDegreePerUserUnit;
        pitch = (*pose_in_base_coordinate_)[i].pitch / kPitchDegreePerUserUnit;
        roll = (*pose_in_base_coordinate_)[i].roll / kRollDegreePerUserUnit;

        (*pose_in_user_unit_)["X"].push_back(x);
        (*pose_in_user_unit_)["Y"].push_back(y);
        (*pose_in_user_unit_)["Z"].push_back(z);
        (*pose_in_user_unit_)["Yaw"].push_back(yaw);
        (*pose_in_user_unit_)["Pitch"].push_back(pitch);
        (*pose_in_user_unit_)["Roll"].push_back(roll);
    }

    return pose_in_user_unit_;
}

bool Converter::convert(Pose & pose)
{
    pose.x /= kXDisplacementPerUserUnit;
    pose.y /= kYDisplacementPerUserUnit;
    pose.z /= kZDisplacementPerUserUnit;
    pose.yaw /= kYawDegreePerUserUnit;
    pose.pitch /= kPitchDegreePerUserUnit;
    pose.roll /= kRollDegreePerUserUnit;

    return true;
}
