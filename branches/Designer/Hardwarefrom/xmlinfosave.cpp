#include "xmlinfosave.h"

XMLInfoSave::XMLInfoSave()
{

}
///
/// \brief XMLInfoSave::FileOpen  判断文件是否存在
/// \param path
/// \return
///
bool XMLInfoSave::FileOpen(QString path)
{
    QFile file(path);
    if(!file.exists())
    {
        return false;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }
    return true;
}

void XMLInfoSave::CreateXmlRoot(QString path)
{
    QFile *file = new QFile();
    file->setFileName(path);
    if(!file->exists())
    {
        file->open(QIODevice::WriteOnly);
        QXmlStreamWriter wr(file);
        wr.setAutoFormatting(true);
        wr.writeStartDocument();
        wr.writeStartElement("data");
        wr.writeStartElement("硬件配置");

        wr.writeStartElement("网口配置");
        wr.writeEndElement();

        wr.writeStartElement("串口配置");
        wr.writeEndElement();

        wr.writeStartElement("网络配置");
        wr.writeEndElement();

        wr.writeStartElement("伺服控制模式");
        AddElement(&wr,"1");
        AddElement(&wr,"8");
        AddTorqueElement(&wr);
        wr.writeEndElement();//私服控制模式

        wr.writeStartElement("OUTPUT");
        wr.writeEndElement();

        wr.writeStartElement("INPUT");
        wr.writeEndElement();

        wr.writeStartElement("坐标系配置");
        wr.writeEndElement();

        wr.writeStartElement("硬件排序");
        wr.writeEndElement();

        wr.writeEndElement();
        wr.writeEndElement();
        wr.writeEndDocument();

        file->close();
    }
}
///
/// \brief XMLInfoSave::SaveInfoXML 增加轴信息   OUTPUT INPUT
/// \param axisinfo
/// \return   false 文件损坏或保存失败     true 保存成功
///
bool XMLInfoSave::SaveInfoXML(HWCONFIGXML_SEROUTPUT_STRU axisinfo,QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement axisElmt = xmlItems.firstChildElement("OUTPUT");
            if(!axisElmt.isNull())
            {
                QDomNodeList outlist = doc.elementsByTagName("伺服部分");
                for(int i = 0 ;i<outlist.size();i++)
                {
                    QDomElement e = outlist.at(i).toElement();
                    if( e.firstChildElement("ID").text() ==axisinfo.id )
                    {
                        axisElmt.removeChild(outlist.at(i));
                    }
                }
                QDomElement OutPut = doc.createElement("伺服部分");
                QDomElement xmlId = doc.createElement("ID");
                QDomText text = doc.createTextNode(axisinfo.id);
                xmlId.appendChild(text);
                QDomElement xmlhwstart = doc.createElement("极限原点");
                text = doc.createTextNode(axisinfo.hwLimitStart);
                xmlhwstart.appendChild(text);
                QDomElement xmlhwend = doc.createElement("极限终点");
                text = doc.createTextNode(axisinfo.hwLimitEnd);
                xmlhwend.appendChild(text);
                QDomElement xmlorg = doc.createElement("原点极限");
                QDomText orgtext = doc.createTextNode(axisinfo.orgLimit);
                xmlorg.appendChild(orgtext);

                QDomElement xmlswstart = doc.createElement("起始软极限");
                text = doc.createTextNode(axisinfo.swLimitStart);
                xmlswstart.appendChild(text);
                QDomElement xmlswend = doc.createElement("终点软极限");
                text = doc.createTextNode(axisinfo.swLimitEnd);
                xmlswend.appendChild(text);
                QDomElement xmlAccel = doc.createElement("加减速系数");
                text = doc.createTextNode(QString("%1").arg(axisinfo.adled));
                xmlAccel.appendChild(text);
                QDomElement Accel = doc.createElement("速度系数");
                QDomText acctext = doc.createTextNode(QString("%1").arg(axisinfo.lead));
                Accel.appendChild(acctext);

                QDomElement xmloffsict = doc.createElement("偏差");
                text = doc.createTextNode(QString ("%1").arg(axisinfo.error));
                xmloffsict.appendChild(text);
                QDomElement xmlmode = doc.createElement("模式");
                text = doc.createTextNode(axisinfo.modelType);
                xmlmode.appendChild(text);

                OutPut.appendChild(xmlId);
                OutPut.appendChild(xmlhwstart);
                OutPut.appendChild(xmlhwend);
                OutPut.appendChild(xmlorg);
                OutPut.appendChild(xmlswstart);
                OutPut.appendChild(xmlswend);
                OutPut.appendChild(xmlAccel);
                OutPut.appendChild(Accel);
                OutPut.appendChild(xmloffsict);
                OutPut.appendChild(xmlmode);
                axisElmt.appendChild(OutPut);
            }
            QDomElement InputItems = xmlItems.firstChildElement("INPUT");
            if(!InputItems.isNull())
            {
                QDomNodeList InputNoteList = doc.elementsByTagName("伺服部分");
                for(int i = 0;i <InputNoteList.size(); i++)
                {
                    QDomElement ei = InputNoteList.at(i).toElement();
                    if(ei.firstChildElement("ID").text() == axisinfo.id)
                    {
                        InputItems.removeChild(InputNoteList.at(i));
                    }
                }
                QDomElement inputElmt = doc.createElement("伺服部分");

                QDomElement input_id = doc.createElement("ID");
                QDomText input_text = doc.createTextNode(axisinfo.id);
                input_id.appendChild(input_text);

                QDomElement input_mode = doc.createElement("模式");
                input_text = doc.createTextNode(axisinfo.modelType);
                input_mode.appendChild(input_text);
                inputElmt.appendChild(input_id);
                inputElmt.appendChild(input_mode);
                InputItems.appendChild(inputElmt);
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
///
/// \brief XMLInfoSave::InsertAxisXML
/// \param axisinfo
/// \param State 0 fix,1 insert
/// \param path
/// \return
///
bool XMLInfoSave::InsertAxisXML(HWCONFIGXML_SEROUTPUT_STRU axisinfo, QString FirstElement,QString ID,int State, QString path)
{
    QFile file(path);
    if(!file.exists())
        return false;
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }
    QTextStream Readstream(&file);
    QTextCodec *VCode = QTextCodec::codecForName("UTF-8");
    Readstream.setCodec(VCode);
    QString XMLdateStr = Readstream.readAll();
    file.close();
    QDomDocument doc;
    if(!doc.setContent(XMLdateStr))
    {
        file.close();
        return false;
    }
    QDomElement docElem = doc.documentElement();
    if(docElem.tagName() =="data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");

        if(!xmlItems.isNull())
        {
            if(FirstElement == "OUTPUT")
            {
                QDomElement OutItems = xmlItems.firstChildElement(FirstElement);
                if(!OutItems.isNull())
                {
                    QDomNodeList Notelist = OutItems.childNodes();//doc.elementsByTagName("伺服部分");
                    for(int i = 0;i< Notelist.size();++i)
                    {
                        QDomElement e = Notelist.at(i).toElement();

                        if( e.firstChildElement("ID").text() == ID )
                        {
                            QDomElement OutPut = doc.createElement("伺服部分");

                            QDomElement xmlId = doc.createElement("ID");
                            QDomText text = doc.createTextNode(axisinfo.id);
                            xmlId.appendChild(text);

                            QDomElement xmlId1 = doc.createElement("ID");
                            QDomText text1 = doc.createTextNode(axisinfo.id);
                            xmlId1.appendChild(text1);

                            QDomElement xmlhwstart = doc.createElement("极限原点");
                            text = doc.createTextNode(axisinfo.hwLimitStart);
                            xmlhwstart.appendChild(text);
                            QDomElement xmlhwend = doc.createElement("极限终点");
                            text = doc.createTextNode(axisinfo.hwLimitEnd);
                            xmlhwend.appendChild(text);
                            QDomElement xmlswstart = doc.createElement("起始软极限");
                            text = doc.createTextNode(axisinfo.swLimitStart);
                            xmlswstart.appendChild(text);
                            QDomElement xmlswend = doc.createElement("终点软极限");
                            text = doc.createTextNode(axisinfo.swLimitEnd);
                            xmlswend.appendChild(text);
                            QDomElement xmlAccel = doc.createElement("速度系数");
                            text = doc.createTextNode(QString("%1").arg(axisinfo.adled));
                            xmlAccel.appendChild(text);

                            QDomElement xmloffsict = doc.createElement("偏差");
                            text = doc.createTextNode(QString ("%1").arg(axisinfo.lead));
                            xmloffsict.appendChild(text);
                            QDomElement xmlmode = doc.createElement("模式");
                            text = doc.createTextNode(axisinfo.modelType);
                            xmlmode.appendChild(text);

                            QDomElement xmlmode1 = doc.createElement("模式");
                            text = doc.createTextNode(axisinfo.modelType);
                            xmlmode1.appendChild(text);

                            OutPut.appendChild(xmlId);
                            OutPut.appendChild(xmlhwstart);
                            OutPut.appendChild(xmlhwend);
                            OutPut.appendChild(xmlswstart);
                            OutPut.appendChild(xmlswend);
                            OutPut.appendChild(xmlAccel);
                            OutPut.appendChild(xmloffsict);
                            OutPut.appendChild(xmlmode);
                            OutItems.insertAfter(OutPut,Notelist.at(i));

                            QDomElement inputItems = xmlItems.firstChildElement("INPUT");
                            QDomNodeList Notelistinput;
                            if(!inputItems.isNull())
                            {
                                Notelistinput = inputItems.childNodes();
                                QDomElement axisinput = doc.createElement("伺服部分");
                                axisinput.appendChild(xmlId1);
                                axisinput.appendChild(xmlmode1);
                                inputItems.insertAfter(axisinput,Notelistinput.at(i));
                            }
                            if(State == 0)//fix axix
                            {
                                OutItems.removeChild(Notelist.at(i));
                                inputItems.removeChild(Notelistinput.at(i));
                            }
                            break;
                        }
                    }
                }
            }
            else if(FirstElement == "INPUT")
            {

                QDomElement InPutItems = xmlItems.firstChildElement(FirstElement);
                if(!InPutItems.isNull())
                {
                    QDomNodeList Notelist = InPutItems.childNodes();//doc.elementsByTagName("伺服部分");
                    for(int i = 0;i< Notelist.size();++i)
                    {
                        QDomElement e = Notelist.at(i).toElement();

                        if( e.firstChildElement("ID").text() == ID )
                        {
                            QDomElement OutPut = doc.createElement("伺服部分");

                            QDomElement xmlId = doc.createElement("ID");
                            QDomText text = doc.createTextNode(axisinfo.id);
                            xmlId.appendChild(text);

                            QDomElement xmlId2 = doc.createElement("ID");
                            QDomText text2 = doc.createTextNode(axisinfo.id);
                            xmlId2.appendChild(text2);

                            QDomElement xmlhwstart = doc.createElement("极限原点");
                            text = doc.createTextNode(axisinfo.hwLimitStart);
                            xmlhwstart.appendChild(text);
                            QDomElement xmlhwend = doc.createElement("极限终点");
                            text = doc.createTextNode(axisinfo.hwLimitEnd);
                            xmlhwend.appendChild(text);
                            QDomElement xmlswstart = doc.createElement("起始软极限");
                            text = doc.createTextNode(axisinfo.swLimitStart);
                            xmlswstart.appendChild(text);
                            QDomElement xmlswend = doc.createElement("终点软极限");
                            text = doc.createTextNode(axisinfo.swLimitEnd);
                            xmlswend.appendChild(text);
                            QDomElement xmlAccel = doc.createElement("速度系数");
                            text = doc.createTextNode(QString("%1").arg(axisinfo.adled));
                            xmlAccel.appendChild(text);

                            QDomElement xmloffsict = doc.createElement("偏差");
                            text = doc.createTextNode(QString ("%1").arg(axisinfo.lead));
                            xmloffsict.appendChild(text);
                            QDomElement xmlmode = doc.createElement("模式");
                            text = doc.createTextNode(axisinfo.modelType);
                            xmlmode.appendChild(text);

                            QDomElement xmlmode2 = doc.createElement("模式");
                            text = doc.createTextNode(axisinfo.modelType);
                            xmlmode2.appendChild(text);

                            QDomElement axisinput = doc.createElement("伺服部分");
                            axisinput.appendChild(xmlId2);
                            axisinput.appendChild(xmlmode2);

                            InPutItems.insertAfter(axisinput,Notelist.at(i));

                            OutPut.appendChild(xmlId);
                            OutPut.appendChild(xmlhwstart);
                            OutPut.appendChild(xmlhwend);
                            OutPut.appendChild(xmlswstart);
                            OutPut.appendChild(xmlswend);
                            OutPut.appendChild(xmlAccel);
                            OutPut.appendChild(xmloffsict);
                            OutPut.appendChild(xmlmode);


                            QDomElement OutPuttItems = xmlItems.firstChildElement("OUTPUT");
                            QDomNodeList Notelistinput;
                            if(!OutPuttItems.isNull())
                            {
                                Notelistinput = OutPuttItems.childNodes();
                                OutPuttItems.insertAfter(OutPut,Notelistinput.at(i));

                            }
                            if(State == 0)//fix axix
                            {
                                OutPuttItems.removeChild(Notelistinput.at(i));
                                InPutItems.removeChild(Notelistinput.at(i));
                            }
                            break;

                        }

                    }
                }
            }
            if (!file.open(QFile::WriteOnly | QFile::Truncate))  //if file is open already Return False;
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
    }
    return true;
}
///
/// \brief XMLInfoSave::DeleteInfoXML  删除节点
/// \param FirstElement  大节点
/// \param TagAtirtname  小节点
/// \param ID    id
/// \param State  0 find element text,1 find attribute value
/// \return  true is ok
///
bool XMLInfoSave::DeleteInfoXML(QString FirstElement,QString Tagname,QString ID, int State,QString path)
{

    QFile file(path);
    if(!file.exists())
        return false;
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }
    QTextStream Readstream(&file);
    QTextCodec *VCode = QTextCodec::codecForName("UTF-8");
    Readstream.setCodec(VCode);
    QString XMLdateStr = Readstream.readAll();
    file.close();
    QDomDocument doc;
    if(!doc.setContent(XMLdateStr))
    {
        file.close();
        return false;
    }
    QDomElement docElem = doc.documentElement();
    if(docElem.tagName() =="data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement OutItems = xmlItems.firstChildElement(FirstElement);
            if(!OutItems.isNull())
            {

                QDomNodeList Notelist = doc.elementsByTagName(Tagname);
                for(int i = 0;i< Notelist.size();++i)
                {
                    QDomElement e = Notelist.at(i).toElement();
                    if(State == 0)
                    {
                        if( e.firstChildElement("ID").text() == ID )
                        {
                            OutItems.removeChild(Notelist.at(i));
                        }
                    }
                    else
                    {
                        if(e.attributeNode("ID").value() == ID )
                        {
                            OutItems.removeChild(Notelist.at(i));
                        }
                    }
                }
            }
        }
    }
    if (!file.open(QFile::WriteOnly | QFile::Truncate))  //if file is open already Return False;
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
///
/// \brief XMLInfoSave::SaveIoXML 增加IO
/// \param ioinfo
/// \return
///
bool XMLInfoSave::SaveIoXML(HWCONFIGXML_IO_STRU ioinfo,QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            if(ioinfo.id.contains("Q"))
            {
                QDomElement srailElmt  = xmlItems.firstChildElement("OUTPUT");
                if(!srailElmt.isNull())
                {
                    QDomNodeList IoNodeList= doc.elementsByTagName("IO部分");
                    for(int i = 0;i < IoNodeList.size();i++)
                    {
                        QDomElement IoE = IoNodeList.at(i).toElement();
                        if(IoE.firstChildElement("ID").text() == ioinfo.id)
                        {
                            srailElmt.removeChild(IoNodeList.at(i));
                        }
                    }
                    QDomElement IO = doc.createElement("IO部分");

                    QDomElement IOId = doc.createElement("ID");
                    QDomText Idtext = doc.createTextNode(ioinfo.id);
                    IOId.appendChild(Idtext);

                    QDomElement tpye = doc.createElement("类型");
                    QDomText typeText = doc.createTextNode(ioinfo.strType);
                    tpye.appendChild(typeText);

                    QDomElement bytenum = doc.createElement("字节数");
                    QDomText bytetext = doc.createTextNode(QString("%1").arg(ioinfo.iByteNum));
                    bytenum.appendChild(bytetext);

                    IO.appendChild(IOId);
                    IO.appendChild(tpye);
                    IO.appendChild(bytenum);

                    srailElmt.appendChild(IO);
//                   QDomElement InputIo = xmlItems.firstChildElement("INPUT");
//                   if(!InputIo.isNull())
//                   {
//                       QDomNodeList IoINPUTNodeList= doc.elementsByTagName("IO部分");
//                       for(int i = 0;i < IoINPUTNodeList.size();i++)
//                       {
//                           QDomElement IoE = IoINPUTNodeList.at(i).toElement();
//                           if(IoE.firstChildElement("ID").text() == ioinfo.id)
//                           {
//                               InputIo.removeChild(IoINPUTNodeList.at(i));
//                           }
//                       }
//                        QDomElement IOInPut = doc.createElement("IO部分");
//                        InputIo.appendChild(IOInPut);
//                   }

                }
            }
            else if(ioinfo.id.contains("E"))
            {
                QDomElement srailElmt  = xmlItems.firstChildElement("INPUT");
                if(!srailElmt.isNull())
                {
                    QDomNodeList IoNodeList= doc.elementsByTagName("IO部分");
                    for(int i = 0;i < IoNodeList.size();i++)
                    {
                        QDomElement IoE = IoNodeList.at(i).toElement();
                        if(IoE.firstChildElement("ID").text() == ioinfo.id)
                        {
                            srailElmt.removeChild(IoNodeList.at(i));
                        }
                    }
                    QDomElement IO = doc.createElement("IO部分");

                    QDomElement IOId = doc.createElement("ID");
                    QDomText Idtext = doc.createTextNode(ioinfo.id);
                    IOId.appendChild(Idtext);

                    QDomElement tpye = doc.createElement("类型");
                    QDomText typeText = doc.createTextNode(ioinfo.strType);
                    tpye.appendChild(typeText);

                    QDomElement bytenum = doc.createElement("字节数");
                    QDomText bytetext = doc.createTextNode(QString("%1").arg(ioinfo.iByteNum));
                    bytenum.appendChild(bytetext);

                    IO.appendChild(IOId);
                    IO.appendChild(tpye);
                    IO.appendChild(bytenum);

                    srailElmt.appendChild(IO);
//                   QDomElement InputIo = xmlItems.firstChildElement("OUTPUT");
//                   if(!InputIo.isNull())
//                   {
//                       QDomNodeList IoINPUTNodeList= doc.elementsByTagName("IO部分");
//                       for(int i = 0;i < IoINPUTNodeList.size();i++)
//                       {
//                           QDomElement IoE = IoINPUTNodeList.at(i).toElement();
//                           if(IoE.firstChildElement("ID").text() == ioinfo.id)
//                           {
//                               InputIo.removeChild(IoINPUTNodeList.at(i));
//                           }
//                       }
//                        QDomElement IOInPut = doc.createElement("IO部分");
//                        InputIo.appendChild(IOInPut);
//                   }

                }
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

bool XMLInfoSave::InsetIoXML(HWCONFIGXML_IO_STRU ioinfo, QString oldID, int State,QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            if(ioinfo.id.contains("Q"))
            {
                srailElmt  = xmlItems.firstChildElement("OUTPUT");
            }
            else
            {
                srailElmt = xmlItems.firstChildElement("INPUT");
            }
            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc.elementsByTagName("IO部分");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.firstChildElement("ID").text() == oldID)
                    {
                        QDomElement IO = doc.createElement("IO部分");

                        QDomElement IOId = doc.createElement("ID");
                        QDomText Idtext = doc.createTextNode(ioinfo.id);
                        IOId.appendChild(Idtext);

                        QDomElement tpye = doc.createElement("类型");
                        QDomText typeText = doc.createTextNode(ioinfo.strType);
                        tpye.appendChild(typeText);

                        QDomElement bytenum = doc.createElement("字节数");
                        QDomText bytetext = doc.createTextNode(QString("%1").arg(ioinfo.iByteNum));
                        bytenum.appendChild(bytetext);

                        IO.appendChild(IOId);
                        IO.appendChild(tpye);
                        IO.appendChild(bytenum);
                        srailElmt.insertAfter(IO,IoNodeList.at(i));
                        if(State == 0) //fix
                        {
                            srailElmt.removeChild(IoNodeList.at(i));

                        }
                        break;
                    }
                }

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

bool XMLInfoSave::DeleteIoXML(HWCONFIGXML_IO_STRU ioinfo)
{
    Q_UNUSED(ioinfo);
    return false;
}

bool XMLInfoSave::SaveSerialXML(HWCONFIGXML_COMPORT_STRU serialinfo,QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement SerialElmt = xmlItems.firstChildElement("串口配置");
            if(!SerialElmt.isNull())
            {
                QDomNodeList SerialNodeList = SerialElmt.elementsByTagName("串口编号");
                for(int i = 0; i < SerialNodeList.size();i++)
                {
                    QDomElement eSerial = SerialNodeList.at(i).toElement();
                    if(eSerial.attributeNode("ID").value() == serialinfo.Id)
                    {
                        SerialElmt.removeChild(SerialNodeList.at(i));
                    }
                }
                QDomElement serialId = doc.createElement("串口编号");
                QDomAttr IdAttr = doc.createAttribute("ID");
                IdAttr.setValue(serialinfo.Id);
                serialId.setAttributeNode(IdAttr);

                QDomElement portName = doc.createElement("端口名");
                QDomText portNametext = doc.createTextNode(serialinfo.portName);
                portName.appendChild(portNametext);

                QDomElement headenddata = doc.createElement("头尾设置");
                QDomText headenddatatext = doc.createTextNode(serialinfo.headendData);
                headenddata.appendChild(headenddatatext);

                QDomElement buadrate = doc.createElement("波特率");
                QDomText buadrateText = doc.createTextNode(serialinfo.baudRate);
                buadrate.appendChild(buadrateText);

                serialId.appendChild(portName);
                serialId.appendChild(buadrate);
                serialId.appendChild(headenddata);

                SerialElmt.appendChild(serialId);

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

bool XMLInfoSave::SaveTcpXML(HWCONFIGXML_INTNET_STRU tcpinfo,QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement NetworklElmt = xmlItems.firstChildElement("网络配置");
            if(!NetworklElmt.isNull())
            {
                QDomNodeList NetworkNodeList = NetworklElmt.elementsByTagName("网络编号");
                for(int i = 0; i < NetworkNodeList.size(); ++i)
                {
                    QDomElement eNetwork = NetworkNodeList.at(i).toElement();
                    if(eNetwork.attributeNode("ID").value() == tcpinfo.Id)
                    {
                        NetworklElmt.removeChild(NetworkNodeList.at(i));
                    }
                }
                QDomElement NetId = doc.createElement("网络编号");
                QDomAttr Idval = doc.createAttribute("ID");
                Idval.setValue(tcpinfo.Id);
                NetId.setAttributeNode(Idval);

                QDomElement TcpType = doc.createElement("协议类型");
                if(tcpinfo.agrType == "")
                {
                    tcpinfo.agrType = "TCP";
                }
                QDomText TcpTypeText = doc.createTextNode(tcpinfo.agrType);
                TcpType.appendChild(TcpTypeText);

                QDomElement ServerType = doc.createElement("服务类型");
                QDomText ServerTypeText = doc.createTextNode(tcpinfo.serType);
                ServerType.appendChild(ServerTypeText);

                QDomElement TcpIp = doc.createElement("IP地址");
                QDomText TcpIpText = doc.createTextNode(tcpinfo.ipAdd);
                TcpIp.appendChild(TcpIpText);

                QDomElement PortName = doc.createElement("端口号");
                QDomText PortNameText = doc.createTextNode(tcpinfo.portName);
                PortName.appendChild(PortNameText);

                NetId.appendChild(TcpType);
                NetId.appendChild(ServerType);
                NetId.appendChild(TcpIp);
                NetId.appendChild(PortName);
                NetworklElmt.appendChild(NetId);


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

bool XMLInfoSave::SaveAirXML(HWCONFIGXML_ARMINFO_STRU arminfo,QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement NetworklElmt = xmlItems.firstChildElement("关节配置");
            if(!NetworklElmt.isNull())
            {
                QDomNodeList eJogNodeList = NetworklElmt.elementsByTagName("关节编号");
                for(int i = 0; i < eJogNodeList.size(); ++i)
                {
                    QDomElement eJ = eJogNodeList.at(i).toElement();
                    if(eJ.attributeNode("ID").value() == arminfo.Id)
                    {
                        NetworklElmt.removeChild(eJogNodeList.at(i));
                    }
                }
                QDomElement armElm = doc.createElement("关节编号");
                QDomAttr armattr = doc.createAttribute("ID");
                armattr.setValue(arminfo.Id);
                armElm.setAttributeNode(armattr);

                QDomElement ArmLength = doc.createElement("臂长");
                QDomText ArmLengthText = doc.createTextNode(QString("%1").arg(arminfo.armlen));
                ArmLength.appendChild(ArmLengthText);

                QDomElement Decrat = doc.createElement("减速比");
                QDomText DecratText = doc.createTextNode(QString("%1").arg(arminfo.decrat));
                Decrat.appendChild(DecratText);

                QDomElement Elegearat = doc.createElement("电子齿轮比");
                QDomText ElegearatText = doc.createTextNode(QString("%1").arg(arminfo.elegearat));
                Elegearat.appendChild(ElegearatText);
                armElm.appendChild(ArmLength);
                armElm.appendChild(Decrat);
                armElm.appendChild(Elegearat);
                NetworklElmt.appendChild(armElm);
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
///
/// \brief XMLInfoSave::SaveCoordinateXML
/// \param coorinfo 坐标信息
/// \param path   地址
/// \param ArmMap 轴关节信息集合
/// \return
///
bool XMLInfoSave::SaveCoordinateXML(COORDINATESTUR coorinfo,QString path, QMap<QString,HWCONFIGXML_ARMINFO_STRU>ArmMap)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement NetworklElmt = xmlItems.firstChildElement("坐标系配置");
            if(!NetworklElmt.isNull())
            {
                QDomNodeList CoorNodeList = NetworklElmt.elementsByTagName("坐标");
                for(int i = 0 ;i < CoorNodeList.size(); i++)
                {
                    QDomElement eCoor = CoorNodeList.at(i).toElement();
                    if(eCoor.attributeNode("ID").value() == coorinfo.coordname)
                    {
                        NetworklElmt.removeChild(CoorNodeList.at(i));
                    }
                }
                QDomElement coorElm = doc.createElement("坐标");
                QDomAttr coorattr = doc.createAttribute("ID");
                coorattr.setValue(coorinfo.coordname);
                coorElm.setAttributeNode(coorattr);

                QDomElement startElm = doc.createElement("起点极限");
                QDomText startElmText = doc.createTextNode(QString("%1").arg(coorinfo.limtstart));
                startElm.appendChild(startElmText);

                QDomElement endElm = doc.createElement("终点极限");
                QDomText endElmText = doc.createTextNode(QString("%1").arg(coorinfo.limtend));
                endElm.appendChild(endElmText);

                QDomElement arm = doc.createElement("轴臂");
                //分割包含轴；
                QStringList AxisList;
                AxisList = coorinfo.contain.split(",");
                for(int d = 0; d < AxisList.size(); ++d)
                {
                    if(ArmMap.contains(QString("轴臂:%1").arg(AxisList[d])))
                    {
                        QDomElement containElm = doc.createElement("轴臂信息");
                        QDomElement armId = doc.createElement("ID");
                        QDomText armIdText = doc.createTextNode(ArmMap[QString("轴臂:%1").arg(AxisList[d])].Id);
                        armId.appendChild(armIdText);
                        QDomElement armlength = doc.createElement("臂长");
                        QDomText lengthText = doc.createTextNode(QString("%1").arg(ArmMap[QString("轴臂:%1").arg(AxisList[d])].armlen));
                        armlength.appendChild(lengthText);
                        QDomElement dcc = doc.createElement("减速比");
                        QDomText dccText = doc.createTextNode(QString("%1").arg(ArmMap[QString("轴臂:%1").arg(AxisList[d])].decrat));
                        dcc.appendChild(dccText);
                        QDomElement Electronic = doc.createElement("电子齿轮比");
                        QDomText ElectronicText = doc.createTextNode(QString("%1").arg(ArmMap[QString("轴臂:%1").arg(AxisList[d])].elegearat));
                        Electronic.appendChild(ElectronicText);
                        containElm.appendChild(armId);
                        containElm.appendChild(armlength);
                        containElm.appendChild(dcc);
                        containElm.appendChild(Electronic);
                        arm.appendChild(containElm);

                    }
                }

                coorElm.appendChild(startElm);
                coorElm.appendChild(endElm);
                coorElm.appendChild(arm);
                NetworklElmt.appendChild(coorElm);
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

void XMLInfoSave::AddElement(QXmlStreamWriter* wr,QString mode)
{
    wr->writeStartElement("模式类型");/////---ff-
    wr->writeAttribute("ID",mode);
    wr->writeTextElement("NOTE","快速定位模式");
    wr->writeStartElement("OUTPUT");//////--rrr-
    wr->writeTextElement("字节数","19");
    wr->writeStartElement("字节组合");/////--gg---
    wr->writeStartElement("字节信息");///--22--
    wr->writeAttribute("note","Contrlworld");
    wr->writeAttribute("key","Contrlworld");
    wr->writeAttribute("type","U16");
    wr->writeAttribute("byte","2");
    wr->writeStartElement("位信息");//1
    wr->writeAttribute("note","开关");
    wr->writeAttribute( "key","Switchon" );
    wr->writeAttribute("bit","0");
    wr->writeEndElement();//1

    wr->writeStartElement("位信息");//2
    wr->writeAttribute("note","Power_enable");
    wr->writeAttribute( "key","Poweron" );
    wr->writeAttribute("bit","1");
    wr->writeEndElement();//2

    wr->writeStartElement("位信息");//3
    wr->writeAttribute("note","停止");
    wr->writeAttribute( "key","Stop" );
    wr->writeAttribute("bit","2");
    wr->writeEndElement();//3

    wr->writeStartElement("位信息");//4
    wr->writeAttribute("note","Enable_operation");
    wr->writeAttribute( "key","Operation" );
    wr->writeAttribute("bit","3");
    wr->writeEndElement();//4

    wr->writeStartElement("位信息");//5
    wr->writeAttribute("note","New_set_point");
    wr->writeAttribute( "key","Run" );
    wr->writeAttribute("bit","4");
    wr->writeEndElement();//5

    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Change_immediately");
    wr->writeAttribute( "key","Immchange" );
    wr->writeAttribute("bit","5");
    wr->writeEndElement();//6

    wr->writeStartElement("位信息");//7
    wr->writeAttribute("note","Absolute_relative");
    wr->writeAttribute( "key","Absolute" );
    wr->writeAttribute("bit","6");
    wr->writeEndElement();//7

    wr->writeStartElement("位信息");//8
    wr->writeAttribute("note","Restet_Fault");
    wr->writeAttribute( "key","Reset" );
    wr->writeAttribute("bit","7");
    wr->writeEndElement();//8

    wr->writeStartElement("位信息");//9
    wr->writeAttribute("note","Halt");
    wr->writeAttribute( "key","Halt" );
    wr->writeAttribute("bit","8");
    wr->writeEndElement();//9

    wr->writeStartElement("位信息");//10
    wr->writeAttribute("note","Change_on_set_point");
    wr->writeAttribute( "key","Changepoint" );
    wr->writeAttribute("bit","9");
    wr->writeEndElement();//10
    wr->writeEndElement();//--22--

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Model");
    wr->writeAttribute( "key","Model" );
    wr->writeAttribute("byte","1");
    wr->writeAttribute("type","I8");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Traget_pos");
    wr->writeAttribute( "key","Trapos" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","I32");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Traget_speed");
    wr->writeAttribute( "key","Traspeed" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","U32");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Accel");
    wr->writeAttribute( "key","Accel" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","U32");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Decel");
    wr->writeAttribute( "key","Decel" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","U32");
    wr->writeEndElement();//1
    wr->writeEndElement();//---gg--
    wr->writeEndElement();//---rr---

    wr->writeStartElement("INPUT");//---ff-
    wr->writeTextElement("字节数","9");
    wr->writeStartElement("字节组合");//--ff--
    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Error_code");
    wr->writeAttribute( "key","Errorcode" );
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","U16");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//---oooo-----
    wr->writeAttribute("note","Statusworld");
    wr->writeAttribute( "key","Statusworld" );
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","U16");

    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Ready_Switch");
    wr->writeAttribute( "key","ReadySw" );
    wr->writeAttribute("bit","0");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Switched_on");
    wr->writeAttribute( "key","Switchedon" );
    wr->writeAttribute("bit","1");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Operation_enabled");
    wr->writeAttribute( "key","Operation" );
    wr->writeAttribute("bit","2");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Fault");
    wr->writeAttribute( "key","Fault" );
    wr->writeAttribute("bit","3");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Voltage_enable");
    wr->writeAttribute( "key","Voltage" );
    wr->writeAttribute("bit","4");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Quick_stop");
    wr->writeAttribute( "key","Stop" );
    wr->writeAttribute("bit","5");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Switch_on_disabled");
    wr->writeAttribute( "key","Switchondisabled" );
    wr->writeAttribute("bit","6");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Warning");
    wr->writeAttribute( "key","Warning" );
    wr->writeAttribute("bit","7");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Remote");
    wr->writeAttribute( "key","Remote" );
    wr->writeAttribute("bit","9");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Inp");
    wr->writeAttribute( "key","Inp" );
    wr->writeAttribute("bit","10");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Internal_limit");
    wr->writeAttribute( "key","Internallimit" );
    wr->writeAttribute("bit","11");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Set_point_acknowledge");
    wr->writeAttribute( "key","Setpointacknowledge" );
    wr->writeAttribute("bit","12");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Following");
    wr->writeAttribute( "key","Following" );
    wr->writeAttribute("bit","13");
    wr->writeEndElement();//6
    wr->writeEndElement();//-------o---
    wr->writeStartElement("字节信息");//
    wr->writeAttribute("note","Model");
    wr->writeAttribute( "key","Model" );
    wr->writeAttribute("byte","1");
    wr->writeAttribute("type","I8");
    wr->writeEndElement();
    wr->writeStartElement("字节信息");
    wr->writeAttribute("note","Current_pos");
    wr->writeAttribute( "key","Curpos" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","I32");
    wr->writeEndElement();
    wr->writeEndElement();///fff
    wr->writeEndElement();//fff
    wr->writeEndElement();//fff
}

void XMLInfoSave::AddTorqueElement(QXmlStreamWriter *wr)
{
    wr->writeStartElement("模式类型");/////---ff-
    wr->writeAttribute("ID","2");
    wr->writeTextElement("NOTE","扭矩模式");
    wr->writeStartElement("OUTPUT");//////--rrr-
    wr->writeTextElement("字节数","17");
    wr->writeStartElement("字节组合");/////--gg---
    wr->writeStartElement("字节信息");///--22--
    wr->writeAttribute("note","Contrlworld");
    wr->writeAttribute("key","Contrlworld");
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","U16");
    wr->writeStartElement("位信息");//1
    wr->writeAttribute("note","Switch_on");
    wr->writeAttribute( "key","Switchon" );
    wr->writeAttribute("bit","0");
    wr->writeEndElement();//1

    wr->writeStartElement("位信息");//2
    wr->writeAttribute("note","Power_enable");
    wr->writeAttribute( "key","Poweron" );
    wr->writeAttribute("bit","1");
    wr->writeEndElement();//2

    wr->writeStartElement("位信息");//3
    wr->writeAttribute("note","Stop");
    wr->writeAttribute( "key","Quick_stop" );
    wr->writeAttribute("bit","2");
    wr->writeEndElement();//3

    wr->writeStartElement("位信息");//4
    wr->writeAttribute("note","Enable_operation");
    wr->writeAttribute( "key","Operation" );
    wr->writeAttribute("bit","3");
    wr->writeEndElement();//4

    wr->writeStartElement("位信息");//5
    wr->writeAttribute("note","New_set_point");
    wr->writeAttribute( "key","Run" );
    wr->writeAttribute("bit","4");
    wr->writeEndElement();//5

    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Change_set_immediately");
    wr->writeAttribute( "key","Immchange" );
    wr->writeAttribute("bit","5");
    wr->writeEndElement();//6

    wr->writeStartElement("位信息");//7
    wr->writeAttribute("note","Absolute_relative");
    wr->writeAttribute( "key","Absolute" );
    wr->writeAttribute("bit","6");
    wr->writeEndElement();//7

    wr->writeStartElement("位信息");//8
    wr->writeAttribute("note","Restet_Fault");
    wr->writeAttribute( "key","Reset" );
    wr->writeAttribute("bit","7");
    wr->writeEndElement();//8

    wr->writeStartElement("位信息");//9
    wr->writeAttribute("note","Halt");
    wr->writeAttribute( "key","Halt" );
    wr->writeAttribute("bit","8");
    wr->writeEndElement();//9

    wr->writeStartElement("位信息");//10
    wr->writeAttribute("note","Change_on_set_point");
    wr->writeAttribute( "key","Changepoint" );
    wr->writeAttribute("bit","9");
    wr->writeEndElement();//10
    wr->writeEndElement();//--22--

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Model");
    wr->writeAttribute( "key","Model" );
    wr->writeAttribute("byte","1");
    wr->writeAttribute("type","I8");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Traget_torque");
    wr->writeAttribute( "key","Tratq" );
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","I16");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Traget_speed");
    wr->writeAttribute( "key","Traspeed" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","U32");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Accel");
    wr->writeAttribute( "key","Accel" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","U32");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Decel");
    wr->writeAttribute( "key","Decel" );
    wr->writeAttribute("byte","4");
    wr->writeAttribute("type","U32");
    wr->writeEndElement();//1
    wr->writeEndElement();//---gg--
    wr->writeEndElement();//---rr---

    wr->writeStartElement("INPUT");//---ff-
    wr->writeTextElement("字节数","7");
    wr->writeStartElement("字节组合");//--ff--
    wr->writeStartElement("字节信息");//1
    wr->writeAttribute("note","Error_code");
    wr->writeAttribute( "key","Errorcode" );
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","U16");
    wr->writeEndElement();//1

    wr->writeStartElement("字节信息");//---oooo-----
    wr->writeAttribute("note","Statusworld");
    wr->writeAttribute( "key","Statusworld" );
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","U16");

    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Ready_Switch");
    wr->writeAttribute( "key","ReadySw" );
    wr->writeAttribute("bit","0");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Switched_on");
    wr->writeAttribute( "key","Switchedon" );
    wr->writeAttribute("bit","1");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Operation_enabled");
    wr->writeAttribute( "key","Operation" );
    wr->writeAttribute("bit","2");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Fault");
    wr->writeAttribute( "key","Fault" );
    wr->writeAttribute("bit","3");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Voltage_enable");
    wr->writeAttribute( "key","Voltage" );
    wr->writeAttribute("bit","4");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Quick_stop");
    wr->writeAttribute( "key","Stop" );
    wr->writeAttribute("bit","5");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Switch_on_disabled");
    wr->writeAttribute( "key","Switchondisabled" );
    wr->writeAttribute("bit","6");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Warning");
    wr->writeAttribute( "key","Warning" );
    wr->writeAttribute("bit","7");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Remote");
    wr->writeAttribute( "key","Remote" );
    wr->writeAttribute("bit","9");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Inp");
    wr->writeAttribute( "key","Inp" );
    wr->writeAttribute("bit","10");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Internal_limit");
    wr->writeAttribute( "key","Internallimit" );
    wr->writeAttribute("bit","11");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Set_point_acknowledge");
    wr->writeAttribute( "key","Setpointacknowledge" );
    wr->writeAttribute("bit","12");
    wr->writeEndElement();//6
    wr->writeStartElement("位信息");//6
    wr->writeAttribute("note","Following");
    wr->writeAttribute( "key","Following" );
    wr->writeAttribute("bit","13");
    wr->writeEndElement();//6
    wr->writeEndElement();//-------o---
    wr->writeStartElement("字节信息");//
    wr->writeAttribute("note","Model");
    wr->writeAttribute( "key","Model" );
    wr->writeAttribute("byte","1");
    wr->writeAttribute("type","I8");
    wr->writeEndElement();
    wr->writeStartElement("字节信息");
    wr->writeAttribute("note","Current_tq");
    wr->writeAttribute( "key","Curtq" );
    wr->writeAttribute("byte","2");
    wr->writeAttribute("type","I16");
    wr->writeEndElement();
    wr->writeEndElement();///fff
    wr->writeEndElement();//fff
    wr->writeEndElement();//fff
}

bool XMLInfoSave::SaveAxisAndIoOrder(QString str, QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt = xmlItems.firstChildElement("硬件排序");

            if(!srailElmt.isNull())
            {
                QDomElement Hardware = doc.createElement("硬件");
                QDomText Hardwaretext = doc.createTextNode(str);
                Hardware.appendChild(Hardwaretext);
                srailElmt.appendChild(Hardware);
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

bool XMLInfoSave::DeleteAxisAndIoOrder(QString str,QString insetstr, QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt = xmlItems.firstChildElement("硬件排序");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc.elementsByTagName("硬件");
                if(insetstr == "")
                {
                    for(int i = 0;i < IoNodeList.size();i++)
                    {
                        QDomElement IoE = IoNodeList.at(i).toElement();
                        if(IoE.text() == str)
                        {
                            srailElmt.removeChild(IoNodeList.at(i));
                            break;
                        }
                    }
                }
                else
                {
                    for(int i = 0;i < IoNodeList.size();i++)
                    {
                        QDomElement IoE = IoNodeList.at(i).toElement();
                        if(IoE.text() == str)
                        {
                            QDomElement Hardware = doc.createElement("硬件");
                            QDomText Hardwaretext = doc.createTextNode(insetstr);
                            Hardware.appendChild(Hardwaretext);
                            srailElmt.insertAfter(Hardware,IoNodeList.at(i));
                            break;
                        }
                    }
                }
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

QVector<QString> XMLInfoSave::GetAxisAndIoOder(QString path)
{
    QVector<QString>HarewareVec;
    HarewareVec.clear();
    QFile file(path);
    if(!file.exists())
    {
        return HarewareVec;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return HarewareVec;
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
        return HarewareVec;
    }
    QDomElement docElem = doc.documentElement();
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt = xmlItems.firstChildElement("硬件排序");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc.elementsByTagName("硬件");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    HarewareVec.push_back(IoE.text());

                }
            }
        }

    }
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
    {
        file.close();
        return HarewareVec;
    }

    QTextStream ts(&file);
    ts.reset();
    ts.setCodec("utf-8");
    doc.save(ts, 4, QDomNode::EncodingFromTextStream);
    file.close();
    return HarewareVec;
}

bool XMLInfoSave::FixAxisAndIoOrder(QString oldName, QString newName, QString path)
{
    QFile file(path);
    if(!file.exists())
    {
        return false;
    }
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }
    if(oldName.contains("IO_"))
    {
        newName = "IO_" + newName;
    }
    else if(oldName.contains("Server_"))
    {
        newName = "Server_" + newName;
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt = xmlItems.firstChildElement("硬件排序");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc.elementsByTagName("硬件");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.text() == oldName)
                    {
                        QDomNode oldnode = IoE.firstChild();
                        IoE.firstChild().setNodeValue(newName);
                        QDomNode newnode = IoE.firstChild();
                        IoE.replaceChild(newnode,oldnode);
                        break;
                    }
                }
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

bool XMLInfoSave::DeleteAllInputOutput(QString path)
{
    QFile file(path);
    if(!file.exists())
        return false;
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }
    QTextStream Readstream(&file);
    QTextCodec *VCode = QTextCodec::codecForName("UTF-8");
    Readstream.setCodec(VCode);
    QString XMLdateStr = Readstream.readAll();
    file.close();
    QDomDocument doc;
    if(!doc.setContent(XMLdateStr))
    {
        file.close();
        return false;
    }
    QDomElement docElem = doc.documentElement();
    if(docElem.tagName() =="data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");

        if(!xmlItems.isNull())
        {
                QDomElement OutItems = xmlItems.firstChildElement("OUTPUT");
                if(!OutItems.isNull())
                {
                    xmlItems.removeChild(OutItems);
//                    QDomNodeList Notelist = doc.elementsByTagName("伺服部分");
//                    for(int i = 0;i< Notelist.size();++i)
//                    {
//                        OutItems.removeChild(Notelist.at(i));

//                    }
                 }
                  QDomElement InputItems = xmlItems.firstChildElement("INPUT");
                  if(!InputItems.isNull())
                  {
//                      QDomNodeList INNodelist = InputItems.elementsByTagName("伺服部分");
//                      for(int j = 0; j<INNodelist.size();++j)
//                      {
//                          InputItems.removeChild(INNodelist.at(j));
//                      }
                      xmlItems.removeChild(InputItems);
                  }
                  QDomElement Out = doc.createElement("OUTPUT");
                  QDomElement IN = doc.createElement("INPUT");
                  xmlItems.appendChild(Out);
                  xmlItems.appendChild(IN);

           }


            if (!file.open(QFile::WriteOnly | QFile::Truncate))  //if file is open already Return False;
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
    return false;

}

bool XMLInfoSave::SaveNetInfo(QString name, QString path)
{
    QFile file(path);
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
    if (docElem.tagName() == "data")
    {
        QDomElement xmlItems = docElem.firstChildElement("硬件配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt = xmlItems.firstChildElement("网口配置");

            if(!srailElmt.isNull())
            {
              xmlItems.removeChild(srailElmt);
            }
            QDomElement netname = doc.createElement("网口配置");
            QDomElement netElm = doc.createElement("网口名称");
            QDomText netext= doc.createTextNode(name);
            netElm.appendChild(netext);
            netname.appendChild(netElm);
            xmlItems.appendChild(netname);
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
