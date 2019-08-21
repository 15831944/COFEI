#include "jsproperty.h"

JSProperty::JSProperty()
{
    ID="-1";
    note="";
    commandString="JS";

    scriptName="";
    dataSave="";
    type="";
    typeID="";
}

QString JSProperty::getScriptName()
{
    return scriptName;
}

void JSProperty::setScriptName(QString val)
{
    scriptName=val;
}

QString JSProperty::getDataSave()
{
    return dataSave;
}

void JSProperty::setDataSave(QString val)
{
    dataSave=val;
}

QString JSProperty::getType()
{
    return type;
}

void JSProperty::setType(QString val)
{
    type=val;
}

QString JSProperty::getTypeID()
{
    return typeID;
}

void JSProperty::setTypeID(QString val)
{
    typeID=val;
}

void JSProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str = QString("%1 %2\n%3").arg(commandString).arg(ID).arg(note);
    emit ValueChange(str);
}

void JSProperty::copyPropertyChild(LibProperyBase *inPro)
{
    JSProperty *in=qobject_cast<JSProperty*>(inPro);
    ID =in->ID;
    note = in->note;

    scriptName=in->scriptName;
    dataSave=in->dataSave;
    type=in->type;
    typeID=in->typeID;

    setID(ID);
}

bool JSProperty::SaveXml(QDomDocument *doc)
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


    QDomElement parentTop  = xmlItems.firstChildElement("运动部分");
    if(parentTop.isNull())
    {
        return false;
    }


    QDomElement parentLow=parentTop.firstChildElement("动态脚本部分");
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
        parentLow=doc->createElement("动态脚本部分");
        parentTop.appendChild(parentLow);
    }



    QDomElement dynamicScript=doc->createElement("动态脚本");

    QDomElement codenum=doc->createElement("编号");
    codenum.appendChild(doc->createTextNode(ID));
    dynamicScript.appendChild(codenum);

    QDomElement sFunname=doc->createElement("脚本功能名");
    sFunname.appendChild(doc->createTextNode(scriptName));
    dynamicScript.appendChild(sFunname);

    QDomElement datas=doc->createElement("数据保存");
    datas.appendChild(doc->createTextNode(dataSave));
    dynamicScript.appendChild(datas);

    QDomElement offsetOj=doc->createElement("偏移对象");
    QDomAttr typeAtt=doc->createAttribute("type");
    typeAtt.setValue(type);
    QDomAttr typeIDAtt=doc->createAttribute("Id");
    typeIDAtt.setValue(typeID);
    offsetOj.setAttributeNode(typeAtt);
    offsetOj.setAttributeNode(typeIDAtt);

    dynamicScript.appendChild(offsetOj);
    parentLow.appendChild(dynamicScript);


    return true;
}
