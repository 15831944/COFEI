#ifndef SPLINE_3D_H
#define SPLINE_3D_H

#include <QVector>
#include <stdafx.h>
#include <interpolation.h>
#include "typedef.h"

namespace MP {

using namespace alglib;

class Spline3D
{
public:
    Spline3D(const QVector<Pose> &samples);
    virtual ~Spline3D();

    void init();
    void calculate(Pose &pos, double p);
    double getLength() const;

    double calculateLength(double p1, double p2);

private:
    void isSamplesAvailable() const;
    void build();
    void calculateLength();

private:
    real_2d_array xyz;
    pspline3interpolant spline_curve;
    const QVector<Pose> &samples;
    double length;
};

}   // end of namespace MP

#endif // SPLINE_3D_H
