#include "relativemove.h"
#include <sys/time.h>
#include <QApplication>
#include <QMessageBox>
RelativeMove::RelativeMove()
{
    m_moduleName = "G02";
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
        int offValue = 0;
        if(item.value == 0)
        {
            offValue = 1;
        }
        QPair<int,int> pairoff = QPair<int,int>(item.bit,offValue);
        m_tempOFFSON.append(pairoff);
    }
}
void RelativeMove::Initialization()
{
    //todo Initialization variable
    BaseCalcFun::MemCopyTempData(tempOutputData);
    isTerminate = false;
    m_isInitStatOk = true;

}
void RelativeMove::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_QP_Stru;
}
void RelativeMove::RunModule()
{
    int i = 0;
    QVector<int> tempVec;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        int ibytePos = GetContrlWorldPos(it.key());
        if(ibytePos < 0)
        {
            m_isInitStatOk = false;
            return;
        }
        tempVec.append(ibytePos);
    }
    RelativeChange();
    //todo init ，send speed,pos
    SetAxisParm();
    ResetAllAxis(tempVec);
    if(!SetAxisInit(tempVec))
    {
        m_isInitStatOk = false;
        return;
    }
    RunAxis();

}

int  RelativeMove::GetExcResult(QString &strMsg)
{
    //todo strMsg is log
    int iresult = -1;
    //轴初始化失败，直接退出
    if(!m_isInitStatOk)
    {
        strMsg = QString("%1 %2 Init False!").arg(m_moduleName).arg(_ps.id);
        return iresult;
    }
    //定义轴的标志
    bool isok[_ps.axisNum];
    for(int i = 0; i < _ps.axisNum;i++)
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
        //to check IO
        bool isToalOk = true;
        int inum = 0;
        for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
        {
            if(inum == _ps.axisNum)
                break;
            //dosomething
            //解析输入数据获取当前位置，到位信号，故障信号
            int tempResult = 0;
            int curPos = 0;
            int ireturn = GetInputData(it.key(),tempResult,curPos);
            if(ireturn == 1)//到位信号
            {
                //判断当前位置是否在范围内
//                QString strTempMsg = "";
//                if(!JudgeSWLimit(it.key(),curPos,strTempMsg))
//                {
//                    iresult = -3;
//                    StopAxis();
//                    strMsg = QString("%1 %2 axis %3 %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(strTempMsg);
//                    return iresult;
//                }
                //当前轴ok标志
                 isok[inum] = true;
            }
            else if(ireturn == 0) //无到位信号，无故障信号
            {
                isok[inum] = false;
            }
            else //故障信号
            {
                iresult = -2;
                StopAxis();
                strMsg = QString("%1 %2 axis %3 return errorcode %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(tempResult);
                return iresult;
            }
            //判断硬极限是否触发
//            QString strTempMsg = "";
//            if(!JudgeHWLimit(it.key(),strTempMsg))
//            {
//                iresult = -4;
//                StopAxis();
//                strMsg = QString("%1 %2 axis %3 %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(strTempMsg);
//                return iresult;
//            }
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
void RelativeMove::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *RelativeMove::Copy()
{
    return (new RelativeMove());
}

int RelativeMove::GetContrlWorldPos(QString strKey)
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

int RelativeMove::GetStatusworldPos(QString strKey)
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

int RelativeMove::GetModelData(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int iResult = -1;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
        return iResult;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return iResult;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        QString strType = it->type;

        if(it->key == "Model")
        {
            iResult = BaseCalcFun::GetByteValue(strType,ibyteNum,ibytePos);
            break;
        }

        ibytePos += ibyteNum;
    }
    return iResult;
}

int RelativeMove::GetInputData(QString strKey, int &ireturn, int &curPos)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int iResult = 0;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
        return false;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return false;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        QString strType = it->type;
        if(it->key == "Statusworld")
        {
            if(BaseAxisOperate::CheckAxisFault(3,ibyteNum,ibytePos))
            {
                iResult = -1;
                break;
            }
            if(BaseAxisOperate::CheckAxisINP(10,ibyteNum,ibytePos))
                iResult = 1;
        }
        if(it->key == "Errorcode")
        {
            ireturn = BaseAxisOperate::GetAxisErrorCode(strType,ibyteNum,ibytePos);
        }
        if(it->key == "Curpos")
        {
            curPos = BaseAxisOperate::GetAxisCurPos(strType,ibyteNum,ibytePos);
        }

        ibytePos += ibyteNum;
    }
    return iResult;
}

bool RelativeMove::JudgeHWLimit(QString strKey, QString &strMsg)
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

bool RelativeMove::JudgeSWLimit(QString strKey, int CurPos, QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitStart;
    if(CurPos < str.toDouble()*1000)
    {
        strMsg = QString("%1 超出软极限起点").arg(CurPos);
        return false;
    }
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitEnd;
    if(CurPos > str.toDouble()*1000)
    {
        strMsg = QString("%1 超出软极限终点 ").arg(CurPos);
        return false;
    }
//    if(qAbs((_ps.axisMap[strKey].pos + _ps.axisMap[strKey].offsetPos)*1000 - CurPos) >100.0)
//    {
//        strMsg = QString("%1 与指定位置偏差大于100.0 ").arg(CurPos);
//        return false;
//    }
    return true;
}

void RelativeMove::StopAxis()
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

bool RelativeMove::SetAxisInit(const QVector<int> &tempVec)
{
    if(CheckAxisInit())
        return true;
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
    QThread::msleep(50);
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
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

bool RelativeMove::CheckAxisInit()
{
    int i = 0;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        int iInbytePos = GetStatusworldPos(it.key());
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
void RelativeMove::ResetAllAxis(const QVector<int> &tempVec)
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(int i = 0;i < tempVec.size();++i)
    {
         BaseAxisOperate::SetAxisReset(7,tempVec[i],tempOutputData);
         QPair<int,int> pair = QPair<int,int>(tempVec[i],1);
         tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
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

void RelativeMove::SetAxisParm()
{
    int i = 0;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        QString strKey = it.key();
        AXISINFO_STRU axisInfoStru = it.value();
        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
        //判断该轴ID是否在硬件配置中
        if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
            return ;
        //获取该轴在驱动器中所在字节的位置
        int iBytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
        //获取模式类型，PP模式还是其他模式
        QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;
        //判断该模式是否在已经存在
        if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
            return ;
        //循环该模式下的OUTPUT的结构体 找到模式，目标速度，目标位置，加速减的字节位置，大小，类型，并赋值
        auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
        for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
        {
            int byteNum = item->byteNum;
            QString strType = item->type;
            if(item->key == "Model")
            {
                BaseAxisOperate::ChangeAxisModel(strType,byteNum,iBytePos,1,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            else if(item->key == "Trapos")
            {
                BaseAxisOperate::SetAxisPos(strType,byteNum,iBytePos,(axisInfoStru.pos /*+axisInfoStru.offsetPos*/)*1000,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            else if(item->key == "Traspeed")
            {
                BaseAxisOperate::SetAxisSpeed(strType,byteNum,iBytePos,axisInfoStru.speed/100.0 *\
                                              ConfigData::GetInstance()->speedRate /100.0 *\
                                              ConfigData::GetInstance()->iSpeed *\
                                              ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].lead,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            else if(item->key == "Accel")
            {
                BaseAxisOperate::SetAxisACC(strType,byteNum,iBytePos,_ps.acc * axisInfoStru.speed/100.0*\
                                            ConfigData::GetInstance()->speedRate /100 *\
                                            ConfigData::GetInstance()->iAccel *\
                                            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            else if(item->key == "Decel")
            {
                BaseAxisOperate::SetAxisDCC(strType,byteNum,iBytePos,_ps.dec *axisInfoStru.speed/100.0*\
                                            ConfigData::GetInstance()->speedRate /100 *\
                                            ConfigData::GetInstance()->iDccel *\
                                            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            iBytePos += byteNum;
        }
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

void RelativeMove::RunAxis()
{
    int i = 0;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        //dosomething
        int ibytePos = GetContrlWorldPos(it.key());
        m_CWPosVec.append(ibytePos);
        BaseAxisOperate::SetAxisRun(4,ibytePos,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(ibytePos,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
    tempBytePosNum.clear();
    for(i = 0;i < m_CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisRunBit(4,m_CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

void RelativeMove::RelativeChange()
{
   // QVector<QPair<int,int>> relativeBytePosNum;
    int i = 0;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        //dosomething
        int ibytePos = GetContrlWorldPos(it.key());
//        BaseCalcFun::SetBitValue(5,ibytePos,0);
//        QThread::msleep(30);
//        BaseCalcFun::SetBitValue(5,ibytePos,1);
//        QThread::msleep(30);
        BaseCalcFun::SetBitValue(6,ibytePos,1);
//        QThread::msleep(30);
//        BaseAxisOperate::SetAxisRun(4,ibytePos);
//        QPair<int,int> pair = QPair<int,int>(ibytePos,1);
//        relativeBytePosNum.append(pair);
    }
//    BaseCalcFun::MemCopyOutputData(tempOutputData,relativeBytePosNum);
    QThread::msleep(20);
}
