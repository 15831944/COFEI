#include "delay.h"
#include <sys/time.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <MyConfig_Shared>
Delay::Delay()
{
    m_moduleName = "T";
}
void Delay::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    m_isDigit = false;
    sTimeOut = "";
}
void Delay::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    sTimeOut = ps.iTimeOut;
}
void Delay::RunModule()
{
    //todo call command for IO
    if (sTimeOut.isEmpty())
    {
        return ;
    }
    QByteArray strByteArray = sTimeOut.toLatin1();
    const char *chString = strByteArray.data();

    while (*chString && *chString>='0' && *chString<='9') chString++;

    if (*chString)
    {
        m_isDigit = false;
    }
    else
    {
        m_isDigit = true;
    }
}

int  Delay::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    uint iTimeOut = -1;
    if (sTimeOut.isEmpty())
    {
        strMsg = QString("延时%1 isEmpty!").arg(sTimeOut);
        return iresult;
    }
    if(m_isDigit)
        iTimeOut = sTimeOut.toUInt();
    else
    {
        QMutexLocker lock(&DBInstanceManager::getDBInstance()->m_rmutex);
        if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(sTimeOut))
            iTimeOut = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[sTimeOut].Initvalue;
    }

    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (timeUse < iTimeOut)
    {
        if(isTerminate)
        {
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(iTimeOut);
            return iresult;
        }
        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= iTimeOut)
            break;
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,50);
    }
    iresult = 1;
    strMsg = QString("延时%1 (%2) 成功!").arg(sTimeOut).arg(iTimeOut);
    return iresult;
}
void Delay::StopModule()
{
    sTimeOut = "";
    isTerminate = false;
}

FunModuleInterface *Delay::Copy()
{
    Delay *t = new Delay();

    return t;
}
