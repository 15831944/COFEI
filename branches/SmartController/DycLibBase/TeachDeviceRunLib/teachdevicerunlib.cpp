#include "teachdevicerunlib.h"
#include <DynamicLib/MainFrame/safethread.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <TeachingDevice/share_value.h>
TeachDeviceRunLib::TeachDeviceRunLib()
{
    m_tempdata = new uchar[InOutPutData::GetInstance()->outPutNum];
    m_EnableIO = "";
    m_speedrate = 0;
    m_jointnames.clear();
    GetIOAddress();
    ShareClass::GetIstance()->m_threadstop = true;
}

TeachDeviceRunLib::~TeachDeviceRunLib()
{
    if(m_tempdata)
    {
        delete m_tempdata;
    }
}
///
/// \brief TeachDeviceRunLib::InitJointsBytePosParam
/// \param jointnames所有关节初始化的参数信息内容
///初始化的控制字节和状态字节的位置
void TeachDeviceRunLib::InitJointsBytePosParam(QVector<QString> jointnames)
{
    InitJointS_ONParam();
    m_OutConByteMap.clear();
    m_OutModelByteMap.clear();
    m_OutTraPosByteMap.clear();
    m_OutTraSpeedByteMap.clear();
    m_OutAccByteMap.clear();
    m_OutDecByteMap.clear();
    m_OutConBitInfoMap.clear();
    m_InStatByteMap.clear();
    m_InCurModelByteMap.clear();
    m_InCurPosByteMap.clear();
    m_InErrorByteMap.clear();
    ShareClass::GetIstance()->m_threadstop = true;
    for(int i = 0; i < jointnames.size();++i)
    {
        GetJointConTrolwordBytePos(jointnames[i]);
        GetJointStatuswordBytePos(jointnames[i]);
    }
}
///
/// \brief TeachDeviceRunLib::SetJointS_ON
/// \param jointname
///设置模式8,关节励磁过程
/// 1:先获取轴的对应的当前位置
/// 2:设置轴的目标位置为当前位置
/// 3:检查模式是否为8,若不为8那么设置模式为8
/// 4:检查是否在励磁状态,若在那么完成return true
/// 5:不在励磁状态设置励磁
/// 6:再次检查,若在return true不在return false
/// -1 关节名字传输有错误失败
/// -2 关节轴对故障
/// -3 模式设置8 Fail
/// -99 励磁失败
int  TeachDeviceRunLib::SetJointS_ON(QVector<QString> jointnames)//多个轴励磁
{
    for(int i = 0; i < jointnames.size(); ++i)
    {
        QString jointname = jointnames[i];
        if(m_InCurPosByteMap.contains(jointname) && m_OutTraPosByteMap.contains(jointname)\
                && m_InCurModelByteMap.contains(jointname) && m_OutModelByteMap.contains(jointname) && \
                m_InStatByteMap.contains(jointname))
        {
            int pos = BaseAxisOperate::GetAxisCurPos("I32",4,m_InCurPosByteMap[jointname]);
            BaseAxisOperate::SetAxisPos("I32",4,m_OutTraPosByteMap[jointname],pos);
            QThread::msleep(30);
            bool modelok = false;
            for(int i = 0; i < 10;++i)
            {
                if(8 == BaseCalcFun::GetByteValue("I8",1,m_InCurModelByteMap[jointname]))
                {
                    modelok = true;
                    break;
                }
                else
                {
                    BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelByteMap[jointname],8);
                    QThread::msleep(30);
                }
            }
            if(!modelok)
            {
                return -3;
            }
        }
        else{
            return -1;
        }
    }
    QVector<QString> soffjoints;
    for(int i = 0; i < jointnames.size(); ++i)
    {
        QString jointname = jointnames[i];
        if(BaseAxisOperate::CheckAxisFault(3,2,m_InStatByteMap[jointname]))
        {
            return -2;
        }
        if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,m_InStatByteMap[jointname]))
        {
            soffjoints.append(jointname);
        }
    }
    if(soffjoints.size() == 0)
    {
        return 1;
    }
    else{
        SetJointS_OFF(soffjoints);
        QVector<int> bytePosVec;
        for(int i = 0 ; i < soffjoints.size(); ++i)
        {
            if(m_OutConByteMap.contains(soffjoints[i]))
            {
                bytePosVec.append(m_OutConByteMap[soffjoints[i]]);
            }
        }
        if(bytePosVec.size() != jointnames.size())
        {
            return -1;
        }
        BaseCalcFun::MemCopyTempData(m_tempdata);
        BaseAxisOperate::SetAxisExcite(m_tempOutOP,bytePosVec,m_tempdata);
        QThread::msleep(100);
        bool son = true;
        for(int i = 0 ; i < soffjoints.size(); ++i)
        {
            if(!BaseAxisOperate::CheckAxisFault(3,2,m_InStatByteMap[soffjoints[i]]))
            {
                return -2;
            }
            if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,m_InStatByteMap[soffjoints[i]]))
            {
                son = false;
                break;
            }
        }
        if(son)
        {
            return 1;
        }
    }
    return -99;
}

void TeachDeviceRunLib::SetJointS_OFF(QVector<QString> jointnames)
{
    QVector<int> bytePosVec;
    for(int i = 0 ; i < jointnames.size(); ++i)
    {
        if(m_OutConByteMap.contains(jointnames[i]))
        {
            bytePosVec.append(m_OutConByteMap[jointnames[i]]);
        }
    }
    if(bytePosVec.size() != jointnames.size())
    {
        return;
    }
    BaseCalcFun::MemCopyTempData(m_tempdata);
    BaseAxisOperate::SetAxisExciteOff(m_tempOffOP,bytePosVec,m_tempdata);
    QThread::msleep(30);
}

void TeachDeviceRunLib::SetJointReset(QVector<QString> jointnames)
{
    for(int i= 0; i < jointnames.size(); ++i)
    {
        if(m_OutConBitInfoMap.contains(jointnames[i]))
        {
            if(m_OutConBitInfoMap[jointnames[i]].contains("Reset")&&m_OutConByteMap.contains(jointnames[i]))
            {
                BaseAxisOperate::SetAxisReset(m_OutConBitInfoMap[jointnames[i]]["Reset"].bit,m_OutConByteMap[jointnames[i]]);
            }
        }
    }
}
///
/// \brief TeachDeviceRunLib::RunJoints
///获取当前位置,设置当前位置的过程
void TeachDeviceRunLib::RunJoints()
{
    QVector<QPair<int,int>> tempBytePosNum;
    ShareClass::GetIstance()->m_threadstop = true;
    while (ShareClass::GetIstance()->m_threadstop) {
        //急停信号
        if(SafeThread::GetInstance()->m_EmStopFlag)
        {
            return;
        }
        //使能IO和倍率判断
        if(!CheckRunStateError())
        {
            QThread::usleep(1200);
            continue;
        }
        tempBytePosNum.clear();
        BaseCalcFun::MemCopyTempData(m_tempdata);
        //多个轴使用内存拷贝
        for(int i = 0; i < m_jointnames.size(); ++i)
        {
            int curpos = 0;
            if(GetJointCurPos(m_jointnames[i],curpos)&&m_OutTraPosByteMap.contains(m_jointnames[i]))
            {
                InOutPutData::GetInstance()->m_pulse_Read_Mutex.lock();
                BaseAxisOperate::SetAxisPos("I32",4,m_OutTraPosByteMap[m_jointnames[i]],curpos + InOutPutData::GetInstance()->Current_pulse*m_speedrate ,m_tempdata);
                InOutPutData::GetInstance()->m_pulse_Read_Mutex.unlock();
                QPair<int,int> pair = QPair<int,int>(m_OutTraPosByteMap[m_jointnames[i]],4);
                tempBytePosNum.append(pair);
            }
        }
        //多个轴使用内存拷贝
        BaseCalcFun::MemCopyOutputData(m_tempdata,tempBytePosNum);
        QThread::usleep(1200);
    }
}

bool TeachDeviceRunLib::GetJointCurPos(QString jointname, int &curpos)
{
    if(m_InCurPosByteMap.contains(jointname))
    {
        curpos = BaseAxisOperate::GetAxisCurPos("I32",4,m_InCurPosByteMap[jointname]) ;
        return true;
    }
    return false;
}

bool TeachDeviceRunLib::SetJointTraPos(QString jointname, int Trapos)
{
    if(m_OutTraPosByteMap.contains(jointname))
    {
        BaseAxisOperate::SetAxisPos("I32",4,m_OutTraPosByteMap[jointname],Trapos);
        return true;
    }
    return false;
}

void TeachDeviceRunLib::SetJointModel(QString jointname,QString modeltype)
{
    if(modeltype == "1")//pp模式 1
    {
        for(int i = 0;i < 10;++i)
        {
            if(1 == BaseCalcFun::GetByteValue("I8",1,m_InCurModelByteMap[jointname]))
            {
                break;
            }
            else
            {
                BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelByteMap[jointname],1);
                QThread::msleep(20);
            }
        }
    }
    else if(modeltype == "8")//插补模式 8
    {
        for(auto item = m_OutTraPosByteMap.begin();item != m_OutTraPosByteMap.end();++item)
        {
            int curPos = 0;
            GetJointCurPos(jointname,curPos);
            BaseAxisOperate::SetAxisPos("I32",4,item.value(),curPos);
            QThread::msleep(20);
        }
        for(int i = 0;i < 10;++i)
        {
            if(8 == BaseCalcFun::GetByteValue("I8",1,m_InCurModelByteMap[jointname]))
            {
                break;
            }
            else
            {
                BaseAxisOperate::ChangeAxisModel("I8",1,m_OutModelByteMap[jointname],8);
                QThread::msleep(20);
            }
        }
    }
}

void TeachDeviceRunLib::SetJointsModel(QVector<QString> jointnames, QString modeltype)
{
    for(int i = 0 ; i < jointnames.size() ; ++i)
    {
        SetJointModel(jointnames[i],modeltype);
    }
}

void TeachDeviceRunLib::Stop()
{
    ShareClass::GetIstance()->m_threadstop = false;
}
///
/// \brief TeachDeviceRunLib::CheckJointFault
/// \param jointname
/// \param errorcode
/// \return
///
int TeachDeviceRunLib::CheckJointFault(QString jointname,int &errorcode)
{
    if(m_InStatByteMap.contains(jointname)&& m_InErrorByteMap.contains(jointname))
    {
        if(BaseAxisOperate::CheckAxisFault(3,2,m_InStatByteMap[jointname]))
        {
            errorcode = BaseAxisOperate::GetAxisErrorCode("U16",2,m_InErrorByteMap[jointname]);
            return -1;
        }
        else{
            return 1;
        }
    }
    return -2;
}

void TeachDeviceRunLib::GetIOAddress()
{
    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains("ENABLE_IO"))
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["ENABLE_IO"].ishwIO)
        {
            m_EnableIO = QString(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["ENABLE_IO"].IOaddr);
        }
    }
    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains("PULSE_A"))
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["PULSE_A"].ishwIO)
        {
            ShareClass::GetIstance()->A_IO_Address = QString(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["PULSE_A"].IOaddr);
        }
    }
    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains("PULSE_B"))
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["PULSE_B"].ishwIO)
        {
            ShareClass::GetIstance()->B_IO_Address = QString(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap["PULSE_B"].IOaddr);
        }
    }
}
///
/// \brief TeachDeviceRunLib::InitJointS_ONParam
///获得励磁数据相关的参数
void TeachDeviceRunLib::InitJointS_ONParam()
{
    m_tempInOP.clear();
    m_tempOutOP.clear();
    m_tempOffOP.clear();
    //获取检查励磁是否成功的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    //获取励磁的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempOutOP.append(pair);
        int offvalue = 0;
        if(item.value == 0)
        {
            offvalue = 1;
        }
        QPair<int,int> pairoff = QPair<int,int>(item.bit,offvalue);
        m_tempOffOP.append(pairoff);
    }
}

void TeachDeviceRunLib::GetJointConTrolwordBytePos(QString jointname)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(jointname))
        return ;
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[jointname].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[jointname].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.begin();
    for(;item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["OUTPUT"].byteInfoVec.end();++item)
    {
        int ibyteNum = item->byteNum;
        if(item->key == "Contrlworld")
        {
            m_OutConByteMap.insert(jointname, ibytePos);
            m_OutConBitInfoMap.insert(jointname,item->bitInfoMap);
        }
        if(item->key == "Model")
        {
            m_OutModelByteMap.insert(jointname, ibytePos);
        }
        else if(item->key == "Trapos")
        {
            m_OutTraPosByteMap.insert(jointname, ibytePos);
        }
        else if(item->key == "Traspeed")
        {
            m_OutTraSpeedByteMap.insert(jointname, ibytePos);
        }
        else if(item->key == "Accel")
        {
            m_OutAccByteMap.insert(jointname, ibytePos);
        }
        else if(item->key == "Decel")
        {
            m_OutDecByteMap.insert(jointname, ibytePos);
        }
        ibytePos += ibyteNum;
    }
}

void TeachDeviceRunLib::GetJointStatuswordBytePos(QString jointname)
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[jointname].iBytePos;
    QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[jointname].modelType;
    if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        return ;
    auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
    for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
    {
        int ibyteNum = it->byteNum;
        if(it->key == "Statusworld")
        {
            m_InStatByteMap.insert(jointname,ibytePos);
        }
        if(it->key == "Model")
        {
            m_InCurModelByteMap.insert(jointname, ibytePos);
        }
        if(it->key == "Curpos")
        {
            m_InCurPosByteMap.insert(jointname, ibytePos);
        }
        if(it->key == "Errorcode")
        {
            m_InErrorByteMap.insert(jointname,ibytePos);
        }
        ibytePos += ibyteNum;
    }
}

bool TeachDeviceRunLib::CheckRunStateError()
{
    //判断IO信号
    if(m_EnableIO == "")
    {
        return false;
    }
    uchar value = 0;
    BaseCalcFun::AnalysisIOString(m_EnableIO,value);
    if(value != 1)
    {
        return false;;
    }
    //倍率值
    if(m_speedrate == 0)
    {
        return false;;
    }
    return true;
}
