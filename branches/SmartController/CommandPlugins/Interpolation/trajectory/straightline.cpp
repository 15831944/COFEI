#include "straightline.h"
#include "unit.h"
#include <stdexcept>

namespace MP {

StraightLine::StraightLine(const QVector<Pose> *samples,
                           const VelocityCurveInfo *velocity_curve_info) : BaseTrajectory(samples, velocity_curve_info)
{
    velocity_curve = nullptr;
    path = nullptr;
    trajectory = nullptr;
}

StraightLine::~StraightLine()
{
    if (path)
        delete path;
    if (trajectory)
        delete trajectory;
}

bool StraightLine::IsSamplesAvailable() const
{
    if (kNumOfSamples != samples->size())
        return false;
    return true;
}

void StraightLine::GetTrajectory(double cycle_in_millsecond,
                                 QVector<Pose> *pose_per_cycle,
                                 bool enable_log,
                                 std::__cxx11::string log_file,
                                 bool re_calculate)
{
    if (trajectory)
        if (!re_calculate)
            return;
    buildTrajectory();

    Pose pose1;
    pose1.yaw = 0;
    pose1.pitch = 0;
    pose1.roll = 0;

    QVector<KDL::Frame> pose2;
    KDL::Frame current_pose;
    for(double t = 0.0; t < trajectory->Duration(); t += cycle_in_millsecond) {
        current_pose = trajectory->Pos(t);
        // unit convert, m to mm
        pose1.x = current_pose.p.x() * 1000;
        pose1.y = current_pose.p.y() * 1000;
        pose1.z = current_pose.p.z() * 1000;
        pose_per_cycle->push_back(pose1);
        pose2.push_back(current_pose);
    }
    current_pose = trajectory->Pos(trajectory->Duration());
    pose1.x = current_pose.p.x() * 1000;
    pose1.y = current_pose.p.y() * 1000;
    pose1.z = current_pose.p.z() * 1000;
    pose_per_cycle->push_back(pose1);
    pose2.push_back(current_pose);

    if (enable_log)
        logTrajectory(log_file, pose2);
}

void StraightLine::GetIncrease(double cycle_in_millsecond,
                               QVector<Pose> *increase_per_cycle,
                               bool enable_log,
                               std::__cxx11::string log_file,
                               bool re_calculate)
{
    QVector<Pose> pose_per_cycle;
    GetTrajectory(cycle_in_millsecond, &pose_per_cycle, enable_log, log_file, re_calculate);
    int size = pose_per_cycle.size();
    Pose increase;
    for (int i = 1; i < size; ++i) {
        increase.x = pose_per_cycle[i].x - pose_per_cycle[i-1].x;
        increase.y = pose_per_cycle[i].y - pose_per_cycle[i-1].y;
        increase.z = pose_per_cycle[i].z - pose_per_cycle[i-1].z;

        std::cout << increase.x << " " << increase.y << " " << increase.z << std::endl;
        increase_per_cycle->push_back(increase);
    }
}

int StraightLine::GetNumberOfPos(double t, double cycle_in_millsecond)
{

}

void StraightLine::buildTrajectory()
{
    if (trajectory) {
        delete trajectory;
        trajectory = nullptr;
    }

    buildPath();
    buildVelocityProfile();

    trajectory = new KDL::Trajectory_Segment(path, velocity_curve->GetVelocityProfile());
}

void StraightLine::buildPath()
{
    if (path) {
        delete path;
        path = nullptr;
    }

    Pose start_point = (*samples)[0];
    Pose end_point = (*samples)[1];

    // unit convert, mm to m
    start_point.scale(0.001);
    end_point.scale(0.001);

    KDL::Frame start = KDL::Frame(KDL::Rotation::RPY(0, 0, 0),
                                  KDL::Vector(start_point.x, start_point.y, start_point.z));
    KDL::Frame end = KDL::Frame(KDL::Rotation::RPY(0, 0, 0),
                                KDL::Vector(end_point.x, end_point.y, end_point.z));

    path = new KDL::Path_Line(start, end, new KDL::RotationalInterpolation_SingleAxis(), 0);
}

void StraightLine::buildVelocityProfile()
{
    if (velocity_curve) {
        delete velocity_curve;
        velocity_curve = nullptr;
    }

    if (!path)
        buildPath();

    double start_pos = 0;
    double end_pos = path->PathLength();
    velocity_curve = new VelocityCurve(start_pos, end_pos, velocity_curve_info);
}

void StraightLine::logTrajectory(std::__cxx11::string log_file, const QVector<KDL::Frame> &pose)
{
    std::ofstream trajectory_log(log_file, std::ios_base::trunc);
    if (!trajectory_log.is_open())
        throw std::runtime_error("create log file fail.");

    trajectory_log << "Path Type: ";
    KDL::Path::IdentifierType pathtype = path->getIdentifier();
    switch (pathtype) {
    case KDL::Path::ID_CIRCLE:
        trajectory_log << " circle";
        break;
    case KDL::Path::ID_LINE:
        trajectory_log << " line";
        break;
    default:
        trajectory_log << " unknown";
        break;
    }
    trajectory_log << "\n";
    trajectory_log << "Length: " << path->PathLength() << "m\n"
                   << "Duration: " << velocity_curve->Duration() << "s\n\n\n";

    for (auto p : pose) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                trajectory_log << p(i, j) << "\t";
        trajectory_log << "\n";
    }

    trajectory_log.close();
}

}   // end of namespace MP
