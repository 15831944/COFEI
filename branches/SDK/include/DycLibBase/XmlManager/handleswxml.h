#ifndef HANDLESWXML_H
#define HANDLESWXML_H
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMap>
#include <QVector>
#include <Comm.h>
class HandleSWXml
{
public:
    HandleSWXml();
public:
    QString ReadSWConfigXml(QString strPath,SWCONFIGXML_MAP &swConfigXml);
    QString WriteSWConfigXml(QString strPath);
    bool WriteSWSerXml(QString strPath,QMap<QString,SWCONFIGXML_QPOSIT_STRU> ,QMap<QString,SWCONFIGXML_LCSP_STRU>);
private:
    void ReadMsg(QXmlStreamReader &reader,QMap<int,QString> &msgMap);
    void ReadAxisInfo(QXmlStreamReader &reader,QMap<QString,AXISINFO_STRU> &axisMap,int iSpeed);
    void ReadAxisTorque(QXmlStreamReader &reader,QMap<QString,TORQUE_INFO_STRU> &axisMap,int iSpeed);//write by daiqiwei 2016-06-06
    void ReadLcspConfig(QXmlStreamReader &reader,QString strElement,QVector<LCSP_CSYSINFO_STRU> &axisVec);
    void ReadLcspOffset(QXmlStreamReader &reader, QString strElement, QMap<QString,double> &posMap);

    void ReadBitValue(QXmlStreamReader &reader,QString strElement,QVector<BIT_VALUE_STRU> &bitValueVec);
    void ReadORGAxisInfo(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_ORG_AXIS_STRU> &axisMap);
    void ReadORGOP(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_ORG_OP_STRU> &swORGopMap);
    void ReadJSConfig(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_JS_STRU> &swJsMap);

    void ReadSWStru(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_CY_STRU &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_QPOSIT_STRU> &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_LCSP_STRU> &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_QTORQUE_STRU> &cmdconVec);//write by daiqiwei 2016-06-06
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,SON_STRU &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_ORG_STRU &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_JOG_STRU> &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_COMM_STRU &cmdconVec);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_MQTT_DATA_STRU> &cmd_map);//korol 2019/03/07
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_JS_STRU> &cmdconVec);
    //
    void ReadSWType(QXmlStreamReader &reader,QString strElement,DOUBLEBUTTON_MAP &bitValueVec);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_CY_STRU> &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_SER_MAP &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_COMM_STRU> &cmdxmlMap,QMap<QString,SWCONFIGXML_MQTT_DATA_STRU>&mqtt_xml_map);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_SAVEPATH_MAP &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_UPLOAD_MAP &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,IFInfo_STRU> &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,TIMERInfo_STRU> &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_WAIT_STRU> &cmdxmlMap);
    void ReadSWType(QXmlStreamReader &reader,QString strElement,PROTECT_EVENT_MAP &cmdxmlMap);
    //write by dai
    void ReadSWType(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_SAFECON_MAP &safecon_map);
    void ReadSWStru(QXmlStreamReader &reader, QString strElement, QMap<QString,INOUTPUT_STRU> &cmdconMap);
    void ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString, SENSOR_LIGHT_STRU> &cmdconVec);
     void ReadSWStru(QXmlStreamReader &reader, QString strElement, QMap<QString, QMap<QString,SWCONFIGXML_SAFESTATE_STRU> > &cmdconVec);
    //write by adi

    //add by sylar.liu, to read pallet infomation
    void ReadAllPallet(QXmlStreamReader &reader, QString strElement, QMap<QString, PalletInfo> &palletInfo);
    void ReadOnePallet(QXmlStreamReader &reader, QString strElement, PalletInfo &info);

    void ReadPalletName(QXmlStreamReader &reader, QString &name, PalletInfo &info);
    void ReadPalletDimension(QXmlStreamReader &reader, QString strElement, PalletInfo &info);

    void ReadPalletCheckpoint(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadFirstFloor(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadLastFloor(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadEndPosition(QXmlStreamReader &reader, PalletInfo &info);

    void ReadBasePoint(QXmlStreamReader &reader, QString strElement, Position &point);
    void ReadPalletMode(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadPalletRunParams(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadOneParams(QXmlStreamReader &reader, QString strElement, _RunRarams &param);
    void ReadPalletAsix(QXmlStreamReader &reader, QString strElement, PalletInfo &info);

    void ReadPalletEndSignalSettings(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadPalletResetSignalSettings(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
    void ReadOneSignalSetting(QXmlStreamReader &reader, SignalSetting &setting);





//    void ReadPalletOffset(QXmlStreamReader &reader, QString strElement, PalletInfo &info);
};

#endif // HANDLESWXML_H
