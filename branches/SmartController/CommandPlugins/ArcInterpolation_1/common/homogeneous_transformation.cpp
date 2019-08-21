#include "homogeneous_transformation.h"

namespace route_planning_typedef {

mat HomogeneousTransformationMatrix::get_trans_rot_matrix(double x_trans, double y_trans, double z_trans,
                                                          double x_rot, double y_rot, double z_rot)
{
    return get_trans_matrix(x_trans, y_trans, z_trans) * get_rot_matrix(x_rot, y_rot, z_rot);
}

mat HomogeneousTransformationMatrix::get_trans_matrix(double x_trans, double y_trans, double z_trans)
{
    return get_x_trans_matrix(x_trans) * get_y_trans_matrix(y_trans) * get_z_trans_matrix(z_trans);
}

mat HomogeneousTransformationMatrix::get_rot_matrix(double x_rot, double y_rot, double z_rot)
{
    return get_x_rot_matrix(x_rot) * get_y_rot_matrix(y_rot) * get_z_rot_matrix(z_rot);
}

mat HomogeneousTransformationMatrix::get_x_trans_matrix(double x_trans)
{
    mat x_trans_matrix(4, 4);
    x_trans_matrix.fill(0);
    x_trans_matrix(0, 3) = x_trans;

    for (int i = 0; i < 4; i++)
        x_trans_matrix(i, i) = 1;

    return x_trans_matrix;
}

mat HomogeneousTransformationMatrix::get_y_trans_matrix(double y_trans)
{
    mat y_trans_matrix(4, 4);
    y_trans_matrix.fill(0);
    y_trans_matrix(1, 3) = y_trans;

    for (int i = 0; i < 4; i++)
        y_trans_matrix(i, i) = 1;

    return y_trans_matrix;
}

mat HomogeneousTransformationMatrix::get_z_trans_matrix(double z_trans)
{
    mat z_trans_matrix(4, 4);
    z_trans_matrix.fill(0);
    z_trans_matrix(2, 3) = z_trans;

    for (int i = 0; i < 4; i++)
        z_trans_matrix(i, i) = 1;

    return z_trans_matrix;
}

mat HomogeneousTransformationMatrix::get_x_rot_matrix(double x_rot)
{
    x_rot = degree2radian(x_rot);

    mat x_rot_matrix(4, 4);
    x_rot_matrix.fill(0);
    x_rot_matrix(0, 0) = x_rot_matrix(3, 3) = 1;
    x_rot_matrix(1, 1) = x_rot_matrix(2, 2) = cos(x_rot);
    x_rot_matrix(1, 2) = -sin(x_rot);
    x_rot_matrix(2, 1) = -x_rot_matrix(1, 2);

    return x_rot_matrix;
}

mat HomogeneousTransformationMatrix::get_y_rot_matrix(double y_rot)
{
    y_rot = degree2radian(y_rot);

    mat y_rot_matrix(4, 4);
    y_rot_matrix.fill(0);
    y_rot_matrix(1, 1) = y_rot_matrix(3, 3) = 1;
    y_rot_matrix(0, 0) = y_rot_matrix(2, 2) = cos(y_rot);
    y_rot_matrix(0, 2) = sin(y_rot);
    y_rot_matrix(2, 0) = -y_rot_matrix(0, 2);

    return y_rot_matrix;
}

mat HomogeneousTransformationMatrix::get_z_rot_matrix(double z_rot)
{
    z_rot = degree2radian(z_rot);

    mat z_rot_matrix(4, 4);
    z_rot_matrix.fill(0);
    z_rot_matrix(2, 2) = z_rot_matrix(3, 3) = 1;
    z_rot_matrix(0, 0) = z_rot_matrix(1, 1) = cos(z_rot);
    z_rot_matrix(0, 1) = -sin(z_rot);
    z_rot_matrix(1, 0) = -z_rot_matrix(0, 1);

    return z_rot_matrix;
}

// angle to radian
double HomogeneousTransformationMatrix::degree2radian(double angle)
{
    return angle * 3.1415926 / 180.0;
}

// radian to angle
double HomogeneousTransformationMatrix::radian2degree(double radian)
{
    return radian * 180.0 / 3.1415926;
}
}
