#include "orgproperty.h"
#include<QVector>
QStringList ORGProperty::AxisInfoString({"Id","高速","低速","方向","正向时间","原点偏移时间","超时"});
QStringList ORGProperty::AxisInfoStringDefalut({"","50","10","1","200","100","50000"});
QMap<QString,QVariant> ORGProperty::AxisInfo=QMap<QString,QVariant>(
                                                        {
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(0),ORGProperty::AxisInfoStringDefalut.at(0)},
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(1),ORGProperty::AxisInfoStringDefalut.at(1)},
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(2),ORGProperty::AxisInfoStringDefalut.at(2)},
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(3),ORGProperty::AxisInfoStringDefalut.at(3)},
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(4),ORGProperty::AxisInfoStringDefalut.at(4)},
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(5),ORGProperty::AxisInfoStringDefalut.at(5)},
                                                       {QString("第%1轴").arg(ORGProperty::AxisInfoNum)+ORGProperty::AxisInfoString.at(6),ORGProperty::AxisInfoStringDefalut.at(6)}
                                                                            });

int ORGProperty::AxisInfoNum=1;
ORGProperty::ORGProperty()
{
    ID="-1";
    note="";
    commandString="ORG";
    timeout="50000";
    Axis.append(QStringList({"X"}));
    AxisNum=1;
}

QString ORGProperty::getTimeout()
{
    return timeout;
}

void ORGProperty::setTimeout(QString val)
{
    timeout=val;
}

QStringList ORGProperty::getAxis()
{
    return Axis;
}

void ORGProperty::setAxis(QStringList val)
{
    Axis=val;
}

int ORGProperty::getAxisNum()
{
    return AxisNum;
}

void ORGProperty::setAxisNum(int val)
{
    if(val<1)
    {
        return;
    }
    AxisNum=val;
    QStringList src(Axis);
    int count=src.count();
    count=AxisNum-count;
    if(count>0)
    {
        for(int i=0;i<count;++i)
        {
            src.append("");
        }
    }
    else if(count<0)
    {
        count=-count;
        for(int i=0;i<count;++i)
        {
            src.removeLast();
        }
    }
    setAxis(src);
}

QMap<QString,QVariant> ORGProperty::getAxisInfo()
{
    return AxisInfo;
}

void ORGProperty::setAxisInfo(QMap<QString,QVariant> val)
{
    AxisInfo=val;
}

int ORGProperty::getAxisInfoNum()
{
    return AxisInfoNum;
}

void ORGProperty::setAxisInfoNum(int val)
{
    if(val<1)
    {
        return;
    }
    int srcAxisNum=AxisInfoNum;
    AxisInfoNum=val;
    QMap<QString,QVariant> src(AxisInfo);
    int count=src.count();
    count=val-count/AxisInfoString.count();
    if(count>0)
    {
        for(int i=0;i<count;++i)
        {
            for(int ii=0;ii<AxisInfoString.count();++ii)
            {
                 src.insert(src.constEnd(),QString("第%1轴").arg(srcAxisNum+1+i)+AxisInfoString.at(ii),"");
            }

        }
    }
    else if(count<0)
    {
        count=-count;
        for(int i=0;i<count;++i)
        {
            for(int j=0;j<AxisInfoString.count();++j)
            {
                src.remove(src.keys().last());
            }
        }
    }
    setAxisInfo(src);
}

void ORGProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str = QString("%1 %2\n%3").arg(commandString).arg(ID).arg(note);
    emit ValueChange(str);
}

void ORGProperty::copyPropertyChild(LibProperyBase *inPro)
{
    ORGProperty *in=qobject_cast<ORGProperty*>(inPro);
    ID =in->ID;
    note = in->note;

    timeout=in->timeout;
    Axis=in->Axis;
    AxisNum=in->AxisNum;

    setID(ID);
}

bool ORGProperty::SaveXml(QDomDocument *doc)
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


    QDomElement parentLow=parentTop.firstChildElement("回原点");
    if(parentLow.isNull())
    {
        parentLow=doc->createElement("回原点");
        parentTop.appendChild(parentLow);
    }

    QDomElement axisPoint=parentLow.firstChildElement("原点轴信息");
    if(axisPoint.isNull())
    {
        axisPoint=doc->createElement("原点轴信息");
        parentLow.appendChild(axisPoint);
    }
    else
    {
        int lenth=axisPoint.childNodes().count();
        for(int ii=0;ii<lenth;++ii)
        {
            axisPoint.removeChild(axisPoint.lastChild());
        }
    }



    //Add AxisInfo to Doc
    for(int i=0;i<AxisInfoNum;++i)
    {
        QDomElement axismsg=doc->createElement("轴信息");
        QDomAttr axismsgID=doc->createAttribute("Id");
        axismsgID.setValue(AxisInfo.values().at(i*AxisInfoString.count()).toString());
        axismsg.setAttributeNode(axismsgID);

        for(int ii=1;ii<AxisInfoString.count();++ii)
        {
            QDomElement  c1=doc->createElement(AxisInfoString.at(ii));
            QDomText t1=doc->createTextNode(AxisInfo.values().at(i*AxisInfoString.count()+ii).toString());
            c1.appendChild(t1);
            axismsg.appendChild(c1);
        }

        axisPoint.appendChild(axismsg);


    }



    QDomElement low1=parentLow.firstChildElement("原点操作信息");
    if(low1.isNull())
    {
        low1=doc->createElement("原点操作信息");
        parentLow.appendChild(low1);
    }
    else
    {
        QDomNodeList child1=low1.childNodes();
        for(int i=0;i<child1.count();++i)
        {
            QDomElement child=child1.at(i).toElement();
            QDomElement codenum=child.firstChildElement("编号");
            if(codenum.text()==ID)
            {
                low1.removeChild(child);
                break;
            }
        }
    }
    QDomElement childOri=doc->createElement("原点信息");
    low1.appendChild(childOri);

    QDomElement child1=doc->createElement("编号");
    QDomText text1=doc->createTextNode(ID);
    child1.appendChild(text1);
    childOri.appendChild(child1);

    QDomElement child2=doc->createElement("超时");
    QDomText text2=doc->createTextNode(timeout);
    child2.appendChild(text2);
    childOri.appendChild(child2);

    QDomElement child3=doc->createElement("轴集合");
    for(int i=0;i<AxisNum;++i)
    {
        QDomElement child4=doc->createElement("轴编号");
        QDomText text4=doc->createTextNode(Axis.at(i));
        child4.appendChild(text4);
        child3.appendChild(child4);
    }
    childOri.appendChild(child3);



    return true;
}
