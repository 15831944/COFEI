#include "burncylinder.h"
#include <sys/time.h>
BurnCylinder::BurnCylinder()
{
    m_moduleName = "M10";
}
void BurnCylinder::Initialization()
{
    //todo Initialization variable
    isTerminate = false;

}
void BurnCylinder::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_CY_Stru;

}
void BurnCylinder::RunModule()
{
    //todo call command for IO

}

int  BurnCylinder::GetExcResult(QString &strMsg)
{
    int iresult = 1;
    if(_ps.soltCompVec.size() == 1)
    {
        QString strKey = _ps.soltCompVec[0];
        uchar bit = _ps.io;
        if(!strKey.isEmpty())
        {
            if(BaseCalcFun::AnalysisIOString(strKey,bit) != 0)
            {
                iresult = -1;
                strMsg = QString("%1 %2 软元件设置错误，请检查！").arg(m_moduleName).arg(_ps.id);
            }
            else
            {
                strMsg = QString("%1 %2 %3 Set OK!").arg(m_moduleName).arg(_ps.id).arg(strKey);
            }
        }
        else
        {
            iresult = -2;
            strMsg = QString("%1 %2 软元件没有设置，请检查！").arg(m_moduleName).arg(_ps.id);
        }
    }
    else if(_ps.soltCompVec.size() == 2)
    {
        uchar bit = _ps.io;
        QString strKey1 = _ps.soltCompVec[0];
        QString strKey2 = _ps.soltCompVec[1];
        if(strKey1 != strKey2)
        {
            if(bit == 1 )
            {
                QMap<QString,uchar> tempOut;
                tempOut.insert(strKey1,bit);
                bit = 0;
                tempOut.insert(strKey2,bit);
                BaseCalcFun::AnalysisIOStringS(tempOut);
                strMsg = QString("%1 %2 Set OK!").arg(m_moduleName).arg(_ps.id);
            }
            else
            {
                bit = 0;
                QMap<QString,uchar> tempOut;
                tempOut.insert(strKey1,bit);
                bit = 1;
                tempOut.insert(strKey2,bit);
                BaseCalcFun::AnalysisIOStringS(tempOut);

                strMsg = QString("%1 %2 Set OK!").arg(m_moduleName).arg(_ps.id);
            }
        }
        else
            strMsg = QString("%1 %2 软元件设置错误，请检查！").arg(m_moduleName).arg(_ps.id);

    }
    return iresult;
}
void BurnCylinder::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *BurnCylinder::Copy()
{
    return (new BurnCylinder());
}
