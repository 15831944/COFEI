#include "jog.h"
#include <sys/time.h>
#include "DatabaseLib/DBInstanceManager/dbinstancemanager.h"
Jog::Jog()
{
    m_moduleName = "JOG";
    tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
}
void Jog::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    isTerminate = false;
    m_isInitStatOk = true;
    m_AxisNum = 0;//轴数等于0

}
void Jog::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_JOG_Stru;

    m_AxisNum  = _ps.jogAxisInfoMap.size();


}
void Jog::RunModule()
{
    //todo call command for IO
         QVector<int> tempVec;
        auto it = _ps.jogAxisInfoMap.begin();

        m_ContrlWorldPos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(it.key(),bitInfoMap,byteInfoMap);
        m_StatusworldPos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(it.key(),InbitInfoMap,InbyteInfoMap);
        if(m_ContrlWorldPos < 0)
        {
            m_isInitStatOk = false;
            return;
        }
        tempVec.append(m_ContrlWorldPos);
        if(!StopMap.contains(it.key()))
        {
            StopThread* p_thread = new StopThread;
            p_thread->bytepos = m_ContrlWorldPos;
            StopMap.insert(it.key(),p_thread);
        }
        else
        {
            StopMap[it.key()]->quit();
            StopMap[it.key()]->wait();
        }

    //todo init ，send speed,pos
    SetAxisParm();
    ResetAllAxis(tempVec);
    if(!SetAxisInit(tempVec))
    {
        m_isInitStatOk = false;
        return;
    }
    //run
    for(auto it = _ps.jogAxisInfoMap.begin();it != _ps.jogAxisInfoMap.end();it++)
    {
        //判断信号是E、还是LE!
        //解析输入数据获取当前位置，到位信号，故障信号
        if(it.value().bitStru.bitName.contains("L"))
        {
            ireturn = BaseCalcFun::Analysis_L(it.value().bitStru.bitName,value);
        }
        else
        {
            ireturn =  BaseCalcFun::AnalysisIOString(it.value().bitStru.bitName,value);//感应停止点位信息
        }
        if(ireturn == 1)//E 信号获取成功 And
        {
            if(value != it.value().bitStru.stat)//得到位信号
            {
                RunAxis();

            }
            else
            {
                 ireturn = 2;
            }
        }
    }


}

int  Jog::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    //轴初始化失败，直接退出
    if(!m_isInitStatOk)
    {
        strMsg = QString("%1 %2 Init False!").arg(m_moduleName).arg(_ps.id);
        return iresult;
    }
    if( ireturn == 2)
    {
         strMsg = QString("%1 axis run ok").arg(_ps.id);
         iresult = 1;
         return iresult;
    }
    //定义轴的标志
    bool isok[m_AxisNum];
    for(int i = 0; i < m_AxisNum;i++)
        isok[i] = false;
    //设置时间进入超时循环
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (timeUse < _ps.timeOut)
    {
        if(isTerminate)//主循环发出退出标志
        {
            iresult = -99;
            StopAxis();
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            return iresult;
        }
        bool isToalOk = true;
        int inum = 0;
        for(auto it = _ps.jogAxisInfoMap.begin();it != _ps.jogAxisInfoMap.end();it++)
        {
            QString bitName = it.value().bitStru.bitName;
            DBInstanceManager::getDBInstance()->m_rmutex.lock();
            if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[bitName].ishwIO)
            {
                QString ioaddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[bitName].IOaddr;
                if(ioaddr.contains("E"))
                {
                    ireturn = BaseCalcFun::AnalysisIOString(ioaddr,value);
                }
            }
            else
            {
                ireturn = 1;
                value = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[bitName].initvalue;
            }
            DBInstanceManager::getDBInstance()->m_rmutex.unlock();
            if(ireturn == 1)
            {
                if(value == it.value().bitStru.stat)//得到位信号
                {
                    //当前轴ok标志
                    isok[inum] = true;
                    StopMap[it.key()]->start();//stop !将当前轴的位置设为0？？
                }
            }
            else  //信号写成了输出Q
            {
                strMsg = QString("The signal was wrongly written!");
                isok[inum] = false;
            }
            QString strTempMsg = "";
            if(!JudgeHWLimit(it.key(),strTempMsg))
            {
                iresult = -4;
                StopMap[it.key()]->start();
                strMsg = QString("%1 %2 axis %3 %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(strTempMsg);
                return iresult;
            }
            if(!isok[inum++])
                isToalOk = false;
        }
        //所有的轴ok
        if(isToalOk)
        {
            iresult = 1;
            strMsg = QString("%1 axis run ok").arg(_ps.id);
            break;
        }
        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= _ps.timeOut)
            break;
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    if(iresult != 1)
    {
        iresult = -11;
        StopAxis();
        strMsg = QString("%1 axis run TimeOut").arg(_ps.id);
        return iresult;
    }
    BaseCalcFun::mDelay(_ps.delayMsec);

    return iresult;
}
void Jog::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *Jog::Copy()
{
    return (new Jog());
}


bool Jog::SetAxisInit(const QVector<int> &tempVec)
{
    if(CheckAxisInit())
        return true;
    BaseAxisOperate::SetAxisExciteOff(MyHW_Share::GetInstance()->m_OFFSON,tempVec,tempOutputData);//Server OFF
    QThread::msleep(50);
    BaseAxisOperate::SetAxisExcite(MyHW_Share::GetInstance()->m_OUTSON,tempVec,tempOutputData);//Server ON
    QThread::msleep(50);

    for(int i = 0;i < 10;++i)
    {
        if(CheckAxisInit())
        {
            return true;
        }
         QThread::msleep(50);
    }
    return false;
}
void Jog::ResetAllAxis(const QVector<int> &tempVec)
{
    BaseCalcFun::MemCopyTempData(tempOutputData);// copy out
    QVector<QPair<int,int>> tempBytePosNum;
    for(int i = 0;i < tempVec.size();++i)
    {
         BaseAxisOperate::SetAxisReset(7,tempVec[i],tempOutputData);
         QPair<int,int> pair = QPair<int,int>(tempVec[i],1);
         tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);//copy in
    QThread::msleep(20);
    tempBytePosNum.clear();
    for(int i = 0;i < tempVec.size();++i)
    {
        BaseAxisOperate::ReAxisResetBit(7,tempVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(tempVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}
bool Jog::JudgeHWLimit(QString strKey, QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    uchar uc = 0;
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitStart;
    if(!str.isEmpty())
    {
        int itype = BaseCalcFun::AnalysisIOString(str,uc);
        if(itype != 1)
        {
            strMsg = "硬极限起点设置错误  ";
            return false;
        }
        if(uc != 0)
        {
            strMsg = "超出硬极限起点  ";
            QString str;
            for(int i = 0; i < InOutPutData::GetInstance()->inPutNum;i++)
            {
                str += QString("%1  ").arg(InOutPutData::GetInstance()->inPutData[i]);
            }
            strMsg += str;
            return false;
        }
    }
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].hwLimitEnd;
    if(!str.isEmpty())
    {
        int itype = BaseCalcFun::AnalysisIOString(str,uc);

        if(itype != 1)
        {
            strMsg = "硬极限终点设置错误  ";
            return false;
        }

        if( uc != 0)
        {
            strMsg = "超出硬极限终点  ";
            QString str;
            for(int i = 0; i < InOutPutData::GetInstance()->inPutNum;i++)
            {
                str += QString("%1  ").arg(InOutPutData::GetInstance()->inPutData[i]);
            }
            strMsg += str;
            return false;
        }
    }

    return true;
}
///
/// \brief Jog::CheckAxisInit
/// \return
///检查轴是否有故障
bool Jog::CheckAxisInit()
{
    for(auto it = _ps.jogAxisInfoMap.begin();it != _ps.jogAxisInfoMap.end();it++)
    {
        int iInbytePos = m_StatusworldPos;
        if(iInbytePos < 0)
            return false;
        //检查是否有故障
        if(BaseAxisOperate::CheckAxisFault(3,2,iInbytePos))
            return false;
        //检查轴是否励磁成功
        if(!BaseAxisOperate::CheckAxisExcited(MyHW_Share::GetInstance()->m_INSON,2,iInbytePos))
            return false;
    }
    return true;
}

void Jog::SetAxisParm()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    auto it = _ps.jogAxisInfoMap.begin();

    QString strKey = it.key();
    JOGAxisInfo_STRU axisInfoStru = it.value();
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    QPair<int,int> pair = QPair<int,int>(byteInfoMap["Model"].bytePos,byteInfoMap["Model"].byteNum);
    tempBytePosNum.append(pair);
    BaseAxisOperate::SetAxisPos(byteInfoMap["Trapos"].strType,byteInfoMap["Trapos"].byteNum,byteInfoMap["Trapos"].bytePos,_ps.MaxPos,tempOutputData);
    QPair<int,int> pair_trapos = QPair<int,int>(byteInfoMap["Trapos"].bytePos,int(byteInfoMap["Trapos"].byteNum));
    tempBytePosNum.append(pair_trapos);
    BaseAxisOperate::SetAxisSpeed(byteInfoMap["Traspeed"].strType,byteInfoMap["Traspeed"].byteNum,byteInfoMap["Traspeed"].bytePos,axisInfoStru.speed/100.0 *\
            ConfigData::GetInstance()->speedRate /100.0 *\
            ConfigData::GetInstance()->iSpeed *\
            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].lead,tempOutputData);
    QPair<int,int> pair_traspeed = QPair<int,int>(byteInfoMap["Traspeed"].bytePos,byteInfoMap["Traspeed"].byteNum);
    tempBytePosNum.append(pair_traspeed);
    BaseAxisOperate::SetAxisACC(byteInfoMap["Accel"].strType,byteInfoMap["Accel"].byteNum,byteInfoMap["Accel"].bytePos,_ps.acc * axisInfoStru.speed/100.0*\
            ConfigData::GetInstance()->speedRate /100 *\
            ConfigData::GetInstance()->iAccel *\
            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled,tempOutputData);
    QPair<int,int> pair_accel = QPair<int,int>(byteInfoMap["Accel"].bytePos,byteInfoMap["Accel"].byteNum);
    tempBytePosNum.append(pair_accel);
    BaseAxisOperate::SetAxisDCC(byteInfoMap["Decel"].strType,byteInfoMap["Decel"].byteNum,byteInfoMap["Decel"].bytePos,_ps.dec *axisInfoStru.speed/100.0*\
            ConfigData::GetInstance()->speedRate /100 *\
            ConfigData::GetInstance()->iDccel *\
            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled,tempOutputData);
    QPair<int,int> pair_decel = QPair<int,int>(byteInfoMap["Decel"].bytePos,byteInfoMap["Decel"].byteNum);
    tempBytePosNum.append(pair_decel);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

void Jog::RunAxis()
{
    int i = 0;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    //dosomething
    int ibytePos = m_ContrlWorldPos;
    m_CWPosVec.append(ibytePos);
    BaseAxisOperate::SetAxisRun(4,ibytePos,tempOutputData);
    QPair<int,int> pair = QPair<int,int>(ibytePos,1);
    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
    tempBytePosNum.clear();
    for(i = 0;i < m_CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisRunBit(4,m_CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
}

void Jog::StopAxis()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(int i = 0;i < m_CWPosVec.size();++i)
    {
         BaseAxisOperate::SetAxisStop(2,m_CWPosVec[i],tempOutputData);
         QPair<int,int> pair = QPair<int,int>(m_CWPosVec[i],1);
         tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
    tempBytePosNum.clear();
    for(int i = 0;i < m_CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisStopBit(2,m_CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

