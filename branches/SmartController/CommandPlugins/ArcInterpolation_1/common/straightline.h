#ifndef STRAIGHTLINE_H
#define STRAIGHTLINE_H

#include "segment.h"
#include "config.h"

namespace route_planning_typedef {

class StraightLine : public Segment
{
public:
    StraightLine(SegmentInfo & segment_info);
    virtual ~StraightLine();

    virtual bool init(QString & errMsg);
    virtual Pose get_increase(int i);
    virtual Pose get_start_teach_point() const;
    virtual Pose get_end_teach_point() const;
    virtual int get_interpolation_num() const;

    virtual QVector<mat> * get_pose(QString & errMsg);
    virtual QVector<mat> * re_get_pose(QString & errMsg);

private:
    bool set_teach_points(QString & errMsg);
    bool set_length(QString & errMsg);
    bool set_interpolation_num(QString & errMsg);

    void calculate_increase();

private:
    const double displacement_per_interpolation_cycle = 0.05;   // 单位: mm
    double length;  // 直线的距离
};

}   // end namespace route_planning_typedef

#endif // STRAIGHTLINE_H
