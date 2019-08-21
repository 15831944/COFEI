#include "Spline3D.h"
#include <fstream>
#include <iostream>

namespace MP{

using namespace alglib;

Spline3D::Spline3D(const QVector<Pose> &sample_points) : samples(sample_points)
{
    init();
}

Spline3D::~Spline3D()
{
}

void Spline3D::init()
{
    build();
    calculateLength();
}

void Spline3D::build()
{
    isSamplesAvailable();

    int size = samples.size();
    double *ptr = new double[size * 3];
    for (int i = 0; i < size; ++i) {
        ptr[i*3] = samples[i].x;
        ptr[i*3 + 1] = samples[i].y;
        ptr[i*3 + 2] = samples[i].z;
    }

    xyz.setcontent(size, 3, ptr);
    pspline3build(xyz, size, 0, 1, spline_curve);

    delete[] ptr;
}

void Spline3D::isSamplesAvailable() const
{
    if(samples.isEmpty())
        throw std::logic_error("Spline3D::checkSamples(), samples is empty.");
}

void Spline3D::calculateLength()
{
    length =  pspline3arclength(spline_curve, 0, 1.0);
}

void Spline3D::calculate(Pose &pos, double p)
{
    pspline3calc(spline_curve, p/length, pos.x, pos.y, pos.z);
}

double Spline3D::getLength() const
{
    return length;
}

double Spline3D::calculateLength(double p1, double p2)
{
    p1 /= length;
    p2 /= length;

    return pspline3arclength(spline_curve, p1, p2);
}

}   // end of namespace MP
