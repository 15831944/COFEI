#include "threadproperty.h"

ThreadProperty::ThreadProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="THREAD";

   threadName="Unnamed";
}

QString ThreadProperty::getXmlString()
{
    return "";
}

//Text Show
void ThreadProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+threadName;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void ThreadProperty::setThreadName(QString val)
{
    threadName=val;
     sendval();
}

QString ThreadProperty::getThreadName()
{
    return threadName;

}

void ThreadProperty::copyPropertyChild(LibProperyBase *inPro)
{
    ThreadProperty *in=qobject_cast<ThreadProperty*>(inPro);
    ID =in->ID;

    value = in->value;
    note = in->note;
    commandString = in->commandString;
    threadName=in->threadName;
    setID(ID);
}
