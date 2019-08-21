#include "lin.h"
#include <MyGlobal.h>
#include <QMutexLocker>
using namespace CoreGlobal;
Lin::Lin()
{
    m_moduleName = "LIN";
}

void Lin::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
}

void Lin::SetModuleParam(SWCONFIGXML_STRU ps)
{
    _ps = ps.IO_stru;
}

void Lin::RunModule()
{

}

int Lin::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    DBInstanceManager::getDBInstance()->m_rmutex.lock();
    if(!DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(_ps.bitName))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        strMsg = QString("%1 %2 Set false,检查设置变量").arg(m_moduleName).arg(_ps.bitName);
        return iresult;
    }
     DBInstanceManager::getDBInstance()->m_rmutex.unlock();
    while (true)
    {
        if(isTerminate)
        {
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.bitName);
            return iresult;
        }

        if(CheckIOBit())
            break;
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,50);
    }
    iresult = 1;
    strMsg = QString("%1 %2 success!").arg(m_moduleName).arg(_ps.bitName);
    return iresult;
}

void Lin::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *Lin::Copy()
{
    Lin *lin = new Lin();

    return lin;
}
bool Lin::CheckIOBit()
{
     QMutexLocker lock(&DBInstanceManager::getDBInstance()->m_rmutex);
    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.bitName].initvalue == _ps.stat)
    {
        return true;
    }
    return false;
}


