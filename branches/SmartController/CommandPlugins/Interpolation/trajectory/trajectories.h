#ifndef PATH_H
#define PATH_H


#include "typedef.h"
#include "BaseTrajectory.h"
#include "xyzr_fouraxis_fk.h"
#include "xyzr_fouraxis_ik.h"

#include <string>

#define INTERPOLATION_CYCLE 0.004

namespace MP {

class Trajectories;
class Trajectory
{
public:
    friend class Trajectories;

    Trajectory(const TrajectoryInfo *info);
    virtual ~Trajectory();

    void init();

    void SetInterpolationCycle(double cycle);
    void SetToolProjectiveLength(double length);
    void SetToolOriginAngle(double angle);

    QString GetId() const;
    int GetInterpolationCount() const;


    QVector<Pose> *GetJointSamples();

    void setCartSamplesFromJoint();
    QVector<Pose> *GetCartSamples();

    Pose GetJointIncrease(int i);
    Position GetCartIncrease(int i);

    QString GetIOPort() const;
    int GetDelayBeforeStart() const;

private:
    void setCartSamples();
    void setJointSamplesFromTeachPoint();
    void setCartSamplesFromTxtFile();
    void scale();
    void setOffsets();

    void calCartIncrease();
    void calJointIncrease();
    void calInterPolationCount();

    double getCurrentAngle();
    double calAngleIncrease();

    void readToolParamsFromFile(QString file);
    bool isDigitalStr(QString str);

    std::string GenerateLogFileName();

public:
    int pos_to_set_io_enable;
    int pos_to_set_io_disable;

private:
    QString id;
    const TrajectoryInfo *info;
    QVector<Pose> joint_samples;
    QVector<Pose> cart_samples;
    QVector<Pose> joint_increase;
    QVector<Pose> cart_increase;
    QVector<Pose> offsets;

    double interpolation_cycle;
    int interpolation_count;

    double tool_projective_length;  // unit: mm
    double tool_origin_angle;       // unit: radian
};

class Trajectories
{
public:
    static Trajectories *GetInstance() {
        static Trajectories p;
        return &p;
    }

    void SetTrajectoriesInfo(const QMap<QString, TrajectoryInfo> *trajectories_info);
    Trajectory &GetTrajectory(const QString id);

private:
    Trajectories();
    virtual ~Trajectories();

private:
    const QMap<QString, TrajectoryInfo> *trajectories_info;
    QMap<QString, Trajectory *> trajectories;
};

}   // end of namespace MP

#endif // PATH_H
