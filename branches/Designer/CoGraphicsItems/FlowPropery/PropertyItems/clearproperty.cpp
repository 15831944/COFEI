#include "clearproperty.h"

ClearProperty::ClearProperty()
{
    ID="-1";
    value=0;
    note="";
    commandString="CLEAR";
}

QString ClearProperty::getXmlString()
{
    return "";
}

//Text Show
void ClearProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+ID;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void ClearProperty::copyPropertyChild(LibProperyBase *inPro)
{
    ClearProperty *in=qobject_cast<ClearProperty*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;
    commandString = in->commandString;
    setID(ID);
}
