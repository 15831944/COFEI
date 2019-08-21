#include "analysishwconfig.h"

int AnalysisHwConfig::GetContrlWorldPos(const QString &Key,QMap<QString,BITINFO_STRU> &bitInfoMap,QMap<QString,BYTEINFO> &byteInfoMap)
{
    int cwBytePos = -1;
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(Key))
    {
        auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[Key];
        int ibytePos = item.iBytePos;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(item.modelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[item.modelType]["OUTPUT"].byteInfoVec.begin();
            for(; it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[item.modelType]["OUTPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;

                if(it->key == "Contrlworld")
                {
                    bitInfoMap = it->bitInfoMap;
                    cwBytePos = ibytePos;
                }
                else if(it->key == "Model")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                else if(it->key == "Trapos")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                else if(it->key == "Traspeed")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                else if(it->key == "Accel")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                else if(it->key == "Decel")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return cwBytePos;
}
int AnalysisHwConfig::GetStatusWorldPos(const QString &strKey, QMap<QString, BITINFO_STRU> &bitInfoMap,QMap<QString,BYTEINFO> &byteInfoMap)
{
    int swBytePos = -1;
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
    {
        int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
        QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
            for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
            {
                int ibyteNum = it->byteNum;
                if(it->key == "Statusworld")
                {
                    bitInfoMap = it->bitInfoMap;
                    swBytePos = ibytePos;
                }
                else if(it->key == "Errorcode")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                else if(it->key == "Curpos")
                {
                    BYTEINFO  byteInfo;
                    byteInfo.strType = it->type;
                    byteInfo.byteNum = ibyteNum;
                    byteInfo.bytePos = ibytePos;
                    byteInfoMap.insert(it->key,byteInfo);
                }
                ibytePos += ibyteNum;
            }
        }
    }
    return swBytePos;
}
