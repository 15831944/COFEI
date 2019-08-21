#include "jogproperty.h"

JogProperty::JogProperty()
{
    commandString = "JOG";
    ID = 1;
    timeout = 1000;
    stupdly = 1000;
    add = 1000;
    dcc = 1000;
    axisId = "";
    Spend = 1000;
    BitName = "E0.0";
    Maxpos = 1;

}

void JogProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+ID;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void JogProperty::copyPropertyChild(LibProperyBase *)
{

}

void JogProperty::SetTimeout(int timout)
{
   timeout = timout;
}

int JogProperty::GetTimeout()
{
    return timeout;
}

void JogProperty::SetStupdly(int dly)
{
    stupdly = dly;
}

int JogProperty::GetStupdly()
{
    return stupdly;
}

void JogProperty::SetAdd(int add_i)
{
    add = add_i;
}

int JogProperty::GetAdd()
{
    return add;
}

void JogProperty::SetDcc(int dcc_i)
{
    dcc = dcc_i;
}

int JogProperty::GetDcc()
{
    return dcc;
}

void JogProperty::SetAxis(QString axisid)
{
    axisId = axisid;
}

QString JogProperty::GetAxis()
{
    return axisId;
}

void JogProperty::SetSpend(int spend_i)
{
    Spend = spend_i;
}

int JogProperty::GetSpend()
{
    return Spend;
}

void JogProperty::SetBitName(QString bitstr)
{
    BitName = bitstr;
}

QString JogProperty::GetBitName()
{
    return BitName;
}

void JogProperty::setDirection(int dec)
{
    Maxpos = dec;
}

int JogProperty::getDirection()
{
    return Maxpos;
}

bool JogProperty::SaveXml(QDomDocument *doc)
{
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
                QDomElement TqmodeEml = srailElmt.firstChildElement("寸动");
                if(TqmodeEml.isNull())
                {
                    QDomElement newtqmode = TqmodeEml.firstChildElement("寸动");
                    srailElmt.appendChild(newtqmode);
                    TqmodeEml = srailElmt.firstChildElement("寸动");
                }

                QDomNodeList IoNodeList= doc->elementsByTagName("寸动信息");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.firstChildElement("编号").text() == ID)
                    {
                        TqmodeEml.removeChild(IoNodeList.at(i));
                    }
                }
                QDomElement IO = doc->createElement("寸动信息");

                QDomElement IOId = doc->createElement("编号");
                QDomText Idtext = doc->createTextNode(ID);
                IOId.appendChild(Idtext);

                QDomElement tpye = doc->createElement("注释");
                QDomText typeText = doc->createTextNode(note);
                tpye.appendChild(typeText);

                QDomElement bytenum = doc->createElement("超时");
                QDomText bytetext = doc->createTextNode(QString("%1").arg(timeout));
                bytenum.appendChild(bytetext);

                QDomElement movelm = doc->createElement("单步延时");
                QDomText movelmtext = doc->createTextNode(QString("%1").arg(stupdly));
                movelm.appendChild(movelmtext);

                QDomElement timelm = doc->createElement("极限");
                QDomText teimelmtext = doc->createTextNode(QString("%1").arg(Maxpos));
                timelm.appendChild(teimelmtext);

                QDomElement stepdlylm = doc->createElement("加速");
                QDomText stepdlylmtext = doc->createTextNode(QString("%1").arg(add));
                stepdlylm.appendChild(stepdlylmtext);

                QDomElement dccemt = doc->createElement("减速");
                QDomText dccemttext = doc->createTextNode(QString("%1").arg(dcc));
                dccemt.appendChild(dccemttext);

                QDomElement AxisVec = doc->createElement("轴集合");
                QDomElement Axisifo = doc->createElement("轴信息");
                QDomAttr axisname = doc->createAttribute("Id");
                axisname.setValue(axisId);
                Axisifo.setAttributeNode(axisname);
                QDomElement voltypersent = doc->createElement("速度百分率");
                QDomText voltypersenttext = doc->createTextNode(QString("%1").arg(Spend));
                voltypersent.appendChild(voltypersenttext);
                QDomElement tqelmt = doc->createElement("比特");
                QDomAttr stopbit = doc->createAttribute("bit");
                stopbit.setValue(BitName);
                QDomAttr bitval = doc->createAttribute("vaule");
                bitval.setValue(QString("%1").arg(value));
                tqelmt.setAttributeNode(stopbit);
                tqelmt.setAttributeNode(bitval);
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
                IO.appendChild(AxisVec);

                TqmodeEml.appendChild(IO);
            }
        }
    }

    return true;
}
