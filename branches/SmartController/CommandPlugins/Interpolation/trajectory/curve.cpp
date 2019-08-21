#include <vector>
#include <fstream>
#include "curve.h"
#include "unit.h"
#include <VelocityCurve.h>

namespace MP {

Curve::Curve(const QVector<Pose> *samples,
             const VelocityCurveInfo *velocity_curve_info) : BaseTrajectory(samples, velocity_curve_info)
{
    velocity_curve = nullptr;
    path = nullptr;
    total_displacement = 0;
}

Curve::~Curve()
{
    if (path)
        delete path;
}

bool Curve::IsSamplesAvailable() const
{
    if (samples->size() < kMinSamplesNumber)
        return false;
    return true;
}

void Curve::GetTrajectory(double cycle_in_millsecond,
                          QVector<Pose> *pose_per_cycle,
                          bool enable_log,
                          std::__cxx11::string log_file,
                          bool re_calculate)
{
    if (path && velocity_curve)
        if (!re_calculate)
            return;
    init();

    Pose pos;
    double t = 0.0;
    for (; t < velocity_curve->Duration(); t += cycle_in_millsecond) {
        path->calculate(pos, meter2millmeter(velocity_curve->Pos(t)));
        pose_per_cycle->push_back(pos);
    }
    path->calculate(pos, path->getLength());
    pose_per_cycle->push_back(pos);

    if (enable_log) {
        GetTotalDisaplacement(cycle_in_millsecond);
        logTrajectory(log_file, *pose_per_cycle);
    }
}

void Curve::GetIncrease(double cycle_in_millsecond,
                        QVector<Pose> *increase_per_cycle,
                        bool enable_log,
                        std::__cxx11::string log_file,
                        bool re_calculate)
{
    QVector<Pose> pose_per_cycle;
    GetTrajectory(cycle_in_millsecond, &pose_per_cycle, enable_log, log_file, re_calculate);
    Pose increase;
    int size = pose_per_cycle.size();
    for (int i = 1; i < size; ++i) {
        increase.x = pose_per_cycle[i].x - pose_per_cycle[i-1].x;
        increase.y = pose_per_cycle[i].y - pose_per_cycle[i-1].y;
        increase.z = pose_per_cycle[i].z - pose_per_cycle[i-1].z;
        increase_per_cycle->push_back(increase);
    }
}

void Curve::GetTotalDisaplacement(double cycle_in_millsecond)
{
    if ((!path) || (!velocity_curve))
        init();

    double p1 = 0.0;
    double p2 = 0.0;
    double t = cycle_in_millsecond;
    total_displacement = 0.0;
    for (; t < velocity_curve->Duration(); t += cycle_in_millsecond) {
        p1 = meter2millmeter(velocity_curve->Pos(t - cycle_in_millsecond));
        p2 = meter2millmeter(velocity_curve->Pos(t));
        total_displacement += path->calculateLength(p1, p2);
    }
    p1 = meter2millmeter(velocity_curve->Pos(t - cycle_in_millsecond));
    p2 = meter2millmeter(velocity_curve->Pos(velocity_curve->Duration()));
    total_displacement += path->calculateLength(p1, p2);
}

int Curve::GetNumberOfPos(double t, double cycle_in_millsecond)
{
    t = velocity_curve->Duration() * 1000 - t;
    if (t < 0 || t > velocity_curve->Duration() * 1000)
        return -1;

    return t / (cycle_in_millsecond * 1000);
}

void Curve::init()
{
    buildPath();
    buildVelocityProfile();
}

void Curve::buildPath()
{
    if (path) {
        delete path;
        path = nullptr;
    }
    path = new Spline3D(*samples);
}

void Curve::buildVelocityProfile()
{
    if (velocity_curve) {
        delete velocity_curve;
        velocity_curve = nullptr;
    }

    double start_pos = 0;
    double end_pos = millmeter2meter(path->getLength());
    velocity_curve = new VelocityCurve (start_pos, end_pos, velocity_curve_info);
}

void Curve::logTrajectory(std::__cxx11::string log_file, const QVector<Pose> &pose)
{
    std::ofstream trajectory_log(log_file, std::ios_base::trunc);
    if (!trajectory_log.is_open())
        throw std::runtime_error("create log file fail.");

    trajectory_log << "Path Type: Spline Curve\n"
                   << "Length: " << path->getLength() << "mm\n"
                   << "Duration: " << velocity_curve->Duration() << "s\n"
                   << "Total displacement: " << total_displacement << "mm\n\n\n";

    trajectory_log << "Sample points:\n";
    for (auto point : *samples)
        trajectory_log << point.x << "\t" << point.y << "\t" << point.z << "\n\n\n";


    trajectory_log << "Interpolation poinsts:\n";
    for (auto p : pose)
        trajectory_log << p.x << " " << p.y << " " << p.z << " " << p.roll << "\n";

    trajectory_log.close();
}

} // end of namespace MP
