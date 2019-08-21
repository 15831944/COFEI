#ifndef HANDLEHWXML_H
#define HANDLEHWXML_H
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMap>
#include <QVector>
#include <Comm.h>
class HandleHWXml
{
public:
    HandleHWXml();
    QString ReadHWConfigXml(QString strPath,HWCONFIGXML_MAP &hwConfigXml);
    QString WriteHWConfigXml(QString strPath);
private:
    void ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_INPUT_MAP &cmdxmlMap,HWCONFIGXML_MODEL_MAP modelMap);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_OUTPUT_MAP &cmdxmlMap,HWCONFIGXML_MODEL_MAP modelMap);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_NETWORK_STRU &cmdxmlstru);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_MODEL_MAP &cmdxmlMap);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_COMPORT_STRU> &cmdxmlMap);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_INTNET_STRU> &cmdxmlMap);
    void ReadHWType(QXmlStreamReader &reader, QString strElement,QMap<QString,MQTT_STRU> &cmdxmlMap);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_WEB_MAP &cmdxmlMap);
    void ReadHWType(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_CSYS_CONFIG_STRU> &cmdxmlMap);

    void ReadHWStru(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_IO_STRU &cmdconVec);
    void ReadHWStru(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_SERINPUT_STRU &cmdconVec);
    void ReadHWStru(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_SEROUTPUT_STRU &cmdconVec);
    void ReadHWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_MODEL_STRU> &cmdconMap);

    void ReadBitInfo(QXmlStreamReader &reader,QMap<QString,BITINFO_STRU> &bitInfoVec);
    void ReadByteInfo(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_MODEL_STRU &byteInfoVec);
};

#endif // HANDLEHWXML_H
