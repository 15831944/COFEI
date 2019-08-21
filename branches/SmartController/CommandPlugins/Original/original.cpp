#include "original.h"


Original::Original()
{
    m_moduleName = "ORG";
    m_ORGLibMap.clear();
    tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    //获取检查励磁是否成功的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    //获取励磁的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempOutOP.append(pair);
    }
}
void Original::Initialization()
{
    //todo Initialization variable
    BaseCalcFun::MemCopyTempData(tempOutputData);
    isTerminate = false;
    isInitStatOk = true;
}
void Original::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
     _ps = ps.swXml_ORG_Stru;
}
void Original::RunModule()
{
    //todo init ，send speed,pos
    QVector<int> tempVec;
    for(auto it = _ps.swXml_ORG_AxisId_Vec.begin();it != _ps.swXml_ORG_AxisId_Vec.end();it++)
    {
        int ibytePos = GetContrlWorldPos(*it);
        if(ibytePos < 0)
        {
            isInitStatOk = false;
            return;
        }
        tempVec.append(ibytePos);
    }
//    if(!SetAxisInit(tempVec))
//    {
//        isInitStatOk = false;
//        return;
//    }
    for(auto item = _ps.swXml_ORG_AxisId_Vec.begin();item != _ps.swXml_ORG_AxisId_Vec.end();++item)
    {
        QString strAxisId = *item;
        if(!m_ORGLibMap.contains(strAxisId))
        {
            if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map.contains(strAxisId))
            {
                MyORGThread *ORGThread = new MyORGThread();
                ORGThread->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[strAxisId]);
                ORGThread->start();
                m_ORGLibMap.insert(strAxisId,ORGThread);
            }
            else
            {
                isInitStatOk = false;
            }
        }
        else
        {
             m_ORGLibMap[strAxisId]->m_result = 1;
             m_ORGLibMap[strAxisId]->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[strAxisId]);
             m_ORGLibMap[strAxisId]->start();
        }
    }
}

int  Original::GetExcResult(QString &strMsg)
{
    //todo strMsg is log
    int iresult = -1;
    if(!isInitStatOk)
    {
        strMsg = QString("%1 %2 参数设置错误,或者轴检查有故障!").arg(m_moduleName).arg(_ps.orgID);
        StopAxis();
        return iresult;
    }

    const int iToalNum = m_ORGLibMap.count();
    int iAxisOk[iToalNum] = {1};
    //设置时间进入超时循环
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (timeUse < _ps.utimeout)
    {
         if(isTerminate)//主循环发出退出标志
         {
             StopAxis();
             iresult = -99;
             strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.orgID);
             return iresult;
         }
        //to check
        int iNum = 0;
        for(auto it = m_ORGLibMap.begin();it != m_ORGLibMap.end();++it)
        {
            if(iNum == iToalNum)
                break;
            if(it.value()->m_result == 0 )
            {
                iAxisOk[iNum] = 0;
            }
            else if(it.value()->m_result == 1)
            {
                iAxisOk[iNum] = 1;
            }
            else
            {
                iAxisOk[iNum] = it.value()->m_result;
            }
            ++iNum;
        }
        int isToalOk = 0;
        for(int i = 0; i < iToalNum; ++i)
        {
            if(iAxisOk[i] < 0)
            {
                isToalOk = iAxisOk[i];
                break;
            }
            else
            {
                if(iAxisOk[i] == 0)
                {
                    isToalOk += 0;
                }
                else
                {
                    isToalOk += 1;
                }
            }
        }
        //所有的轴ok
        if(isToalOk < 0)
        {
            iresult = -2;
            strMsg = QString("%1 回原点失败  %2").arg(_ps.orgID).arg(isToalOk);
            StopAxis();
            return iresult;
        }
        else if(isToalOk == 0)
        {
            iresult = 1;
            strMsg = QString("%1 回原点成功").arg(_ps.orgID);
            break;
        }
        gettimeofday(&tpEnd,NULL);
        timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
        if(timeUse >= _ps.utimeout)
            break;
        QThread::msleep(10);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    if(iresult != 1)
    {
        iresult = -11;
        strMsg = QString("%1 axis run TimeOut").arg(_ps.orgID);
        StopAxis();
        return iresult;
    }
    QThread::msleep(20);

    return iresult;
}

void Original::StopModule()
{
    isTerminate = false;
    isInitStatOk = true;
}

FunModuleInterface *Original::Copy()
{
    Original *org = new Original();

    return org;
}
bool Original::SetAxisInit(const QVector<int> &tempVec)
{
    if(CheckAxisInit())
        return true;
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
    QThread::msleep(100);
    if(!CheckAxisInit())
        return false;
    return true;
}
bool Original::CheckAxisInit()
{
    for(auto it = _ps.swXml_ORG_AxisId_Vec.begin();it != _ps.swXml_ORG_AxisId_Vec.end();it++)
    {
        int iInbytePos = GetStatusworldPos(*it);
        if(iInbytePos < 0)
            return false;
        //检查是否有故障
        if(BaseAxisOperate::CheckAxisFault(3,2,iInbytePos))
            return false;
        //检查轴是否励磁成功
        if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,iInbytePos))
            return false;
    }
    return true;
}
void Original::StopAxis()
{
//    uchar *tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
//    BaseCalcFun::MemCopyTempData(tempOutputData);
//    for(auto it = _ps.swXml_ORG_AxisId_Vec.begin();it != _ps.swXml_ORG_AxisId_Vec.end();it++)
//    {
//        int ibytePos = GetContrlWorldPos((*it));
//        BaseAxisOperate::SetAxisStop(2,ibytePos,tempOutputData);
//    }
//    BaseCalcFun::MemCopyOutputData(tempOutputData);
//    QThread::msleep(20);
//    BaseCalcFun::MemCopyTempData(tempOutputData);
//    for(auto it = _ps.swXml_ORG_AxisId_Vec.begin();it != _ps.swXml_ORG_AxisId_Vec.end();it++)
//    {
//        int ibytePos = GetContrlWorldPos(*it);
//        BaseAxisOperate::ReAxisStopBit(2,ibytePos,tempOutputData);
//    }
//    BaseCalcFun::MemCopyOutputData(tempOutputData);
    for(auto it = m_ORGLibMap.begin(); it != m_ORGLibMap.end(); ++it)
    {
        it.value()->Stop(false);
        it.value()->quit();
        it.value()->wait();
    }
}
int  Original::GetContrlWorldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return -1;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return -2;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
    {
        if(item->key == "Contrlworld")
        {
            break;
        }
        int ibyteNum = item->byteNum;
        ibytePos += ibyteNum;
    }

    return ibytePos;
}
int  Original::GetStatusworldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
        return -1;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return -2;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        if(it->key == "Statusworld")
        {
            break;
        }
        int ibyteNum = it->byteNum;
        ibytePos += ibyteNum;
    }
    return ibytePos;
}
