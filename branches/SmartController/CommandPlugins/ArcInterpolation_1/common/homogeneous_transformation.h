#ifndef HOMOGENEOUS_TRANSFORMATIONMATRIX_H
#define HOMOGENEOUS_TRANSFORMATIONMATRIX_H

// include matrix lib
// Armadillo documentation is available at:
// http://arma.sourceforge.net/docs.html
#include <armadillo>
#include <cmath>


using namespace arma;
namespace route_planning_typedef {

// 坐标系的齐次变换矩阵
// 旋转以角度为单位
class HomogeneousTransformationMatrix
{
public:
    static mat get_trans_rot_matrix(double x_trans, double y_trans, double z_trans, double x_rot, double y_rot, double z_rot);
    static mat get_trans_matrix(double x_trans, double y_trans, double z_trans);
    static mat get_rot_matrix(double x_rot, double y_rot, double z_rot);

    static mat get_x_trans_matrix(double x_trans);
    static mat get_y_trans_matrix(double y_trans);
    static mat get_z_trans_matrix(double z_trans);

    static mat get_x_rot_matrix(double x_rot);
    static mat get_y_rot_matrix(double y_rot);
    static mat get_z_rot_matrix(double z_rot);

    static double degree2radian(double angle);
    static double radian2degree(double radian);
};
}   // end namespace route_planning_typedef

#endif // HOMOGENEOUS_TRANSFORMATIONMATRIX_H
