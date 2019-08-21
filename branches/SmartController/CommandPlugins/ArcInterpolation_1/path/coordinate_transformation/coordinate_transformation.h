#ifndef SETPARAMS_H
#define SETPARAMS_H

#include "config/config.h"
#include "typedef.h"
#include "homogeneous_transformation.h"

using namespace route_planning_typedef;

/*
 * 绝对式编码器
 * 以各编码器的零位置，为机械基坐标系的原点
 * 在工作平台上定义一个基准坐标系
 * 为了简化,机械基坐标系和基准坐标系的x, y, z轴平行，两者之间只有偏移, 偏移量通过轴测量得出
 * 理论上，工件应该严格精确的放置在工作平台上的基准位置
 * 工件在工作平台上的实际位置和基准位置之间的偏差，由相机给出
 */

// 坐标系变换
class CoordinateTransformation
{
public:
    CoordinateTransformation();

    bool init(QString & errMsg);
    /*
     * 设置工件工作平台基准坐标系到机械基坐标系之间的偏移和旋转角度
     * x, y, z轴正方向的偏移为正，负方向的偏移为负
     * x轴: 由+z轴往+y轴的旋转方向为正，由+y轴往+z轴的旋转方向为负
     * y轴: 由+z轴往+x轴的旋转方向为正，由+x轴往+z轴的旋转方向为负
     * z轴: 由+x轴往+y轴的旋转方向为正，由+y轴往+x轴的旋转方向为负
     */
    void set_workobject_platform_base_2_device_base(TransRot trans_rot);

    /*
     * 设置工件位置基准坐标系到工作平台基准坐标系的偏移和旋转角度
     * x, y, z轴正方向的偏移为正，负方向的偏移为负
     * x轴: 由+z轴往+y轴的旋转方向为正，由+y轴往+z轴的旋转方向为负
     * y轴: 由+z轴往+x轴的旋转方向为正，由+x轴往+z轴的旋转方向为负
     * z轴: 由+x轴往+y轴的旋转方向为正，由+y轴往+x轴的旋转方向为负
     */
    void set_workobject_base_coordinate_2_workobject_platform_base(TransRot trans_rot);

    /*
     * 设置轮廓坐标系坐标系到工件位置基准坐标系偏移和旋转角度
     * x, y, z轴正方向的偏移为正，负方向的偏移为负
     * x轴: 由+z轴往+y轴的旋转方向为正，由+y轴往+z轴的旋转方向为负
     * y轴: 由+z轴往+x轴的旋转方向为正，由+x轴往+z轴的旋转方向为负
     * z轴: 由+x轴往+y轴的旋转方向为正，由+y轴往+x轴的旋转方向为负
     */
    void set_workobject_profile_coordinate_2_workobject_base_coordinate(TransRot trans_rot);

    // 指定相机校准文件的路径
    void set_calibration_file(QString file);

    // 获得工件位置变换到基坐标系的变换矩阵
    mat get_transformation_matrix(QString & errMsg);

private:
    // 获得相机发送的校准信息
    TransRot get_calibration(QString & errMsg);

    // 获得工件基准坐标系到基坐标系的变换矩阵
    mat get_workobject_platform_base_2_device_base_matrix() const;

    // 获得工件实际位置到理论位置的变换矩阵
    mat get_workobject_base_coordinate_2_workobject_platform_base_matrix(QString & errMsg);

    mat get_workobject_profile_coordinate_2_workobject_base_matrix() const;

public:
    // 工作平台上基准坐标系到机械基坐标系的偏移量和旋转角度
    // 单位: mm
    TransRot workobject_platform_base_2_device_base;

    // 工件实际位置和工作平台上的基准坐标系之间的偏移量和旋转角度
    // 单位: mm
    // 由相机给出,从指定的文件中读取
    TransRot workobject_base_coordinate_2_workobject_platform_base;

    // 工件坐标系和工件位置基准点之间的偏移量和旋转角度
    // 单位: mm
    TransRot workobject_profile_coordinate_2_workobject_base_coordinate;

private:
    QString calibration_file;   // 相机给出的校准文件的路径
    Config *config; // 配置信息
};

#endif // SETPARAMS_H
