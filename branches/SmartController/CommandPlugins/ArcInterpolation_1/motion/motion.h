#ifndef MOTION_H
#define MOTION_H

#include "converter/converter.h"
#include "end/end_params.h"
#include "motioncontrol/motioncontrol.h"
#include "segment.h"

class Motion
{
public:
    Motion();
    virtual ~Motion();

    bool init(QString & errMsg);
    void set_data(QVector<mat> * const pose);
    void set_data(QVector<QVector<mat>> * const pose);
    void set_data(QVector<Segment *> segments);

    void set_stop_var(bool * stop);
    bool run(QString & errMsg);

private:
    bool *stop;

    EndParams *  end_params;
    Converter * converter;
    MotionControl * motion_control;

    QVector<mat> * pose_in_base;

    QVector<Segment *> segments;
    QVector<MotionParams> * motion_params;

    Config * config;
};

#endif // MOTION_H
