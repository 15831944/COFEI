#include "lout.h"
#include <MyGlobal.h>
#include <QMutexLocker>
using namespace CoreGlobal;

Lout::Lout()
{
    m_moduleName = "LOUT";
}
FunModuleInterface *Lout::Copy()
{
    Lout *out = new Lout();

    return out;
}
void Lout::Initialization()
{
    isTerminate = false;
}

void Lout::SetModuleParam(SWCONFIGXML_STRU ps)
{
    _ps = ps.IO_stru;
}

void Lout::RunModule()
{

}

int Lout::GetExcResult(QString &strMsg)
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
    DBInstanceManager::getDBInstance()->m_mutex.lock();
    DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.bitName].initvalue = _ps.stat;
    DBInstanceManager::getDBInstance()->m_mutex.unlock();
    iresult = 1;
    strMsg = QString("%1 %2 set success!").arg(m_moduleName).arg(_ps.bitName);
    return iresult;
}

void Lout::StopModule()
{
    isTerminate = false;
}
