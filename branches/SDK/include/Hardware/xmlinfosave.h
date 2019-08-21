#ifndef XMLINFOSAVE_H
#define XMLINFOSAVE_H

#include <QObject>
#include <QDataStream>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QFile>
#include <Comm.h>
#include <QDomElement>
#include <QTextCodec>
#include "addcoordinate.h"

class XMLInfoSave
{
public:
    XMLInfoSave();
private:
    bool FileOpen(QString path);
public:
    void CreateXmlRoot(QString path);
    //axis
    bool SaveInfoXML(HWCONFIGXML_SEROUTPUT_STRU axisinfo,QString path);
    bool InsertAxisXML(HWCONFIGXML_SEROUTPUT_STRU axisinfo,QString FirstElement,QString ID,int State,QString path);
    bool DeleteInfoXML(QString FirstElement,QString TagAtirtname,QString ID,int State,QString path);
    //io
    bool SaveIoXML(HWCONFIGXML_IO_STRU ioinfo,QString path);
    bool InsetIoXML(HWCONFIGXML_IO_STRU ioinfo,QString oldID ,int State,QString path);
    bool DeleteIoXML(HWCONFIGXML_IO_STRU ioinfo);
    //serial
    bool SaveSerialXML(HWCONFIGXML_COMPORT_STRU serialinfo,QString path);
    bool DeleteSerialXML(HWCONFIGXML_COMPORT_STRU serialinfo);
    //tcpip
    bool SaveTcpXML(HWCONFIGXML_INTNET_STRU tcpinfo,QString path);
    bool DeleteTcpXML(HWCONFIGXML_INTNET_STRU tcpinfo);
    //关节信息
    bool SaveAirXML(HWCONFIGXML_ARMINFO_STRU arminfo,QString path);
    //坐标信息
    bool SaveCoordinateXML(COORDINATESTUR coorinfo,QString path, QMap<QString,HWCONFIGXML_ARMINFO_STRU>ArmMap);
    void AddElement(QXmlStreamWriter *wr,QString mode);
    void AddTorqueElement(QXmlStreamWriter *wr);
    //增加硬件增加记录轴--IO排序
    bool SaveAxisAndIoOrder(QString str,QString path);
    bool DeleteAxisAndIoOrder(QString str,QString insetstr,QString path);
    QVector<QString>GetAxisAndIoOder(QString path);
    bool FixAxisAndIoOrder(QString oldName,QString newName,QString path);
    //删除OUTPUT INPUT下面的所有节点
    bool DeleteAllInputOutput(QString path);

    bool SaveNetInfo(QString name,QString path);
};

#endif // XMLINFOSAVE_H
