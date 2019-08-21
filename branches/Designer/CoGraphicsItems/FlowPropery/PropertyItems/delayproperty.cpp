#include "delayproperty.h"

DelayProperty::DelayProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="T";

    delayTime="100";
}

QString DelayProperty::getXmlString()
{
    return "";
}


//Text Show
void DelayProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+delayTime;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void DelayProperty::setValue(int val)
{
    value=val;
     sendval();
}

QString DelayProperty::getDelayTime()
{
    return delayTime;
}

void DelayProperty::setDelayTime(QString val)
{
    delayTime = val;
      sendval();
}

void DelayProperty::copyPropertyChild(LibProperyBase *inPro)
{
    DelayProperty *in=qobject_cast<DelayProperty*>(inPro);
    ID =in->ID;
    delayTime =in->delayTime;
    value = in->value;
    note = in->note;
    commandString = in->commandString;
    setID(ID);
}
