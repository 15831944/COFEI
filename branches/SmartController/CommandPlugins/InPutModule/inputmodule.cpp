#include "inputmodule.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <MyGlobal.h>
using namespace CoreGlobal;
InPutModule::InPutModule()
{
    m_moduleName = "IN";
}
FunModuleInterface *InPutModule::Copy()
{
    InPutModule *inpt = new InPutModule();

    return inpt;
}

void InPutModule::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    m_CommOk = true;
    m_ioValue = 0;
    m_ioPos = 0;
}
void InPutModule::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.IO_stru;
}
void InPutModule::RunModule()
{
    //todo call command for IO

}

int  InPutModule::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    while (true)
    {
        if(isTerminate)
        {
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.bitName);
            return iresult;
        }
        int checkvalue = CheckIOBit();
        if(checkvalue == -1)
        {
            strMsg = QString("%1 %2 参数 false,检查设置变量").arg(m_moduleName).arg(_ps.bitName);
            return iresult;
        }
        else if(checkvalue == 1)
        {
            break;
        }
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    iresult = 1;
    strMsg = QString("%1 %2 success!").arg(m_moduleName).arg(_ps.bitName);
    return iresult;
}
void InPutModule::StopModule()
{
    isTerminate = false;
}
int InPutModule::CheckIOBit()
{
    uchar ivalue = 0;
    int iresult = -1;
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(_ps.bitName))
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.bitName].ishwIO)
        {
            QString ioAddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[_ps.bitName].IOaddr;
            if(BaseCalcFun::AnalysisIOString(ioAddr,ivalue) == 1)
            {
                if(ivalue != _ps.stat)
                    iresult = 0;
                else
                    iresult = 1;
            }
        }
    }
    return iresult;
}
