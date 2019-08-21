#include "testinterface.h"
#include <QThread>
#include <QApplication>
#include <QMessageBox>
#include<iostream>

TestInterface::TestInterface()
{
    m_moduleName = "M00";
}
void TestInterface::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    isInitStatOk = true;

}
void TestInterface::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_CY_Stru;
}
void TestInterface::RunModule()
{
    //todo call command for IO
    uchar bit = _ps.io;
    if(_ps.soltCompVec.size() == 1)
    {
        QString strKey = _ps.soltCompVec[0];
        if(!strKey.isEmpty())//动点原点检查无误后开始 输出输出点，并检查输出状态
        {
            if(BaseCalcFun::AnalysisIOString(strKey,bit) != 0)
            {
                m_iError = -5;
                isInitStatOk = false;
                return;
            }
        }
    }

    else if(_ps.soltCompVec.size() == 2)//双电磁阀
    {

        QString strKey = _ps.soltCompVec[0];
        bit = _ps.io;
        uchar bittow;
        if(_ps.io == 1)
        {
            bittow = 0;
        }
        else if (_ps.io == 0)
        {
            bittow = 1;
        }
        if(!strKey.isEmpty() && !_ps.soltCompVec[1].isEmpty() )//动点原点检查无误后开始 输出输出点，并检查输出状态
        {
            QMap<QString,uchar> outmap;
            outmap.insert(strKey,bit);
            outmap.insert(_ps.soltCompVec[1],bittow);
            if(BaseCalcFun::AnalysisIOStringS(outmap) != 0)
            {
                m_iError = -5;
                isInitStatOk = false;
                return;
            }

        }
    }

}
int  TestInterface::GetExcResult(QString &strMsg)
{
    //todo strMsg is log
    int iresult = -1;
    if(!isInitStatOk)
    {
        strMsg = QString("%1 %2 Init stat is Error %3!").arg(m_moduleName).arg(_ps.id).arg(m_iError);
        return iresult;
    }
    int i = 0;
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (timeUse < _ps.timeOut)
    {
        ++i;
        if(isTerminate)
        {
            iresult = -99;
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            return iresult;
        }
        //to check IO
        if(CheckBasePoint() && CheckMovePoint())
        {
            iresult = 1;
            strMsg = _ps.MsgMap[iresult + 1];
            break;
        }

        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= _ps.timeOut)
            break;
        QThread::msleep(5);
        QApplication::processEvents();
    }
    if(iresult != 1)
    {
        iresult = -11;
        strMsg = _ps.MsgMap[1];
        return iresult;
    }
    if(_ps.delayMsec > 0)
        BaseCalcFun::mDelay(_ps.delayMsec);
    return iresult;
}
void TestInterface::StopModule()
{

}

FunModuleInterface *TestInterface::Copy()
{
    TestInterface *t = new TestInterface();
    return t;
}

bool TestInterface::CheckBasePoint()
{
    bool isok = true;
    uchar bit = _ps.io;
    uchar ivalue = 0;
    QString strValue = _ps.basePoint;
    if(!strValue.isEmpty())
    {
        if(BaseCalcFun::AnalysisIOString(strValue,ivalue) == 1)
        {
            if(bit == 1)
            {
                if(ivalue != 0)
                    isok = false;
            }
            else
            {
                if(ivalue != 1)
                    isok = false;
            }
        }
        else
            isok = false;
    }
    return isok;
}
bool TestInterface::CheckMovePoint()
{
    bool isok = true;
    uchar bit = _ps.io;
    uchar ivalue = 0;
    QString strValue = _ps.movePoint;
    if(!strValue.isEmpty())
    {
        if(BaseCalcFun::AnalysisIOString(strValue,ivalue) == 1)
        {
            if(bit == 1)
            {
                if(ivalue != 1)
                    isok = false;
            }
            else
            {
                if(ivalue != 0)
                    isok = false;
            }
        }
        else
            isok = false;
    }
    return isok;
}

 int TestInterface::AnalysisIOStringS(QMap<QString,uchar> &ValueMap)
 {
     int iresult = -1;

     for(auto itor = ValueMap.begin() ;itor != ValueMap.end();++itor)

     {

         QStringList strList = itor.key().split(".");

         if(strList.count() > 1)

         {

             QString strKey = strList[0];

             int bitPos = QString(strList[1]).toInt();

             if(ConfigData::GetInstance()->hwIOMap.contains(strKey))

             {

                 int iByteNum = ConfigData::GetInstance()->hwIOMap[strKey].iByteNum;

                 int iBytePos = ConfigData::GetInstance()->hwIOMap[strKey].iBytePos;

                 if(iByteNum * 8 > bitPos)

                 {

                     int ibyte = bitPos /8;

                     iBytePos += ibyte;

                     bitPos = bitPos % 8;

                     if(strKey.contains("Q"))

                     {

                         QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);

                         if(iBytePos < InOutPutData::GetInstance()->outPutNum)

                         {

                             iresult = 0;

                             if(itor.value() == 1)

                             {

                                 uchar iValue = 1;

                                 iValue = iValue << bitPos;

                                 InOutPutData::GetInstance()->outPutData[iBytePos] = InOutPutData::GetInstance()->outPutData[iBytePos] | iValue;

                             }

                             else

                             {

                                 uchar iValue = 0;

                                 iValue = ~(1 << bitPos);

                                 InOutPutData::GetInstance()->outPutData[iBytePos] = InOutPutData::GetInstance()->outPutData[iBytePos] & iValue;

                             }




                         }

                     }

                     else if(strKey.contains("E"))

                     {

                         QMutexLocker locker(&InOutPutData::GetInstance()->m_inLock);

                         if(iBytePos < InOutPutData::GetInstance()->inPutNum)

                         {

                             iresult = 1;

                             itor.value() = (InOutPutData::GetInstance()->inPutData[iBytePos] >> bitPos)&1;

                         }

                     }

                 }

             }

         }

     }

     return iresult;

 }
