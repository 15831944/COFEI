#include "arc.h"

namespace route_planning_typedef {
using namespace arma;

Arc::Arc(SegmentInfo & segment_info) : Segment(segment_info)
{
    increase_per_interpolation_cycle = new QVector<Pose>;
}

Arc::~Arc()
{
    delete increase_per_interpolation_cycle;
}

bool Arc::Init(QString & errMsg)
{
    if (!Segment::Init(errMsg))
        return false;

    QVector<Position> * pos = get_position(errMsg);
    int size = pos->size();
    if (size >= 3)
        middle = (*pos)[size/2];
    else {
        errMsg = "file size is too short";
        return false;
    }

    set_center_and_radius_of_arc_from_position(errMsg);

    arc_posture_strategy = new KeepPostureWithWorkObjectCoordinateSystem;

    return true;
}

// to do...
// 抽象共同的部分，封装变化的部分
QVector<mat> * Arc::get_pose(QString & errMsg)
{
    if (!pose) {
        pose = new QVector<mat>;
        if (!pose) {
            errMsg = "get_pose() : memory allocale fail.";
            return NULL;
        }
    }

    if (pose->isEmpty()) {
        QVector<Position> * pos = get_position(errMsg);
        if (!pos)
            return NULL;

        int val = arc_posture_strategy->get(pos, nullptr, pose);
        if (val)
            return NULL;
    }

    return pose;
}

QVector<mat> * Arc::re_get_pose(QString & errMsg)
{
    if (!pose) {
        pose = new QVector<mat>;
        if (!pose) {
            errMsg = "get_pose() : memory allocale fail.";
            return NULL;
        }
    }
    pose->clear();

    QVector<Position> * pos = get_position(errMsg);
    if (!pos)
        return NULL;

    int val = arc_posture_strategy->get(pos, nullptr, pose);
    if (val)
        return NULL;

    return pose;
}

void Arc::set_middle_point(Position third_point)
{
    this->middle = third_point;
}

Position Arc::get_middle_point() const
{
    return middle;
}

void Arc::set_center(Position center)
{
    this->center = center;
}

Position Arc::get_center() const
{
    return center;
}

void Arc::set_radius(double radius)
{
    this->radius = radius;
}

double Arc::get_radius() const
{
    return radius;
}

int KeepPostureWithArcCenter::get(QVector<Position> * position, Posture * posture, QVector<mat> * pose)
{

}


int KeepPostureWithWorkObjectCoordinateSystem::get(QVector<Position> * position, Posture * posture, QVector<mat> * pose)
{
    int size = position->size();
    if (size <= 0)
        return -1;  // no position data
    if (!pose)
        return -2;  // pose not init

    for (int i = 0; i < size; i++) {
        mat m = (*position)[i].get_homogeneous_transformation_matrix() * posture->get_homogeneous_transformation_matrix();
        pose->push_back(m);
    }

    return 0;
}

// to do better...
// to create a thread, producotr-consumer
Pose Arc::get_increase(int i)
{
    if (increase_per_interpolation_cycle->isEmpty())
        get_increase();

    if (i >= increase_per_interpolation_cycle->size())
        return Pose(0, 0, 0, 0, 0, 0);

    return (*increase_per_interpolation_cycle)[i];
}

Pose Arc::get_start_teach_point() const
{
    return start_teach_point;
}

Pose Arc::get_end_teach_point() const
{
    return end_teach_point;
}

int Arc::get_interpolation_num() const
{
    return interpolation_num;
}

bool Arc::init(QString & errMsg)
{
    if (!set_teach_points(errMsg))
        return false;

    if (!set_center_and_radius_of_arc_from_teach_point(errMsg))
        return false;

    set_angle_per_interpolation_cycle(errMsg);

    set_homogeneous_transformation_matrix_to_base(errMsg);
    set_arc_type(errMsg);
    set_central_angle(errMsg);
    set_interpolation_num(errMsg);

    get_increase();

    return true;
}

bool Arc::set_teach_points(QString &errMsg)
{
    qDebug() << "Arc: set teach points" << endl;
    if (3 != teach_points.size()) {
        qDebug() << "Error: 圆弧有且仅能有三个示教点";
        errMsg = "Error: 圆弧有且仅能有三个示教点";
        return false;
    }

    // 各个方向对应的轴Id
    const QString x_axis = config->motion_info->axises["X"].id;
    const QString y_axis = config->motion_info->axises["Y"].id;
    const QString z_axis = config->motion_info->axises["Z"].id;
    const QString yaw_axis = config->motion_info->axises["Yaw"].id;
    const QString pitch_axis = config->motion_info->axises["Pitch"].id;
    const QString roll_axis = config->motion_info->axises["Roll"].id;

    // 快速定位中定义的示教点的Id
    const QString & start_teach_point_id = teach_points[1];
    const QString & middle_teach_point_id = teach_points[2];
    const QString & end_teach_point_id = teach_points[3];
    if (start_teach_point_id == middle_teach_point_id || start_teach_point_id == end_teach_point_id || middle_teach_point_id == end_teach_point_id) {
        qDebug() << "Error: 圆弧示教点有重复";
        errMsg = "Error: 圆弧示教点有重复";
        return false;
    }

    qDebug() << "start_teach_point id: " << start_teach_point_id << "\n"
             << "middle_point id:" << middle_teach_point_id << "\n"
             << "end_teach_pointid: " << end_teach_point_id << endl;

    auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map;
    if (!it.contains(start_teach_point_id)) {
        qDebug() << "Error: 没有在快速定位中找到指定的起点示教点";
        errMsg = "Error: 没有在快速定位中找到指定的起点示教点";
        return false;
    }
    if (!it.contains(middle_teach_point_id)) {
        qDebug() << "Error: 没有在快速定位中找到指定的中间示教点";
        errMsg = "Error: 没有在快速定位中找到指定的中间示教点";
        return false;

    }
    if (!it.contains(end_teach_point_id)) {
        qDebug() << "Error: 没有在快速定位中找到指定的终点示教点";
        errMsg = "Error: 没有在快速定位中找到指定的终点示教点";
        return false;
    }


    for (auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.begin();
         item != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.end(); item ++) {
        //从快速定位部分获取校准点信息
        if (start_teach_point_id == item.key()) {
            start_teach_point.x = item.value().axisMap[x_axis].pos;
            start_teach_point.y = item.value().axisMap[y_axis].pos;
            start_teach_point.z = item.value().axisMap[z_axis].pos;
            start_teach_point.yaw = item.value().axisMap[yaw_axis].pos;
            start_teach_point.pitch = item.value().axisMap[pitch_axis].pos;
            start_teach_point.roll = item.value().axisMap[roll_axis].pos;
        }

        if (middle_teach_point_id == item.key()) {
            middle_teach_point.x = item.value().axisMap[x_axis].pos;
            middle_teach_point.y = item.value().axisMap[y_axis].pos;
            middle_teach_point.z = item.value().axisMap[z_axis].pos;
            middle_teach_point.yaw = item.value().axisMap[yaw_axis].pos;
            middle_teach_point.pitch = item.value().axisMap[pitch_axis].pos;
            middle_teach_point.roll = item.value().axisMap[roll_axis].pos;
        }

        if (end_teach_point_id == item.key()) {
            end_teach_point.x = item.value().axisMap[x_axis].pos;
            end_teach_point.y = item.value().axisMap[y_axis].pos;
            end_teach_point.z = item.value().axisMap[z_axis].pos;
            end_teach_point.yaw = item.value().axisMap[yaw_axis].pos;
            end_teach_point.pitch = item.value().axisMap[pitch_axis].pos;
            end_teach_point.roll = item.value().axisMap[roll_axis].pos;
        }
    }
    // 读取的数值的单位为0.0001,缩放为以0.001为单位
    // 即位移为1mm,角度为1度
    start_teach_point.scale(0.1);
    middle_teach_point.scale(0.1);
    end_teach_point.scale(0.1);

    qDebug() << "start_teach_point:"
             << "\n\tx: " << start_teach_point.x
             << "\n\ty: " << start_teach_point.y
             << "\n\tz: " << start_teach_point.z
             << "\n\tyaw: " << start_teach_point.yaw
             << "\n\tpitch: " << start_teach_point.pitch
             << "\b\troll: " << start_teach_point.roll << endl;

    qDebug() << "end_teach_point:"
             << "\n\tx: " << end_teach_point.x
             << "\n\ty: " << end_teach_point.y
             << "\n\tz: " << end_teach_point.z
             << "\n\tyaw: " << end_teach_point.yaw
             << "\n\tpitch: " << end_teach_point.pitch
             << "\b\troll: " << end_teach_point.roll << endl;

    qDebug() << "third_teach_poit:"
             << "\n\tx: " << middle_teach_point.x
             << "\n\ty: " << middle_teach_point.y
             << "\n\tz: " << middle_teach_point.z
             << "\n\tyaw: " << middle_teach_point.yaw
             << "\n\tpitch: " << middle_teach_point.pitch
             << "\n\troll: " << middle_teach_point.roll << endl;

    return true;
}

bool Arc::set_center_and_radius_of_arc_from_teach_point(QString & errMsg)
{
    start.x = start_teach_point.x + tool_projection_length * cos(degree2radian(start_teach_point.roll + tool_init_degree));
    start.y = start_teach_point.y + tool_projection_length * sin(degree2radian(start_teach_point.roll + tool_init_degree));
    start.z = start_teach_point.z;

    end.x = end_teach_point.x + tool_projection_length * cos(degree2radian(end_teach_point.roll + tool_init_degree));
    end.y = end_teach_point.y + tool_projection_length * sin(degree2radian(end_teach_point.roll + tool_init_degree));
    end.z = end_teach_point.z;

    middle.x = middle_teach_point.x + tool_projection_length * cos(degree2radian(middle_teach_point.roll + tool_init_degree));
    middle.y = middle_teach_point.y + tool_projection_length * sin(degree2radian(middle_teach_point.roll + tool_init_degree));
    middle.z = middle_teach_point.z;

    qDebug() << "start: " << start.x << " " << start.y << " " << start.z << "\n"
             << "end: " << end.x << " " << end.y << " " << end.z << "\n"
             << "middle: " << middle.x << " " << middle.y << " " << middle.z << "\n"
             << "start teach point: " << start_teach_point.x << " " << start_teach_point.y << " " << start_teach_point.z << "\n"
             << "middle teach point: " << middle_teach_point.x << " " << middle_teach_point.y << " " << middle_teach_point.z << "\n"
             << "end teach point: " << end_teach_point.x << " " << end_teach_point.y << " " << end_teach_point.z << "\n"
             << "center: " << center.x << " " << center.y << " " << center.z << endl;

    if (set_center_and_radius_of_arc_from_position(errMsg))
        return true;
    else return false;
}

bool Arc::calibration(QString & errMsg)
{
    mat m;
    if (!get_offset(m)) {
        errMsg = "get calibration fail.";
        return false;
    }

    colvec _start(4);
    _start(0, 0) = start.x;
    _start(1, 0) = start.y;
    _start(2, 0) = start.z;
    _start(3, 0) = 1;
    _start = m * _start;
    start.x = _start(0, 0);
    start.y = _start(1, 0);
    start.z = _start(2, 0);

    colvec _middle(4);
    _middle(0, 0) = middle.x;
    _middle(1, 0) = middle.y;
    _middle(2, 0) = middle.z;
    _middle(3, 0) = 1;
    _middle = m * _middle;
    middle.x = _middle(0, 0);
    middle.y = _middle(1, 0);
    middle.z = _middle(2, 0);

    colvec _end(4);
    _end(0, 0) = end.x;
    _end(1, 0) = end.y;
    _end(2, 0) = end.z;
    _end(3, 0) = 1;
    _end = m * _end;
    end.x = _end(0, 0);
    end.y = _end(1, 0);
    end.z = _end(2, 0);

    colvec _center(4);
    _center(0, 0) = center.x;
    _center(1, 0) = center.y;
    _center(2, 0) = center.z;
    _center(3, 0) = 1;
    _center = m * _center;
    center.x = _center(0, 0);
    center.y = _center(1, 0);
    center.z = _center(2, 0);

    return true;
}

// 已知圆弧上的三个点，计算圆心和半径
bool Arc::set_center_and_radius_of_arc_from_position(QString & errMsg)
{
    if (is_equal(start, middle) || is_equal(start, end) || is_equal(middle, end)) {
        qDebug() << "示教点的坐标位置过于接近，请重新示教";
        errMsg = "示教点的坐标位置过于接近，请重新示教";
        return false;
    }

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
        return true;

    qDebug() << "圆弧信息计算错误.";
    errMsg = "圆弧信息计算错误.";
    return false;
}

bool Arc::set_arc_type(QString &errMsg)
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
    else {
        qDebug() << "can not jude arc type.";
        errMsg = "can not jude arc type.";
        return false;
    }

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

    return true;
}

bool Arc::set_central_angle(QString & errMsg)
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
    if (0 == norm(center_to_start)) {
        qDebug() << "norm of center_to_start is 0" << endl;
        errMsg = "norm of center_to_start is 0";
        return false;
    }
    if (0 == norm(center_to_end)) {
        qDebug() << "norm of center_to_end is 0" << endl;
        errMsg = "norm of center_to_end is 0";
        return false;
    }

    central_degree = acos(dot(center_to_start, center_to_end) / (norm(center_to_start) * norm(center_to_end)));
    if (MAJOR == type)
        central_degree = 360 * M_PI / 180 - central_degree;
    qDebug() << "central degree: " << central_degree << endl;

    return true;
}

bool Arc::set_angle_per_interpolation_cycle(QString & errMsg)
{
    if (radius <= 0) {
        errMsg = "Invalid arc radius.";
        return false;
    }

    angle_per_interpolation_cycle = displacement_per_interpolation_cycle / radius;     // 单位为：弧度
    return true;
}

bool Arc::set_interpolation_num(QString & errMsg)
{
    if (angle_per_interpolation_cycle <= 0) {
        errMsg = "Invalid angle per interpolation cycle.";
        return false;
    }

    interpolation_num = central_degree / angle_per_interpolation_cycle;
    return true;
}

// 以圆弧所在平面，建立一个新的坐标系，该坐标系到和基坐标系的旋转变换关系
// 注意，这里只计算旋转变换关系，没有位置偏移关系
void Arc::set_homogeneous_transformation_matrix_to_base(QString & errMsg)
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
void Arc::get_increase()
{
    get_increase_trigonometric_method();
    //    get_increase_vector_method();
}

// 向量方法计算插补增量
void Arc::get_increase_vector_method()
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
    double angle_increase_in_degree = (end_teach_point.roll - start_teach_point.roll) / interpolation_num;

    // 工具姿态变换的旋转矩阵
    mat tool_rot_matrix = HomogeneousTransformationMatrix::get_z_rot_matrix(angle_increase_in_degree);

    // 基坐标系中表示工具的向量
    colvec current_tool_in_base(4), previous_tool_in_base(4);
    current_tool_in_base(0, 0) = start_teach_point.x - start.x;
    current_tool_in_base(1, 0) = start_teach_point.y - start.y;
    current_tool_in_base(2, 0) = start_teach_point.z - start.z;
    current_tool_in_base(3, 0) = 1;
    previous_tool_in_base = current_tool_in_base;

    for (int i = 0; i < interpolation_num; i++) {
        // 二次逼近圆弧插补算法
        second_approximation(current_position_in_arc_coordinate_system(0, 0),
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
        qDebug() << "vector calculate method:\n" << "\tincrease.x: " << increase.x << " "
                 << "increase.y: " << increase.y << " "
                 << "increase.z: " << increase.z << " "
                 << "increase.yaw: " << increase.yaw << " "
                 << "increase.pitch: " << increase.pitch << " "
                 << "increase.roll: " << increase.roll << endl;
    }
}

// 三角函数方法计算插补增量
void Arc::get_increase_trigonometric_method()
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
    double current_angle = degree2radian(start_teach_point.roll + tool_init_degree);
    double previous_angle =  current_angle;

    // 起点示教点和终点示教点的姿态不一定一样，所以要渐变过去
    // to do... 可以增加中间示教点的数量，更加细致的控制中间点的位姿
    double angle_in_degree = (end_teach_point.roll - start_teach_point.roll) / interpolation_num;
    double angle_in_radian = angle_in_degree * M_PI / 180;

    for (int i = 0; i < interpolation_num; i++) {
        second_approximation(current(0, 0), current(1, 0), trans(0, 0), trans(1, 0));
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

        qDebug() << "trigonometric calculate method:\n" << "\tincrease.x: " << increase.x << " "
                 << "increase.y: " << increase.y << " "
                 << "increase.z: " << increase.z << " "
                 << "increase.yaw: " << increase.yaw << " "
                 << "increase.pitch: " << increase.pitch << " "
                 << "increase.roll: " << increase.roll << endl;
    }
}

// 一次逼近圆弧插补算法
void Arc::primary_approximation(const double & x, const double & y,
                                double & x_increase, double & y_increase)
{
    double k = angle_per_interpolation_cycle;

    x_increase = -direction * k * y;
    y_increase = direction * k * x;
}

// 二次逼近圆弧插补算法
void Arc::second_approximation(const double & x, const double & y,
                               double & x_increase, double & y_increase)
{
    double k1 = 0.5 * (angle_per_interpolation_cycle) * (angle_per_interpolation_cycle);
    double k2 = angle_per_interpolation_cycle;

    x_increase = -direction * k1 * x - direction * k2 * y;
    y_increase = direction * k2 * x - direction * k1 * y;
}

//void Arc::get_add()
//{
//    double k1 = 0.5 * (angle_per_interpolation_cycle) * (angle_per_interpolation_cycle);
//    double k2 = angle_per_interpolation_cycle;

//    // 以圆心为原点，圆心指向圆弧起点为+x轴，建立一个新的坐标系
//    // current为当前位置在该新坐标系中的坐标(用列向量表示)
//    // 初始时为圆弧起点的位置
//    colvec current(3);
//    current(0, 0) = radius;
//    current(1, 0) = 0;
//    current(2, 0) = 0;

//    colvec current_in_base(3);
//    current_in_base = transform_matrix * current;
//    colvec previous_in_base(3);
//    previous_in_base = current_in_base;
//    qDebug() << "start: " << start.x - center.x << " " << start.y - center.y << " " << start.z - center.z << endl;
//    qDebug() << "current in base: " << current_in_base(0, 0) << " "
//                                    << current_in_base(1, 0) << " "
//                                    << current_in_base(2, 0) << endl;
//    // 基坐标系中表示工具的向量
//    colvec tool(4);
//    tool(0, 0) = start_teach_point.x - start.x;
//    tool(1, 0) = start_teach_point.y - start.y;
//    tool(2, 0) = start_teach_point.z - start.z;
//    tool(3, 0) = 1;
//    qDebug() << "tool: " << tool(0, 0) << " " << tool(1, 0) << " " << tool(2, 0) << " " << tool(3, 0) << endl;

//    // 每个插补周期，每个平移方向的增量
//    double x_trans, y_trans, z_trans;

//    colvec trans(3);
//    Pose increase;

//    // C轴的起始角度
//    double current_angle = degree2radian(start_teach_point.roll + tool_init_degree);
//    double previous_angle =  current_angle;

//    // 起点示教点和终点示教点的姿态不一定一样，所以要渐变过去
//    // to do... 可以增加中间示教点的数量，更加细致的控制中间点的位姿
//    double angle_in_degree = (end_teach_point.roll - start_teach_point.roll) / interpolation_num;
//    double angle_in_radian = angle_in_degree * M_PI / 180;

//    mat matrix = HomogeneousTransformationMatrix::get_z_rot_matrix(angle_in_degree);
//    std::cout << "matrix: " << matrix << std::endl;

//    for (int i = 0; i < interpolation_num; i++) {
//        // 一次逼近圆弧插补算法
////        x_trans = -direction * angle_per_interpolation_cycle * current(1, 0);
////        y_trans = direction * angle_per_interpolation_cycle * current(0, 0);
////        z_trans = 0;

//        // 二次逼近圆弧插补算法
//        x_trans = -direction * k1 * current(0, 0) + (-direction * k2 * current(1, 0));
//        y_trans = direction * k2 * current(0, 0) + (-direction * k1 * current(1, 0));
//        z_trans = 0;

//        previous_in_base = transform_matrix * current;

//        current(0, 0) += x_trans;
//        current(1, 0) += y_trans;
//        current(2, 0) += z_trans;

//        trans(0, 0) = x_trans;
//        trans(1, 0) = y_trans;
//        trans(2, 0) = z_trans;
//        trans = transform_matrix * trans; // 将圆弧平面坐标系中的位移增量，转换为基坐标系中的位移增量
//        qDebug() << "trans: " << x_trans << " " << y_trans << " " << z_trans << endl;

//        current_in_base = transform_matrix * current;
//        qDebug() << "trans cal another way: " << current_in_base(0, 0) - previous_in_base(0, 0) << " "
//                                              << current_in_base(1, 0) - previous_in_base(1, 0) << " "
//                                              << current_in_base(2, 0) - previous_in_base(2, 0) << endl;
//        colvec previous(3);
//        previous(0, 0) = previous_in_base(0, 0) + tool(0, 0);
//        previous(1, 0) = previous_in_base(1, 0) + tool(1, 0);
//        previous(2, 0) = previous_in_base(2, 0) + tool(2, 0);

//        qDebug() << "tool before transform: " << tool(0, 0) << " " << tool(1, 0) << " " << tool(2, 0) << endl;
//        tool = -(matrix * (-tool));
//        qDebug() << "tool after transform: " << tool(0, 0) << " " << tool(1, 0) << " " << tool(2, 0) << endl;

//        colvec cur(3);
//        cur(0, 0) = current_in_base(0, 0) + tool(0, 0);
//        cur(1, 0) = current_in_base(1, 0) + tool(1, 0);
//        cur(2, 0) = current_in_base(2, 0) + tool(2, 0);
//        qDebug() << "cal in vector: " << cur(0, 0) - previous(0, 0) << " " << cur(1, 0) - previous(1, 0) << " " << cur(2, 0) - previous(2, 0) << endl;

//        increase.x = trans(0, 0);
//        increase.y = trans(1, 0);
//        increase.z = trans(2, 0);
//        increase.yaw = 0;
//        increase.pitch = 0;
//        increase.roll = angle_in_degree;

//        // 补偿旋转产生的位移
//        current_angle += angle_in_radian;
//        increase.x -= tool_projection_length * (cos(current_angle) - cos(previous_angle));
//        increase.y -= tool_projection_length * (sin(current_angle) - sin(previous_angle));
//        previous_angle = current_angle;

//        qDebug() << "increase: " << increase.x << " " << increase.y << " " << increase.z << " "
//                                 << increase.yaw << " " << increase.pitch << " " << increase.roll << endl;

//        if (!is_equal((cur(0, 0) - previous(0, 0)), increase.x)) {
//            qDebug() << "not equal: " << endl;
//            exit(0);
//        }
//        if (!is_equal((cur(1, 0) - previous(1, 0)), increase.y)) {
//            qDebug() << "not equal: " << endl;
//            exit(0);
//        }

//        increase_per_interpolation_cycle->push_back(increase);
//    }
//}

}   // end namespace route_planning_typedef
