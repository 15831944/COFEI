#include "motioncontrol.h"
#include <QApplication>

using namespace CoreGlobal;

MotionControl::MotionControl()
{
    //tempOutputData是发送给伺服的一个完整的数据结构
    tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];

    GetExcitationParams();
}

MotionControl::~MotionControl()
{

}

//获取励磁相关的参数
bool MotionControl::GetExcitationParams()
{
    bool isOk = true;

    //获取检查励磁是否成功的参数（bit，vaule）
    //获取励磁操作返回的数据，表示励磁返回数据的是哪些位和值的含义
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec)
    {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    if (m_tempInOP.length() <= 0)
        isOk = false;

    //获取如何励磁的参数（bit，vaule）
    //如何励磁，应该设置的位和应该设置的值
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempOutOP.append(pair);
        int offValue = 0;
        if(0 == item.value)
            offValue = 1;

        //如何关闭励磁
        QPair<int,int> pairoff = QPair<int,int>(item.bit,offValue);
        m_tempOFFSON.append(pairoff);
    }
    if (m_tempOutOP.length() <= 0)
        isOk = false;

    return isOk;
}

void MotionControl::Init()
{
    //初始化tempOutputData
    BaseCalcFun::MemCopyTempData(tempOutputData);

    //初始化
    isLEAxis = false;
    m_isInitStatOk = true;
    ierror = 0;
    m_CWPosVec.clear();
    m_CWRunPosMap.clear();
    m_SWPosVec.clear();
    m_OutModelPosVec.clear();
    m_InModelPosVec.clear();
}

void MotionControl::Start()
{
    for(auto it = axisMap->begin();it != axisMap->end();it++)
    {
        //根据硬件配置中轴的ID,获取控制字节的位置，第几个字节是控制字节
        int ibytePos = GetContrlWordPos(it.key());
        if(ibytePos < 0)
        {
            m_isInitStatOk = false;
            ierror = -1;
            return;
        }

        //记录控制字节的位置
        m_CWPosVec.append(ibytePos);

        //获取并记录状态字节的位置，是第几个字节
        ibytePos = GetStatuswordPos(it.key());
        m_SWPosVec.append(ibytePos);

        //获取Input-Model字节的位置，是第几个字节
        ibytePos = GetInModelPos(it.key());
        m_InModelPosVec.append(ibytePos);

        //获取Output-Model字节的位置，是第几个字节
        ibytePos = GetOutModelPos(it.key());
        m_OutModelPosVec.append(ibytePos);
    }

    //todo init ，send speed,pos

    SetAxisCWBit6();
    SetAxisParm();
    if(!SetAxisInit(m_CWPosVec))
    {
        m_isInitStatOk = false;
        ierror = -2;
        return;
    }
    //run
    RunAxis();
}

void MotionControl::SetAxisCWBit6()
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

///
/// \brief MotionControl::GetInputData
/// \param strKey 轴号
/// \param ireturn 获取errorcode的值
/// \param curPos 获取当前位置
/// \return 返回当前的状态 0未到位且无错 1到位无错 -1有错
///
int MotionControl::GetInputData(QString strKey,int &ireturn,int &curPos)
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

int MotionControl::GetInModelPos(QString strKey)
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

int MotionControl::GetOutModelPos(QString strKey)
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
/// \brief MotionControl::JudgeHWLimit
/// \param strKey 轴号
/// \param strMsg   信息
/// \return
/// 校验轴是否超出硬极限
bool MotionControl::JudgeHWLimit(QString strKey,QString &strMsg)
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
/// \brief MotionControl::JudgeSWLimit
/// \param strKey 轴号
/// \param CurPos 当前位置
/// \param strMsg 信息
/// \return
/// 校验当前位置 不超出软极限，与设置的位置偏差小于正负20
bool MotionControl::JudgeSWLimit(QString strKey,int CurPos,QString &strMsg)
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

    //当前版本，暂时不对误差进行判断。
    //    if(qAbs(((*_ps.axisMap)[strKey].pos + (*_ps.axisMap)[strKey].offsetPos)*1000 - CurPos) > 200.0)
    //    {
    //        strMsg = QString("%1 与指定位置偏差大于200.0 ").arg(CurPos);
    //        return false;
    //    }
    return true;
}

///
/// \brief moitonControl::GetContrlWorldPos
/// \param strKey 轴号
/// \return
///根据轴号获取该轴控制字的位置
int  MotionControl::GetContrlWordPos(QString strKey)
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

///
/// \brief MotionControl::GetStatusworldPos
/// \param strKey 轴号
/// \return
///根据轴号获取该轴状态字的位置
int  MotionControl::GetStatuswordPos(QString strKey)
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

    //to be better
    //get deviation
    GetDeviation(strKey);

    return ibytePos;
}

///
/// \brief MotionControl::SetAxisInit
/// \param
/// \return 初始化成功true，否则false
///设置轴的初始化 流程如下注释
bool MotionControl::SetAxisInit(const QVector<int> &tempVec)
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

bool MotionControl::CheckAxisInit()
{
    for(auto it = axisMap->begin();it != axisMap->end();it++)
    {
        int iInbytePos = GetStatuswordPos(it.key());
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
/// \brief MotionControl::SetAxisParm
/// \param
/// \param
/// 设置模式，目标速度，目标位置，加减速
void MotionControl::SetAxisParm()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(auto it = axisMap->begin();it != axisMap->end();it++)
    {
        QString strKey = it.key();

        //        AXISINFO_STRU axisInfoStru = it.value();
        AxisParams axisInfoStru = it.value();

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
                BaseAxisOperate::SetAxisPos(strType,byteNum,iBytePos,(axisInfoStru.pos +axisInfoStru.offsetPos)*1000,tempOutputData);
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
                BaseAxisOperate::SetAxisACC(strType,byteNum,iBytePos, axisInfoStru.acc * axisInfoStru.speed/100.0*\
                                            ConfigData::GetInstance()->speedRate /100 *\
                                            ConfigData::GetInstance()->iAccel *\
                                            ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strKey].adled,tempOutputData);
                QPair<int,int> pair = QPair<int,int>(iBytePos,byteNum);
                tempBytePosNum.append(pair);
            }
            else if(item->key == "Decel")
            {
                BaseAxisOperate::SetAxisDCC(strType,byteNum,iBytePos, axisInfoStru.dec * axisInfoStru.speed/100.0*\
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
/// \brief MotionControl::StopAxis
/// \param strKey 轴号
/// 轴停止
void MotionControl::StopAxis()
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

///
/// \brief MotionControl::RunAxis
/// 轴跑起来
void MotionControl::RunAxis()
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

void MotionControl::ChangeAxisORGModel()
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
    for(int i = 0;i < 100;++i)
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
    for(int i = 0;i < 50;++i)
    {
        for(auto it = m_CWRunPosMap.begin();it != m_CWRunPosMap.end();++it)
        {
            BaseAxisOperate::SetAxisRun(4,it.value());
        }
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }

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

void MotionControl::GetDeviation(QString str)
{
    int deviation = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[str].error;
    if (!m_deviation.contains(str))
        m_deviation.insert(str, deviation);
}

QString MotionControl::GetStatus()
{

}
