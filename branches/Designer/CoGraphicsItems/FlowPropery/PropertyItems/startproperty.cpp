#include "startproperty.h"

StartProperty::StartProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="START";
}

QString StartProperty::getXmlString()
{
    return "";
}

//Text Show
void StartProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString;
    str=str+"\n"+note;
    emit  ValueChange(str);
}


void StartProperty::copyPropertyChild(LibProperyBase *inPro)
{
    StartProperty *in=qobject_cast<StartProperty*>(inPro);
    ID =in->ID;
    value=in->value;
    note = in->note;
    commandString = in->commandString;
    setID(ID);
}
