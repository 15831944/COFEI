#include "callproperty.h"

CallProperty::CallProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="CALL";

    threadName="Unnamed";
}

QString CallProperty::getXmlString()
{
    return "";
}

//Text Show
void CallProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+threadName;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void CallProperty::setThreadName(QString val)
{
    threadName=val;
    sendval();
}

QString CallProperty::getThreadName()
{
    return threadName;
}

void CallProperty::copyPropertyChild(LibProperyBase *inPro)
{
    CallProperty *in=qobject_cast<CallProperty*>(inPro);
    ID =in->ID;

    value = in->value;
    note = in->note;
    commandString = in->commandString;
    threadName=in->threadName;
    setID(ID);
}
