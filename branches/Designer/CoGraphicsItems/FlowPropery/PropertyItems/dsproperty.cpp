#include "dsproperty.h"

DSProperty::DSProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="DS";


    IN_IO_0="";
    IN_IO_0_Value=0;

    IN_IO_1="";
    IN_IO_1_Value=0;
}

QString DSProperty::getXmlString()
{
    return "";
}

//Text Show
void DSProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+ID;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

QString DSProperty::getIN_IO_0()
{
    return IN_IO_0;
}
void DSProperty::setIN_IO_0(QString val)
{
    IN_IO_0=val;
    sendval();
}

int DSProperty::getIN_IO_0_Value()
{
    return IN_IO_0_Value;
}
void DSProperty::setIN_IO_0_Value(int val)\
{
    IN_IO_0_Value=val;
     sendval();
}


QString DSProperty::getIN_IO_1()
{
    return IN_IO_1;
}
void DSProperty::setIN_IO_1(QString val)
{
    IN_IO_1=val;
     sendval();
}

int DSProperty::getIN_IO_1_Value()
{
    return IN_IO_1_Value;
}
void DSProperty::setIN_IO_1_Value(int val)\
{
    IN_IO_1_Value=val;
     sendval();
}


void DSProperty::copyPropertyChild(LibProperyBase *inPro)
{
    DSProperty *in=qobject_cast<DSProperty*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;
    commandString = in->commandString;

    IN_IO_0=in->IN_IO_0;
    IN_IO_0_Value=in->IN_IO_0_Value;

    IN_IO_1=in->IN_IO_1;
    IN_IO_1_Value=in->IN_IO_1_Value;
    setID(ID);
}

bool DSProperty::SaveXml(QDomDocument *doc)
{

    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() == "Data")
    {
        QDomElement xmlItems = docElem.firstChildElement("程序配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt  = xmlItems.firstChildElement("启动按钮配置");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc->elementsByTagName("Start");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.attribute("Mode") == ID)
                    {
                        srailElmt.removeChild(IoNodeList.at(i));
                    }
                }
                QDomElement IO = doc->createElement("Start");
                QDomAttr bitname = doc->createAttribute("Mode");
                bitname.setValue(ID);
                IO.setAttributeNode(bitname);

                QDomElement IN_IO1 = doc->createElement("IN_IO");
                QDomAttr bit_1 = doc->createAttribute("bit");
                bit_1.setValue(IN_IO_0);
                IN_IO1.setAttributeNode(bit_1);
                QDomAttr bit_2 = doc->createAttribute("value");
                bit_2.setValue(QString("%1").arg(IN_IO_0_Value));
                IN_IO1.setAttributeNode(bit_2);

                QDomElement IN_IO2 = doc->createElement("IN_IO");
                QDomAttr bit_3 = doc->createAttribute("bit");
                bit_3.setValue(IN_IO_1);
                IN_IO2.setAttributeNode(bit_3);
                QDomAttr bit_4 = doc->createAttribute("value");
                bit_4.setValue(QString("%1").arg(IN_IO_1_Value));
                IN_IO2.setAttributeNode(bit_4);

                IO.appendChild(IN_IO1);
                IO.appendChild(IN_IO2);
                srailElmt.appendChild(IO);
            }
        }
    }

    return true;
}
