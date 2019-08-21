#include "outputmodule.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <MyGlobal.h>
using namespace CoreGlobal;

OutPutModule::OutPutModule()
{
    m_moduleName = "OUT";
}
FunModuleInterface *OutPutModule::Copy()
{
    OutPutModule *out = new OutPutModule();

    return out;
}
void OutPutModule::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
}
void OutPutModule::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.IO_stru;
}
void OutPutModule::RunModule()
{
    //todo call command for IO send data if SMT
}

int  OutPutModule::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    uchar ivalue = _ps.stat;
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(_ps.bitName))
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.bitName].ishwIO)
        {
            QString ioAddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.bitName].IOaddr;
            if(BaseCalcFun::AnalysisIOString(ioAddr,ivalue) != 0)
            {
                strMsg = QString("%1 %2 Set false --> OutPutNum %3!").arg(m_moduleName).arg(_ps.bitName).arg(InOutPutData::GetInstance()->outPutNum);
                return iresult;
            }
            QThread::msleep(5);
            iresult = 1;
            strMsg = QString("%1 %2 set success!").arg(m_moduleName).arg(_ps.bitName);
        }
        else
        {
            strMsg = QString("%1 %2 参数 error isnot HWIO!").arg(m_moduleName).arg(_ps.bitName);
        }
    }
    else
    {
        strMsg = QString("%1 %2 参数 error isnot find!").arg(m_moduleName).arg(_ps.bitName);
    }
    return iresult;
}
void OutPutModule::StopModule()
{
    isTerminate = false;
}
