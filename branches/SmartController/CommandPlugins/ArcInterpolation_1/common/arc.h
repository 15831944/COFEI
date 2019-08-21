#ifndef ARC_H
#define ARC_H

#include "segment.h"
#include <QDebug>
#include <armadillo>
#include <QQueue>

namespace route_planning_typedef {

// 抽象基类，定义一个姿态策略接口
class ArcPostureStrategy
{
public:
    virtual int get(QVector<Position> * position, Posture * posture, QVector<mat> * pose) = 0;
};

// 具体策略类
// 圆心到圆弧上的点为一向量，和该向量保持姿态,即和圆弧圆心保持姿态
class KeepPostureWithArcCenter : public ArcPostureStrategy
{
public:
    int get(QVector<Position> * position, Posture * posture, QVector<mat> * pose);
};

// 具体策略类
// 工件坐标系原点到圆弧上的点为一向量，和该向量保持姿态,即和工件坐标系原点保持姿态
class KeepPostureWithWorkObjectCoordinateSystem : public ArcPostureStrategy
{
public:
    int get(QVector<Position> * position, Posture * posture, QVector<mat> * pose);
};

// 表示一段圆弧
class Arc : public Segment
{
public:
    enum arcType{MAJOR, SEMICIRCLE, MINOR}; // 优弧, 半圆, 劣弧
    enum arcDirection{POSITIVE = 1, NEGTIVE = -1}; // 圆弧方向，正向 or 负向, 定义从+x轴到+y轴的方向为正方向（逆时针方向），从+y轴到+x轴的方向为负方向（顺时针方向）

    Arc(SegmentInfo & segment_info);
    virtual ~Arc();

    virtual bool Init(QString & errMsg);

    virtual QVector<mat> * get_pose(QString & errMsg);
    virtual QVector<mat> * re_get_pose(QString & errMsg);


    virtual bool init(QString & errMsg);
    virtual Pose get_increase(int i);
    virtual Pose get_start_teach_point() const;
    virtual Pose get_end_teach_point() const;
    virtual int get_interpolation_num() const;

    void set_middle_point(Position middle);
    Position get_middle_point() const;

    void set_center(Position center);
    Position get_center() const;

    void set_radius(double radius);
    double get_radius() const;

private:

    bool set_teach_points(QString & errMsg);
    bool set_center_and_radius_of_arc_from_position(QString & errMsg);   // 从坐标位置计算圆弧圆心
    bool set_center_and_radius_of_arc_from_teach_point(QString & errMsg);    // 从示教点计算圆弧圆心
    bool calibration(QString & errMsg);
    bool set_arc_type(QString & errMsg);
    bool set_central_angle(QString & errMsg);
    bool set_interpolation_num(QString & errMsg);
    bool set_angle_per_interpolation_cycle(QString & errMsg);    // 根据每个插补周期的合成位移（即弦长），计算每个插补周期转过的角度
    void set_homogeneous_transformation_matrix_to_base(QString & errMsg);
    void get_increase();

    void get_increase_vector_method();  // 向量方法计算插补增量
    void get_increase_trigonometric_method();   // 三角函数方法计算插补增量

    void primary_approximation(const double & x, const double & y,
                               double & x_increase, double & y_increase);   // 平面圆弧, 一次逼近圆弧插补算法,根据当前位置计算增量
    void second_approximation(const double & x, const double & y,
                              double & x_increase, double & y_increase);    // 平面圆弧, 二次逼近圆弧插补算法,根据当前位置计算增量

private:
    Pose middle_teach_point; // 第三个圆弧上的示教点
    const double displacement_per_interpolation_cycle = 0.05;    // 单位: mm
    double angle_per_interpolation_cycle;
    double central_degree;  // 圆弧圆心角
    mat arc_coordinate_system_2_base;  // 以圆弧所在平面，建立一个新的坐标系．该坐标系旋转到基坐标系的变换矩阵; 注意，只是旋转关系

    Position middle;   // 圆弧上的第三点
    Position center;
    double radius;

    arcType type;
    arcDirection direction;

    ArcPostureStrategy * arc_posture_strategy;
};
}   // end namespace route_planning_typedef

#endif // ARC_H
