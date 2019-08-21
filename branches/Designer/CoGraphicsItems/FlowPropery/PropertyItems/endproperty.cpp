#include "endproperty.h"

EndProperty::EndProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="END";
}

QString EndProperty::getXmlString()
{
    return "";
}

//Text Show
void EndProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString;
    str=str+"\n"+note;
    emit  ValueChange(str);
}


void EndProperty::copyPropertyChild(LibProperyBase *inPro)
{
    EndProperty *in=qobject_cast<EndProperty*>(inPro);
    ID =in->ID;

    value = in->value;
    note = in->note;
    commandString = in->commandString;
    setID(ID);
}
