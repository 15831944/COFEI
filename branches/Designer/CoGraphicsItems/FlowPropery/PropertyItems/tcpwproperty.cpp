#include "tcpwproperty.h"

TCPWProperty::TCPWProperty()
{
    ID="-1";
    note="";
    commandString="TCP_W";

    netID="-1";
    dataSave="";
    IP="";
    data="";
}

QString TCPWProperty::getNetID()
{
    return netID;
}

void TCPWProperty::setNetID(QString val)
{
    netID=val;
}

QString TCPWProperty::getDataSave()
{
    return dataSave;
}

void TCPWProperty::setDataSave(QString val)
{
    dataSave=val;
}

QString TCPWProperty::getIP()
{
    return IP;
}

void TCPWProperty::setIP(QString val)
{
    IP=val;
}

QString TCPWProperty::getData()
{
    return data;
}

void TCPWProperty::setData(QString val)
{
    data=val;
}

void TCPWProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str = QString("%1 %2\n%3").arg(commandString).arg(ID).arg(note);
    emit ValueChange(str);
}

void TCPWProperty::copyPropertyChild(LibProperyBase *inPro)
{
    TCPWProperty *in=qobject_cast<TCPWProperty*>(inPro);
    ID =in->ID;
    note = in->note;

    netID=in->netID;
    dataSave=in->dataSave;
    IP=in->IP;
    data=in->data;

    setID(ID);
}

bool TCPWProperty::SaveXml(QDomDocument *doc)
{
    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() != "Data")
    {
       return false;
    }

    QDomElement xmlItems = docElem.firstChildElement("程序配置");
    if(xmlItems.isNull())
    {
        return false;
    }


    QDomElement parentTop  = xmlItems.firstChildElement("通信部分");
    if(parentTop.isNull())
    {
        return false;
    }

    QDomElement parentLow  = parentTop.firstChildElement("网络通信");

    if(!parentLow.isNull())
    {

        QDomNodeList childScriptNode=parentLow.childNodes();
        for(int i=0;i<childScriptNode.count();++i)
        {
            QDomElement child=childScriptNode.at(i).toElement();
            QDomElement codenum=child.firstChildElement("编号");
            if(codenum.text()==ID)
            {
                parentLow.removeChild(child);
                break;
            }
        }
    }
    else
    {
        parentLow=doc->createElement("网络通信");
        parentTop.appendChild(parentLow);
    }



    QDomElement childtop=doc->createElement("通信数据");

    QDomElement codenum=doc->createElement("编号");
    codenum.appendChild(doc->createTextNode(ID));
    childtop.appendChild(codenum);

    QDomElement netidnode=doc->createElement("网络编号");
    netidnode.appendChild(doc->createTextNode(netID));
    childtop.appendChild(netidnode);

    QDomElement datasnode=doc->createElement("数据保存");
    datasnode.appendChild(doc->createTextNode(dataSave));
    childtop.appendChild(datasnode);

    QDomElement ipnode=doc->createElement("IP地址");
    ipnode.appendChild(doc->createTextNode(IP));
    childtop.appendChild(ipnode);

    QDomElement datanode=doc->createElement("数据");
    datanode.appendChild(doc->createTextNode(data));
    childtop.appendChild(datanode);

    parentLow.appendChild(childtop);


    return true;
}
