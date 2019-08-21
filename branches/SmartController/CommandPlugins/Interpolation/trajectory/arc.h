#ifndef ARC_H
#define ARC_H

#include "BaseTrajectory.h"
#include <QDebug>
#include <armadillo>
#include <QQueue>


namespace MP {


using namespace arma;

// 表示一段圆弧
class Arc : public BaseTrajectory
{
public:
    enum arcType{MAJOR, SEMICIRCLE, MINOR}; // 优弧, 半圆, 劣弧
    enum arcDirection{POSITIVE = 1, NEGTIVE = -1}; // 圆弧方向，正向 or 负向, 定义从+x轴到+y轴的方向为正方向（逆时针方向），从+y轴到+x轴的方向为负方向（顺时针方向）

    Arc(const TrajectoryInfo *trajectory_info);
    virtual ~Arc();

    virtual void init();
    virtual int getInterpolationCount() const;
    virtual double getDisplacementPerCycle(int i) const;
    virtual Position getInterpolationPoint(int i) const;

private:
    void isTeachPointsAvailable() const;
    void calculateCenterAndRadiusOfArcFromTeachPoints();
    void calculateCenterAndRadiusOfArcInCartesian();
    void calculateArcType();
    void calculateArcAngle();
    void calclulateInterpolationNum();
    void calculateAnglePerInterpolationCycle();    // 根据每个插补周期的合成位移（即弦长），计算每个插补周期转过的角度
    void calculateHomogeneousTransformationMatrix2Base();
    void calculateIncrease();

    void vectorMethod2CalculateIncrease();  // 向量方法计算插补增量
    void trigonometricMethod2CalculateIncrease();   // 三角函数方法计算插补增量

    void primaryApproximation(const double & x, const double & y,
                              double & x_increase, double & y_increase);   // 平面圆弧, 一次逼近圆弧插补算法,根据当前位置计算增量
    void secondApproximation(const double & x, const double & y,
                             double & x_increase, double & y_increase);    // 平面圆弧, 二次逼近圆弧插补算法,根据当前位置计算增量

private:
    Pose start_teach_point, middle_teach_point, end_teach_point;

    const double displacement_per_interpolation_cycle = 0.05;    // 单位: mm
    double angle_per_interpolation_cycle;
    double central_degree;  // 圆弧圆心角
    mat arc_coordinate_system_2_base;  // 以圆弧所在平面，建立一个新的坐标系．该坐标系旋转到基坐标系的变换矩阵; 注意，只是旋转关系

    Position start, middle, end;
    Position center;
    double radius;

    arcType type;
    arcDirection direction;
};


}   // end of namespace MP

#endif // ARC_H
