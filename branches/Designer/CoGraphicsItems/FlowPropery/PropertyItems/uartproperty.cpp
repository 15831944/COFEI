#include "uartproperty.h"
#include <QMetaEnum>
UartProperty::UartProperty()
{
    SeriportID = "";
    SavePath = "";
    CommandStr = RS_R;
}

QString UartProperty::getSeriportID()
{
    return SeriportID;
}

void UartProperty::setSeriportID(QString id)
{
    SeriportID = id;
     sendval();
}

QString UartProperty::getSavePath()
{
    return SavePath;
}

void UartProperty::setSavePath(QString path)
{
    SavePath = path;
     sendval();
}

QString UartProperty::getDataCompare()
{
    return DataCompare;
}

void UartProperty::setDataCompare(QString data)
{
    DataCompare = data;
     sendval();
}

QString UartProperty::getCheckStr()
{
    return Start;
}

void UartProperty::setCheckStr(QString str)
{
    Start = str;
    sendval();
}

QString UartProperty::getEndStr()
{
    return End;
}

void UartProperty::setEndStr(QString str)
{
    End = str;
}

UartProperty::UARTCOMMAND UartProperty::getCommandStr()
{
    return CommandStr;
}

void UartProperty::setCommandStr(UartProperty::UARTCOMMAND command)
{
    CommandStr = command;
    sendval();
}

bool UartProperty::SaveXml(QDomDocument *doc)
{
    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() == "Data")
    {
        QDomElement xmlItems = docElem.firstChildElement("程序配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt  = xmlItems.firstChildElement("通信部分");
            QDomElement serialElm = srailElmt.firstChildElement("串口通信");

            if(serialElm.isNull())
            {
                QDomElement el = doc->createElement("串口通信");
                srailElmt.appendChild(el);
            }
            QDomNodeList IoNodeList= doc->elementsByTagName("通信数据");
            for(int i = 0;i < IoNodeList.size();i++)
            {
                QDomElement IoE = IoNodeList.at(i).toElement();
                if(IoE.firstChildElement("编号").text() == ID)
                {
                    serialElm.removeChild(IoNodeList.at(i));
                }
            }
            QDomElement IO = doc->createElement("通信数据");

            QDomElement IOId = doc->createElement("编号");
            QDomText Idtext1 = doc->createTextNode(ID);
            IOId.appendChild(Idtext1);

            QDomElement IOINFO = doc->createElement("串口编号");
            QDomText Idtext2 = doc->createTextNode(SeriportID);
            IOINFO.appendChild(Idtext2);

            QDomElement SaveElm = doc->createElement("数据保存");
            QDomText savetext = doc->createTextNode(SavePath);
            SaveElm.appendChild(savetext);

            QDomElement IOWarn = doc->createElement("数据");
            QDomText Idtext3 = doc->createTextNode(DataCompare);
            IOWarn.appendChild(Idtext3);

            QDomElement IOTimeout = doc->createElement("IP地址");
            QDomText Idtext4 = doc->createTextNode(Start+";"+End);
            IOTimeout.appendChild(Idtext4);


            IO.appendChild(IOId);
            IO.appendChild(IOINFO);
            IO.appendChild(SaveElm);
            IO.appendChild(IOWarn);
            IO.appendChild(IOTimeout);
            serialElm.appendChild(IO);
        }
    }

    return true;
}

void UartProperty::sendval(QString val)
{
    Q_UNUSED(val)
   QMetaEnum matem = QMetaEnum::fromType<UARTCOMMAND>();
   QString str = QString("%1 %2 \n%5").arg(matem.valueToKey(CommandStr)).arg(ID).arg(note);
   emit  ValueChange(str);
}

void UartProperty::copyPropertyChild(LibProperyBase *)
{
//    Q_UNUSED(val)
//    QString str=commandString+" "+threadName;
//    str=str+"\n"+note;
//    emit  ValueChange(str);
}
