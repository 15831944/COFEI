#include "axisrunpos.h"
#include <QApplication>
#include <QDebug>
AxisRunPos::AxisRunPos(SWCONFIGXML_QPOSIT_STRU ps)
{
    _ps = ps;
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
    GetInitParam();
}

int AxisRunPos::RungoPos(QString &strMsg)
{
    int iresult = 0;
    if(!m_initok)
    {
        EndAxisBit();
        strMsg = QString("axis 初始化错误，xml配置有误，请检查");
        return -1;
    }
    bool isok[_ps.axisNum];
    for(int i = 0; i < _ps.axisNum;i++)
        isok[i] = false;
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
        QString strTempMsg = "";
        if(!JudgeSWLimit(it.key(),curPos,strTempMsg))
        {
            iresult = -3;
            StopAxis();
            strMsg = QString("%1 %2 axis %3 %4").arg("快速定位").arg(_ps.id).arg(it.key()).arg(strTempMsg);
            return iresult;
        }
        if(ireturn == 0) //无故障信号
        {
            if(_ps.note.contains("G02"))//相对运动的到位判断
            {
                if(!BaseAxisOperate::CheckAxisINP(10,2,m_CurPosByteMap[it.key()]))
                {
                    isok[inum] = false;
                }
                else{
                    isok[inum] = true;
                }
            }
            else
            {
                if(qAbs(_ps.axisMap[it.key()].pos*1000 - curPos) > m_errorMap[it.key()])
                {
                    isok[inum] = false;
                }
                else{
                    isok[inum] = true;
                }
            }
        }
        else //故障信号
        {
            iresult = -2;
            StopAxis();
            strMsg = QString("%1 %2 axis %3 return errorcode %4").arg("快速定位").arg(_ps.id).arg(it.key()).arg(tempResult);
            return iresult;
        }
        //判断硬极限是否触发
        strTempMsg = "";
        if(!JudgeHWLimit(it.key(),strTempMsg))
        {
            iresult = -4;
            StopAxis();
            strMsg = QString("%1 %2 axis %3 %4").arg("快速定位").arg(_ps.id).arg(it.key()).arg(strTempMsg);
            return iresult;
        }
        if(!isok[inum++])
            isToalOk = false;
    }
    //所有的轴ok
    if(isToalOk)
    {
        iresult = 1;
        EndAxisBit();
        strMsg = QString("%1 axis run ok").arg(_ps.id);
        return iresult;
    }
    QThread::msleep(5);
    QApplication::processEvents(QEventLoop::AllEvents,100);
    return iresult;
}

void AxisRunPos::StopgoPos()
{
    StopAxis();
}

void AxisRunPos::GetInitParam()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    m_errorMap.clear();
    m_ConPosMap.clear();
    m_StaPosMap.clear();
    m_OutModelPosMap.clear();
    m_InModelPosMap.clear();
    m_TraPosByteMap.clear();
    m_TraSpeedByteMap.clear();
    m_TraAccByteMap.clear();
    m_TraDccByteMap.clear();
    m_CurPosByteMap.clear();
    m_SWPosVec.clear();
    m_CWPosVec.clear();
    m_errcoderMap.clear();
    m_initok = true;
    int i = 0;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        GetContrlWorldPos(it.key());//得到控制字相关参数
        GetStatusworldPos(it.key());//得到状态字相关参数
    }
}

bool AxisRunPos::CheckInit()
{
    int i = 0;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        if(m_StaPosMap[it.key()] < 0)
            return false;
        //检查是否有故障
        if(BaseAxisOperate::CheckAxisFault(3,2,m_StaPosMap[it.key()]))
            return false;
        //检查轴是否励磁成功
        if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,m_StaPosMap[it.key()]))
            return false;
    }
    return true;
}

void AxisRunPos::GetContrlWorldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return ;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    int er = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].error;
    if(!m_errorMap.contains(strKey))
        m_errorMap.insert(strKey,er);
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
    {
        int ibyteNum = item->byteNum;
        if(item->key == "Contrlworld")
        {
            m_ConPosMap.insert(strKey, ibytePos);
            m_CWPosVec.append(ibytePos);
        }
        if(item->key == "Model")
        {
            m_OutModelPosMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Trapos")
        {
            m_TraPosByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Traspeed")
        {
            m_TraSpeedByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Accel")
        {
            m_TraAccByteMap.insert(strKey, ibytePos);
        }
        else if(item->key == "Decel")
        {
            m_TraDccByteMap.insert(strKey, ibytePos);
        }
        ibytePos += ibyteNum;
    }
    return ;

}

void AxisRunPos::GetStatusworldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        if(it->key == "Statusworld")
        {
            m_StaPosMap.insert(strKey,ibytePos);
            m_SWPosVec.append(ibytePos);
        }
        if(it->key == "Model")
        {
            m_InModelPosMap.insert(strKey, ibytePos);
        }
        if(it->key == "Curpos")
        {
            m_CurPosByteMap.insert(strKey, ibytePos);
        }
        if(it->key == "Errorcode")
        {
            m_errcoderMap.insert(strKey, ibytePos);
        }
        ibytePos += ibyteNum;
    }
    return ;
}

bool AxisRunPos::SetInitaxis()
{
    if(CheckInit())
        return true;
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,m_CWPosVec,tempOutputData);
    QThread::msleep(50);
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,m_CWPosVec,tempOutputData);
    QThread::msleep(200);
    for(int i = 0;i < 10;++i)
    {
        if(CheckInit())
        {
            return true;
        }
        QThread::msleep(100);
    }
    return false;
}

void AxisRunPos::SetsumRunParam()
{
    m_initok = true;
    SetAxisCWBit6();
    SetAxisParm();
    if(!SetInitaxis())
    {
        m_initok = false;
        return;
    }
    StartAxis();
}

void AxisRunPos::SetAxisCWBit6()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = m_CWPosVec.begin();it != m_CWPosVec.end();++it)
    {
        if(!_ps.note.contains("G02"))//区分相对运动与绝对运动
        {
            BaseCalcFun::SetBitValue(6,*it,0,tempOutputData);

        }
        else{
            BaseCalcFun::SetBitValue(6,*it,1,tempOutputData);
        }
        QPair<int,int> pair = QPair<int,int>(*it,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    if(_ps.note.contains("G02"))// 增加相对运动部分位置赋值
    {
        for(auto it = m_CurPosByteMap.begin(); it!= m_CurPosByteMap.end();++it)
        {
            int curpos =  BaseAxisOperate::GetAxisCurPos("I32",4,it.value());
            BaseAxisOperate::SetAxisPos("I32",4,m_TraPosByteMap[it.key()],curpos);
        }
    }
}

void AxisRunPos::SetAxisParm()
{
    int i = 0;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    QPair<int,int> pair;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        QString strKey = it.key();
        AXISINFO_STRU axisInfoStru = it.value();
        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
        BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelPosMap[strKey],1,tempOutputData);
        pair = QPair<int,int>(m_OutModelPosMap[strKey],1);
        tempBytePosNum.append(pair);
        BaseAxisOperate::SetAxisPos("I32",4,m_TraPosByteMap[strKey],axisInfoStru.pos*1000,tempOutputData);
        pair = QPair<int,int>(m_TraPosByteMap[strKey],4);
        tempBytePosNum.append(pair);
        BaseAxisOperate::SetAxisSpeed("U32",4,m_TraSpeedByteMap[strKey],axisInfoStru.speed/100.0 *\
                                      ConfigData::GetInstance()->speedRate /100.0 *\
                                      ConfigData::GetInstance()->iSpeed *\
                                      ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].lead*0.5,tempOutputData);
        pair = QPair<int,int>(m_TraSpeedByteMap[strKey],4);
        tempBytePosNum.append(pair);
        BaseAxisOperate::SetAxisACC("U32",4,m_TraAccByteMap[strKey],axisInfoStru.speed/100.0*\
                                    ConfigData::GetInstance()->speedRate /100 *\
                                    ConfigData::GetInstance()->iAccel *\
                                    ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled*0.5,tempOutputData);
        pair = QPair<int,int>(m_TraAccByteMap[strKey],4);
        tempBytePosNum.append(pair);
        BaseAxisOperate::SetAxisDCC("U32",4,m_TraDccByteMap[strKey],axisInfoStru.speed/100.0*\
                                    ConfigData::GetInstance()->speedRate /100 *\
                                    ConfigData::GetInstance()->iDccel *\
                                    ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled*0.5,tempOutputData);
        pair = QPair<int,int>(m_TraDccByteMap[strKey],4);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

void AxisRunPos::StartAxis()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = m_CWPosVec.begin();it != m_CWPosVec.end();++it)
    {
        BaseAxisOperate::SetAxisRun(4,*it,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(*it,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
}

bool AxisRunPos::JudgeHWLimit(QString strKey, QString &strMsg)
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

bool AxisRunPos::JudgeSWLimit(QString strKey, int CurPos, QString &strMsg)
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
    //    if(qAbs((_ps.axisMap[strKey].pos + _ps.axisMap[strKey].offsetPos)*1000 - CurPos) > 200.0)
    //    {
    //        strMsg = QString("%1 与指定位置偏差大于200.0 ").arg(CurPos);
    //        return false;
    //    }
    return true;
}

void AxisRunPos::StopAxis()
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
    QThread::msleep(30);
    tempBytePosNum.clear();
    for(int i = 0;i < m_CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisStopBit(2,m_CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
    EndAxisBit();
}

int AxisRunPos::GetInputData(QString strKey, int &ireturn, int &curPos)
{
    int iResult = 0;
    if(BaseAxisOperate::CheckAxisFault(3,2,m_StaPosMap[strKey]))
    {
        ireturn = BaseAxisOperate::GetAxisErrorCode("U16",2,m_errcoderMap[strKey]);
        iResult = -1;
    }
    curPos = BaseAxisOperate::GetAxisCurPos("I32",4,m_CurPosByteMap[strKey]);
    return iResult;
}

void AxisRunPos::EndAxisBit()
{
    m_initok = true;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = m_CWPosVec.begin();it != m_CWPosVec.end();++it)
    {
        BaseAxisOperate::ReAxisRunBit(4,*it,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(*it,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}
