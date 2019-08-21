#include "cylinderproperty.h"
#include"QDebug"

CylinderProperty::CylinderProperty()
{
    ID="-1";
    note="";
    value=0;
    commandString="M00";

    softOut="Q";
    originPoint="E";
    movePoint="E";
    timeOut=12000;
    stepDelay=10;

    tips1="";
    key1=1;

    tips2="";
    key2=1;

    tips3="";
    key3=1;

    tips4="";
    key4=1;

    tips5="";
    key5=1;


}

QString CylinderProperty::getXmlString()
{
    return "";
}


//Text Show
void CylinderProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+softOut+" "+QString::number(value);
    str=str+"\n"+note;
    emit  ValueChange(str);
}

QString CylinderProperty::getSoftOut(){return softOut;}
void CylinderProperty::setSoftOut(QString val)
{
    softOut = val;
    sendval();
}

void CylinderProperty::setValue(int val)

{
    value=val;
    sendval();
}



QString CylinderProperty::getOriginPoint(){return originPoint;}
void CylinderProperty::setOriginPoint(QString val){originPoint = val;}


QString CylinderProperty::getMovePoint(){return movePoint;}
void CylinderProperty::setMovePoint(QString val){movePoint = val;}

int CylinderProperty::getTimeOut(){return timeOut;}
void CylinderProperty::setTimeOut(int val){timeOut = val;}

int CylinderProperty::getStepDelay(){return stepDelay;}
void CylinderProperty::setStepDelay(int val){stepDelay = val;}

QString CylinderProperty::getTips1(){return tips1;}
void CylinderProperty::setTips1(QString val){tips1 = val;}
int CylinderProperty::getKey1(){return key1;}
void CylinderProperty::setKey1(int val){key1 = val;}

QString CylinderProperty::getTips2(){return tips2;}
void CylinderProperty::setTips2(QString val){tips2 = val;}
int CylinderProperty::getKey2(){return key2;}
void CylinderProperty::setKey2(int val){key2 = val;}

QString CylinderProperty::getTips3(){return tips3;}
void CylinderProperty::setTips3(QString val){tips3 = val;}
int CylinderProperty::getKey3(){return key3;}
void CylinderProperty::setKey3(int val){key3 = val;}

QString CylinderProperty::getTips4(){return tips4;}
void CylinderProperty::setTips4(QString val){tips4 = val;}
int CylinderProperty::getKey4(){return key4;}
void CylinderProperty::setKey4(int val){key4 = val;}

QString CylinderProperty::getTips5(){return tips5;}
void CylinderProperty::setTips5(QString val){tips5 = val;}
int CylinderProperty::getKey5(){return key5;}
void CylinderProperty::setKey5(int val){key5 = val;}

void CylinderProperty::copyPropertyChild(LibProperyBase *inPro)
{
    CylinderProperty *in=qobject_cast<CylinderProperty*>(inPro);
    ID=in->ID;
    note=in->note;
    value=in->value;
    commandString=in->commandString;

    softOut=in->softOut;
    originPoint=in->originPoint;
    movePoint=in->movePoint;
    timeOut=in->timeOut;
    stepDelay=in->stepDelay;

    tips1=in->tips1;
    key1=in->key1;

    tips2=in->tips2;
    key2=key2;

    tips3=in->tips3;
    key3=in->key3;

    tips4=in->tips4;
    key4=in->key4;

    tips5=in->tips5;
    key5=in->key5;
    setID(ID);
}

bool CylinderProperty::SaveXml(QDomDocument *doc)
{
   // file->open(QIODevice::WriteOnly);
    QDomElement docElem = doc->documentElement();
    if (docElem.tagName() == "Data")
    {
        QDomElement xmlItems = docElem.firstChildElement("程序配置");
        if(!xmlItems.isNull())
        {
            QDomElement srailElmt;
            srailElmt  = xmlItems.firstChildElement("汽缸集合");

            if(!srailElmt.isNull())
            {
                QDomNodeList IoNodeList= doc->elementsByTagName("汽缸");
                for(int i = 0;i < IoNodeList.size();i++)
                {
                    QDomElement IoE = IoNodeList.at(i).toElement();
                    if(IoE.firstChildElement("编号").text() == ID)
                    {
                        srailElmt.removeChild(IoNodeList.at(i));
                    }
                }
                QDomElement IO = doc->createElement("汽缸");

                QDomElement IOId = doc->createElement("编号");
                QDomText Idtext = doc->createTextNode(ID);
                IOId.appendChild(Idtext);

                QDomElement tpye = doc->createElement("软元件");
                QDomText typeText = doc->createTextNode(softOut);
                tpye.appendChild(typeText);

                QDomElement bytenum = doc->createElement("原点");
                QDomText bytetext = doc->createTextNode(originPoint);
                bytenum.appendChild(bytetext);

                QDomElement movelm = doc->createElement("原点");
                QDomText movelmtext = doc->createTextNode(movePoint);
                movelm.appendChild(movelmtext);

                QDomElement timelm = doc->createElement("超时");
                QDomText teimelmtext = doc->createTextNode(QString("%1").arg(timeOut));
                timelm.appendChild(teimelmtext);

                QDomElement stepdlylm = doc->createElement("单步延时");
                QDomText stepdlylmtext = doc->createTextNode(QString("%1").arg(stepDelay));
                stepdlylm.appendChild(stepdlylmtext);

                IO.appendChild(IOId);
                IO.appendChild(tpye);
                IO.appendChild(bytenum);
                IO.appendChild(movelm);
                IO.appendChild(timelm);
                IO.appendChild(stepdlylm);

                srailElmt.appendChild(IO);
            }
        }
    }

    return true;
}
