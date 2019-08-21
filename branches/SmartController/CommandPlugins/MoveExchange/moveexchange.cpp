#include "moveexchange.h"
#include <sys/time.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
MoveExchange::MoveExchange()
{
    m_moduleName = "DMOV";
}
void MoveExchange::Initialization()
{
    //todo Initialization variable
    isTerminate = false;

}
void MoveExchange::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.move_var;


}
void MoveExchange::RunModule()
{
    //todo call command for IO

}

int  MoveExchange::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    DBInstanceManager::getDBInstance()->m_rmutex.lock();
    if(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap.contains(_ps.Variable)&& DBInstanceManager::getDBInstance()->DBVarStru.regtextMap.contains(_ps.Value))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[_ps.Variable] = DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[_ps.Value];
        iresult = 1;
        strMsg = "regtest 变量赋值成功";
    }
    else if(DBInstanceManager::getDBInstance()->DBVarStru.countMap.contains(_ps.Variable)&& DBInstanceManager::getDBInstance()->DBVarStru.countMap.contains(_ps.Value))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.countMap[_ps.Variable] = DBInstanceManager::getDBInstance()->DBVarStru.countMap[_ps.Value];
        iresult = 2;
        strMsg = "count 变量赋值成功";
    }
    else if(DBInstanceManager::getDBInstance()->DBVarStru.timeMap.contains(_ps.Variable)&& DBInstanceManager::getDBInstance()->DBVarStru.timeMap.contains(_ps.Value))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.timeMap[_ps.Variable] = DBInstanceManager::getDBInstance()->DBVarStru.timeMap[_ps.Value];
        iresult = 3;
        strMsg = "time 变量赋值成功";
    }
    else if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(_ps.Variable)&& DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(_ps.Value))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.Variable] = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.Value];
        iresult = 4;
        strMsg = "Switch 变量赋值成功";
    }
    else if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(_ps.Variable)&& DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(_ps.Value))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        DBInstanceManager::getDBInstance()->m_mutex.lock();
        DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.Variable] = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.Value];
        iresult = 5;
        strMsg = "regnum 变量赋值成功";
    }
    else
    {
        strMsg = "变量类型不匹配　或者变量不存在";
    }
    DBInstanceManager::getDBInstance()->m_mutex.unlock();
    return iresult;
}
void MoveExchange::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *MoveExchange::Copy()
{
    MoveExchange *exchange = new MoveExchange();
    return exchange;
}
