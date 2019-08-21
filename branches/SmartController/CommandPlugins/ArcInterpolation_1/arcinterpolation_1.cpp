#include "arcinterpolation_1.h"
#include <sys/time.h>
#include <MyGlobal.h>
#include <QApplication>
#include <QDebug>
#include <stdexcept>

ArcInterpolation_1::ArcInterpolation_1()
{
    m_moduleName = "G04";
    path = path->GetInstance();
    motion = nullptr;
}

void ArcInterpolation_1::Initialization()
{
    // 判断是否急停的变量
    isTerminate = false;
    isOk = true;

    if (!motion)
        motion = new Motion;
}
void ArcInterpolation_1::SetModuleParam(SWCONFIGXML_STRU ps)
{
    QString name = ps.dispensingParam.param1;
    msg = "路径: " + name;

    auto segments = path->mode_2_get(name, msg);
    if (nullptr == segments) {
        isOk = false;
        return;
    }
    motion->set_data(*segments);

    if (!motion->init(msg))
        isOk = false;
}

void ArcInterpolation_1::RunModule()
{
    if (isOk) {
        motion->set_stop_var(&isTerminate);

        if (!motion->run(msg))
            isOk = false;
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
    return (new ArcInterpolation_1());
}
