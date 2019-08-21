#include "waitproperty.h"

WAITProperty::WAITProperty()
{
    ID="-1";
    note="";
    commandString="WAIT";

    information="";
    warning="";
    timeout= -1;
}

bool WAITProperty::SaveXml(QDomDocument *doc)
{
    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() == "Data")
    {
        QDomElement xmlItems = docElem.firstChildElement("程序配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt  = xmlItems.firstChildElement("等待指令集合");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc->elementsByTagName("等待指令");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.firstChildElement("编号").text() == ID)
                    {
                        srailElmt.removeChild(IoNodeList.at(i));
                    }
                }
                QDomElement IO = doc->createElement("等待指令");

                QDomElement IOId = doc->createElement("编号");
                QDomText Idtext1 = doc->createTextNode(ID);
                IOId.appendChild(Idtext1);

                QDomElement IOINFO = doc->createElement("信息");
                QDomText Idtext2 = doc->createTextNode(information);
                IOINFO.appendChild(Idtext2);

                QDomElement IOWarn = doc->createElement("报警");
                QDomText Idtext3 = doc->createTextNode(warning);
                IOWarn.appendChild(Idtext3);

                QDomElement IOTimeout = doc->createElement("超时");
                QDomText Idtext4 = doc->createTextNode(QString::number(timeout,10));
                IOTimeout.appendChild(Idtext4);


                IO.appendChild(IOId);
                IO.appendChild(IOINFO);
                IO.appendChild(IOWarn);
                IO.appendChild(IOTimeout);
                srailElmt.appendChild(IO);
            }
        }
    }

    return true;
}

QString WAITProperty::getWarning()
{
    return warning;
}

void WAITProperty::setWarning(QString val)
{
    warning=val;
}

QString WAITProperty::getInformation()
{
    return information;
}

void WAITProperty::setInformation(QString val)
{
    information=val;
}

int WAITProperty::getTimeOut()
{
    return timeout;
}

void WAITProperty::setTimeout(int val)
{
    timeout=val;
}

void WAITProperty::sendval(QString val)
{
    Q_UNUSED(val)

    QString str=commandString+" "+ID;
    str=str+"\n"+note;

    emit  ValueChange(str);
}

void WAITProperty::copyPropertyChild(LibProperyBase *inPro)
{
    WAITProperty *in=qobject_cast<WAITProperty*>(inPro);
    ID =in->ID;
    note = in->note;

    information=in->information;
    warning=in->warning;
    timeout=in->timeout;
    setID(ID);
}
