#include "rapidposition.h"
#include <QApplication>
#include <QMessageBox>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
RapidPosition::RapidPosition()
{
    m_moduleName = "G00";
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
FunModuleInterface *RapidPosition::Copy()
{
    RapidPosition *pp = new RapidPosition();

    return pp;
}
void RapidPosition::Initialization()
{
    //todo Initialization variable
    BaseCalcFun::MemCopyTempData(tempOutputData);
    isTerminate = false;
    isLEAxis = false;
    m_isInitStatOk = true;
    ierror = 0;
    m_errorMap.clear();
    m_CWPosVec.clear();
    //m_CWRunPosMap.clear();
    m_SWPosVec.clear();
    m_OutModelPosVec.clear();
    m_InModelPosVec.clear();
}
void RapidPosition::SetModuleParam(SWCONFIGXML_STRU ps)
{
    //todo Save struct
    _ps = ps.swXml_QP_Stru;
    isLEAxis = _ps.note.contains("LE");
}
void RapidPosition::RunModule()
{
    int i = 0;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        int ibytePos = GetContrlWorldPos(it.key());
        if(ibytePos < 0)
        {
            m_isInitStatOk = false;
            ierror = -1;
            return;
        }
//        int tempResult = 0;
//        int curPos = 0;
//        GetInputData(it.key(),tempResult,curPos);
//        if(qAbs(curPos - (it.value().pos +it.value().offsetPos) *1000) > 500)
//        {
//            m_CWRunPosMap.insert(it.key(),ibytePos);
//        }
        m_CWPosVec.append(ibytePos);
        ibytePos = GetStatusworldPos(it.key());
        m_SWPosVec.append(ibytePos);
        ibytePos = GetInModelPos(it.key());
        m_InModelPosVec.append(ibytePos);
        ibytePos = GetOutModelPos(it.key());
        m_OutModelPosVec.append(ibytePos);
    }
    //todo init ，send speed,pos
    SetAxisCWBit6();
    SetModelParam();
    SetAxisParam();
    if(!SetAxisInit(m_CWPosVec))
    {
        m_isInitStatOk = false;
        ierror = -2;
        return;
    }
    //run
    RunAxis();
}
void RapidPosition::SetAxisCWBit6()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = m_CWPosVec.begin();it != m_CWPosVec.end();++it)
    {
        BaseCalcFun::SetBitValue(6,*it,0,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(*it,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
}

void RapidPosition::SetModelParam()
{
    int i = 0;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        QString strKey = it.key();
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
            iBytePos += byteNum;
        }
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);

    for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();it++)
    {
        if((i++) == _ps.axisNum)
            break;
        QString strKey = it.key();
        int iBytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
        int inmbytepos= GetInModelPos(strKey);
        for(int i = 0 ; i < 10 ;++i)
        {
            if(1 == BaseCalcFun::GetByteValue("I8",1,inmbytepos))//记住修改点模式检测的map
            {
                break;
            }
            else
            {
                BaseAxisOperate::ChangeAxisModel("I8",1,iBytePos,1);
                QThread::msleep(20);
            }
        }
    }
}
int  RapidPosition::GetExcResult(QString &strMsg)
{
    //todo strMsg is log
    int iresult = -1;
    //轴初始化失败，直接退出
    if(!m_isInitStatOk)
    {
        strMsg = QString("%1 %2 Init False! errorCode %3").arg(m_moduleName).arg(_ps.id).arg(ierror);
        return iresult;
    }
    //*******************************************


    //*******************************************
    //定义轴的标志
    bool isok[_ps.axisNum];
    for(int i = 0; i < _ps.axisNum;i++)
        isok[i] = false;
    //设置时间进入超时循环
    bool isLeIO = false;
    bool isPosInt = false;
    struct timeval tpStart,tpEnd;
    float timeUse = 0;
    gettimeofday(&tpStart,NULL);
    while (true)
    {
        if(isTerminate)//主循环发出退出标志
        {
            iresult = -99;
            StopAxis();
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(_ps.id);
            return iresult;
        }
        if(isLEAxis)
        {
            QStringList leList = _ps.note.split("/");
            for(int i = 0;i < leList.count();++i)
            {
                uchar uc = 0;
                if(BaseCalcFun::Analysis_L(leList[i],uc) != 1)
                {
                    iresult = -1;
                    StopAxis();
                    strMsg = QString("%1 %2 LE配置不对!").arg(m_moduleName).arg(_ps.id);
                    return iresult;
                }
                if(uc == 1 && !isLeIO)
                {
                    StopAxis();
                    ChangeAxisORGModel();
                    isLeIO = true;
                    break;
                }
            }
            for(auto it = _ps.axisMap.begin();it != _ps.axisMap.end();++it)
            {
                int tempResult = 0;
                int curPos = 0;
                GetInputData(it.key(),tempResult,curPos);
                if( qAbs(curPos - it.value().pos*1000 ) < 50000)
                {
                    StopAxis();
                    ChangeAxisORGModel();
                    isPosInt = true;
                    break;
                }
            }
            if(isPosInt || isLeIO)
            {
                bool isRun = true;
                for(int i = 0;i < leList.count();++i)
                {
                    uchar uc = 0;
                    BaseCalcFun::Analysis_L(leList[i],uc);
                    if(uc != 0)
                    {
                        isRun = false;
                        break;
                    }
                }
                if(isRun)
                {
                    isLeIO = false;
                    isPosInt = false;
                    RunAxis();
                }
            }
        }
        else
        {
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
                    strMsg = QString("%1 %2 axis %3 %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(strTempMsg);
                    return iresult;
                }
                if(ireturn == 0) //无故障信号
                {

                    double offsetvalue = 0.0;
                    DBInstanceManager::getDBInstance()->m_rmutex.lock();
                    if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(_ps.axisMap[it.key()].offsetPos))
                    {
                        offsetvalue =DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[_ps.axisMap[it.key()].offsetPos].Initvalue;
                    }
                    else{
                        offsetvalue = _ps.axisMap[it.key()].offsetPos.toDouble();
                    }
                    DBInstanceManager::getDBInstance()->m_rmutex.unlock();
                    if(qAbs((_ps.axisMap[it.key()].pos + offsetvalue)*1000 - curPos) > m_errorMap[it.key()])
                    {
                        isok[inum] = false;
                    }
                    else
                        isok[inum] = true;
                }
                else //故障信号
                {
                    iresult = -2;
                    StopAxis();
                    strMsg = QString("%1 %2 axis %3 return errorcode %4").arg(m_moduleName).arg(_ps.id).arg(it.key()).arg(tempResult);
                    return iresult;
                }
                //判断硬极限是否触发
                strTempMsg = "";
                if(!JudgeHWLimit(it.key(),strTempMsg))
                {
                    iresult = -4;
                    StopAxis();
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
        }
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
    if(_ps.delayMsec > 0)
        QThread::msleep(_ps.delayMsec);

    return iresult;
}

void RapidPosition::StopModule()
{
    isTerminate = false;
    m_isInitStatOk = true;

    if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(_ps.id))
    {
        for(auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[_ps.id].axisMap.begin();\
            it != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[_ps.id].axisMap.end();\
            ++it)
        {
            DBInstanceManager::getDBInstance()->m_rmutex.lock();
            if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(it.value().offsetPos))
            {
                DBInstanceManager::getDBInstance()->m_rmutex.unlock();
                continue;
            }
            DBInstanceManager::getDBInstance()->m_rmutex.unlock();
            it.value().offsetPos = "0";
        }
    }
}
///
/// \brief RapidPosition::GetInputData
/// \param strKey 轴号
/// \param ireturn 获取errorcode的值
/// \param curPos 获取当前位置
/// \return 返回当前的状态 0未到位且无错 1到位无错 -1有错
///
int RapidPosition::GetInputData(QString strKey,int &ireturn,int &curPos)
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
//            if(BaseAxisOperate::CheckAxisINP(10,ibyteNum,ibytePos))
//                iResult = 1;
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
int RapidPosition::GetInModelPos(QString strKey)
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
        int ibyteNum = it->byteNum;
        if(it->key == "Model")
        {
            break;
        }

        ibytePos += ibyteNum;
    }
    return ibytePos;
}

int RapidPosition::GetOutModelPos(QString strKey)
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
        int ibyteNum = item->byteNum;
        if(item->key == "Model")
        {
            break;
        }
        ibytePos += ibyteNum;
    }

    return ibytePos;
}
///
/// \brief RapidPosition::JudgeHWLimit
/// \param strKey 轴号
/// \param strMsg   信息
/// \return
/// 校验轴是否超出硬极限
bool RapidPosition::JudgeHWLimit(QString strKey,QString &strMsg)
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
/// \brief RapidPosition::JudgeSWLimit
/// \param strKey 轴号
/// \param CurPos 当前位置
/// \param strMsg 信息
/// \return
/// 校验当前位置 不超出软极限，与设置的位置偏差小于正负20
bool RapidPosition::JudgeSWLimit(QString strKey,int CurPos,QString &strMsg)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    QString str;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return false;
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitStart;
    if(CurPos < str.toDouble()*1000)
    {
        strMsg = QString("%1 超出软极限起点%2").arg(CurPos).arg(str.toDouble());
        return false;
    }
    str = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].swLimitEnd;
    if(CurPos > str.toDouble()*1000)
    {
        strMsg = QString("%1 超出软极限终点%2 ").arg(CurPos).arg(str.toDouble());
        return false;
    }
//    if(qAbs((_ps.axisMap[strKey].pos + _ps.axisMap[strKey].offsetPos)*1000 - CurPos) > 200.0)
//    {
//        strMsg = QString("%1 与指定位置偏差大于200.0 ").arg(CurPos);
//        return false;
//    }
    return true;
}
///
/// \brief RapidPosition::GetContrlWorldPos
/// \param strKey 轴号
/// \return
///根据轴号获取该轴控制字的位置
int  RapidPosition::GetContrlWorldPos(QString strKey)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(strKey))
        return -1;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].iBytePos;
    int er = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].error;
    if(!m_errorMap.contains(strKey))
        m_errorMap.insert(strKey,er);
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
///
/// \brief RapidPosition::GetStatusworldPos
/// \param strKey 轴号
/// \return
///根据轴号获取该轴状态字的位置
int  RapidPosition::GetStatusworldPos(QString strKey)
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
///
/// \brief RapidPosition::SetAxisInit
/// \param
/// \return 初始化成功true，否则false
///设置轴的初始化 流程如下注释
bool RapidPosition::SetAxisInit(const QVector<int> &tempVec)
{
    if(CheckAxisInit())
        return true;
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
    QThread::msleep(50);
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
    QThread::msleep(200);
    for(int i = 0;i < 10;++i)
    {
        if(CheckAxisInit())
        {
            return true;
        }
         QThread::msleep(100);
    }
    return false;
}
bool RapidPosition::CheckAxisInit()
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

///
/// \brief RapidPosition::SetAxisParm
/// \param
/// \param
/// 设置模式，目标速度，目标位置，加减速
void RapidPosition::SetAxisParam()
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
                qDebug()<<"Trapos:"<<axisInfoStru.pos;
                //新增偏移值为变量
                double offsetvalue = 0.0;
                DBInstanceManager::getDBInstance()->m_rmutex.lock();
                if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(axisInfoStru.offsetPos))
                {
                    offsetvalue =DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[axisInfoStru.offsetPos].Initvalue;
                }
                else{
                    offsetvalue = axisInfoStru.offsetPos.toDouble();
                }
                DBInstanceManager::getDBInstance()->m_rmutex.unlock();
                qDebug()<<"strKey:"<<strKey << "偏移变量"<<axisInfoStru.offsetPos<<"偏移的值:"<<offsetvalue;
                BaseAxisOperate::SetAxisPos(strType,byteNum,iBytePos,(axisInfoStru.pos + offsetvalue)*1000,tempOutputData);
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
                qDebug()<<"plugins speed :"<<axisInfoStru.speed/100.0 *\
                          ConfigData::GetInstance()->speedRate /100.0 *\
                          ConfigData::GetInstance()->iSpeed *\
                          ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].lead;
            }
            else if(item->key == "Accel")
            {
                BaseAxisOperate::SetAxisACC(strType,byteNum,iBytePos,axisInfoStru.speed/100.0*\
                                            ConfigData::GetInstance()->speedRate /100 *\
                                            ConfigData::GetInstance()->iAccel *\
                                            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            else if(item->key == "Decel")
            {
                BaseAxisOperate::SetAxisDCC(strType,byteNum,iBytePos,axisInfoStru.speed/100.0*\
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
///
/// \brief RapidPosition::RunAxis
/// 轴跑起来
void RapidPosition::RunAxis()
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
    QThread::msleep(30);
    tempBytePosNum.clear();
    for(auto it = m_CWPosVec.begin();it != m_CWPosVec.end();++it)
    {
        BaseAxisOperate::ReAxisRunBit(4,*it,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(*it,1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

///
/// \brief RapidPosition::StopAxis
/// \param strKey 轴号
/// 轴停止
void RapidPosition::StopAxis()
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
}

void RapidPosition::ChangeAxisORGModel()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    tempBytePosNum.clear();
    for(int i = 0; i < m_OutModelPosVec.size();++i)
    {
        BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelPosVec[i],6,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_OutModelPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(150);
    for(int i = 0;i < 10;++i)
    {
        bool isok = true;
        for(int i = 0; i < m_InModelPosVec.size();++i)
        {
            if(6 != BaseCalcFun::GetByteValue("I8",1,m_InModelPosVec[i]))
            {
                isok = false;
                BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelPosVec[i],6);
                QThread::msleep(50);
                break;
            }
        }
        if(isok) break;
    }
    //for(int i = 0;i < 50;++i)
    {
        for(auto it = m_CWPosVec.begin();it != m_CWPosVec.end();++it)
        {
            BaseAxisOperate::SetAxisRun(4,*it);
        }
    }

/*
    tempBytePosNum.clear();
    for(auto it = m_CWRunPosMap.begin();it != m_CWRunPosMap.end();++it)
    {
        BaseAxisOperate::SetAxisRun(4,it.value(),tempOutputData);
        QPair<int,int> pair = QPair<int,int>(it.value(),1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(50);
    for(int i = 0;i < 100;++i)
    {
        bool isok = true;
        for(auto it = m_CWRunPosMap.begin();it != m_CWRunPosMap.end();++it)
        {
            int ireturn = 0;
            int curPos = -1;
            GetInputData(it.key(),ireturn,curPos);
            if(curPos > 100 || curPos < -100)
            {
                isok = false;
                break;
            }
        }
        if(isok)
            break;
        else
        {
            tempBytePosNum.clear();
            for(auto it = m_CWRunPosMap.begin();it != m_CWRunPosMap.end();++it)
            {
                BaseAxisOperate::SetAxisRun(4,it.value(),tempOutputData);
                QPair<int,int> pair = QPair<int,int>(it.value(),1);
                tempBytePosNum.append(pair);
            }
            BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
            QThread::msleep(50);
        }
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }

    tempBytePosNum.clear();
    for(int i = 0;i < m_CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisRunBit(4,m_CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(50);
*/
    tempBytePosNum.clear();
    for(int i = 0; i < m_OutModelPosVec.size();++i)
    {
        BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelPosVec[i],1,tempOutputData);
        QPair<int,int> pair = QPair<int,int>(m_OutModelPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
}

