#include "waitmodule.h"
#include <sys/time.h>
WaitModule::WaitModule()
{
    m_moduleName = "WAIT";
}
void WaitModule::Initialization()
{
    //todo Initialization variable
    isTerminate = false;

}
void WaitModule::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_WAIT_Stru;
}
void WaitModule::RunModule()
{
    //todo call command for IO

}

int  WaitModule::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    StackManager st;
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    if(_ps.uTimeout > 0)
        gettimeofday(&tpStart,NULL);
    while (true)
    {
        if(isTerminate)
        {
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.Id);
            return iresult;
        }

        if(st.AnalysisEvent(_ps.strEvent))
        {
            iresult = 1;
            strMsg = QString("%1 %2 success!").arg(m_moduleName).arg(_ps.Id);
            break;
        }
        if(_ps.uTimeout > 0)
        {
            gettimeofday(&tpEnd,NULL);
            timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
            if(timeUse >= _ps.uTimeout)
            {
                iresult = -1;
                strMsg = QString("%1 %2 TimeOut %3!").arg(m_moduleName).arg(_ps.Id).arg(_ps.strError);
                break;
            }
        }
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,50);
    }
    return iresult;
}
void WaitModule::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *WaitModule::Copy()
{
    WaitModule *w = new WaitModule();
    return w;
}
