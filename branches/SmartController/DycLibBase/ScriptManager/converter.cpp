#include "converter.h"
#include <QFile>
#include <QTextStream>
#include <MyHW_Share.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
Converter::Converter()
{

}

QString Converter::GetFileContent (QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream in(&file);//读写文本
    return in.readAll();
}

bool Converter::CreateFile(QString filePath, QString strContent)
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
int Converter::GetAxisCurPos(const QString AxisId)
{
    int curPos = -10000000;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.find(AxisId);
    if(item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end())
    {
        int ibytePos = item.value().iBytePos;
        QString ModelType = item.value().modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
            for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;
                if(it->key == "Curpos")
                {
                    curPos = BaseAxisOperate::GetAxisCurPos(it->type,ibyteNum,ibytePos);
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return curPos;
}
int Converter::GetAxisCurtq(const QString AxisId)
{
    int tq = -10000000;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.find(AxisId);
    if(item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end())
    {
        int ibytePos = item.value().iBytePos;
        QString ModelType = item.value().modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
            for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;
                if(it->key == "Curtq")
                {
                    tq = BaseAxisOperate::GetAxisCurPos(it->type,ibyteNum,ibytePos);//获取实时扭矩
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return tq;
}
int Converter::GetAxisErrorCode(const QString AxisId)
{
    int errorCode = 0;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.find(AxisId);
    if(item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end())
    {
        int ibytePos = item.value().iBytePos;
        QString ModelType = item.value().modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
            for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;
                if(it->key == "Errorcode")
                {
                    errorCode = BaseAxisOperate::GetAxisErrorCode(it->type,ibyteNum,ibytePos);
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return errorCode;
}
int Converter::GetAxisFault(const QString AxisId)
{
    int iFault = 0;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.find(AxisId);
    if(item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end())
    {
        int ibytePos = item.value().iBytePos;
        QString ModelType = item.value().modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
            for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;
                if(it->key == "Statusworld")
                {
                    iFault =  BaseAxisOperate::CheckAxisFault(3,ibyteNum,ibytePos);
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return iFault;
}
int Converter::GetAxisReadySw(const QString AxisId)
{
    int iReady = 0;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.find(AxisId);
    if(item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end())
    {
        int ibytePos = item.value().iBytePos;
        QString ModelType = item.value().modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
            for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;
                if(it->key == "Statusworld")
                {
                    iReady = BaseCalcFun::GetBitValue(0,ibyteNum,ibytePos);
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return iReady;
}
///
/// \brief Converter::GetVarValueText
/// \param varname 变量的名字
/// \param vartype变量的类型寄存器类型 计数类 开关类型
/// \return 变量的的值字符串形式
///
QString Converter::GetVarValueText(QString varname, QString vartype)
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
