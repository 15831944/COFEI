#ifndef SEGMENT_H
#define SEGMENT_H

#include <QString>
#include <QVector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QFile>
#include <QDataStream>
#include "typedef.h"
#include "position.h"
#include "config.h"

namespace route_planning_typedef {

// 表示一段的基类
class Segment : public SegmentInfo
{
public:
    Segment(SegmentInfo & segment_info);
    virtual ~Segment();

    virtual bool Init(QString & errMsg);

    virtual bool init(QString & errMsg) = 0;
    virtual Pose get_increase(int i) = 0;
    virtual Pose get_start_teach_point() const = 0;
    virtual Pose get_end_teach_point() const = 0;
    virtual int get_interpolation_num() const = 0;

    virtual QVector<mat> * get_pose(QString & errMsg) = 0;
    virtual QVector<mat> * re_get_pose(QString & errMsg) = 0;

    QVector<Position> * get_position(QString & errMsg);
    Posture get_posture() const;

    QString get_file() const;
    SegmentType get_type() const;

    void set_start(Position start);
    Position get_start() const;

    void set_end(Position end);
    Position get_end() const;

    double degree2radian(double degree);
    double radian2degree(double radian);

    bool is_equal(const Position & p1, const Position & p2);
    bool is_equal(const double & a, const double & b);

    bool get_offset(mat & m);

    bool set_tool_projection_length();  // 获取末端工具在xoy平面的投影长度
    bool set_tool_init_angle();    // 获取工具初始角度

protected:
    bool open_calibration_file(QString & errMsg);
    void close_calibration_file();

    bool read(QString & errMsg);

public:
    Position start, end;
    Pose start_teach_point, end_teach_point;    // 起点和终点示教点
    QVector<Position> * position;   // 段上点的位置集合
    QVector<mat> * pose;   // 段上点的位姿集合
    Config * config;

    int interpolation_num;  // 插补次数
    QVector<Pose> * increase_per_interpolation_cycle;    //  每个插补周期，每个方向的增量

    double tool_projection_length;  // 末端工具在xoy平面的投影长度
    double tool_init_degree;    // 工具初始角度

protected:
    mat offset_matrix;

private:
    QFile * calibration_file;
    QDataStream * stream;
    struct {
        double x_trans, y_trans;    // x, y轴方向的位置偏移
        double z_rot;   // z轴方向的旋转偏移
    } offset;
};

}   // end namespace route_planning_typedef

#endif // SEGMENT_H
