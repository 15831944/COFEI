#include "coordinate_transformation.h"

// to do...
// get config from file
CoordinateTransformation::CoordinateTransformation()
{
    config = config->GetInstance();
}

bool CoordinateTransformation::init(QString & errMsg)
{
    set_workobject_platform_base_2_device_base(config->motion_info->trans_rot_1);
    set_workobject_profile_coordinate_2_workobject_base_coordinate(config->motion_info->trans_rot_2);
    set_calibration_file(config->motion_info->camera_calibration_file);
}

void CoordinateTransformation::set_workobject_platform_base_2_device_base(TransRot trans_rot)
{
    workobject_platform_base_2_device_base = trans_rot;
}

void CoordinateTransformation:: set_workobject_profile_coordinate_2_workobject_base_coordinate(TransRot trans_rot)
{
    workobject_profile_coordinate_2_workobject_base_coordinate = trans_rot;
}

void CoordinateTransformation::set_calibration_file(QString file)
{
    calibration_file = file;
}

mat CoordinateTransformation::get_transformation_matrix(QString & errMsg)
{
    mat m1 = get_workobject_platform_base_2_device_base_matrix();
    mat m2 = get_workobject_base_coordinate_2_workobject_platform_base_matrix(errMsg);
    mat m3 = get_workobject_profile_coordinate_2_workobject_base_matrix();

    return m1 * m2 * m3;
}

mat CoordinateTransformation::get_workobject_platform_base_2_device_base_matrix() const
{
    HomogeneousTransformationMatrix m;
    return m.get_trans_rot_matrix(workobject_platform_base_2_device_base.x_trans,
                                  workobject_platform_base_2_device_base.y_trans,
                                  workobject_platform_base_2_device_base.z_trans,
                                  workobject_platform_base_2_device_base.x_rot,
                                  workobject_platform_base_2_device_base.y_rot,
                                  workobject_platform_base_2_device_base.z_rot);
}

mat CoordinateTransformation::get_workobject_base_coordinate_2_workobject_platform_base_matrix(QString & errMsg)
{
    TransRot trans_rot = get_calibration(errMsg);

    HomogeneousTransformationMatrix m;
    return m.get_trans_rot_matrix(trans_rot.x_trans, trans_rot.y_trans, trans_rot.z_trans,
                                  trans_rot.x_rot, trans_rot.y_rot, trans_rot.z_rot);
}

mat CoordinateTransformation::get_workobject_profile_coordinate_2_workobject_base_matrix() const
{
    HomogeneousTransformationMatrix m;
    return m.get_trans_rot_matrix(workobject_profile_coordinate_2_workobject_base_coordinate.x_trans,
                                  workobject_profile_coordinate_2_workobject_base_coordinate.y_trans,
                                  workobject_profile_coordinate_2_workobject_base_coordinate.z_trans,
                                  workobject_profile_coordinate_2_workobject_base_coordinate.x_rot,
                                  workobject_profile_coordinate_2_workobject_base_coordinate.y_rot,
                                  workobject_profile_coordinate_2_workobject_base_coordinate.z_rot);
}

TransRot CoordinateTransformation::get_calibration(QString & errMsg)
{
    TransRot trans_rot;
    std::string str;
    std::string f = calibration_file.toStdString();

    std::fstream file(f);

    str = "";
    double x_trans, y_trans, z_trans, x_rot, y_rot, z_rot;

    file.seekp(0);
    std::getline(file, str);
    std::stringstream strin(str);

    // to do better
    strin >> trans_rot.x_trans >> trans_rot.y_trans >> trans_rot.z_trans >> trans_rot.x_rot >> trans_rot.y_rot >> trans_rot.z_rot;

    return trans_rot;
}
