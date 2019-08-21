#include "safethread.h"
#include <QDebug>
#include <DycLibBase/LogManager/logmanager.h>
#include <QApplication>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
using namespace CoreGlobal;
//类静态指针的定义
SafeThread *SafeThread::INSTANCE = NULL;
QMap<int,SENSOR_LIGHT_STRU> SafeThread::m_LightMap = QMap<int,SENSOR_LIGHT_STRU>();
bool SafeThread::m_ispangle = false;
QTimer *SafeThread::m_LightTimer = NULL;
QMap<int,QMap<QString,bool>> m_LevelFlagMap;
QMutex *SafeThread::m_Mutex = NULL;

///
/// \brief SafeThread::SafeThread
///构造函数，start实现该线程的运行
SafeThread::SafeThread()
{
    m_LevelFlagMap.clear();
    m_FlowIOStateMap.clear();
    m_ExitFlag = false;
    m_StopFlag = false;
    m_EmStopFlag = false;
    m_ResetFlag = false;
    m_StartFlag = false;
    m_FlowAlarmLevel = 16;
    m_Mutex = new QMutex();
    AnalysisSafeData();
    QThread::msleep(100);
    start();
}
void SafeThread::AnalysisSafeData()
{
    m_LevelMap.clear();

    QMap<QString,bool> temp;
    temp.insert("EmStopFlag",false);
    temp.insert("StopFlag",true);
    temp.insert("ResetFlag",false);
    temp.insert("StartFlag",false);

    auto items =ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map;
    for(auto item = items.begin();item != items.end();++item)
    {
        //初始化子流程IO状态
        if(item.key() != "总状态")
        {
            m_FlowIOStateMap.insert(item.key(),temp);
        }

        QString flowName = item.key();
        for(auto it = item.value().begin();it != item.value().end();++it)
        {
            int ilevel = it.value().level.toInt();
            if(m_LevelMap.contains(ilevel))
            {
                m_LevelMap[ilevel].insert(flowName,it.value().BitInfoVec);
            }
            else
            {
                QMap<QString,QVector<SWCONFIGXML_SAFEBITINFO_STRU>>  temp;
                temp.insert(flowName,it.value().BitInfoVec);
                m_LevelMap.insert(ilevel,temp);
            }
            if(m_LevelFlagMap.contains(ilevel))
            {
                m_LevelFlagMap[ilevel].insert(flowName,true);
            }
            else{
                QMap<QString,bool>  temp;
                temp.insert(flowName,true);
                m_LevelFlagMap.insert(ilevel,temp);
            }
        }
    }
    //由变量名字过渡到实际的IO
    auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_THREE_SOFTWARE_Map;
    for(auto ior = it.begin();ior != it.end();++ior)
    {
        QString bitname = ior.value().bitName;
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(bitname))
        {
            if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[bitname].ishwIO)
            {
                QString bitName = QString::fromUtf8(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[bitname].IOaddr);
                ior.value().bitName = bitName;
            }
        }
    }
}

QString SafeThread::GetSafeState(int itype)
{
    QString str;
    switch (itype) {
    case 1:
    case 2:
        str = "EmStopFlag";
        break;
    case 3:
        str = "StopFlag";
        break;
    case 4:
        str = "ResetFlag";
        break;
    case 5:
        str = "StartFlag";
        break;
    case 6:
        str = "ORGFlag";
        break;
    default:
        str = "";
        break;
    }
    return str;
}

void SafeThread::SetFlowAlarmLevel(int Level)
{
    // 获取当前信号等级对应的灯
    if(!m_LightMap.contains(Level))
    {
        QMutexLocker lock(m_Mutex);
        m_LightMap.clear();
        auto ior = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_LIGHT_CONFIG_Map;
        QString id = QString::number(Level);
        if(ior.contains(id))
        {
            m_LightMap[Level]=ior[id];
        }
    }
    // 设置灯快闪,慢闪,常亮
    //    SetLightStatus(AlarmLevel);
    m_FlowAlarmLevel = Level;
}

///
/// \brief SafeThread::InitSafeVariable
///对成员变量初始化
void SafeThread::InitSafeVariable()
{
    //m_StopFlag = false;
    //m_AlarmLevel = 6;
    m_ispangle = false;
    if(m_LightTimer != NULL)
        m_LightTimer->stop();
    QMutexLocker lock(m_Mutex);
    m_LightMap.clear();
    ClearIOValue();
}

void SafeThread::SetNorLightIOValue(QString str, int value)
{
    QStringList strList = str.split(".");
    if(strList.count() > 1)
    {
        QString strKey = strList[0];
        int bitPos = QString(strList[1]).toInt();
        if(ConfigData::GetInstance()->hwIOMap.contains(strKey))
        {
            int iByteNum = ConfigData::GetInstance()->hwIOMap[strKey].iByteNum;
            int iBytePos = ConfigData::GetInstance()->hwIOMap[strKey].iBytePos;
            uchar ivalue = BaseCalcFun::GetOutBitValue(bitPos,iByteNum,iBytePos);
            if(ivalue != value)
            {
                ivalue = value;
                BaseCalcFun::AnalysisIOString(str,ivalue);
            }
        }
    }
}

void SafeThread::ClearIOValue()
{
    auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_THREE_SOFTWARE_Map;
    for(auto ior = it.begin();ior != it.end();++ior)
    {
        int ivalue = 0;
        if(ivalue == ior.value().stat)
        {
            ivalue = 1;
        }
        SetNorLightIOValue(ior.value().bitName,ivalue);
    }
}

void SafeThread::SetLightStatus(int AlarmLevel)
{
    auto Levels = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_LIGHT_CONFIG_Map;
    QString strAlarmLevel = QString("%1").arg(AlarmLevel);
    auto AlarmLevelofLight = Levels[strAlarmLevel];
    auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_THREE_SOFTWARE_Map;
    for(auto ior = it.begin();ior != it.end();++ior)
    {
        // 等级对应的灯 亮灯
        if(AlarmLevelofLight.light.contains(ior.key()))
        {
            m_LightTimer->stop();
            if(AlarmLevelofLight.name.compare("escape") == 0)
            {
                // 快闪
                m_ispangle = false;
                m_LightTimer->start(200);
            }
            else if(AlarmLevelofLight.name.compare("slow") == 0)
            {
                // 慢闪
                m_ispangle = false;
                m_LightTimer->start(500);
            }
            else
            {
                // 常亮
                SetNorLightIOValue(ior.value().bitName,ior.value().stat);
            }
        }
        else
        {
            //其他的灯 灭灯
            int ivalue = 0;
            if(ivalue == ior.value().stat)
            {
                ivalue = 1;
            }
            SetNorLightIOValue(ior.value().bitName,ivalue);
        }
    }
}
////
/// \brief SafeThread::run
///重载老基类的run
/// 实现对急停位，停止位的检查
/// 根据运行的等级实现三色灯
void SafeThread::run()
{
    if(m_LevelMap.size() < 1)
        return;
    if(m_LightTimer == NULL)
    {
        m_LightTimer = new QTimer();
        connect(m_LightTimer,&QTimer::timeout,this,SafeThread::SetTimerForLight,Qt::QueuedConnection);
    }
    QMap<int,SENSOR_LIGHT_STRU> flowLightMap;
    flowLightMap.clear();
    QMap<QString,bool> isInitStateMap;
    for(auto item = m_LevelMap.begin();item != m_LevelMap.end();++item)
    {
        int ilevel = item.key();
        if(ilevel==1||ilevel==2)
        {
            for(auto it = item.value().begin();it != item.value().end();++it)
            {
                QString strName = it.key();
                isInitStateMap.insert(strName,true);
            }
        }
    }
    while (!m_ExitFlag)
    {
        //scan IO
        for(auto Level = m_LevelMap.begin();Level != m_LevelMap.end();++Level)
        {
            int ilevel = Level.key();
            for(auto StateName = Level.value().begin();StateName != Level.value().end();++StateName)
            {
                QString strName = StateName.key();
                bool flag = true;
                for(auto FlowIO = StateName.value().begin();FlowIO != StateName.value().end();++FlowIO)
                {
                    QString strType = FlowIO->sensor_bit.bitName;
                    uchar value = 0;
                    //增加软IO信号的判断,从数据库中获取的内容
                    DBInstanceManager::getDBInstance()->m_rmutex.lock();
                    if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(strType))
                    {
                        if(!DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[strType].ishwIO)
                        {
                            value = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[strType].initvalue;
                        }
                        else{
                            QString iostr = QString(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[strType].IOaddr);
                            BaseCalcFun::AnalysisIOString(iostr,value);
                        }
                    }
                    DBInstanceManager::getDBInstance()->m_rmutex.unlock();
                    if(value == FlowIO->sensor_bit.stat)
                    {
                        flag = false;
                        if(m_FlowFlagMap.contains(ilevel))
                        {
                            m_FlowFlagMap[ilevel].insert(strName,FlowIO->info);
                        }
                        else
                        {
                            QMap<QString,QString> temp;
                            temp.insert(strName,FlowIO->info);
                            m_FlowFlagMap.insert(ilevel,temp);
                        }

                        if(!(strName.compare("总状态") == 0))
                        {
                            if(ilevel <= 2)
                            {
                                if(!m_FlowIOStateMap[strName]["EmStopFlag"])
                                    m_FlowIOStateMap[strName]["EmStopFlag"] = true;
                            }
                        }

                        if(strName.compare("总状态") == 0)
                            goto TotalStateError;
                        break;
                    }
                }
                if(flag)//等级对应IO已经不在状态
                {
                    //子流程IO状态恢复
                    if(ilevel <= 2)
                    {
                        if(strName.compare("总状态") == 0 && !(m_LevelFlagMap[ilevel]["总状态"]))
                        {
                            //qDebug() << "总急停退出";
                            emit SafeSignals("总状态",GetSafeState(ilevel),tr("%1急停退出").arg(strName));
                        }
                        else
                        {
                            if(m_FlowIOStateMap[strName]["EmStopFlag"])
                            {
                                m_FlowIOStateMap[strName]["EmStopFlag"] = false;
                            }

                            if(!(m_LevelFlagMap[ilevel][strName]))
                            {
                                //qDebug() << QString("%1子急停退出").arg(strName);
                                emit SafeSignals(strName,GetSafeState(ilevel),tr("%1子急停退出").arg(strName));
                            }
                        }
                    }

                    if(m_LevelFlagMap.contains(ilevel))
                    {
                        if(m_LevelFlagMap[ilevel].contains(strName))
                        {
                            if(!m_LevelFlagMap[ilevel][strName])
                            {
                                m_LevelFlagMap[ilevel][strName] = true;
                            }
                        }
                    }
                }
            }
        }

TotalStateError:
        // control
        bool isIOTriggered = false;
        bool isFlowEmStop = false;
        if(m_FlowFlagMap.size() > 0)
        {
            isIOTriggered = true;
            QList<QString> tempName;
            for(auto item = m_FlowFlagMap.begin();item != m_FlowFlagMap.end();++item)
            {
                m_AlarmLevel = item.key();
                // 总流程状态
                if(item.value().contains("总状态"))
                {
                    if(m_LevelFlagMap[m_AlarmLevel]["总状态"])
                    {
                        if(m_FlowFlagMap.size() > 0)
                        {
                            if(m_AlarmLevel <= 2)
                            {
                                m_EmStopFlag = true;
                                if(isInitStateMap.contains("总状态"))
                                {
                                    isInitStateMap["总状态"] = false;
                                }
                                isFlowEmStop = true;
                            }
                            emit SafeSignals("总状态",GetSafeState(m_AlarmLevel),item.value()["总状态"]);
                            m_LevelFlagMap[m_AlarmLevel]["总状态"] = false;
                            break;
                        }
                    }
                }
                //子流程状态
                for(auto it = item.value().begin();it != item.value().end();++it)
                {
                    if(m_LevelFlagMap[m_AlarmLevel][it.key()])
                    {
                        if(!tempName.contains(it.key()))
                        {
                            if(m_AlarmLevel <= 2)
                            {
                                if(isInitStateMap.contains(it.key()))
                                {
                                    isInitStateMap[it.key()] = false;
                                }
                                isFlowEmStop = true;
                            }
                            emit SafeSignals(it.key(),GetSafeState(m_AlarmLevel),it.value());
                            m_LevelFlagMap[m_AlarmLevel][it.key()] = false;
                        }
                    }
                }
            }
            m_FlowFlagMap.clear();
        }

        // set Light
        if(!isIOTriggered)
        {
            m_EmStopFlag = false;
            if(m_FlowAlarmLevel == 10)
            {
                m_FlowAlarmLevel = 16;
            }
            if(m_LightTimer == NULL)
                continue;
            if(m_LightMap.contains(1)||m_LightMap.contains(2))
            {
                InitSafeVariable();
            }
            auto ior = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_LIGHT_CONFIG_Map;
            QString id = QString::number(m_FlowAlarmLevel);
            if(ior.contains(id))
            {
                if(!flowLightMap.contains(m_FlowAlarmLevel))
                {
                    //qDebug()<<"流程状态:"<<m_FlowAlarmLevel;
                    InitSafeVariable();
                    flowLightMap.clear();
                    flowLightMap.insert(m_FlowAlarmLevel,ior[id]);
                    m_Mutex->lock();
                    m_LightMap = flowLightMap;
                    m_Mutex->unlock();
                    m_LightTimer->stop();
                    // 定义常亮
                    SetLightStatus(m_FlowAlarmLevel);
                }
            }
        }
        else
        {
            // 设置急停,光栅 对应的信号灯
            if(m_AlarmLevel < 3 || isFlowEmStop)
            {
                auto ior = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_LIGHT_CONFIG_Map;
                QString id = QString::number(m_AlarmLevel);
                if(ior.contains(id))
                {
                    if(!m_LightMap.contains(m_AlarmLevel))
                    {
                        //qDebug()<<"等级:"<<m_AlarmLevel;
                        flowLightMap.clear();
                        m_LightTimer->stop();
                        m_Mutex->lock();
                        m_LightMap[m_AlarmLevel]=ior[id];
                        m_Mutex->unlock();

                        if(ior[id].name.compare("escape") == 0)
                        {
                            m_ispangle = false;
                            m_LightTimer->start(200);
                        }
                        else if(ior[id].name.compare("slow") == 0)
                        {
                            m_ispangle = false;
                            m_LightTimer->start(500);
                        }
                        else
                        {
                            SetLightStatus(m_AlarmLevel);
                        }
                    }
                }
            }
            else
            {
                auto ior = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_LIGHT_CONFIG_Map;
                QString id = QString::number(m_FlowAlarmLevel);
                if(ior.contains(id))
                {
                    if(!flowLightMap.contains(m_FlowAlarmLevel))
                    {
                        qDebug()<<"长按按钮效果的流程状态:"<<m_FlowAlarmLevel;
                        InitSafeVariable();
                        flowLightMap.clear();
                        flowLightMap.insert(m_FlowAlarmLevel,ior[id]);
                        QMutexLocker lock(m_Mutex);
                        m_LightMap = flowLightMap;
                        m_LightTimer->stop();
                        SetLightStatus(m_FlowAlarmLevel);
                    }
                }

            }
        }
        QThread::msleep(1);
        QApplication::processEvents();
    }
}

void SafeThread::SetTimerForLight()
{
    auto item = m_LightMap.begin();
    auto it = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_THREE_SOFTWARE_Map;

    for(auto ior = it.begin();ior != it.end();++ior)
    {
        if(item.value().light.contains(ior.key()))
        {
            if(!m_ispangle)
            {
                SetNorLightIOValue(ior.value().bitName,ior.value().stat);
            }
            else
            {
                int stat = 0;
                if(stat == ior.value().stat)
                {
                    stat = 1;
                }
                SetNorLightIOValue(ior.value().bitName,stat);
            }
        }
        else
        {
            int ivalue = 0;
            if(ivalue == ior.value().stat)
            {
                ivalue = 1;
            }
            SetNorLightIOValue(ior.value().bitName,ivalue);
        }
    }
    m_ispangle = !m_ispangle;
}
///
/// \brief SafeThread::GetInstance
/// \return 该类的静态指针
/// 实例化该类
SafeThread* SafeThread::GetInstance()
{
    if (!INSTANCE)
    {
        INSTANCE = new SafeThread();
    }

    return INSTANCE;
}
///
/// \brief SafeThread::CloseInstance
///关闭该线程类
void SafeThread::CloseInstance()
{
    m_ExitFlag = true;

    deleteLater();
}
