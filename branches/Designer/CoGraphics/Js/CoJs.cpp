#include "CoJs.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStandardPaths>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <DycLibBase/XmlManager/xmlmanager.h>
#include <QTableWidget>
JsORGThread::JsORGThread()
{
    m_OrgLib = new PackLib;
    m_OrgResult = 1;
}

JsORGThread::~JsORGThread()
{
    delete m_OrgLib;
    m_OrgLib = NULL;
}

void JsORGThread::run()
{
    m_OrgResult = m_OrgLib->RunAxis();
}
void JsORGThread::SetParam(SWCONFIGXML_ORG_AXIS_STRU orgAxis)
{
    m_OrgLib->SetParam(orgAxis);
}

void JsORGThread::Stop(bool isNor /*= true*/)
{
    m_OrgLib->Stop(isNor);
}

CoJS::CoJS()
{
    m_ORGThread = NULL;
    m_isORG = false;
    SaveXmlIsOK = true;
    m_StausBytePos = -1;
    m_ContrlBytePos = -1;
    m_StausBitInfoMap.clear();
    m_StausByteInfoMap.clear();
    m_ContrlBitInfoMap.clear();
    m_ContrlByteInfoMap.clear();
    m_AxisTimer = new QTimer;
    Init();
}
void CoJS::Init()
{
    if(m_AxisTimer)
    {
        connect(m_AxisTimer,&QTimer::timeout,this,&CoJS::AxisTimer);
        m_AxisTimer->start(300);
    }
    connect(this,&CoJS::SendUpdateXMLEvent,this,&CoJS::UpdateSwSerXML);
}
CoJS::~CoJS()
{
    if(m_ORGThread != NULL)
    {
        m_ORGThread->Stop();
        m_ORGThread->quit();
        m_ORGThread->wait();
        delete m_ORGThread;
    }
    m_ORGThread = NULL;
    m_AxisTimer->stop();
    delete m_AxisTimer;
    m_AxisTimer = NULL;
}

QString CoJS::GetFileContent (QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&file);//读写文本
    return in.readAll();
}

QString CoJS::GetVarValueText(QString varname, QString vartype)
{
    QString result = "";
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_rmutex);
    if(vartype.compare("RegText") == 0)
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.regtextMap.contains(varname))
        {
            result = DBInstanceManager::getDBInstance()->DBVarStru.regtextMap[varname].textval;
        }
        return result;
    }
    else if(vartype.compare("RegNum") == 0)
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(varname))
        {
            result = QString::number(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[varname].Initvalue);
        }
        return result;
    }
    else if(vartype.compare("Cnt") == 0)
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.countMap.contains(varname))
        {
            result = QString::number(DBInstanceManager::getDBInstance()->DBVarStru.countMap[varname].curvalue);
        }
        return result;
    }
    else if(vartype.compare("Time") == 0)
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.countMap.contains(varname))
        {
            result = QString::number(DBInstanceManager::getDBInstance()->DBVarStru.timeMap[varname].curvalue);
        }
        return result;
    }
    else if(vartype.compare("Switch") == 0)
    {
        if(DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap.contains(varname))
        {
            if(!DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[varname].ishwIO)
            {
                result = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[varname].initvalue;
            }
            else {
                QString IOaddr = DBInstanceManager::getDBInstance()->DBVarStru.SwitchMap[varname].IOaddr;
                int value = -1;
                if(IOaddr.contains("E"))
                {
                    uchar uc = 0;
                    if(1 == BaseCalcFun::AnalysisIOString(IOaddr,uc))
                    {
                        if(uc != 0)
                        {
                            uc = 1;
                        }
                        value = uc;
                    }
                }else if(IOaddr.contains("Q"))
                {
                    uchar uc = 0;
                    if(1 == BaseCalcFun::GetOutIOQBitValue(IOaddr,uc))
                    {
                        if(uc != 0)
                        {
                            uc = 1;
                        }
                        value = uc;
                    }
                }
                result = QString::number(value);
            }
        }
        return result;
    }
    return result;
}

bool CoJS::CreateFile(QString filePath, QString strContent)
{
    QFile file(filePath);
    if ( file.open( QIODevice::WriteOnly ) )
    {
        QTextStream stream( &file );
        stream << strContent;
        file.close();
    }
    else
        return false;
    return true;
}
int CoJS::GetAxisCurPos(const QString AxisId)
{
    QString strCurpos = AxisId +tr("_Curpos");
    int pos = -10000000;
    if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strCurpos))
        pos = DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strCurpos].inputvalue;
    return pos;

}
int CoJS::GetAxisCurtq(const QString AxisId)
{
    QString strCurtq = AxisId +tr("_Curtq");
    int tq = -10000000;
    if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strCurtq))
        tq = DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strCurtq].inputvalue;
    return tq;
}
int CoJS::GetAxisErrorCode(const QString AxisId)
{
    QString strError = AxisId +tr("_Errorcode");
    return DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strError].inputvalue;
}
int CoJS::GetAxisFault(const QString AxisId)
{
    QString strFault = AxisId +tr("_Fault");
    return DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strFault].inputvalue;
}
int CoJS::GetAxisReadySw(const QString AxisId)
{
    QString strReady = AxisId +tr("_ReadySw");
    return DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strReady].inputvalue;
}
void CoJS::SetSpeed(const uint speed)
{
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
    DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_SPEED"].Initvalue = speed;
    ConfigData::GetInstance()->iSpeed = speed;
}

void CoJS::SetAcc(const uint acc)
{
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
    DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_ACC"].Initvalue = acc;
    ConfigData::GetInstance()->iAccel = acc;
}

void CoJS::SetDcc(const uint dcc)
{
    QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
    DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_DEC"].Initvalue = dcc;
    ConfigData::GetInstance()->iDccel = dcc;
}

void CoJS::SetRatio(const uchar uc)
{
     QMutexLocker locker(&DBInstanceManager::getDBInstance()->m_mutex);
     DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_SPEEDRATE"].Initvalue = uc;
     ConfigData::GetInstance()->speedRate = uc;
}

bool CoJS::SetAxisSon(const QString strAxis,uchar uc)
{
    m_AxisName = strAxis;
    m_StausBytePos =  MyHW_Share::GetInstance()->AnalysisInPutBytes(strAxis,m_StausBitInfoMap,m_StausByteInfoMap);
    if(m_StausBytePos < 0)
        return false;
    //检查是否有故障
    if(BaseAxisOperate::CheckAxisFault(3,2,m_StausBytePos))
        return false;
    m_ContrlBytePos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(strAxis,m_ContrlBitInfoMap,m_ContrlByteInfoMap);
    if(m_ContrlBytePos < 0)
        return false;
    if(uc == 0)
    {
        BaseAxisOperate::SetAxisExciteOff(MyHW_Share::GetInstance()->m_OFFSON,m_ContrlBytePos);
    }
    else
    {
        if(BaseAxisOperate::CheckAxisExcited(MyHW_Share::GetInstance()->m_INSON,2,m_StausBytePos))
        {
            return true;
        }//增加检查是否励磁的参数,若在励磁状态
        BaseAxisOperate::SetAxisExciteOff(MyHW_Share::GetInstance()->m_OFFSON,m_ContrlBytePos);
        if(m_StausByteInfoMap.contains("Curpos"))
        {
            int curPos = BaseAxisOperate::GetAxisCurPos(m_StausByteInfoMap["Curpos"].strType,
                                                        m_StausByteInfoMap["Curpos"].byteNum,m_StausByteInfoMap["Curpos"].bytePos);
            BaseAxisOperate::SetAxisPos(m_ContrlByteInfoMap["Trapos"].strType,
                                        m_ContrlByteInfoMap["Trapos"].byteNum,m_ContrlByteInfoMap["Trapos"].bytePos,curPos);
        }
        BaseAxisOperate::SetAxisExcite(MyHW_Share::GetInstance()->m_OUTSON,m_ContrlBytePos);
        if(!BaseAxisOperate::CheckAxisExcited(MyHW_Share::GetInstance()->m_INSON,2,m_StausBytePos))
        {
            return false;
        }
    }
    return true;
}

bool CoJS::SetAxisReset(const QString strAxis)
{
    m_AxisName = strAxis;
    m_ContrlBytePos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(strAxis,m_ContrlBitInfoMap,m_ContrlByteInfoMap);
    if(m_ContrlBytePos < 0)
        return false;
    BaseAxisOperate::SetAxisReset(7,m_ContrlBytePos);
    return true;
}

bool CoJS::SetAxisORG(const QString strAxis)
{
     m_AxisName = strAxis;
     m_StausBytePos =  MyHW_Share::GetInstance()->AnalysisInPutBytes(strAxis,m_StausBitInfoMap,m_StausByteInfoMap);
     if(m_StausBytePos < 0)
         return false;
    if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map.contains(strAxis))
    {
        if(!m_isORG)
        {
            m_isORG = true;
            if(m_ORGThread == NULL)
            {
                m_ORGThread = new JsORGThread;
                m_ORGThread->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[strAxis]);
                MyHW_Share::GetInstance()->InsertORGStatus(strAxis,m_ORGThread);
                m_ORGThread->start();
            }
            else
            {
                m_ORGThread->m_OrgResult = 1;
                m_ORGThread->SetParam(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_ORG_Map.swXml_ORG_Axis_Map[strAxis]);
                m_ORGThread->start();
            }
        }
    }
    return true;
}

bool CoJS::SetAxisStop(const QString strAxis)
{
    if(MyHW_Share::GetInstance()->m_AxisORG.contains(strAxis))
    {
        dynamic_cast<JsORGThread*>(MyHW_Share::GetInstance()->m_AxisORG[strAxis])->Stop();
    }
    else
    {
        m_ContrlBytePos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(strAxis,m_ContrlBitInfoMap,m_ContrlByteInfoMap);
        if(m_ContrlBytePos < 0)
            return false;
        BaseAxisOperate::SetAxisStop(2,m_ContrlBytePos);
    }
    return true;
}

bool CoJS::WriteSWXml()
{
    if(SaveXmlIsOK)
    {
        SaveXmlIsOK = false;
        QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp1;
        QMap<QString,SWCONFIGXML_LCSP_STRU> temp2;
        for(auto it = DBInstanceManager::getDBInstance()->DBVarStru.qpMap.begin();
            it != DBInstanceManager::getDBInstance()->DBVarStru.qpMap.end(); ++it)
        {
            SWCONFIGXML_QPOSIT_STRU QPstru;
            QPstru.id = it.value().pkey;
            QPstru.note = it.value().note;
            QPstru.acc = it.value().acc;
            QPstru.dec = it.value().dec;
            QPstru.timeOut = it.value().timeOut;
            QPstru.speed = 100;
            QPstru.delayMsec = it.value().delay;
            QStringList list = QString(it.value().axisidinfo).split(";");
            QPstru.axisNum = list.size();
            foreach (QString str, list) {
                if(str.isEmpty() || !str.contains(":"))
                    continue;
                QStringList list = str.split(":");
                AXISINFO_STRU axisStru;
                QStringList list1 = list[1].split(",");
                if(list1.size() > 1)
                    axisStru.speed = QString(list1[1]).toFloat();
                else
                    axisStru.speed = 100;
                if(list1.size() > 2)
                {
                    if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(list1[2]))
                    {
                      axisStru.offsetPos = list1[2];
                    }
                }
                if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(list1[0]))
                    axisStru.pos = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[list1[0]].Initvalue;
                QPstru.axisMap.insert(list[0],axisStru);
            }
            temp1.insert(it.key(),QPstru);
        }
//        for(auto it = DBInstanceManager::getDBInstance()->DBVarStru.cspMap.begin();
//            it != DBInstanceManager::getDBInstance()->DBVarStru.cspMap.end(); ++it)
//        {
//            SWCONFIGXML_LCSP_STRU CSPstru;
//            CSPstru.id = "";
//            CSPstru.note = "";
//            CSPstru.speed = -1;
//            CSPstru.acc = -1;
//            CSPstru.timeOut = -1;
//            CSPstru.delayMsec = -1;
//            QVector<LCSP_CSYSINFO_STRU> LCSPCPos;
//            QMap<QString ,double> LCSPOffsetPosMap;//增加坐标偏移位置--------2017-11-24
//            temp2.insert(it.key(),CSPstru);

//        }
        emit SendUpdateXMLEvent(temp1,temp2);
    }
    return true;
}
void CoJS::UpdateSwSerXML(QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp,QMap<QString,SWCONFIGXML_LCSP_STRU> temp2)
{
    XmlManager xml;
    QString path=QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹"+"/参数配置/软件参数配置.xml";
    xml.WriteSWSerXml(path,temp,temp2);
    ConfigData::GetInstance()->m_mutex.lock();
    ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map = temp;
    ConfigData::GetInstance()->m_mutex.unlock();
//    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
//                                                      DBInstanceManager::getDBInstance()->DBVarStru.qpMap);
//    DBInstanceManager::getDBInstance()->WriteDataToDB(tr("Variablemanagement"),
//                                                      DBInstanceManager::getDBInstance()->DBVarStru.cspMap);
    SaveXmlIsOK = true;
}
bool CoJS::SetAxisRun(const QString strAxis,int Trapos)
{
     m_AxisName = strAxis;
     m_StausBytePos =  MyHW_Share::GetInstance()->AnalysisInPutBytes(strAxis,m_StausBitInfoMap,m_StausByteInfoMap);
     if(m_StausBytePos < 0)
         return false;
    m_ContrlBytePos = MyHW_Share::GetInstance()->AnalysisOutPutBytes(strAxis,m_ContrlBitInfoMap,m_ContrlByteInfoMap);
    if(m_ContrlBytePos < 0)
        return false;
    uint Traspeed = ConfigData::GetInstance()->iSpeed * ConfigData::GetInstance()->speedRate/200.0 ;
    uint accel = ConfigData::GetInstance()->iAccel * ConfigData::GetInstance()->speedRate/200.0 ;
    uint dccel = ConfigData::GetInstance()->iDccel * ConfigData::GetInstance()->speedRate/200.0 ;
    Traspeed *=  ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strAxis].lead;
    dccel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strAxis].adled;
    accel *= ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[strAxis].adled;
    if(m_ContrlByteInfoMap.contains("Trapos"))
    {
        BaseAxisOperate::SetAxisPos(m_ContrlByteInfoMap["Trapos"].strType,m_ContrlByteInfoMap["Trapos"].byteNum,
                m_ContrlByteInfoMap["Trapos"].bytePos,Trapos);
    }
    if(m_ContrlByteInfoMap.contains("Tratq"))
    {
        BaseAxisOperate::SetAxisPos(m_ContrlByteInfoMap["Tratq"].strType,m_ContrlByteInfoMap["Tratq"].byteNum,
                m_ContrlByteInfoMap["Tratq"].bytePos,Trapos*10);
    }
    if(m_ContrlByteInfoMap.contains("Traspeed"))
    {
        BaseAxisOperate::SetAxisSpeed(m_ContrlByteInfoMap["Traspeed"].strType,m_ContrlByteInfoMap["Traspeed"].byteNum,
                m_ContrlByteInfoMap["Traspeed"].bytePos,Traspeed);
    }
    if(m_ContrlByteInfoMap.contains("Accel"))
    {
        BaseAxisOperate::SetAxisACC(m_ContrlByteInfoMap["Accel"].strType,m_ContrlByteInfoMap["Accel"].byteNum,
                m_ContrlByteInfoMap["Accel"].bytePos,accel);
    }
    if(m_ContrlByteInfoMap.contains("Decel"))
    {
        BaseAxisOperate::SetAxisDCC(m_ContrlByteInfoMap["Decel"].strType,m_ContrlByteInfoMap["Decel"].byteNum,
                m_ContrlByteInfoMap["Decel"].bytePos,dccel);
    }
    if(m_ContrlBitInfoMap.contains("Run"))
    {
        BaseAxisOperate::SetAxisRun(m_ContrlBitInfoMap["Run"].bit,m_ContrlBytePos);
    }
    return true;
}
void CoJS::AxisTimer()
{
    if(m_ORGThread != NULL &&m_ORGThread->m_OrgResult != 1)
    {
         m_isORG = false;
    }
    if(m_StausBitInfoMap.contains("Fault"))
    {
        if(BaseAxisOperate::CheckAxisFault(m_StausBitInfoMap["Fault"].bit,2,m_StausBytePos))
        {
            SetAxisStop(m_AxisName);
        }
    }
}
