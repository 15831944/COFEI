#include "Interpolation.h"
#include <sys/time.h>
#include <MyGlobal.h>
#include <QApplication>
#include <QDebug>
#include <stdexcept>
#include <exception>
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <float.h>
#include <cmath>

ArcInterpolation_1::ArcInterpolation_1()
{
    m_moduleName = "G04";
    trajectories = Trajectories::GetInstance();
    trajectories->SetTrajectoriesInfo(&(Config::GetInstance()->motion_info->trajectories_info));
    motion = nullptr;
}

void ArcInterpolation_1::Initialization()
{
    try {
    // 判断是否急停的变量
    isTerminate = false;
    isOk = true;

    if (!motion)
        motion = new Motion;
    motion->Init();

//    Config::GetInstance()->R_current_angle = motion->GetAxisCurrentPos("J4") * 0.001;
    }
    catch (std::exception &e) {
        isOk = false;
        msg = QString(e.what());
    }
}

void ArcInterpolation_1::SetModuleParam(SWCONFIGXML_STRU ps)
{
    try {
    QString trajectory_id = ps.dispensingParam.param1;

    msg = "路径: " + trajectory_id;
    Trajectory &traj = trajectories->GetTrajectory(trajectory_id);
    motion->SetTrajectory(traj);
    motion->SetAxisesID(Config::GetInstance()->motion_info->axises_info[Config::GetInstance()->motion_info->trajectories_info[trajectory_id].path_info.axis_group]);
    }
    catch (std::exception &e) {
        isOk = false;
        msg = QString(e.what());
    }
}

void ArcInterpolation_1::RunModule()
{
    try {
        if (isOk) {
            motion->SetStopVar(&isTerminate);
            motion->Run();
        }
    }
    catch (std::exception &e) {
        isOk = false;
        msg = QString(e.what());
    }
}

int  ArcInterpolation_1::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if (isOk) {
        msg += " 执行成功";
        iresult = 0;
    }
    QApplication::processEvents();

    strMsg = msg;
    return iresult;
}

void ArcInterpolation_1::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *ArcInterpolation_1::Copy()
{
    ArcInterpolation_1 *inter = new ArcInterpolation_1;
    return inter;
}
