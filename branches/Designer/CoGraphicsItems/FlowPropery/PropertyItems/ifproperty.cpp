#include "ifproperty.h"
#include <QMetaEnum>

IFProperty::IFProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="IF";

    resultType=Stop;
    conditionType=Data;
}

QString IFProperty::getXmlString()
{
    return "";
}

//Text Show
void IFProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QMetaEnum metaEnum=QMetaEnum::fromType<IFCONDITION>();

    QString str=commandString+" "+ID+" "+metaEnum.valueToKey(resultType);//resultType;
    if(resultType==Stop||resultType == clear)
    {
        str=str+"\n"+note;
    }
    else
    {
        str=str+labelName+"\n"+note;
    }
    emit  ValueChange(str);
}

void IFProperty::setResultType(IFCONDITION val)
{
    resultType=val;
    sendval();
}

IFProperty::IFCONDITION IFProperty::getResultType()
{
    return resultType;
}


void IFProperty::setLabelName(QString val)
{
    labelName=val;
    sendval();
}

QString IFProperty::getLabelName()
{
    return labelName;
}


IFProperty::IFCONDITIONTYPE IFProperty::getConditionType()
{
    return conditionType;
}

void IFProperty::setConditionType(IFProperty::IFCONDITIONTYPE val)
{
    conditionType=val;
}


QString IFProperty::getConditionValue()
{
    return conditionValue;
}

void IFProperty::setConditionValue(QString val)
{
    conditionValue=val;
}


void IFProperty::copyPropertyChild(LibProperyBase *inPro)
{
    IFProperty *in=qobject_cast<IFProperty*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;
    commandString = in->commandString;

    conditionType=in->conditionType;
    resultType=in->resultType;
    setID(ID);
}

bool IFProperty::SaveXml(QDomDocument *doc)
{
    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() == "Data")
    {
        QDomElement xmlItems = docElem.firstChildElement("程序配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt  = xmlItems.firstChildElement("条件判断");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc->elementsByTagName("条件信息");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.firstChildElement("编号").text() == ID)
                    {
                        srailElmt.removeChild(IoNodeList.at(i));
                    }
                }
                QDomElement IO = doc->createElement("条件信息");

                QDomElement IOId = doc->createElement("编号");
                QDomText Idtext = doc->createTextNode(ID);
                IOId.appendChild(Idtext);

                QDomElement tpye = doc->createElement("条件类型");
                QDomAttr typeIdex = doc->createAttribute("type");
                QString str;
                if(conditionType==Data)
                {

                   str="数据";
                }
                else
                {
                    str="计数";
                }
                typeIdex.setValue(str);
                tpye.setAttributeNode(typeIdex);
                QDomElement info = doc->createElement("信息");
                QDomText ifnotxt = doc->createTextNode(conditionValue);
                info.appendChild(ifnotxt);
                tpye.appendChild(info);

                IO.appendChild(IOId);
                IO.appendChild(tpye);
                srailElmt.appendChild(IO);
            }
        }
    }

    return true;
}
