#include "handleswxml.h"
#include <QtXml/QDomDocument>
#include <QTextCodec>
#include <QDebug>
HandleSWXml::HandleSWXml()
{

}
QString HandleSWXml::ReadSWConfigXml(QString strPath,SWCONFIGXML_MAP &swConfigXml)
{
    QString msg("");

    QFile file(strPath);
    if(!file.exists())
    {
        msg = QString("%1 is not exists!").arg(strPath);
    }

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString text = stream.readAll();
        QDomDocument doc;
        QString errorMsg = "";
        int errorRow = -1;
        int errorColumn = -1;
        if(!doc.setContent(text,&errorMsg,&errorRow,&errorColumn))
        {
            msg = QString("%1\nXML 解析错误！\nError line: %2").arg(strPath).arg(errorRow);
            file.close();
            return msg;
        }
        file.close();
    }

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //构建QXmlStreamReader对象
        QXmlStreamReader reader(&file);
        while(!reader.atEnd())
        {
            reader.readNext();

            //判断是否是节点的开始
            if(reader.isStartElement())
            {
                //判断当前节点的名字是否为汽缸集合
                if(reader.name() == "汽缸集合")
                {
                    ReadSWType(reader,"汽缸集合",swConfigXml.swXml_CY_Map);
                }
                if(reader.name() == "数据保存")
                {
                    ReadSWType(reader,"数据保存",swConfigXml.swXml_SavePath_Map);
                }
                if(reader.name() == "数据上传")
                {
                    ReadSWType(reader,"数据上传",swConfigXml.swXml_Upload_Map);
                }
                if(reader.name() == "条件判断")
                {
                    ReadSWType(reader,"条件判断",swConfigXml.swXml_IF_Map);
                }
                if(reader.name() == "保护事件配置")
                {
                    ReadSWType(reader,"保护事件配置",swConfigXml.swXml_ProtectEvent_Map);
                }
                if(reader.name() == "计时集合")
                {
                    ReadSWType(reader,"计时集合",swConfigXml.swXml_TIMER_Map);
                }
                if(reader.name() == "等待指令集合")
                {
                    ReadSWType(reader,"等待指令集合",swConfigXml.swXml_Wait_Map);
                }
                if(reader.name() == "通信部分")
                {
                    ReadSWType(reader,"通信部分",swConfigXml.swXml_COMM_Map,swConfigXml.swXml_MQTT_Map);
                }
                if(reader.name() == "运动部分")
                {
                    ReadSWType(reader,"运动部分",swConfigXml.swXml_SER_Map);
                }
                if(reader.name() == "启动按钮配置")
                {
                    ReadSWType(reader,"启动按钮配置",swConfigXml.swXml_DOUBLEBUTTON_Map);
                }
                if(reader.name() == "安全配置")
                {
                    ReadSWType(reader,"安全配置",swConfigXml.swXml_SAFETYCONFIG_Map);
                }
                if (reader.name() == "托盘指令")
                {
                    ReadAllPallet(reader, "托盘指令", swConfigXml.swXml_PalletInfo);
                }
            }
            //节点结束、并且节点名字为Data（含有子节点）
            else if(reader.isEndElement() && reader.name() == "Data")
            {
                break;
            }
        }
        file.close();
    }
    else
    {
        msg = QString("Open file %1 failure").arg(strPath);
    }

    return msg;
}
void HandleSWXml::ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_COMM_STRU> &cmdxmlMap,QMap<QString,SWCONFIGXML_MQTT_DATA_STRU>&mqtt_xml_map)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "串口通信")
            {
                SWCONFIGXML_COMM_STRU commdata;
                ReadSWStru(reader,"串口通信",commdata);
                if(!cmdxmlMap.contains("串口通信"))
                    cmdxmlMap.insert("串口通信",commdata);
            }
            else if(reader.name() == "网络通信")
            {
                SWCONFIGXML_COMM_STRU commdata;
                ReadSWStru(reader,"网络通信",commdata);
                if(!cmdxmlMap.contains("网络通信"))
                    cmdxmlMap.insert("网络通信",commdata);
            }
            else if(reader.name() == "MQtt通讯")
            {
                //SWCONFIGXML_MQTT_DATA_STRU mqttdata;
                ReadSWStru(reader,"MQtt通讯",mqtt_xml_map);
//                if(!mqtt_xml_map.contains(mqttdata.id))
//                {
//                    mqtt_xml_map.insert(mqttdata.id,mqttdata);
//                }

            }

        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWType(QXmlStreamReader &reader, QString strElement, SWCONFIGXML_SAVEPATH_MAP &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "保存路径")
            {
                SWCONFIGXML_SAVEPATH_STRU commdata;
                QXmlStreamAttributes attributes = reader.attributes();
                //判断是否存在属性"bit,vaule"
                if(attributes.hasAttribute("key"))
                {
                    commdata.key = attributes.value("key").toString();
                }
                if(attributes.hasAttribute("address"))
                {
                    commdata.address = attributes.value("address").toString();
                }
                cmdxmlMap.savePathVec.append(commdata);
            }

        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWType(QXmlStreamReader &reader, QString strElement, SWCONFIGXML_UPLOAD_MAP &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "通讯数据")
            {
                SWCONFIGXML_UPLOAD_STRU commdata;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            commdata.Id = reader.readElementText();
                        }
                        else if(reader.name() == "数据回复")
                        {
                            commdata.dataRecovery = reader.readElementText();
                        }
                        else if(reader.name() == "数据采集")
                        {
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "数据保存")
                                    {
                                        commdata.dataSetKeyVec.append(reader.readElementText());
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "数据采集")
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "通讯数据")
                        {
                            break;
                        }
                    }
                }
                if(!cmdxmlMap.swXmlUpLoadMap.contains(commdata.Id))
                {
                    cmdxmlMap.swXmlUpLoadMap.insert(commdata.Id,commdata);
                }
            }


        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWType(QXmlStreamReader &reader, QString strElement, QMap<QString, IFInfo_STRU> &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "条件信息")
            {
                IFInfo_STRU commdata;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            commdata.id = reader.readElementText();
                        }
                        else if(reader.name() == "条件类型")
                        {
                            QXmlStreamAttributes attributes = reader.attributes();
                            //判断是否存在属性"type,funName"
                            if(attributes.hasAttribute("type"))
                            {
                                commdata.type = attributes.value("type").toString();
                            }
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "信息")
                                    {
                                        commdata.strEvent = reader.readElementText();
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "条件类型")
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "条件信息")
                        {
                            break;
                        }
                    }
                }
                if(!cmdxmlMap.contains(commdata.id))
                {
                    cmdxmlMap.insert(commdata.id,commdata);
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_COMM_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "通信数据")
            {
                SWCONFIGXML_COMM_DATA_STRU commdata;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            commdata.id = reader.readElementText();
                        }
                        else if(reader.name() == "串口编号" || reader.name() == "网络编号")
                        {
                            commdata.commId = reader.readElementText();
                        }
                        else if(reader.name() == "数据")
                        {
                            commdata.data = reader.readElementText();
                        }
                        else if(reader.name() == "数据保存")
                        {
                            commdata.savePathKey = reader.readElementText();
                        }
                        else if (reader.name() =="IP地址")
                        {
                            commdata.Ip = reader.readElementText();
                        }
                        else if (reader.name() =="数据格式")
                        {
                            commdata.style = reader.readElementText();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "通信数据")
                        {
                            break;
                        }
                    }
                }
                if(!cmdconVec.commDataMap.contains(commdata.id))
                {
                    cmdconVec.commDataMap.insert(commdata.id,commdata);
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }

}

void HandleSWXml::ReadSWStru(QXmlStreamReader &reader, QString strElement, QMap<QString,SWCONFIGXML_MQTT_DATA_STRU> &cmd_map)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "通信数据")
            {
                SWCONFIGXML_MQTT_DATA_STRU commdata;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "ID")
                        {
                            commdata.id = reader.readElementText();
                        }
                        else if(reader.name() == "MQtt编号")
                        {
                            commdata.clinet_id = reader.readElementText();
                        }
                        else if(reader.name() == "主题")
                        {
                            commdata.topical = reader.readElementText();
                        }
                        else if(reader.name() == "数据保存")
                        {
                            commdata.data_save = reader.readElementText();
                        }
                        else if (reader.name() =="数据")
                        {
                            commdata.data_contain = reader.readElementText();
                        }
                        else if (reader.name() =="超时")
                        {
                            commdata.timeout = reader.readElementText();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "通信数据")
                        {
                            break;
                        }
                    }
                }
                if(!cmd_map.contains(commdata.id))
                {
                    cmd_map.insert(commdata.id,commdata);
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_CY_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "编号")
            {
                cmdconVec.id = reader.readElementText();
            }
            else if(reader.name() == "软元件集合")
            {
                //todo
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "软元件")
                        {
                            cmdconVec.soltCompVec.append(reader.readElementText());
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "软元件集合")
                        {
                            break;
                        }
                    }
                }
            }
            else if(reader.name() == "原点")
            {
                //todo
                cmdconVec.basePoint = reader.readElementText();
            }
            else if(reader.name() == "动点")
            {
                //todo
                cmdconVec.movePoint = reader.readElementText();
            }
            else if(reader.name() == "超时")
            {
                //todo
                cmdconVec.timeOut = reader.readElementText().toInt();
            }
            else if(reader.name() == "单步延时")
            {
                //todo
                cmdconVec.delayMsec = reader.readElementText().toInt();
            }
            else if(reader.name() == "信息集合")
            {
                //todo
                ReadMsg(reader,cmdconVec.MsgMap);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadBitValue(QXmlStreamReader &reader,QString strElement,QVector<BIT_VALUE_STRU> &bitValueVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "比特")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                BIT_VALUE_STRU bitvalue;
                //判断是否存在属性"bit,vaule"
                if(attributes.hasAttribute("bit"))
                {
                    bitvalue.bit = attributes.value("bit").toInt();
                }
                if(attributes.hasAttribute("vaule"))
                {
                    bitvalue.value = attributes.value("vaule").toInt();
                }

                bitValueVec.append(bitvalue);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_JS_STRU> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "动态脚本")
            {
                ReadJSConfig(reader,"动态脚本",cmdconVec);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadJSConfig(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_JS_STRU> &swJsMap)
{
    SWCONFIGXML_JS_STRU swJsStru;
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "编号")
            {
                swJsStru.id = reader.readElementText();
            }
            else if(reader.name() == "脚本功能名")
            {
                swJsStru.funtionName = reader.readElementText();
            }
            else if(reader.name() == "数据保存")
            {
                swJsStru.savePathKey = reader.readElementText();
            }
            else if(reader.name() == "偏移对象")
            {
                QXmlStreamAttributes attributes = reader.attributes();

                //判断是否存在属性"bit,vaule"
                if(attributes.hasAttribute("type"))
                {
                    swJsStru.offsetObject.type = attributes.value("type").toString();
                }
                if(attributes.hasAttribute("Id"))
                {
                    swJsStru.offsetObject.id = attributes.value("Id").toString();
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                if(!swJsMap.contains(swJsStru.id))
                {
                    swJsMap.insert(swJsStru.id,swJsStru);
                }
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,SON_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "输出")
            {
                ReadBitValue(reader,"输出",cmdconVec.OutPutVec);
            }
            else if(reader.name() == "输入")
            {
                ReadBitValue(reader,"输入",cmdconVec.InPutVec);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_QPOSIT_STRU> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "快速定位信息")
            {
                SWCONFIGXML_QPOSIT_STRU swQP;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            swQP.id = reader.readElementText();
                        }
                        else if(reader.name() == "注释")
                        {
                            swQP.note = reader.readElementText();
                        }
                        else if(reader.name() == "速度百分率")
                        {
                            //todo
                            swQP.speed = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "加速")
                        {
                            //todo
                            swQP.acc = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "减速")
                        {
                            //todo
                            swQP.dec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "轴数")
                        {
                            //todo
                            swQP.axisNum = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "超时")
                        {
                            //todo
                            swQP.timeOut = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "单步延时")
                        {
                            //todo
                            swQP.delayMsec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "轴集合")
                        {
                            //todo
                            ReadAxisInfo(reader,swQP.axisMap,swQP.speed);
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "快速定位信息")
                        {
                            break;
                        }
                    }
                }
                if(!cmdconVec.contains(swQP.id))
                {
                    cmdconVec.insert(swQP.id,swQP);
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWStru(QXmlStreamReader &reader, QString strElement, QMap<QString, SWCONFIGXML_LCSP_STRU> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "插补信息")
            {
                SWCONFIGXML_LCSP_STRU swLcsp;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            swLcsp.id = reader.readElementText();
                        }
                        else if(reader.name() == "注释")
                        {
                            swLcsp.note = reader.readElementText();
                        }
                        else if(reader.name() == "速度")
                        {
                            //todo
                            swLcsp.speed = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "加速")
                        {
                            //todo
                            swLcsp.acc = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "超时")
                        {
                            //todo
                            swLcsp.timeOut = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "单步延时")
                        {
                            //todo
                            swLcsp.delayMsec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "直线位置")
                        {
                            //todo
                            ReadLcspConfig(reader,"直线位置",swLcsp.LCSPCPos);
                        }
                        else if(reader.name() == "坐标位置偏移信息")
                        {
                            //todo
                            ReadLcspOffset(reader,"坐标位置偏移信息",swLcsp.LCSPOffsetPosMap);
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "插补信息")
                        {
                            break;
                        }
                    }
                }
                if(!cmdconVec.contains(swLcsp.id))
                {
                    cmdconVec.insert(swLcsp.id,swLcsp);
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadLcspOffset(QXmlStreamReader &reader,QString strElement,QMap<QString,double> &posMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "X坐标")
            {
                 double d = reader.readElementText().toDouble();
                 posMap.insert("X坐标",d);
            }
            else if(reader.name() == "Y坐标")
            {
                double d = reader.readElementText().toDouble();
                posMap.insert("Y坐标",d);
            }
            else if(reader.name() == "Z坐标")
            {
                double d = reader.readElementText().toDouble();
                posMap.insert("Z坐标",d);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
//扭矩模式节点解析　  write by daiqiwei 2016-06-06
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_QTORQUE_STRU> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "扭矩信息")
            {
                //SWCONFIGXML_QPOSIT_STRU swQP;
                SWCONFIGXML_QTORQUE_STRU swQP;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            swQP.id = reader.readElementText();
                        }
                        else if(reader.name() == "注释")
                        {
                            swQP.note = reader.readElementText();
                        }
                        else if(reader.name() == "速度百分率")
                        {
                            //todo
                            swQP.speed = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "加速")
                        {
                            //todo
                            swQP.acc = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "减速")
                        {
                            //todo
                            swQP.dec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "轴数")
                        {
                            //todo
                            swQP.axisNum = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "超时")
                        {
                            //todo
                            swQP.timeOut = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "单步延时")
                        {
                            //todo
                            swQP.delayMsec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "轴集合")
                        {
                            //todo
                            ReadAxisTorque(reader,swQP.torqueMap,swQP.speed);
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "扭矩信息")
                        {
                            break;
                        }
                    }
                }
                if(!cmdconVec.contains(swQP.id))
                {
                    cmdconVec.insert(swQP.id,swQP);
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadMsg(QXmlStreamReader &reader,QMap<int,QString> &msgMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "信息")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                int ikey = -99;
                QString strText = "";
                //判断是否存在属性"key"
                if(attributes.hasAttribute("key"))
                {
                    ikey = attributes.value("key").toInt();
                }
                if(attributes.hasAttribute("内容"))
                {
                    strText = attributes.value("内容").toString();
                }
                //内容
                if(!msgMap.contains(ikey))
                    msgMap.insert(ikey,strText);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == "信息集合")
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadAxisInfo(QXmlStreamReader &reader,QMap<QString,AXISINFO_STRU> &axisMap,int iSpeed)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "轴信息")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                QString strId = "";
                //判断是否存在属性"Id"
                if(attributes.hasAttribute("Id"))
                {
                    strId = attributes.value("Id").toString();
                }
                AXISINFO_STRU axisInfo;
                axisInfo.speed = iSpeed;
                while(!reader.atEnd())
                {
                    reader.readNext();

                    if(reader.isStartElement())
                    {
                        if(reader.name() == "位置")
                        {
                            axisInfo.pos = reader.readElementText().toDouble();
                        }
                        else if(reader.name() == "速度百分率")
                        {
                            axisInfo.speed = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "位置偏移量")
                        {
                            axisInfo.offsetPos = reader.readElementText().toDouble();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "轴信息")
                        {
                            break;
                        }
                    }
                }

                if(!axisMap.contains(strId))
                    axisMap.insert(strId,axisInfo);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == "轴集合")
            {
                break;
            }
        }
    }
}
//扭矩信息节点的“轴集合”节点解析　　write by daiqiwei 2016-06-06
void HandleSWXml::ReadAxisTorque(QXmlStreamReader &reader,QMap<QString,TORQUE_INFO_STRU> &axisMap,int iSpeed)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "轴信息")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                QString strId = "";
                //判断是否存在属性"Id"
                if(attributes.hasAttribute("Id"))
                {
                    strId = attributes.value("Id").toString();
                }
                //AXISINFO_STRU axisInfo;
                TORQUE_INFO_STRU axisInfo;
                axisInfo.speed = iSpeed;
                while(!reader.atEnd())
                {
                    reader.readNext();

                    if(reader.isStartElement())
                    {
                        if(reader.name() == "扭矩")
                        {
                            axisInfo.torque_value= reader.readElementText().toDouble();
                        }
                        else if(reader.name() == "速度百分率")
                        {
                            axisInfo.speed= reader.readElementText().toInt();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "轴信息")
                        {
                            break;
                        }
                    }
                }

                if(!axisMap.contains(strId))
                    axisMap.insert(strId,axisInfo);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == "轴集合")
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadLcspConfig(QXmlStreamReader &reader, QString strElement, QVector<LCSP_CSYSINFO_STRU> &axisVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "轴集合")
            {
                LCSP_CSYSINFO_STRU lcspInfo;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "触发信息")
                        {
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "触发集合")
                                    {
                                        LCSP_TRIGGERINFO_STRU LcsptgInfo;
                                        while(!reader.atEnd())
                                        {
                                            reader.readNext();
                                            if(reader.isStartElement())
                                            {
                                                if(reader.name() == "触发I0信息")
                                                {
                                                    QXmlStreamAttributes attributes = reader.attributes();
                                                    if(attributes.hasAttribute("bit"))
                                                    {
                                                        LcsptgInfo.bitName = attributes.value("bit").toString();
                                                    }
                                                    if(attributes.hasAttribute("value"))
                                                    {
                                                        LcsptgInfo.stat = attributes.value("value").toInt();
                                                    }
                                                }
                                                if(reader.name() == "触发位置")
                                                {
                                                    while(!reader.atEnd())
                                                    {
                                                        reader.readNext();
                                                        if(reader.isStartElement())
                                                        {
                                                            LCSP_TRIGGER_CPOS_STRU lcsptgp;
                                                            if(reader.name() == "坐标信息")
                                                            {
                                                                QXmlStreamAttributes attributes = reader.attributes();
                                                                if(attributes.hasAttribute("Id"))
                                                                {
                                                                    lcsptgp.id = attributes.value("Id").toString();
                                                                }
                                                                if(attributes.hasAttribute("pos"))
                                                                {
                                                                    lcsptgp.pos = attributes.value("pos").toDouble();
                                                                }
                                                                LcsptgInfo.LCSPTrigCPos.append(lcsptgp);
                                                            }
                                                        }
                                                        if(reader.isEndElement())
                                                        {
                                                            if(reader.name() == "触发位置")
                                                            {
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            if(reader.isEndElement())
                                            {
                                                if(reader.name() == "触发集合")
                                                {
                                                    lcspInfo.LCSPTrigInfo.append(LcsptgInfo);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "触发信息")
                                    {
                                        break;
                                    }
                                }
                            }

                        }
                        if(reader.name() == "坐标位置信息")
                        {
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "坐标信息")
                                    {
                                        LCSP_CPOSINFO_STRU lcpsAxisInfo;
                                        QXmlStreamAttributes attributes = reader.attributes();
                                        QString strId = "";
                                        if(attributes.hasAttribute("Id"))
                                        {
                                            strId = attributes.value("Id").toString();
                                        }
                                        while(!reader.atEnd())
                                        {
                                            reader.readNext();
                                            if(reader.isStartElement())
                                            {
                                                if(reader.name() == "目标位置")
                                                {
                                                    lcpsAxisInfo.pos = reader.readElementText().toDouble();
                                                }
                                            }
                                            if(reader.isEndElement())
                                            {
                                                if(reader.name() == "坐标信息")
                                                {
                                                    break;
                                                }
                                            }
                                        }
                                        if(!lcspInfo.LCSCPosMap.contains(strId))
                                        {
                                            lcspInfo.LCSCPosMap.insert(strId,lcpsAxisInfo);
                                        }
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "坐标位置信息")
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "轴集合")
                        {
                            break;
                        }
                    }
                }
                axisVec.append(lcspInfo);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadSWType(QXmlStreamReader &reader,QString strElement,QMap<QString,TIMERInfo_STRU> &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "计时")
            {
                TIMERInfo_STRU cmdconStru;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            cmdconStru.id= reader.readElementText();
                        }
                        else if(reader.name() == "注释")
                        {
                            cmdconStru.note= reader.readElementText();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "计时")
                        {
                            break;
                        }
                    }
                }
                if(!cmdxmlMap.contains(cmdconStru.id))
                {
                    cmdxmlMap.insert(cmdconStru.id,cmdconStru);
                }

            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWType(QXmlStreamReader &reader, QString strElement, QMap<QString, SWCONFIGXML_WAIT_STRU> &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "等待指令")
            {
                SWCONFIGXML_WAIT_STRU cmdconStru;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            cmdconStru.Id= reader.readElementText();
                        }
                        else if(reader.name() == "信息")
                        {
                            cmdconStru.strEvent= reader.readElementText();
                        }
                        else if(reader.name() == "报警")
                        {
                            cmdconStru.strError = reader.readElementText();
                        }
                        else if(reader.name() == "超时")
                        {
                            cmdconStru.uTimeout = reader.readElementText().toUShort();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "等待指令")
                        {
                            if(!cmdxmlMap.contains(cmdconStru.Id))
                            {
                                cmdxmlMap.insert(cmdconStru.Id,cmdconStru);
                            }
                            break;
                        }
                    }
                }

            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWType(QXmlStreamReader &reader, QString strElement, PROTECT_EVENT_MAP &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "轴保护")
            {
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "轴信息")
                        {
                            PROTECT_EVENT_STRU cmdconStru;
                            QXmlStreamAttributes attributes = reader.attributes();
                            cmdconStru.id = attributes.value("Id").toString();
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "事件")
                                    {
                                        cmdconStru.strEvent = reader.readElementText();
                                    }
                                    if(reader.name() == "注释")
                                    {
                                        cmdconStru.strNote = reader.readElementText();
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "轴信息")
                                    {
                                        if(!cmdxmlMap.axisEventMap.contains(cmdconStru.id))
                                            cmdxmlMap.axisEventMap.insert(cmdconStru.id,cmdconStru);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "轴保护")
                        {
                            break;
                        }
                    }
                }
            }

            if(reader.name() == "坐标保护")
            {
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "坐标信息")
                        {
                            PROTECT_EVENT_STRU cmdconStru;
                            QXmlStreamAttributes attributes = reader.attributes();
                            cmdconStru.id = attributes.value("Id").toString();
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "事件")
                                    {
                                        cmdconStru.strEvent = reader.readElementText();
                                    }
                                    if(reader.name() == "注释")
                                    {
                                        cmdconStru.strNote = reader.readElementText();
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "坐标信息")
                                    {
                                        if(!cmdxmlMap.coordEventMap.contains(cmdconStru.id))
                                            cmdxmlMap.coordEventMap.insert(cmdconStru.id,cmdconStru);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "坐标保护")
                        {
                            break;
                        }
                    }

                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWType(QXmlStreamReader &reader, QString strElement, QMap<QString, SWCONFIGXML_CY_STRU> &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "汽缸")
            {
                SWCONFIGXML_CY_STRU cmdconStru;
                QString str = reader.name().toString();
                ReadSWStru(reader,str,cmdconStru);
                if(!cmdxmlMap.contains(cmdconStru.id))
                {
                    cmdxmlMap.insert(cmdconStru.id,cmdconStru);
                }
                else
                {
                    cmdxmlMap[cmdconStru.id] = cmdconStru;
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
//write by dai  DOUBLE_BUTTON_START        ////QMap<QString,QVector<INOUTPUT_STRU>> &bitValueVec
void HandleSWXml::ReadSWType(QXmlStreamReader &reader,QString strElement,DOUBLEBUTTON_MAP &bitValueVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name()=="Start")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                //判断是否存在属性"bit,vaule"
                if(attributes.hasAttribute("Mode"))
                {
                    DOUBLEBUTTON_STRU doubleBtnStru;
                    doubleBtnStru.id = attributes.value("Mode").toString();
                    while(!reader.atEnd())
                    {
                        reader.readNext();
                        if(reader.isStartElement())
                        {
                            if(reader.name()=="IN_IO")
                            {
                                INOUTPUT_STRU bitvalue;//先定义一个存放节点属性的结构体
                                QXmlStreamAttributes attributes = reader.attributes();
                                if(attributes.hasAttribute("bit"))
                                {
                                    bitvalue.bitName=attributes.value("bit").toString();
                                }
                                if(attributes.hasAttribute("value"))
                                {
                                    bitvalue.stat=attributes.value("value").toInt();
                                }
                                doubleBtnStru.doubleButtonInfoVec.append(bitvalue);
                            }

                        }
                        if(reader.isEndElement())
                        {
                            if(reader.name() == "Start")
                            {
                                break;
                            }
                        }

                    }
                    if(!bitValueVec.doubleButtonInfoMap.contains(doubleBtnStru.id))
                    {
                        bitValueVec.doubleButtonInfoMap.insert(doubleBtnStru.id,doubleBtnStru);
                    }

                }
            }

        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
//write by dai  DOUBLE_BUTTON_START

//write by dai  SAFETY_CONFIG
void HandleSWXml::ReadSWType(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_SAFECON_MAP &safecon_map)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "三色灯软元件")
            {
                QString str = reader.name().toString();
                ReadSWStru(reader,str,safecon_map.swXml_THREE_SOFTWARE_Map);
            }
            else if(reader.name() == "三色灯配置")
            {
                QString str = reader.name().toString();

                ReadSWStru(reader,str,safecon_map.swXml_LIGHT_CONFIG_Map);

            }
            else if(reader.name() == "安全状态")
            {
                //QMap<QString,SWCONFIGXML_QPOSIT_STRU> cmdconMap;
                QString str = reader.name().toString();
                //QMap<QString,QMap<QString,QMap<QString,QMap<QString,SWCONFIGXML_BUTTONSTATE_STRU>>>> &cd;
                ReadSWStru(reader,str,safecon_map.swXml_SAFESTATE_Map);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
////以上是三色灯配置和急停、停止、复位、暂停按钮配置


//add by sylar.liu
//read pallet info from xml
void HandleSWXml::ReadAllPallet(QXmlStreamReader &reader, QString strElement, QMap<QString, PalletInfo> &palletInfo)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString str = reader.name().toString();

            if ("托盘信息" == str)
            {
                QString name;
                PalletInfo p;

                ReadPalletName(reader, name, p);
                ReadOnePallet(reader, str, p);

                if (!palletInfo.contains(name))
                    palletInfo.insert(name, p);
            }

        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadPalletName(QXmlStreamReader &reader, QString &palletName, PalletInfo &info)
{
    QXmlStreamAttributes attributes = reader.attributes();
    if(attributes.hasAttribute("name"))
    {
        palletName = attributes.value("name").toString();
        info.palletName = palletName;
    }

}

void HandleSWXml::ReadOnePallet(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString str = reader.name().toString();

            if ("轴信息" == str)
                ReadPalletAsix(reader, str, info);
            if ("行列层数" == str)
                ReadPalletDimension(reader, str, info);
            else if ("校准位置" == str)
                ReadPalletCheckpoint(reader, str, info);
            else if ("工作模式" == str)
                ReadPalletMode(reader, str, info);
            else if ("运动参数" == str)
                ReadPalletRunParams(reader, str, info);
            else if ("结束信号配置" == str)
                ReadPalletEndSignalSettings(reader, str, info);
            else if ("托盘重置配置" == str)
                ReadPalletResetSignalSettings(reader, str, info);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadPalletAsix(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString str = reader.name().toString();
            QString asixName;

            if ("X" || "Y" || "Z" == str)
                asixName = reader.readElementText();
            if (!info.coordinateToAxis.contains(str))
                info.coordinateToAxis.insert(str, asixName);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadPalletDimension(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            double value;
            QString str = reader.name().toString();

            if ("行数" == str)
                str = "row";
            else if ("列数" == str)
                str = "column";
            else if ("层数" == str)
                str = "floor";

            value = reader.readElementText().toDouble();

            if (!info.palletDimension.contains(str))
                info.palletDimension.insert(str, value);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadPalletCheckpoint(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString str = reader.name().toString();

            if ("第一层托盘" == str)
                ReadFirstFloor(reader, str, info);
            else if ("最后一层托盘" == str)
                ReadLastFloor(reader, str, info);
            else if ("结束位置" == str)
                ReadEndPosition(reader, info);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadFirstFloor(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString p;
            QString str = reader.name().toString();

            if ("左上" || "右上" || "左下" || "右下" == str)
                p = reader.readElementText();

            //conver Chinese to English
            if ("左上" == str)
                str = "leftTop";
            else if ("右上" == str)
                str = "rightTop";
            else if ("左下" == str)
                str = "leftBottom";
            else if ("右下" == str)
                str = "rightBottom";

            if (!info.palletCheckpoint.contains(str))
                info.palletCheckpoint.insert(str, p);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;

    }
}

void HandleSWXml::ReadLastFloor(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString p;
            QString str = reader.name().toString();

            if ("左上" == str)
            {
                str = "last_leftTop";
                p = reader.readElementText();
            }

            if (!info.palletCheckpoint.contains(str))
                info.palletCheckpoint.insert(str, p);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadEndPosition(QXmlStreamReader &reader, PalletInfo &info)
{
        QString p = reader.readElementText();
        QString str = "endPosition";

        if (!info.palletCheckpoint.contains(str))
            info.palletCheckpoint.insert(str, p);
}

void HandleSWXml::ReadPalletEndSignalSettings(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            SignalSetting setting;
            QString str = reader.name().toString();

            if ("本层结束信号" || "全部结束信号" == str)
            {
               if ("本层结束信号" == str)
                   str = "thisFloorIsEnd";
               else str = "allFloorIsEnd";

               ReadOneSignalSetting(reader, setting);

               if (!info.endSignal.contains(str))
                   info.endSignal.insert(str, setting);
            }
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadPalletResetSignalSettings(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            SignalSetting setting;
            QString str = reader.name().toString();

            if ("托盘重置信号" == str)
            {
                str = "resetSignal";
                ReadOneSignalSetting(reader, setting);
            }

            if (!info.resetSignal.contains(str))
                info.resetSignal.insert(str, setting);
        }
        else if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
    }
}

void HandleSWXml::ReadOneSignalSetting(QXmlStreamReader &reader, SignalSetting &setting)
{
    QXmlStreamAttributes attributes = reader.attributes();

    if(attributes.hasAttribute("bit"))
    {
        setting.bit = attributes.value("bit").toString();
    }
    if(attributes.hasAttribute("value"))
    {
        setting.value = attributes.value("value").toInt();
    }
}

/*
    To Do
    增加校准偏移量的部分。
*/
//void HandleSWXml::ReadPalletOffset(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
//{
//    while (!reader.atEnd())
//    {
//        reader.readNext();
//        if (reader.isStartElement())
//        {
//            double value;
//            QString str = reader.name().toString();

//            if ("X" || "Y" || "Z" == str)
//            {
//                value = reader.readElementText().toDouble();
//                if (!info.offset.contains(str))
//                    info.offset.insert(str, value);
//            }
//        }
//        else {
//            if (reader.isEndElement())
//                if (reader.name() == strElement)
//                    break;
//        }
//    }
//}

void HandleSWXml::ReadBasePoint(QXmlStreamReader &reader, QString strElement, Position &point)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString str = reader.name().toString();

            if ("X" == str)
                point.x = reader.readElementText().toDouble();
            else if ("Y" == str)
                point.y = reader.readElementText().toDouble();
            else if ("Z" == str)
                point.z = reader.readElementText().toDouble();
        }
        else {
            if (reader.isEndElement())
                if (reader.name() == strElement)
                    break;
        }
    }
}

void HandleSWXml::ReadPalletMode(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            int mode;
            QString str = reader.name().toString();
            if ("抓取模式" == str)
                str = "captureMode";
            else if ("移动方向" == str)
                str = "moveOrientation";
            else if ("运动模式" == str)
                str = "moveMode";
            mode = reader.readElementText().toInt();

            if (!info.palletMode.contains(str))
                info.palletMode.insert(str, mode);
        }
        else if (reader.isEndElement())
        {
                if (reader.name() == strElement)
                    break;
        }
    }
}

void HandleSWXml::ReadPalletRunParams(QXmlStreamReader &reader, QString strElement, PalletInfo &info)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            _RunRarams param;
            QString n;
            QString str = reader.name().toString();

            if ("递增" == str)
                n = "Increase";
            else if ("定位" == str)
                n = "Locate";
            ReadOneParams(reader, str, param);

            if (!info.runParams.contains(n))
                info.runParams.insert(n, param);
        }
        else if (reader.isEndElement())
            if (reader.name() == strElement)
                break;
    }
}

void HandleSWXml::ReadOneParams(QXmlStreamReader &reader, QString strElement, _RunRarams &param)
{
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            QString str = reader.name().toString();

            if ("超时" == str)
                param.timeout = reader.readElementText().toDouble();
            else if ("单步延时" == str)
                param.delay = reader.readElementText().toDouble();
            else if ("速度百分率" == str)
                param.speed = reader.readElementText().toDouble();
            else if ("加速" == str)
                param.acc = reader.readElementText().toDouble();
            else if ("减速" == str)
                param.dec = reader.readElementText().toDouble();
        }
        else if (reader.isEndElement())
            if (reader.name() == strElement)
                break;
    }
}

//三色灯软元件
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,INOUTPUT_STRU>&cmdconVec)
{
    QString str="";
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())//如果光标指向开始标记
        {
            if(reader.name()=="灯名称")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("ID"))
                {
                    str= attributes.value("ID").toString();

                    while(!reader.atEnd())
                    {
                        //INOUTPUT_STRU bitvalue;
                        reader.readNext();
                        if(reader.isStartElement())//如果光标指向开始标记
                        {
                            if(reader.name()=="软元件")
                            {
                                INOUTPUT_STRU bitvalue;

                                QXmlStreamAttributes attributes = reader.attributes();
                                if(attributes.hasAttribute("bitname"))//确保括号内有bitname属性
                                {
                                    bitvalue.bitName=attributes.value("bitname").toString();

                                }
                                if(attributes.hasAttribute("value"))//确保括号内有value属性
                                {
                                    bitvalue.stat=attributes.value("value").toInt();
                                }
                                if(!cmdconVec.contains(str))
                                {
                                    cmdconVec.insert(str,bitvalue);
                                }
                            }

                        }
                        if(reader.isEndElement() && reader.name()=="灯名称")
                        {
                            //tempVec.append(bitvalue);

                            break;
                        }

                    }
                }

            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() ==strElement)
            {
                break;
            }
        }
    }
}
//以上是三色灯软元件
//以下是三色灯配置
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,SENSOR_LIGHT_STRU> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name()=="信号等级")
            {
                SENSOR_LIGHT_STRU temSTRU;
                QString str="";

                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("ID"))
                {
                    str= attributes.value("ID").toString();

                    while(!reader.atEnd())//
                    {
                        reader.readNext();
                        if(reader.isStartElement())
                        {
                            if(reader.name()=="类型")
                            {
                                temSTRU.name=reader.readElementText();//内容为 "快闪"
                            }
                            if(reader.name()=="亮灯种类")
                            {
                                while(!reader.atEnd())//"信号灯" 节点循环开始
                                {

                                    reader.readNext();
                                    QString  str_num="";
                                    if(reader.isStartElement())//如果光标指向开始标记
                                    {
                                        if(reader.name()=="信号灯")
                                        {
                                            str_num=reader.readElementText();
                                            if(!temSTRU.light.contains(str_num))
                                            {
                                                temSTRU.light.append(str_num);
                                            }

                                        }
                                    }
                                    if(reader.isEndElement())
                                    {
                                        if(reader.name()=="亮灯种类")
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if(reader.isEndElement())
                        {
                            if(reader.name()=="信号等级")
                            {
                                break;
                            }
                        }
                    }

                }
                if(!cmdconVec.contains(str))
                {
                    cmdconVec.insert(str,temSTRU);
                }
            }

        }
        if(reader.isEndElement() && reader.name() ==strElement)
        {
            break;
        }

    }

}


//以上是三色灯配置

//以下是按钮安全状态
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,QMap<QString,SWCONFIGXML_SAFESTATE_STRU>> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            QMap<QString,SWCONFIGXML_SAFESTATE_STRU> temp;
            if(reader.name()=="总状态")
            {
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name()=="安全信息")
                        {
                            QXmlStreamAttributes attributes = reader.attributes();
                            if(attributes.hasAttribute("ID"))
                            {
                                QString str = attributes.value("ID").toString();
                                SWCONFIGXML_SAFESTATE_STRU button_Stru;
                                while(!reader.atEnd())
                                {
                                    reader.readNext();
                                    if(reader.isStartElement())
                                    {
                                        if(reader.name()=="信号等级")
                                        {
                                            button_Stru.level=reader.readElementText();
                                        }
                                        if(reader.name()=="信号点位集合")
                                        {
                                            while(!reader.atEnd())
                                            {
                                                reader.readNext();

                                                if(reader.isStartElement())
                                                {
                                                    SWCONFIGXML_SAFEBITINFO_STRU bitInfo;
                                                    if(reader.name()=="信号点位")
                                                    {
                                                        QXmlStreamAttributes attributes1 = reader.attributes();
                                                        if(attributes1.hasAttribute("bitname"))
                                                        {
                                                            bitInfo.sensor_bit.bitName = attributes1.value("bitname").toString();
                                                        }
                                                        if(attributes1.hasAttribute("value"))
                                                        {
                                                            bitInfo.sensor_bit.stat= attributes1.value("value").toInt();
                                                        }
                                                        while(!reader.atEnd())
                                                        {
                                                            reader.readNext();
                                                            if(reader.isStartElement())
                                                            {
                                                                if(reader.name()=="输出状态信息")
                                                                {
                                                                    bitInfo.info=reader.readElementText();
                                                                }
                                                            }
                                                            if(reader.isEndElement())
                                                            {
                                                                if(reader.name() == "信号点位")
                                                                {
                                                                    button_Stru.BitInfoVec.append(bitInfo);
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                if(reader.isEndElement())
                                                {
                                                    if(reader.name() == "信号点位集合")
                                                    {
                                                        break;
                                                    }
                                                }
                                            }
                                        }

                                    }
                                    if(reader.isEndElement() && reader.name()=="安全信息")
                                    {
                                        if(!temp.contains(str))
                                        {
                                            temp.insert(str,button_Stru) ;
                                        }
                                        break;

                                    }

                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "总状态")
                        {
                            cmdconVec.insert("总状态",temp);
                            break;
                        }
                    }
                }
            }
            else if(reader.name()=="子状态")
            {
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        temp.clear();
                        if(reader.name()=="子流程")
                        {
                            QXmlStreamAttributes attributes = reader.attributes();
                            if(attributes.hasAttribute("name"))
                            {
                                QString str = attributes.value("name").toString();
                                while(!reader.atEnd())
                                {
                                    reader.readNext();
                                    if(reader.isStartElement())
                                    {
                                        if(reader.name()=="安全信息")
                                        {
                                            QXmlStreamAttributes attributes = reader.attributes();
                                            if(attributes.hasAttribute("ID"))
                                            {
                                                QString strID = attributes.value("ID").toString();
                                                SWCONFIGXML_SAFESTATE_STRU button_Stru;
                                                while(!reader.atEnd())
                                                {
                                                    reader.readNext();
                                                    if(reader.isStartElement())
                                                    {
                                                        if(reader.name()=="信号等级")
                                                        {
                                                            button_Stru.level=reader.readElementText();
                                                        }
                                                        if(reader.name()=="信号点位集合")
                                                        {
                                                            while(!reader.atEnd())
                                                            {
                                                                reader.readNext();

                                                                if(reader.isStartElement())
                                                                {
                                                                    SWCONFIGXML_SAFEBITINFO_STRU bitInfo;
                                                                    if(reader.name()=="信号点位")
                                                                    {
                                                                        QXmlStreamAttributes attributes1 = reader.attributes();
                                                                        if(attributes1.hasAttribute("bitname"))
                                                                        {
                                                                            bitInfo.sensor_bit.bitName = attributes1.value("bitname").toString();
                                                                        }
                                                                        if(attributes1.hasAttribute("value"))
                                                                        {
                                                                            bitInfo.sensor_bit.stat= attributes1.value("value").toInt();
                                                                        }
                                                                        while(!reader.atEnd())
                                                                        {
                                                                            reader.readNext();
                                                                            if(reader.isStartElement())
                                                                            {
                                                                                if(reader.name()=="输出状态信息")
                                                                                {
                                                                                    bitInfo.info=reader.readElementText();
                                                                                }
                                                                            }
                                                                            if(reader.isEndElement())
                                                                            {
                                                                                if(reader.name() == "信号点位")
                                                                                {
                                                                                    button_Stru.BitInfoVec.append(bitInfo);
                                                                                    break;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                if(reader.isEndElement())
                                                                {
                                                                    if(reader.name() == "信号点位集合")
                                                                    {
                                                                        break;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(reader.isEndElement() && reader.name()=="安全信息")
                                                    {
                                                        if(!temp.contains(strID))
                                                        {
                                                            temp.insert(strID,button_Stru) ;
                                                        }
                                                        break;

                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if(reader.isEndElement())
                                    {
                                        if(reader.name() == "子流程")
                                        {
                                            cmdconVec.insert(str,temp);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "子状态")
                        {
                            break;
                        }
                    }
                }
            }

        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }

    }
}
//write by dai  SAFETY_CONFIG

void HandleSWXml::ReadSWType(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_SER_MAP &cmdxmlMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "励磁")
            {
                QString str = reader.name().toString();

                ReadSWStru(reader,str,cmdxmlMap.son_Stru);
            }
            else if(reader.name() == "回原点")
            {
                QString str = reader.name().toString();
                ReadSWStru(reader,str,cmdxmlMap.swXml_ORG_Map);

            }
            else if(reader.name() == "寸动")
            {
                QString str = reader.name().toString();
                ReadSWStru(reader,str,cmdxmlMap.swXml_JOG_Map);

            }
            else if(reader.name() == "动态脚本部分")
            {
                QString str = reader.name().toString();
                ReadSWStru(reader,str,cmdxmlMap.swXml_JS_Map);

            }
            else if(reader.name() == "快速定位")
            {
                QString str = reader.name().toString();
                ReadSWStru(reader,str,cmdxmlMap.swXml_QP_Map);
            }
            else if(reader.name() == "直线插补模式")
            {
                QString str = reader.name().toString();
                ReadSWStru(reader,str,cmdxmlMap.swXml_LCSP_Map);
            }
            else if(reader.name()=="扭矩模式")      //2017-06-06  write by daiqiwei
            {
                QString str=reader.name().toString();
                ReadSWStru(reader,str,cmdxmlMap.swXml_QTorque_Map);

            }
            //            if(reader.isEndElement())
            //            {
            //                if(reader.name() == strElement)
            //                {
            //                    break;
            //                }
            //            }

        } else {
            if(reader.isEndElement())
            {
                if(reader.name() == strElement)
                {
                    break;
                }
            }
        }

    }
}
void HandleSWXml::ReadSWStru(QXmlStreamReader &reader, QString strElement, QMap<QString, SWCONFIGXML_JOG_STRU> &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "寸动信息")
            {
                SWCONFIGXML_JOG_STRU swJOG;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            swJOG.id = reader.readElementText();
                        }
                        else if(reader.name() == "注释")
                        {
                            swJOG.note = reader.readElementText();
                        }
                        else if(reader.name() == "加速")
                        {
                            //todo
                            swJOG.acc = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "减速")
                        {
                            //todo
                            swJOG.dec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "超时")
                        {
                            //todo
                            swJOG.timeOut = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "单步延时")
                        {
                            //todo
                            swJOG.delayMsec = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "方向")
                        {
                            //todo
                            swJOG.strDirection = reader.readElementText();
                        }
                        else if(reader.name() == "极限")
                        {
                            swJOG.MaxPos = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "轴集合")
                        {
                            //todo
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "轴信息")
                                    {
                                        JOGAxisInfo_STRU JogaxisInfo;
                                        QXmlStreamAttributes attributes = reader.attributes();
                                        //判断是否存在属性"Id"
                                        if(attributes.hasAttribute("Id"))
                                        {
                                            JogaxisInfo.axisId = attributes.value("Id").toString();
                                        }
                                        while(!reader.atEnd())
                                        {
                                            reader.readNext();

                                            if(reader.isStartElement())
                                            {
                                                if(reader.name() == "速度百分率")
                                                {
                                                    JogaxisInfo.speed = reader.readElementText().toInt();
                                                }
                                                else if(reader.name() == "比特")
                                                {
                                                    QXmlStreamAttributes attributes = reader.attributes();
                                                    //判断是否存在属性"bit,vaule"
                                                    if(attributes.hasAttribute("bit"))
                                                    {
                                                        JogaxisInfo.bitStru.bitName = attributes.value("bit").toString();
                                                    }
                                                    if(attributes.hasAttribute("vaule"))
                                                    {
                                                        JogaxisInfo.bitStru.stat = attributes.value("vaule").toInt();
                                                    }
                                                }
                                            }
                                            if(reader.isEndElement())
                                            {
                                                if(reader.name() == "轴信息")
                                                {
                                                    break;
                                                }
                                            }
                                        }

                                        if(!swJOG.jogAxisInfoMap.contains(JogaxisInfo.axisId))
                                            swJOG.jogAxisInfoMap.insert(JogaxisInfo.axisId,JogaxisInfo);
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "轴集合")
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "寸动信息")
                        {
                            if(!cmdconVec.contains(swJOG.id))
                            {
                                cmdconVec.insert(swJOG.id,swJOG);
                            }
                            break;
                        }
                    }
                }

            }

        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadSWStru(QXmlStreamReader &reader,QString strElement,SWCONFIGXML_ORG_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "原点轴信息")
            {
                QString str = reader.name().toString();
                ReadORGAxisInfo(reader,str,cmdconVec.swXml_ORG_Axis_Map);

            }
            if(reader.name() == "原点操作信息")
            {
                QString str = reader.name().toString();
                ReadORGOP(reader,str,cmdconVec.swXml_ORG_OP_Map);
            }

        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
void HandleSWXml::ReadORGOP(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_ORG_OP_STRU> &swORGopMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "原点信息")
            {
                SWCONFIGXML_ORG_OP_STRU orgOPStru;
                while(!reader.atEnd())
                {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            orgOPStru.orgID = reader.readElementText();
                        }
                        else if(reader.name() == "超时")
                        {
                            orgOPStru.utimeout = reader.readElementText().toUInt();
                        }
                        else if(reader.name() == "轴集合")
                        {
                            while(!reader.atEnd())
                            {
                                reader.readNext();
                                if(reader.isStartElement())
                                {
                                    if(reader.name() == "轴编号")
                                    {
                                        orgOPStru.swXml_ORG_AxisId_Vec.append(reader.readElementText());
                                    }
                                }
                                if(reader.isEndElement())
                                {
                                    if(reader.name() == "轴集合")
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "原点信息")
                        {
                            break;
                        }
                    }
                }

                if(!swORGopMap.contains(orgOPStru.orgID))
                    swORGopMap.insert(orgOPStru.orgID,orgOPStru);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleSWXml::ReadORGAxisInfo(QXmlStreamReader &reader,QString strElement,QMap<QString,SWCONFIGXML_ORG_AXIS_STRU> &axisMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "轴信息")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                QString strId = "";
                //判断是否存在属性"Id"
                if(attributes.hasAttribute("Id"))
                {
                    strId = attributes.value("Id").toString();
                }
                SWCONFIGXML_ORG_AXIS_STRU axisInfo;
                axisInfo.axisId = strId;
                while(!reader.atEnd())
                {
                    reader.readNext();

                    if(reader.isStartElement())
                    {
                        if(reader.name() == "高速")
                        {
                            axisInfo.S1 = reader.readElementText().toUInt();
                        }
                        else if(reader.name() == "低速")
                        {
                            axisInfo.S2 = reader.readElementText().toUInt();
                        }
                        else if(reader.name() == "方向")
                        {
                            axisInfo.direction = reader.readElementText().toUShort();
                        }
                        else if(reader.name() == "正向时间")
                        {
                            axisInfo.directTime = reader.readElementText().toInt();
                        }
                        else if(reader.name() == "原点偏移时间")
                        {
                            axisInfo.orgmDelay = reader.readElementText().toUShort();
                        }
                        else if(reader.name() == "超时")
                        {
                            axisInfo.orgTimeout = reader.readElementText().toUInt();
                        }
                        else if(reader.name() == "类型")//新增原点模式解析
                        {
                            axisInfo.orgmode = reader.readElementText();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "轴信息")
                        {
                            break;
                        }
                    }
                }

                if(!axisMap.contains(strId))
                    axisMap.insert(strId,axisInfo);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
QString HandleSWXml::WriteSWConfigXml(QString strPath)
{
    QString msg("");

    QFile file(strPath);
    if(!file.exists())
    {
        msg = QString("%1 is not exists!").arg(strPath);
    }


    return msg;
}
bool HandleSWXml::WriteSWSerXml(QString strPath,QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp,QMap<QString,SWCONFIGXML_LCSP_STRU> temp1)
{
    QFile file(strPath);
    if(!file.exists())
    {
        return false;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }
    QTextStream vReadStream(&file);
    QTextCodec *vCodec = QTextCodec::codecForName("UTF-8");
    vReadStream.setCodec(vCodec);
    QString vXmlDataStr = vReadStream.readAll();
    file.close();
    QDomDocument doc;
    if(!doc.setContent(vXmlDataStr))
    {
        file.close();
        return false;
    }
    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "Data")
    {
        QDomElement xmlItems = docElem.firstChildElement("程序配置");
        if(!xmlItems.isNull())
        {
            QDomElement xmlItem = xmlItems.firstChildElement("运动部分");
            if(!xmlItem.isNull())
            {
                QDomNode node = xmlItem.firstChild();
                while(!node.isNull())
                {
                    if(node.toElement().tagName() == "快速定位")
                    {
                        xmlItem.removeChild(node);
                    }
                    node = node.nextSibling();
                }
                node = xmlItem.firstChild();
                while(!node.isNull())
                {
                    if(node.toElement().tagName() == "直线插补模式")
                    {
                        xmlItem.removeChild(node);
                    }
                    node = node.nextSibling();
                }
                QDomElement xmlQP = doc.createElement("快速定位");
                for(auto item = temp.begin();item != temp.end();++item)
                {
                    QDomElement xmlQPinfo = doc.createElement("快速定位信息");
                    QDomElement xmlId = doc.createElement("编号");
                    QDomText text = doc.createTextNode(item.value().id);
                    xmlId.appendChild(text);
                    QDomElement xmlNote = doc.createElement("注释");
                    text = doc.createTextNode(item.value().note);
                    xmlNote.appendChild(text);
                    QDomElement xmlTimeout = doc.createElement("超时");
                    text = doc.createTextNode(QString("%1").arg(item.value().timeOut));
                    xmlTimeout.appendChild(text);
                    QDomElement xmldelay = doc.createElement("单步延时");
                    text = doc.createTextNode(QString("%1").arg(item.value().delayMsec));
                    xmldelay.appendChild(text);
                    QDomElement xmlSpeed = doc.createElement("速度百分率");
                    text = doc.createTextNode(QString("%1").arg(item.value().speed));
                    xmlSpeed.appendChild(text);
                    QDomElement xmlAccel = doc.createElement("加速");
                    text = doc.createTextNode(QString("%1").arg(item.value().acc));
                    xmlAccel.appendChild(text);
                    QDomElement xmldccel = doc.createElement("减速");
                    text = doc.createTextNode(QString("%1").arg(item.value().dec));
                    xmldccel.appendChild(text);
                    QDomElement xmlaxisNum = doc.createElement("轴数");
                    text = doc.createTextNode(QString("%1").arg(item.value().axisNum));
                    xmlaxisNum.appendChild(text);

                    QDomElement xmlaxiss= doc.createElement("轴集合");
                    for(auto it = item.value().axisMap.begin();it != item.value().axisMap.end();++it)
                    {
                        QDomElement xmlaxisinfo= doc.createElement("轴信息");
                        xmlaxisinfo.setAttribute("Id", it.key());
                        QDomElement xmlaxisspeed= doc.createElement("速度百分率");
                        QDomText tx = doc.createTextNode(QString("%1").arg(it.value().speed));
                        xmlaxisspeed.appendChild(tx);
                        xmlaxisinfo.appendChild(xmlaxisspeed);

                        QDomElement xmlaxisPos= doc.createElement("位置");
                        tx = doc.createTextNode(QString("%1").arg(it.value().pos));
                        xmlaxisPos.appendChild(tx);
                        xmlaxisinfo.appendChild(xmlaxisPos);

                        QDomElement xmlaxisoffset= doc.createElement("位置偏移量");
                        tx = doc.createTextNode(QString("%1").arg(it.value().offsetPos));
                        xmlaxisoffset.appendChild(tx);
                        xmlaxisinfo.appendChild(xmlaxisoffset);

                        xmlaxiss.appendChild(xmlaxisinfo);
                    }
                    xmlQPinfo.appendChild(xmlId);
                    xmlQPinfo.appendChild(xmlNote);
                    xmlQPinfo.appendChild(xmlTimeout);
                    xmlQPinfo.appendChild(xmldelay);
                    xmlQPinfo.appendChild(xmlSpeed);
                    xmlQPinfo.appendChild(xmlAccel);
                    xmlQPinfo.appendChild(xmldccel);
                    xmlQPinfo.appendChild(xmlaxisNum);
                    xmlQPinfo.appendChild(xmlaxiss);
                    xmlQP.appendChild(xmlQPinfo);
                }
                xmlItem.appendChild(xmlQP);
                QDomElement xmlCSP = doc.createElement("直线插补模式");
                for(auto item = temp1.begin();item != temp1.end();++item)
                {
                    QDomElement xmlCSPinfo = doc.createElement("插补信息");
                    QDomElement xmlId = doc.createElement("编号");
                    QDomText text = doc.createTextNode(item.value().id);
                    xmlId.appendChild(text);
                    QDomElement xmlNote = doc.createElement("注释");
                    text = doc.createTextNode(item.value().note);
                    xmlNote.appendChild(text);
                    QDomElement xmlTimeout = doc.createElement("超时");
                    text = doc.createTextNode(QString("%1").arg(item.value().timeOut));
                    xmlTimeout.appendChild(text);
                    QDomElement xmldelay = doc.createElement("单步延时");
                    text = doc.createTextNode(QString("%1").arg(item.value().delayMsec));
                    xmldelay.appendChild(text);
                    QDomElement xmlSpeed = doc.createElement("速度");
                    text = doc.createTextNode(QString("%1").arg(item.value().speed));
                    xmlSpeed.appendChild(text);
                    QDomElement xmlAccel = doc.createElement("加速");
                    text = doc.createTextNode(QString("%1").arg(item.value().acc));
                    xmlAccel.appendChild(text);

                    QDomElement xmlLCSPPos= doc.createElement("直线位置");

                    for(auto it = item.value().LCSPCPos.begin();it != item.value().LCSPCPos.end();++it)
                    {
                        QDomElement xmlaxisVec= doc.createElement("轴集合");
                        QDomElement xmltiginfo= doc.createElement("触发信息");
                        for(auto itor = it->LCSPTrigInfo.begin();itor != it->LCSPTrigInfo.end();++itor)
                        {
                            QDomElement xmltigVec= doc.createElement("触发集合");
                            QDomElement xmltigIoInfo= doc.createElement("触发I0信息");
                            xmltigIoInfo.setAttribute("bit", itor->bitName);
                            xmltigIoInfo.setAttribute("value", itor->stat);
                            QDomElement xmltigPos= doc.createElement("触发位置");
                            for(auto i = itor->LCSPTrigCPos.begin();i != itor->LCSPTrigCPos.end();++i)
                            {
                                QDomElement xmlAxisInfo= doc.createElement("坐标信息");
                                xmlAxisInfo.setAttribute("Id", i->id);
                                xmlAxisInfo.setAttribute("pos", i->pos);
                                xmltigPos.appendChild(xmlAxisInfo);
                            }
                            xmltigVec.appendChild(xmltigIoInfo);
                            xmltigVec.appendChild(xmltigPos);

                            xmltiginfo.appendChild(xmltigVec);
                        }
                        QDomElement xmltigPosinfo= doc.createElement("坐标位置信息");
                        for(auto itor = it->LCSCPosMap.begin();itor != it->LCSCPosMap.end();++itor)
                        {
                            QDomElement xmlaxisinfo= doc.createElement("坐标信息");
                            xmlaxisinfo.setAttribute("Id", itor.key());
                            QDomElement xmltrgPos= doc.createElement("目标位置");
                            text = doc.createTextNode(QString("%1").arg(itor.value().pos));
                            xmltrgPos.appendChild(text);
                            xmlaxisinfo.appendChild(xmltrgPos);

                            xmltigPosinfo.appendChild(xmlaxisinfo);
                        }
                        xmlaxisVec.appendChild(xmltiginfo);
                        xmlaxisVec.appendChild(xmltigPosinfo);

                        xmlLCSPPos.appendChild(xmlaxisVec);
                    }

                    QDomElement xmloffsetPos= doc.createElement("坐标位置偏移信息");
                    QDomElement xmlXcoord= doc.createElement("X坐标");
                    text = doc.createTextNode(QString("%1").arg(0.0));
                    xmlXcoord.appendChild(text);
                    QDomElement xmlYcoord= doc.createElement("Y坐标");
                    text = doc.createTextNode(QString("%1").arg(0.0));
                    xmlYcoord.appendChild(text);
                    QDomElement xmlZcoord= doc.createElement("Z坐标");
                    text = doc.createTextNode(QString("%1").arg(0.0));
                    xmlZcoord.appendChild(text);
                    xmloffsetPos.appendChild(xmlXcoord);
                    xmloffsetPos.appendChild(xmlYcoord);
                    xmloffsetPos.appendChild(xmlZcoord);

                    xmlCSPinfo.appendChild(xmlId);
                    xmlCSPinfo.appendChild(xmlNote);
                    xmlCSPinfo.appendChild(xmlTimeout);
                    xmlCSPinfo.appendChild(xmldelay);
                    xmlCSPinfo.appendChild(xmlSpeed);
                    xmlCSPinfo.appendChild(xmlAccel);
                    xmlCSPinfo.appendChild(xmlLCSPPos);
                    xmlCSPinfo.appendChild(xmloffsetPos);
                    xmlCSP.appendChild(xmlCSPinfo);
                }

                xmlItem.appendChild(xmlCSP);
            }
        }

    }
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        file.close();
        return false;
    }

    QTextStream ts(&file);
    ts.reset();
    ts.setCodec("utf-8");
    doc.save(ts, 4, QDomNode::EncodingFromTextStream);
    file.close();
    return true;
}
