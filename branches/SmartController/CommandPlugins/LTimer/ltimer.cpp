#include "ltimer.h"
#include <sys/time.h>
LTimer::LTimer()
{
    m_moduleName = "T01";
    m_TimerId = "xxYY";
    gettimeofday(&tpStart,NULL);
}
void LTimer::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    m_timeuse = 0.0;

}
void LTimer::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_TI_Stru;
    _ps.status = _ps.status.toLower();
}
void LTimer::RunModule()
{
    //todo call command for IO
}

int  LTimer::GetExcResult(QString &strMsg)
{
    int iresult = 1;
    if(_ps.status.compare("start") == 0)
    {
        gettimeofday(&tpStart,NULL);
        strMsg = QString("%1 开始计时!").arg(_ps.note);
        m_TimerId = _ps.id;
    }
    else if(_ps.status.compare("end") == 0)
    {
        if(m_TimerId != _ps.id)
        {
            iresult = -1;
            strMsg = QString("开始计时ID:%1 计时结束ID:%2 不同!").arg(m_TimerId).arg(_ps.id);
        }
        else
        {
            gettimeofday(&tpEnd,NULL);
            m_timeuse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
            strMsg = QString("%1 计时结束:%2 ms !").arg(_ps.note).arg(m_timeuse);
        }
    }

    return iresult;
}
void LTimer::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *LTimer::Copy()
{
    return (new LTimer());
}
