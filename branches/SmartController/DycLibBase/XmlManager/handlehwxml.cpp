#include "handlehwxml.h"

HandleHWXml::HandleHWXml()
{

}

QString HandleHWXml::ReadHWConfigXml(QString strPath,HWCONFIGXML_MAP &hwConfigXml)
{
    QString msg("");

     QFile file(strPath);
     if(!file.exists())//文件不存在
     {
         msg = QString("%1 is not exists!").arg(strPath);
     }

     if(file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         //构建QXmlStreamReader对象
         QXmlStreamReader reader(&file);
         while(!reader.atEnd())//没有读到XML结尾
         {
             reader.readNext();//读下一个节点

             //判断是否是节点的开始
             if(reader.isStartElement())
             {
                 //判断当前节点的名字是否为硬件配置集合
                 if(reader.name() == "网口配置")
                 {
                     ReadHWType(reader,"网口配置",hwConfigXml.hwConfigXmlNetworkStru);
                 }
                 if(reader.name() == "串口配置")
                 {
                     ReadHWType(reader,"串口配置",hwConfigXml.hwConfigXmlComportMap);
                 }
                 if(reader.name() == "网络配置")
                 {
                     ReadHWType(reader,"网络配置",hwConfigXml.hwConfigXmlIntnetMap);
                 }
                 if(reader.name() == "Mqtt配置")
                 {
                     ReadHWType(reader,"Mqtt配置",hwConfigXml.hwConfigxmlMqttMap);
                 }
                 else if(reader.name() == "伺服控制模式")
                 {
                     ReadHWType(reader,"伺服控制模式",hwConfigXml.hwConfigXmlModelMap);
                 }
                 else if(reader.name() == "WEB配置")
                 {
                     ReadHWType(reader,"WEB配置",hwConfigXml.hwConfigXmlWebMap);
                 }
                 else if(reader.name() == "INPUT")
                 {
                     ReadHWType(reader,"INPUT",hwConfigXml.hwConfigXmlInputMap,hwConfigXml.hwConfigXmlModelMap);
                 }
                 else if(reader.name() == "OUTPUT")
                 {
                     ReadHWType(reader,"OUTPUT",hwConfigXml.hwConfigXmlOutputMap,hwConfigXml.hwConfigXmlModelMap);
                 }
                 else if(reader.name() == "坐标系配置")
                 {
                     ReadHWType(reader,"坐标系配置",hwConfigXml.hwConfigXmlCoordinateMap);
                 }
             }
             //节点结束、并且节点名字为Data（含有子节点）
             else if(reader.isEndElement() && reader.name() == "Data")//读到xml结尾的data节点
             {
                 break;
             }
         }
         file.close();
     }
     else
     {
         msg = QString("Open file %1 failure").arg(strPath);//打开失败
     }

    return msg;
}

QString HandleHWXml::WriteHWConfigXml(QString strPath)
{
    return strPath;
}
 void HandleHWXml::ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_MODEL_MAP &cmdxmlMap)
 {
     cmdxmlMap.hwModelMap.clear();
     while(!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "模式类型")//快速定位模式
             {
                QXmlStreamAttributes attributes = reader.attributes();//定义属性变量
                QMap<QString,HWCONFIGXML_MODEL_STRU> cmdconStru;
                //判断是否存在属性"ID"
                if(attributes.hasAttribute("ID"))
                {
                   QString id = attributes.value("ID").toString();//将属性为ID的值
                   QString str = reader.name().toString(); //读节点名字
                   ReadHWStru(reader,str,cmdconStru);
                   if(!cmdxmlMap.hwModelMap.contains(id))
                   {
                       cmdxmlMap.hwModelMap.insert(id,cmdconStru);
                   }
                }
                else
                {
                    //报错
                }
             }
         }
         if(reader.isEndElement())//读到末尾元素
         {
             if(reader.name() == strElement)
             {
                 break;
             }
         }
     }
 }
 void HandleHWXml::ReadHWType(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_COMPORT_STRU> &cmdxmlMap)
 {
     cmdxmlMap.clear();
     while(!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "串口编号")
             {
                QXmlStreamAttributes attributes = reader.attributes();
                //判断是否存在属性"ID"
                if(attributes.hasAttribute("ID"))
                {
                    HWCONFIGXML_COMPORT_STRU  hwComport;
                    hwComport.Id = attributes.value("ID").toString();
                    while(!reader.atEnd())
                    {
                       reader.readNext();
                       if(reader.isStartElement())
                       {
                           if(reader.name() == "端口名")
                           {
                               //todo
                               hwComport.portName = reader.readElementText();

                           }
                           else if(reader.name() == "波特率")
                           {
                               //todo
                                 hwComport.baudRate = reader.readElementText();
                           }
                           else if(reader.name() == "头尾设置")
                           {
                               //todo
                                 hwComport.headendData = reader.readElementText();
                           }
                       }
                       if(reader.isEndElement())
                       {
                           if(reader.name() == "串口编号")
                           {
                               break;
                           }
                       }
                    }
                    if(!cmdxmlMap.contains(hwComport.Id))
                    {
                       cmdxmlMap.insert(hwComport.Id,hwComport);
                    }
                }
                else
                {
                    //报错
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

 void HandleHWXml::ReadHWType(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_INTNET_STRU> &cmdxmlMap)
 {
     cmdxmlMap.clear();
     while(!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "网络编号")
             {
                QXmlStreamAttributes attributes = reader.attributes();
                //判断是否存在属性"ID"
                if(attributes.hasAttribute("ID"))
                {
                    HWCONFIGXML_INTNET_STRU  hwIntnet;
                    hwIntnet.Id = attributes.value("ID").toString();
                    while(!reader.atEnd())
                    {
                       reader.readNext();
                       if(reader.isStartElement())
                       {
                           if(reader.name() == "协议类型")
                           {
                               //todo
                               hwIntnet.agrType = reader.readElementText();

                           }
                           else if(reader.name() == "服务类型")
                           {
                               //todo
                                 hwIntnet.serType = reader.readElementText();
                           }
                           else if(reader.name() == "IP地址")
                           {
                               //todo
                                 hwIntnet.ipAdd = reader.readElementText();
                           }
                           else if(reader.name() == "端口号")
                           {
                               //todo
                                 hwIntnet.portName = reader.readElementText();
                           }
                       }
                       if(reader.isEndElement())
                       {
                           if(reader.name() == "网络编号")
                           {
                               break;
                           }
                       }
                    }
                    if(!cmdxmlMap.contains(hwIntnet.Id))
                    {
                       cmdxmlMap.insert(hwIntnet.Id,hwIntnet);
                    }
                }
                else
                {
                    //报错
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

 void HandleHWXml::ReadHWType(QXmlStreamReader &reader, QString strElement, QMap<QString, MQTT_STRU> &cmdxmlMap)
 {
     cmdxmlMap.clear();
     while (!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "网络编号")
             {
                 QXmlStreamAttributes attributes = reader.attributes();
                 //判断是否存在属性"ID"
                 if(attributes.hasAttribute("ID"))
                 {
                     MQTT_STRU single_mqtt_stru;
                     QMap<QString ,QString> single_mqtt_map;
                     single_mqtt_stru.id = attributes.value("ID").toString();
                     while(!reader.atEnd())
                     {
                         reader.readNext();
                         if(reader.isStartElement())
                         {
                             if(reader.name() == "IP地址")
                             {
                                 single_mqtt_stru.ip = reader.readElementText();
                             }
                             if(reader.name() == "端口号")
                             {
                                 single_mqtt_stru.port = reader.readElementText();
                             }
                             if(reader.name() == "订阅")
                             {
                                 single_mqtt_map.clear();
                                 while(!reader.atEnd())
                                 {
                                     reader.readNext();
                                     if(reader.isStartElement())
                                     {
                                         if(reader.name() == "主题")
                                         {
                                             QXmlStreamAttributes attbutes = reader.attributes();
                                             if(attbutes.hasAttribute("topical")&& attbutes.hasAttribute("variable"))
                                             {
                                                 single_mqtt_map.insert(attbutes.value("topical").toString(),attbutes.value("variable").toString());
                                             }
                                         }
                                     }
                                     if(reader.isEndElement())
                                     {
                                         if(reader.name() == "订阅")
                                         {
                                             break;
                                         }
                                     }
                                 }
                             }
                         }
                         if(reader.isEndElement())
                         {
                             if(reader.name() == "网络编号")
                             {
                                 break;
                             }
                         }
                     }
                     single_mqtt_stru.topicalMap = single_mqtt_map;
                     if(!cmdxmlMap.contains(single_mqtt_stru.id))
                     {
                         cmdxmlMap.insert(single_mqtt_stru.id,single_mqtt_stru);
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

 void HandleHWXml::ReadHWType(QXmlStreamReader &reader, QString strElement, HWCONFIGXML_WEB_MAP &cmdxmlMap)
 {
     cmdxmlMap.hwconfigWebMap.clear();
     while(!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "WEB地址")
             {
                 HWCONFIGXML_WEB_STRU hwWeb;
                 while(!reader.atEnd())
                 {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "编号")
                        {
                            //todo
                            hwWeb.Id = reader.readElementText();
                        }
                        else if(reader.name() == "URL")
                        {
                            //todo
                              hwWeb.URL = reader.readElementText();
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "WEB地址")
                        {
                            break;
                        }
                    }
                 }
                 if(!cmdxmlMap.hwconfigWebMap.contains(hwWeb.Id))
                 {
                     cmdxmlMap.hwconfigWebMap.insert(hwWeb.Id,hwWeb);
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

 void HandleHWXml::ReadHWType(QXmlStreamReader &reader, QString strElement, QMap<QString, HWCONFIGXML_CSYS_CONFIG_STRU> &cmdxmlMap)
 {
     cmdxmlMap.clear();
     while(!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "坐标")
             {
                 HWCONFIGXML_CSYS_CONFIG_STRU hwCsys;
                 QXmlStreamAttributes attributes = reader.attributes();
                 //判断是否存在属性"note"
                 if(attributes.hasAttribute("ID"))
                 {
                    hwCsys.Id = attributes.value("ID").toString();
                 }
                 while(!reader.atEnd())
                 {
                    reader.readNext();
                    if(reader.isStartElement())
                    {
                        if(reader.name() == "起点极限")
                        {
                            //todo
                            hwCsys.stalim = reader.readElementText().toDouble();
                        }
                        else if(reader.name() == "终点极限")
                        {
                            //todo
                            hwCsys.endlim = reader.readElementText().toDouble();
                        }
                        else if(reader.name() == "轴臂")
                        {
                            while(!reader.atEnd())
                            {
                               reader.readNext();
                               if(reader.isStartElement())
                               {
                                   if(reader.name() == "轴臂信息")
                                   {
                                       HWCONFIGXML_ARMINFO_STRU hwArmInfo;
                                       while(!reader.atEnd())
                                       {
                                          reader.readNext();
                                          if(reader.isStartElement())
                                          {
                                              if(reader.name() == "ID")
                                              {
                                                  //todo
                                                  hwArmInfo.Id = reader.readElementText();
                                              }
                                              else if(reader.name() == "臂长")
                                              {
                                                  //todo
                                                  hwArmInfo.armlen = reader.readElementText().toDouble();
                                              }
                                              else if(reader.name() == "减速比")
                                              {
                                                  //todo
                                                  hwArmInfo.decrat = reader.readElementText().toDouble();
                                              }
                                              else if(reader.name() == "电子齿轮比")
                                              {
                                                  //todo
                                                  hwArmInfo.elegearat = reader.readElementText().toDouble();
                                              }
                                          }
                                          if(reader.isEndElement())
                                          {
                                              if(reader.name() == "轴臂信息")
                                              {
                                                  if(!hwCsys.hwArmInfoMap.contains(hwArmInfo.Id))
                                                  {
                                                      hwCsys.hwArmInfoMap.insert(hwArmInfo.Id,hwArmInfo);
                                                  }
                                                  break;
                                              }
                                          }
                                       }
                                   }
                               }
                               if(reader.isEndElement())
                               {
                                   if(reader.name() == "轴臂")
                                   {
                                       break;
                                   }
                               }
                            }
                        }
                    }
                    if(reader.isEndElement())
                    {
                        if(reader.name() == "坐标")
                        {
                            if(!cmdxmlMap.contains(hwCsys.Id))
                            {
                                cmdxmlMap.insert(hwCsys.Id,hwCsys);
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

//QMap<QString,HWCONFIGXML_IO_STRU> hwIOMap;
//QMap<QString,HWCONFIGXML_SERINPUT_STRU> hwSerInputMap;
void HandleHWXml::ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_INPUT_MAP &cmdxmlMap,HWCONFIGXML_MODEL_MAP modelMap)
{
    cmdxmlMap.hwIOMap.clear();
    cmdxmlMap.hwSerInputMap.clear();
    int iBytePos = 0;//先初始化为零
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "IO部分")//配置输入模块数量
            {
                HWCONFIGXML_IO_STRU cmdconStru;
                cmdconStru.iBytePos = iBytePos;
                QString str = reader.name().toString();
                ReadHWStru(reader,str,cmdconStru);
                if(cmdconStru.strType.compare("EC") == 0)//判断是IO部分
                    iBytePos += cmdconStru.iByteNum ;//计算输入模块数量
                if(!cmdxmlMap.hwIOMap.contains(cmdconStru.id))
                {
                    cmdxmlMap.hwIOMap.insert(cmdconStru.id,cmdconStru);
                }
            }
            else if(reader.name() == "伺服部分")
            {
                HWCONFIGXML_SERINPUT_STRU cmdconStru;
                cmdconStru.iBytePos = iBytePos;
                QString str = reader.name().toString();
                ReadHWStru(reader,str,cmdconStru);
                if(modelMap.hwModelMap.contains(cmdconStru.modelType))
                    cmdconStru.iByteNum = modelMap.hwModelMap[cmdconStru.modelType]["INPUT"].iByteNum;//连续取两次键值（从最外层开始取值）
                iBytePos += cmdconStru.iByteNum ;

                if(!cmdxmlMap.hwSerInputMap.contains(cmdconStru.id))
                {
                    cmdxmlMap.hwSerInputMap.insert(cmdconStru.id,cmdconStru);
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
void HandleHWXml::ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_OUTPUT_MAP &cmdxmlMap,HWCONFIGXML_MODEL_MAP modelMap)
{
    cmdxmlMap.hwIOMap.clear();
    cmdxmlMap.hwSerOutputMap.clear();
    int iBytePos = 0;
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "IO部分")
            {
                HWCONFIGXML_IO_STRU cmdconStru;
                cmdconStru.iBytePos = iBytePos;
                QString str = reader.name().toString();
                ReadHWStru(reader,str,cmdconStru);
                iBytePos += cmdconStru.iByteNum ;
                if(!cmdxmlMap.hwIOMap.contains(cmdconStru.id))
                {
                    cmdxmlMap.hwIOMap.insert(cmdconStru.id,cmdconStru);
                }
            }
            else if(reader.name() == "伺服部分")
            {
                HWCONFIGXML_SEROUTPUT_STRU cmdconStru;
                cmdconStru.iBytePos = iBytePos;
                QString str = reader.name().toString();
                ReadHWStru(reader,str,cmdconStru);

                if(modelMap.hwModelMap.contains(cmdconStru.modelType))
                    cmdconStru.iByteNum = modelMap.hwModelMap[cmdconStru.modelType]["OUTPUT"].iByteNum;
                iBytePos += cmdconStru.iByteNum ;

                if(!cmdxmlMap.hwSerOutputMap.contains(cmdconStru.id))
                {
                    cmdxmlMap.hwSerOutputMap.insert(cmdconStru.id,cmdconStru);
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
void HandleHWXml::ReadHWType(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_NETWORK_STRU &cmdxmlstru)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "网口名称")
            {
                  cmdxmlstru.portName = reader.readElementText();//取元素文本
            }
        }
        if(reader.isEndElement())//读到结尾
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void HandleHWXml::ReadHWStru(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_IO_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "ID")
            {
                cmdconVec.id = reader.readElementText();
            }
            else if(reader.name() == "类型")
            {
                //todo
                cmdconVec.strType = reader.readElementText();
            }
            else if(reader.name() == "字节数")
            {
                //todo
                cmdconVec.iByteNum = reader.readElementText().toInt();
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
//伺服控制模式
void HandleHWXml::ReadHWStru(QXmlStreamReader &reader,QString strElement,QMap<QString,HWCONFIGXML_MODEL_STRU> &cmdconMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "INPUT")
            {
                //todo
               HWCONFIGXML_MODEL_STRU  cmdconVec;
               ReadByteInfo(reader,"INPUT",cmdconVec);//解析INPUT节点
               if(!cmdconMap.contains("INPUT"))
                   cmdconMap.insert("INPUT",cmdconVec);
            }
            else if(reader.name() == "OUTPUT")
            {
                //todo
               HWCONFIGXML_MODEL_STRU  cmdconVec;//解析OUTPUT节点
               ReadByteInfo(reader,"OUTPUT",cmdconVec);
               if(!cmdconMap.contains("OUTPUT"))
                   cmdconMap.insert("OUTPUT",cmdconVec);
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

void HandleHWXml::ReadHWStru(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_SERINPUT_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "ID")
            {
                cmdconVec.id = reader.readElementText();
            }
            else if(reader.name() == "模式")
            {
                //todo
                cmdconVec.modelType = reader.readElementText();

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

void HandleHWXml::ReadHWStru(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_SEROUTPUT_STRU &cmdconVec)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "ID")
            {
                cmdconVec.id = reader.readElementText();
            }
            else if(reader.name() == "模式")
            {
                //todo
                cmdconVec.modelType = reader.readElementText();
            }
            else if(reader.name() == "速度系数")
            {
                //todo
                cmdconVec.lead = reader.readElementText().toInt();
                if(cmdconVec.lead < 1)
                    cmdconVec.lead = 1;
            }
            else if(reader.name() == "加减速系数")
            {
                //todo
                cmdconVec.adled = reader.readElementText().toInt();
                if(cmdconVec.adled < 1)
                    cmdconVec.adled = 1;
            }
            else if(reader.name() == "极限原点")
            {
                //todo
                cmdconVec.hwLimitStart = reader.readElementText();
            }
            else if(reader.name() == "原点极限")//新增原点极限节点
            {
                //todo
                cmdconVec.orgLimit = reader.readElementText();
            }
            else if(reader.name() == "极限终点")
            {
                //todo
                cmdconVec.hwLimitEnd = reader.readElementText();
            }
            else if(reader.name() == "起始软极限")
            {
                //todo
                cmdconVec.swLimitStart = reader.readElementText();
            }
            else if(reader.name() == "终点软极限")
            {
                //todo
                cmdconVec.swLimitEnd = reader.readElementText();
            }
            else if(reader.name() == "偏差")
            {
                //todo
                cmdconVec.error = reader.readElementText().toInt();
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
 void HandleHWXml::ReadByteInfo(QXmlStreamReader &reader,QString strElement,HWCONFIGXML_MODEL_STRU &modelInfo)
 {
     while(!reader.atEnd())
     {
         reader.readNext();
         if(reader.isStartElement())
         {
             if(reader.name() == "字节数")
             {
                modelInfo.iByteNum = reader.readElementText().toInt();
             }
             else if(reader.name() == "字节信息")//字节组合节点不解析，直接跳过
             {
                QXmlStreamAttributes attributes = reader.attributes();
                BYTEINFO_STRU byteInfo;
                //判断是否存在属性"note"
                if(attributes.hasAttribute("note"))
                {
                   byteInfo.name = attributes.value("note").toString();
                }
                if(attributes.hasAttribute("key"))
                {
                   byteInfo.key = attributes.value("key").toString();
                }
                if(attributes.hasAttribute("byte"))
                {
                   byteInfo.byteNum = attributes.value("byte").toInt();
                }
                if(attributes.hasAttribute("type"))
                {
                   byteInfo.type = attributes.value("type").toString();
                }

                ReadBitInfo(reader,byteInfo.bitInfoMap);//继续解析下一行位信息节点
                modelInfo.byteInfoVec.append(byteInfo);
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

void HandleHWXml::ReadBitInfo(QXmlStreamReader &reader,QMap<QString ,BITINFO_STRU> &bitInfoMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name() == "位信息")
            {
                QXmlStreamAttributes attributes = reader.attributes();
               BITINFO_STRU bitInfo;
               //判断是否存在属性"name"
               if(attributes.hasAttribute("note"))
               {
                  bitInfo.name = attributes.value("note").toString();
               }
               if(attributes.hasAttribute("key"))
               {
                  bitInfo.key = attributes.value("key").toString();
               }
               if(attributes.hasAttribute("bit"))
               {
                  bitInfo.bit = attributes.value("bit").toInt();
               }

               bitInfoMap.insert(bitInfo.key,bitInfo);
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == "字节信息")
            {
                break;
            }
        }
    }
}
