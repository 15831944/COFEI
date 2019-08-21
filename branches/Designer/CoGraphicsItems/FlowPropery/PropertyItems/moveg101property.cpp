#include "moveg101property.h"

MoveG101Property::MoveG101Property()
{
    ID="-1";
    value=0;
    note="";
    commandString="G101";
     timeout = 10;
     dly = 10;
     Velocitypercent = 1;
     acc = 1;
     dcc = 1;
     AxisNum = 1;
     AxisInfo ="";
     AxisVltypert = 1;
     tq = 0;
}

QString MoveG101Property::getXmlString()
{
    return "";
}

bool MoveG101Property::SaveXml(QDomDocument *doc)
{
    // file->open(QIODevice::WriteOnly);
     QDomElement docElem = doc->documentElement();
     if (docElem.tagName() == "Data")
     {
         QDomElement xmlItems = docElem.firstChildElement("程序配置");
         if(!xmlItems.isNull())
         {
             QDomElement srailElmt;
             srailElmt  = xmlItems.firstChildElement("运动部分");

             if(!srailElmt.isNull())
             {
                 QDomElement TqmodeEml = srailElmt.firstChildElement("扭矩模式");
                 if(TqmodeEml.isNull())
                 {
                     QDomElement newtqmode = TqmodeEml.firstChildElement("扭矩模式");
                     srailElmt.appendChild(newtqmode);
                     TqmodeEml = srailElmt.firstChildElement("扭矩模式");
                 }
                 if(TqmodeEml.isNull())
                 {
                     return false;
                 }
                 QDomNodeList IoNodeList= doc->elementsByTagName("扭矩信息");
                 for(int i = 0;i < IoNodeList.size();i++)
                 {
                     QDomElement IoE = IoNodeList.at(i).toElement();
                     if(IoE.firstChildElement("编号").text() == pointname)
                     {
                         TqmodeEml.removeChild(IoNodeList.at(i));
                     }
                 }
                 QDomElement IO = doc->createElement("扭矩信息");

                 QDomElement IOId = doc->createElement("编号");
                 QDomText Idtext = doc->createTextNode(pointname);
                 IOId.appendChild(Idtext);

                 QDomElement tpye = doc->createElement("注释");
                 QDomText typeText = doc->createTextNode(note);
                 tpye.appendChild(typeText);

                 QDomElement bytenum = doc->createElement("超时");
                 QDomText bytetext = doc->createTextNode(QString("%1").arg(timeout));
                 bytenum.appendChild(bytetext);

                 QDomElement movelm = doc->createElement("单步延时");
                 QDomText movelmtext = doc->createTextNode(QString("%1").arg(dly));
                 movelm.appendChild(movelmtext);

                 QDomElement timelm = doc->createElement("速度百分率");
                 QDomText teimelmtext = doc->createTextNode(QString("%1").arg(Velocitypercent));
                 timelm.appendChild(teimelmtext);

                 QDomElement stepdlylm = doc->createElement("加速");
                 QDomText stepdlylmtext = doc->createTextNode(QString("%1").arg(acc));
                 stepdlylm.appendChild(stepdlylmtext);

                 QDomElement dccemt = doc->createElement("减速");
                 QDomText dccemttext = doc->createTextNode(QString("%1").arg(dcc));
                 dccemt.appendChild(dccemttext);

                 QDomElement axisnum = doc->createElement("轴数");
                 QDomText axisnumtext = doc->createTextNode(QString("%1").arg(AxisNum));
                 axisnum.appendChild(axisnumtext);

                 QDomElement AxisVec = doc->createElement("轴集合");
                 QDomElement Axisifo = doc->createElement("轴信息");
                 QDomAttr axisname = doc->createAttribute("Id");
                 axisname.setValue(AxisInfo);
                 Axisifo.setAttributeNode(axisname);
                 QDomElement voltypersent = doc->createElement("速度百分率");
                 QDomText voltypersenttext = doc->createTextNode(QString("%1").arg(AxisVltypert));
                 voltypersent.appendChild(voltypersenttext);
                 QDomElement tqelmt = doc->createElement("扭矩");
                 QDomText tqelmttext = doc->createTextNode(QString("%1").arg(tq));
                 tqelmt.appendChild(tqelmttext);
                 Axisifo.appendChild(voltypersent);
                 Axisifo.appendChild(tqelmt);
                 AxisVec.appendChild(Axisifo);



                 IO.appendChild(IOId);
                 IO.appendChild(tpye);
                 IO.appendChild(bytenum);
                 IO.appendChild(movelm);
                 IO.appendChild(timelm);
                 IO.appendChild(stepdlylm);
                 IO.appendChild(dccemt);
                 IO.appendChild(axisnum);
                 IO.appendChild(AxisVec);

                 TqmodeEml.appendChild(IO);
             }
         }
     }

     return true;
}

//Text Show
void MoveG101Property::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+pointname;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

QString MoveG101Property::getPointname()
{
    return pointname;
}

void MoveG101Property::setPointName(QString val)
{
    pointname=val;
    sendval();
}

int MoveG101Property::getTimeOut()
{
    return timeout;
}

void MoveG101Property::setTimeout(int tme)
{
    timeout = tme;
}

int MoveG101Property::getdly()
{
    return dly;
}

void MoveG101Property::setdly(int dlyval)
{
    dly = dlyval;
}

int MoveG101Property::getVelocitypercent()
{
    return Velocitypercent;
}

void MoveG101Property::setVelocitypercent(int velocitypcent)
{
    Velocitypercent = velocitypcent;
}

int MoveG101Property::getacc()
{
    return acc;
}

void MoveG101Property::setacc(int ac)
{
    acc = ac;
}

int MoveG101Property::getdcc()
{
    return dcc;
}

void MoveG101Property::setdcc(int dc)
{
    dcc = dc;
}

int MoveG101Property::getAxisNum()
{
    return AxisNum;
}

void MoveG101Property::setAxisNum(int axisnum)
{
    AxisNum = axisnum;
}

QString MoveG101Property::getAxisInfo()
{
    return AxisInfo;
}

void MoveG101Property::setAxisInfo(QString axininfo)
{
    AxisInfo = axininfo;
}

int MoveG101Property::getAxisVltypert()
{
    return AxisVltypert;
}

void MoveG101Property::setAxisVltypert(int Avp)
{
    AxisVltypert = Avp;
}

int MoveG101Property::gettq()
{
    return tq;
}

void MoveG101Property::settq(int tqval)
{
    tq = tqval;
}

void MoveG101Property::copyPropertyChild(LibProperyBase *inPro)
{
    MoveG101Property *in=qobject_cast<MoveG101Property*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;

    setID(ID);
}
