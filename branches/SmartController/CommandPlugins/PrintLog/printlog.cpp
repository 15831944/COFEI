#include "printlog.h"
#include <sys/time.h>
PrintLog::PrintLog()
{
    m_moduleName = "LOG";
}
void PrintLog::Initialization()
{
    //todo Initialization variable
    isTerminate = false;

}
void PrintLog::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    m_log = ps.DIY_Info.log;
    _ps = ps.DIY_Info;

}
void PrintLog::RunModule()
{
    //todo call command for IO

}

int  PrintLog::GetExcResult(QString &strMsg)
{
    int iresult = 100;
    if(_ps.ErrStr == "0")//报警
    {
        strMsg = m_log;
        iresult = -1;
    }
    else if(_ps.ErrStr == "1")//正常
    {
        strMsg = m_log;
    }
    else
    {
        strMsg = "指令使用不对！";
    }
    return iresult;
}
void PrintLog::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *PrintLog::Copy()
{
    return (new PrintLog());
}
