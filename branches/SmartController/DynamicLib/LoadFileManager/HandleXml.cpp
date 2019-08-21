#include "HandleXml.h"
#include <QDebug>
#include <QStandardPaths>
#include <QMessageBox>
#include <DycLibBase/LogManager/logmanager.h>
HandleXml::HandleXml(QObject *parent) : QObject(parent)
{
    m_isAnalsUIXml = false;
}
HandleXml::~HandleXml()
{
    if(InOutPutData::GetInstance()->outPutData != NULL)
        delete[] InOutPutData::GetInstance()->outPutData;
    if(InOutPutData::GetInstance()->inPutData != NULL)
        delete[] InOutPutData::GetInstance()->inPutData;
    InOutPutData::GetInstance()->outPutData = NULL;
    InOutPutData::GetInstance()->inPutData = NULL;
}
bool HandleXml::DeleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
        {
            DeleteDirectory(fi.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}

void HandleXml::SetAnalsUIXml(bool isAnals)
{
    m_isAnalsUIXml = isAnals;
}
void HandleXml::FaultReset()
{

}
void HandleXml::AnalysXml()
{
    XmlManager xml;
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    QString flowPath = path + "/运行配置/流程";
    QString str = xml.ReadFlowPath(flowPath,ConfigData::GetInstance()->xmlMap.flowTxt);
    if(!str.isEmpty())
        CoreLog::QLog_Warning(defaultLogModule,str);
    QString ModifPath = path + "/运行配置/总复位";
    str = xml.ReadResetPath(ModifPath,ConfigData::GetInstance()->xmlMap.flowTxt);
    if(!str.isEmpty())
        CoreLog::QLog_Warning(defaultLogModule,str);
    QString orgPath = path + "/运行配置/回原点";;
    str = xml.ReadORGPath(orgPath,ConfigData::GetInstance()->xmlMap.flowTxt);
    if(!str.isEmpty())
        CoreLog::QLog_Warning(defaultLogModule,str);
    QString ConfigPath = path+"/参数配置/软件参数配置.xml";
    str = xml.ReadSWConfigXml(ConfigPath,ConfigData::GetInstance()->xmlMap.swConfigXml);
    if(!str.isEmpty())
    {
        CoreLog::QLog_Error(defaultLogModule,str);
        QMessageBox::warning(NULL,QString("提示"),str);
        return;
    }
    if(m_isAnalsUIXml)
    {
        ConfigPath = path+"/参数配置/UI配置.xml";
        str = xml.ReadUIXml(ConfigPath,ConfigData::GetInstance()->xmlMap.uiXml);
        if(!str.isEmpty())
            CoreLog::QLog_Error(defaultLogModule,str);
    }
    ConfigPath = path+"/参数配置/硬件配置.xml";
    str = xml.ReadHWConfigXml(ConfigPath,ConfigData::GetInstance()->xmlMap.hwConfigXml);
    if(!str.isEmpty())
        CoreLog::QLog_Error(defaultLogModule,str);
    int iNum = GetByteNumber<QMap<QString,HWCONFIGXML_IO_STRU>,QMap<QString,HWCONFIGXML_SERINPUT_STRU>>(
                ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwIOMap,
                ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap);
    if(iNum > 0)
    {
        InOutPutData::GetInstance()->inPutNum = iNum;
        InOutPutData::GetInstance()->inPutData = new uchar[iNum];
        memset(InOutPutData::GetInstance()->inPutData,0,iNum);
    }

    iNum = GetByteNumber<QMap<QString,HWCONFIGXML_IO_STRU>,QMap<QString,HWCONFIGXML_SEROUTPUT_STRU>>(
                ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwIOMap,
                ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap);
    if(iNum > 0)
    {
        InOutPutData::GetInstance()->outPutNum = iNum;
        InOutPutData::GetInstance()->outPutData = new uchar[iNum];
        memset(InOutPutData::GetInstance()->outPutData,0,iNum);
    }
}
template<typename T>
void HandleXml::InitIOData(T t1,int inum)
{
    for(int i = 0; i < inum;++i)
    {
        qDebug() << t1[i];
    }
}

template<typename T,typename T2>
int HandleXml::GetByteNumber(T t1,T2 t2)
{
    int iNum = 0;
    for(auto it = t1.begin(); it != t1.end();it++)
    {
        if(it.value().strType == "EC")
            iNum += it.value().iByteNum;
        ConfigData::GetInstance()->hwIOMap.insert(it.key(),it.value());
    }
    for(auto it2 = t2.begin(); it2 != t2.end();it2++)
    {
        iNum += it2.value().iByteNum;

    }
    return iNum;
}
