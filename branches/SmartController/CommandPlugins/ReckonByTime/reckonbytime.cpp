#include "reckonbytime.h"
#include <sys/time.h>
#include <QVariant>
#include <QDebug>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
ReckonByTime::ReckonByTime()
{
    m_moduleName = "CYT";
}
void ReckonByTime::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
}
void ReckonByTime::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.DIY_Info;
}
void ReckonByTime::RunModule()
{
    //todo call command for IO

}

int  ReckonByTime::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    DBInstanceManager::getDBInstance()->m_rmutex.lock();
    if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(_ps.ErrStr))
    {
        DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        if(_ps.log.toInt() == 0)
        {
            double time_now = QDateTime::currentDateTime().toMSecsSinceEpoch();//获取ms时间戳
            DBInstanceManager::getDBInstance()->m_mutex.lock();
            DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.ErrStr].lowvalue = time_now;//起始时间存入数值型寄存器存于下限值结构
            DBInstanceManager::getDBInstance()->m_mutex.unlock();
            strMsg = QString("ID:%1 计时开始").arg(_ps.ErrStr);
            iresult  = 1;
        }
        else if(_ps.log.toInt() == 1){
            double time_now = QDateTime::currentDateTime().toMSecsSinceEpoch();
            DBInstanceManager::getDBInstance()->m_rmutex.lock();
            double starttime =  DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.ErrStr].lowvalue;
            DBInstanceManager::getDBInstance()->m_rmutex.unlock();
            double cyctime = time_now - starttime;
            DBInstanceManager::getDBInstance()->m_mutex.lock();
            DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.ErrStr].Initvalue = cyctime;
            DBInstanceManager::getDBInstance()->m_mutex.unlock();
            iresult  = 2;
            strMsg = QString("ID%1计时结束,CT:%2ms").arg(_ps.ErrStr).arg(cyctime);
        }
    }
    else{
        strMsg = QString("CYT ID:%1 数值型寄存器变量不存在").arg(_ps.ErrStr);
    }
    DBInstanceManager::getDBInstance()->m_rmutex.unlock();
    return iresult;
}
void ReckonByTime::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *ReckonByTime::Copy()
{
    return (new ReckonByTime());
}
