#include "subproperty.h"
#include <QMetaEnum>
SubProperty::SubProperty()
{
    ID="-1";
    value=0;
    note="";
    //commandString="SUB_FLOW";
    FlowType = CALL;
    threadName="Unnamed";
}

QString SubProperty::getXmlString()
{
    return "";
}

//Text Show
void SubProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QMetaEnum matem = QMetaEnum::fromType<SUBFLOW>();
    QString str=QString("%1 %2").arg(matem.valueToKey(FlowType)).arg(threadName);
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void SubProperty::setThreadName(QString val)
{
    threadName=val;
    sendval();
}

QString SubProperty::getThreadName()
{
    return threadName;
}

void SubProperty::setFlowType(SubProperty::SUBFLOW subflow)
{
    FlowType = subflow;
    sendval();
}

SubProperty::SUBFLOW SubProperty::getFlowType()
{
    return FlowType;
}

void SubProperty::copyPropertyChild(LibProperyBase *inPro)
{
    SubProperty *in=qobject_cast<SubProperty*>(inPro);
    ID =in->ID;

    value = in->value;
    note = in->note;
    commandString = in->commandString;
    threadName=in->threadName;
    setID(ID);
}
