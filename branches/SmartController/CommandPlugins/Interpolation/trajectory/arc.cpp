#include "arc.h"
#include "unit.h"
#include <stdexcept>
#include "homogeneous_transformation.h"

namespace MP {


using namespace arma;

Arc::Arc(const TrajectoryInfo *trajectory_info) : BaseTrajectory(trajectory_info)
{
    increase_per_interpolation_cycle = new QVector<Pose>;
}

Arc::~Arc()
{
    delete increase_per_interpolation_cycle;
}

int Arc::getInterpolationCount() const
{
    return interpolation_count;
}

double Arc::getDisplacementPerCycle(int i) const
{}

Position Arc::getInterpolationPoint(int i) const
{}

void Arc::init()
{
    isTeachPointsAvailable();
    calculateCenterAndRadiusOfArcFromTeachPoints();

    calculateAnglePerInterpolationCycle();

    calculateHomogeneousTransformationMatrix2Base();
    calculateArcType();
    calculateArcAngle();
    calclulateInterpolationNum();

    calculateIncrease();
}

void Arc::isTeachPointsAvailable() const
{
    if (3 != trajectory_info->path_info.teach_points.size())
        throw std::logic_error(("Arc must have 3 teach points, " +
                                trajectory_info->id + " has " + trajectory_info->path_info.teach_points.size()).toStdString());

    for (int i = 0; i < trajectory_info->path_info.teach_points.size(); ++i)
        for (int j = i+1; j < trajectory_info->path_info.teach_points.size(); ++j)
            if (trajectory_info->path_info.teach_points[i] == trajectory_info->path_info.teach_points[j])
                throw std::logic_error((trajectory_info->id + ": has repeat teach points.").toStdString());
}

void Arc::calculateCenterAndRadiusOfArcFromTeachPoints()
{
    start_teach_point = teach_points[0];
    middle_teach_point = teach_points[1];
    end_teach_point = teach_points[2];

    start.x = start_teach_point.x + tool_projection_length * cos(degree2radian(start_teach_point.roll + tool_degree_offsett));
    start.y = start_teach_point.y + tool_projection_length * sin(degree2radian(start_teach_point.roll + tool_degree_offsett));
    start.z = start_teach_point.z;

    end.x = end_teach_point.x + tool_projection_length * cos(degree2radian(end_teach_point.roll + tool_degree_offsett));
    end.y = end_teach_point.y + tool_projection_length * sin(degree2radian(end_teach_point.roll + tool_degree_offsett));
    end.z = end_teach_point.z;

    middle.x = middle_teach_point.x + tool_projection_length * cos(degree2radian(middle_teach_point.roll + tool_degree_offsett));
    middle.y = middle_teach_point.y + tool_projection_length * sin(degree2radian(middle_teach_point.roll + tool_degree_offsett));
    middle.z = middle_teach_point.z;

    qDebug() << "start: " << start.x << " " << start.y << " " << start.z << "\n"
             << "end: " << end.x << " " << end.y << " " << end.z << "\n"
             << "middle: " << middle.x << " " << middle.y << " " << middle.z << "\n"
             << "start teach point: " << start_teach_point.x << " " << start_teach_point.y << " " << start_teach_point.z << "\n"
             << "middle teach point: " << middle_teach_point.x << " " << middle_teach_point.y << " " << middle_teach_point.z << "\n"
             << "end teach point: " << end_teach_point.x << " " << end_teach_point.y << " " << end_teach_point.z << "\n"
             << "center: " << center.x << " " << center.y << " " << center.z << endl;

    calculateCenterAndRadiusOfArcInCartesian();
}

// 已知圆弧上的三个点，计算圆心和半径
void Arc::calculateCenterAndRadiusOfArcInCartesian()
{
    mat mat1(3, 3);
    mat1(0, 0) = start.y * end.z - start.y * middle.z - start.z * end.y +
            start.z * middle.y + end.y * middle.z - middle.y * end.z;
    mat1(0, 1) = -start.x * end.z + start.x * middle.z + start.z * end.x -
            start.z * middle.x - end.x * middle.z + middle.x * end.z;
    mat1(0, 2) = start.x * end.y - start.x * middle.y - start.y * end.x +
            start.y * middle.x + end.x * middle.y - middle.x * end.y;
    mat1(1, 0) = (end.x - start.x) * 2;
    mat1(1, 1) = (end.y - start.y) * 2;
    mat1(1, 2) = (end.z - start.z) * 2;
    mat1(2, 0) = (middle.x - start.x) * 2;
    mat1(2, 1) = (middle.y - start.y) * 2;
    mat1(2, 2) = (middle.z - start.z) * 2;

    mat mat2(3, 1);
    mat2(0, 0) = -start.x * end.y * middle.z + start.x * middle.y * end.z + end.x * start.y * middle.z -
            middle.x * start.y * end.z - end.x * middle.y * start.z + middle.x * end.y * start.z;
    mat2(1, 0) = start.x * start.x + start.y * start.y + start.z * start.z -
            (end.x * end.x + end.y * end.y + end.z * end.z);
    mat2(2, 0) = start.x * start.x + start.y * start.y + start.z * start.z -
            (middle.x * middle.x + middle.y * middle.y + middle.z * middle.z);

    mat mat3 = -inv(mat1) * mat2;

    // 计算三个点到中心的距离，测试结果是否正确
    double r1, r2, r3;
    r1 = sqrt((mat3(0, 0) - start.x) * (mat3(0, 0) - start.x) +
              (mat3(1, 0) - start.y) * (mat3(1, 0) - start.y) +
              (mat3(2, 0) - start.z) * (mat3(2, 0) - start.z));

    r2 = sqrt((mat3(0, 0) - end.x) * (mat3(0, 0) - end.x) +
              (mat3(1, 0) - end.y) * (mat3(1, 0) - end.y) +
              (mat3(2, 0) - end.z) * (mat3(2, 0) - end.z));

    r3 = sqrt((mat3(0, 0) - middle.x) * (mat3(0, 0) - middle.x) +
              (mat3(1, 0) - middle.y) * (mat3(1, 0) - middle.y) +
              (mat3(2, 0) - middle.z) * (mat3(2, 0) - middle.z));

    radius = r1;   // 圆弧半径
    center = Position{mat3(0, 0), mat3(1, 0), mat3(2, 0)}; // 圆弧圆心
    qDebug() << "center: " << mat3(0, 0) << " " << mat3(1, 0) << " " << mat3(2, 0) << endl;
    qDebug() << "radius of cirule: " << r1 << "; " << r2 << "; " << r3 << "\n"
             << "*********************************" << endl;

    if (is_equal(r1, r2) && is_equal(r1, r3) && is_equal(r2, r3))
        return;

    throw std::logic_error((trajectory_info->id + ": calculate arc error.").toStdString());
}

void Arc::calculateArcType()
{
    colvec center_to_start(3);
    center_to_start(0, 0) = start.x - center.x;
    center_to_start(1, 0) = start.y - center.y;
    center_to_start(2, 0) = start.z - center.z;

    colvec center_to_middle(3);
    center_to_middle(0, 0) = middle.x - center.x;
    center_to_middle(1, 0) = middle.y - center.y;
    center_to_middle(2, 0) = middle.z - center.z;

    colvec center_to_end(3);
    center_to_end(0, 0) = end.x - center.x;
    center_to_end(1, 0) = end.y - center.y;
    center_to_end(2, 0) = end.z - center.z;

    colvec start_to_end(3);
    start_to_end(0, 0) = end.x - start.x;
    start_to_end(1, 0) = end.y - start.y;
    start_to_end(2, 0) = end.z - start.z;

    colvec n1 = cross(center_to_start, start_to_end);
    if (0 != norm(n1))
        n1 /= norm(n1);

    // 圆弧起点指向中间点的向量
    colvec start_to_middle(3);
    start_to_middle(0, 0) = middle.x - start.x;
    start_to_middle(1, 0) = middle.y - start.y;
    start_to_middle(2, 0) = middle.z - start.z;

    // 圆弧中间点指向终点的向量
    colvec middle_to_end(3);
    middle_to_end(0, 0) = end.x - middle.x;
    middle_to_end(1, 0) = end.y - middle.y;
    middle_to_end(2, 0) = end.z - middle.z;

    colvec n2 = cross(start_to_middle, middle_to_end);
    if (0 != norm(n2))
        n2 /= norm(n2);

    std::cout << n1(2, 0) << std::endl;
    std::cout << n2(2, 0) << std::endl;

    // 判断是优弧还是劣弧
    //    if (0 == n1(2,0) && 0 == n2(2,0))
    //        type = SEMICIRCLE;
    //    else if (n1(2, 0) == n2(2, 0))
    //        type = MINOR;
    //    else if (n1(2, 0) == -n2(2, 0))
    //        type = MAJOR;
    //    else {
    //        qDebug() << "can not jude arc type.";
    //        errMsg = "can not jude arc type.";
    //        exit(0);
    //        return false;
    //    }
    if (0 == n1(2,0) && 0 == n2(2,0))
        type = SEMICIRCLE;
    else if (is_equal(n1(2, 0), n2(2, 0)))
        type = MINOR;
    else if (is_equal(n1(2, 0), -n2(2, 0)))
        type = MAJOR;
    else
        throw std::logic_error((trajectory_info->id + ": can not calculate arc type.").toStdString());

    qDebug() << "arc type: ";
    switch (type) {
    case MAJOR:
        qDebug() << "major arc";
        break;
    case MINOR:
        qDebug() << "minor arc";
        break;
    case SEMICIRCLE:
        qDebug() << "semicircle";
        break;
    default:
        qDebug() << "unknown type";
    }

    // 判断是顺弧还是逆弧
    direction = POSITIVE;
    center_to_middle = inv(arc_coordinate_system_2_base) * center_to_middle;    // 从基坐标系变换到圆弧坐标系
    center_to_end = inv(arc_coordinate_system_2_base) * center_to_end;
    if (MINOR == type || SEMICIRCLE == type) {
        if (center_to_middle(1, 0) < 0)
            direction = NEGTIVE;
    }
    else if (MAJOR == type) {
        if (center_to_end(1, 0) > 0)
            direction = NEGTIVE;
    }

    qDebug() << "arc direction: ";
    switch  (direction) {
    case POSITIVE:
        qDebug() << "Position";
        break;
    case NEGTIVE:
        qDebug() << "Negtive";
        break;
    default:
        qDebug() << "unknown direciton.";
    }
}

void Arc::calculateArcAngle()
{
    colvec center_to_start(3);
    center_to_start(0, 0) = start.x - center.x;
    center_to_start(1, 0) = start.y - center.y;
    center_to_start(2, 0) = start.z - center.z;

    colvec center_to_end(3);
    center_to_end(0, 0) = end.x - center.x;
    center_to_end(1, 0) = end.y - center.y;
    center_to_end(2, 0) = end.z - center.z;

    // 利用向量的点积求圆心角
    // 根据优弧还是劣弧确定圆心角
    if (0 == norm(center_to_start))
        throw std::logic_error((trajectory_info->id + ": norm of center to start is 0.").toStdString());
    if (0 == norm(center_to_end))
        throw std::logic_error((trajectory_info->id + ": norm of center to end is 0.").toStdString());

    central_degree = acos(dot(center_to_start, center_to_end) / (norm(center_to_start) * norm(center_to_end)));
    if (MAJOR == type)
        central_degree = 360 * M_PI / 180 - central_degree;
    qDebug() << "central degree: " << central_degree << endl;
}

void Arc::calculateAnglePerInterpolationCycle()
{
    if (radius <= 0)
        throw std::logic_error((trajectory_info->id +  ": invalid arc radius.").toStdString());

    angle_per_interpolation_cycle = displacement_per_interpolation_cycle / radius;     // 单位为：弧度
}

void Arc::calclulateInterpolationNum()
{
    if (angle_per_interpolation_cycle <= 0)
        throw std::logic_error((trajectory_info->id + ": invalid angle per interpolation cycle.").toStdString());

    interpolation_count = central_degree / angle_per_interpolation_cycle;
}

// 以圆弧所在平面，建立一个新的坐标系，该坐标系到和基坐标系的旋转变换关系
// 注意，这里只计算旋转变换关系，没有位置偏移关系
void Arc::calculateHomogeneousTransformationMatrix2Base()
{
    // 基坐标系x轴的单位向量
    colvec x1_unit_vector(3);
    x1_unit_vector(0, 0) = 1;
    x1_unit_vector(1, 0) = 0;
    x1_unit_vector(2, 0) = 0;

    // 基坐标系y轴的单位向量
    colvec y1_unit_vector(3);
    y1_unit_vector(0, 0) = 0;
    y1_unit_vector(1, 0) = 1;
    y1_unit_vector(2, 0) = 0;

    // 基坐标系z轴的单位向量
    colvec z1_unit_vector(3);
    z1_unit_vector(0, 0) = 0;
    z1_unit_vector(1, 0) = 0;
    z1_unit_vector(2, 0) = 1;

    // 圆弧新坐标系x轴的单位向量
    // to do...当模长为０时，即起点，终点和圆心在一条直线上时,需要其他的算法计算圆弧平面的法向量
    colvec x2_unit_vector(3);
    x2_unit_vector(0, 0) = start.x - center.x;
    x2_unit_vector(1, 0) = start.y - center.y;
    x2_unit_vector(2, 0) = start.z - center.z;
    if (0 != norm(x2_unit_vector))
        x2_unit_vector /= norm(x2_unit_vector);

    // 从圆弧圆心指向圆弧终点的单位向量
    colvec center_to_end(3);
    center_to_end(0, 0) = end.x - center.x;
    center_to_end(1, 0) = end.y - center.y;
    center_to_end(2, 0) = end.z - center.z;
    if (0 != norm(center_to_end))
        center_to_end /= norm(center_to_end);

    colvec z2_unit_vector(3);
    z2_unit_vector(0, 0) = 0;
    z2_unit_vector(1, 0) = 0;
    z2_unit_vector(2, 0) = 0;
    if (0 != norm(cross(x2_unit_vector, center_to_end)))
        z2_unit_vector = cross(x2_unit_vector, center_to_end) / norm(cross(x2_unit_vector, center_to_end));

    colvec y2_unit_vector(3);
    y2_unit_vector = cross(x2_unit_vector, z2_unit_vector);

    mat m1(3, 3);
    m1.row(0) = trans(x1_unit_vector);
    m1.row(1) = trans(y1_unit_vector);
    m1.row(2) = trans(z1_unit_vector);

    mat m2(3, 3);
    m2.row(0) = trans(x2_unit_vector);
    m2.row(1) = trans(y2_unit_vector);
    m2.row(2) = trans(z2_unit_vector);

    // 利用两个坐标系的单位向量，计算变换矩阵
    arc_coordinate_system_2_base = m1 * trans(m2);
}

// to do better...
// to create a thread, producotr-consumer
void Arc::calculateIncrease()
{
    trigonometricMethod2CalculateIncrease();
    //    get_increase_vector_method();
}

// 向量方法计算插补增量
void Arc::vectorMethod2CalculateIncrease()
{
    // 以圆心为原点，圆心指向圆弧起点为+x轴，建立一个新的坐标系
    // current为当前位置在该新坐标系中的坐标(用列向量表示)
    // 初始时为圆弧起点的位置
    colvec current_position_in_arc_coordinate_system(3);
    current_position_in_arc_coordinate_system(0, 0) = radius;
    current_position_in_arc_coordinate_system(1, 0) = 0;
    current_position_in_arc_coordinate_system(2, 0) = 0;

    // 在基坐标系中的向量表示
    colvec current_position_in_base(3), previous_position_in_base(3);
    current_position_in_base = arc_coordinate_system_2_base * current_position_in_arc_coordinate_system;
    previous_position_in_base = current_position_in_base;

    // 每个插补周期，X Y Z方向的增量
    double x_trans, y_trans, z_trans;

    Pose increase;

    // 起点示教点和终点示教点的姿态不一定一样，所以要渐变过去
    // to do... 可以增加中间示教点的数量，更加细致的控制中间点的位姿
    double angle_increase_in_degree = (end_teach_point.roll - start_teach_point.roll) / interpolation_count;

    // 工具姿态变换的旋转矩阵
    mat tool_rot_matrix = HomogeneousTransformationMatrix::get_z_rot_matrix(angle_increase_in_degree);

    // 基坐标系中表示工具的向量
    colvec current_tool_in_base(4), previous_tool_in_base(4);
    current_tool_in_base(0, 0) = start_teach_point.x - start.x;
    current_tool_in_base(1, 0) = start_teach_point.y - start.y;
    current_tool_in_base(2, 0) = start_teach_point.z - start.z;
    current_tool_in_base(3, 0) = 1;
    previous_tool_in_base = current_tool_in_base;

    for (int i = 0; i < interpolation_count; i++) {
        // 二次逼近圆弧插补算法
        secondApproximation(current_position_in_arc_coordinate_system(0, 0),
                            current_position_in_arc_coordinate_system(1, 0), x_trans, y_trans);
        z_trans = 0;

        current_position_in_arc_coordinate_system(0, 0) += x_trans;
        current_position_in_arc_coordinate_system(1, 0) += y_trans;;
        current_position_in_arc_coordinate_system(2, 0) += z_trans;
        current_position_in_base = arc_coordinate_system_2_base * current_position_in_arc_coordinate_system;
        current_tool_in_base = tool_rot_matrix * current_tool_in_base;

        increase.x = current_position_in_base(0, 0) + current_tool_in_base(0, 0)
                - previous_position_in_base(0, 0) - previous_tool_in_base(0, 0);
        increase.y = current_position_in_base(1, 0) + current_tool_in_base(1, 0)
                - previous_position_in_base(1, 0) - previous_tool_in_base(1, 0);
        increase.z = current_position_in_base(2, 0) + current_tool_in_base(2, 0)
                - previous_position_in_base(2, 0) - previous_tool_in_base(2, 0);
        increase.yaw = 0;
        increase.pitch = 0;
        increase.roll = angle_increase_in_degree;

        previous_position_in_base = current_position_in_base;
        previous_tool_in_base = current_tool_in_base;

        increase_per_interpolation_cycle->push_back(increase);
    }
}

// 三角函数方法计算插补增量
void Arc::trigonometricMethod2CalculateIncrease()
{
    // 以圆心为原点，圆心指向圆弧起点为+x轴，建立一个新的坐标系
    // current为当前位置在该新坐标系中的坐标(用列向量表示)
    // 初始时为圆弧起点的位置
    colvec current(3);
    current(0, 0) = radius;
    current(1, 0) = 0;
    current(2, 0) = 0;

    // 每个插补周期，X Y Z方向的增量
    colvec trans(3);

    Pose increase;

    // C轴的起始角度
    double current_angle = degree2radian(start_teach_point.roll + tool_degree_offsett);
    double previous_angle =  current_angle;

    // 起点示教点和终点示教点的姿态不一定一样，所以要渐变过去
    // to do... 可以增加中间示教点的数量，更加细致的控制中间点的位姿
    double angle_in_degree = (end_teach_point.roll - start_teach_point.roll) / interpolation_count;
    double angle_in_radian = angle_in_degree * M_PI / 180;

    for (int i = 0; i < interpolation_count; i++) {
        secondApproximation(current(0, 0), current(1, 0), trans(0, 0), trans(1, 0));
        trans(2, 0) = 0;

        current(0, 0) += trans(0, 0);
        current(1, 0) += trans(1, 0);
        current(2, 0) += trans(2, 0);
        trans = arc_coordinate_system_2_base * trans; // 将圆弧平面坐标系中的位移增量，转换为基坐标系中的位移增量

        increase.x = trans(0, 0);
        increase.y = trans(1, 0);
        increase.z = trans(2, 0);
        increase.yaw = 0;
        increase.pitch = 0;
        increase.roll = angle_in_degree;

        // 补偿旋转产生的位移
        current_angle += angle_in_radian;
        increase.x -= tool_projection_length * (cos(current_angle) - cos(previous_angle));
        increase.y -= tool_projection_length * (sin(current_angle) - sin(previous_angle));
        previous_angle = current_angle;

        increase_per_interpolation_cycle->push_back(increase);
    }
}

// 一次逼近圆弧插补算法
void Arc::primaryApproximation(const double & x, const double & y,
                               double & x_increase, double & y_increase)
{
    double k = angle_per_interpolation_cycle;

    x_increase = -direction * k * y;
    y_increase = direction * k * x;
}

// 二次逼近圆弧插补算法
void Arc::secondApproximation(const double & x, const double & y,
                              double & x_increase, double & y_increase)
{
    double k1 = 0.5 * (angle_per_interpolation_cycle) * (angle_per_interpolation_cycle);
    double k2 = angle_per_interpolation_cycle;

    x_increase = -direction * k1 * x - direction * k2 * y;
    y_increase = direction * k2 * x - direction * k1 * y;
}


}   // end of namespace MP
