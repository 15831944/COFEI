#include "outproperty.h"

OutProperty::OutProperty()
{
    ID = -1;;   
    value = -1;
    note = "";
    commandString = "OUT";

    BitName = "";
}

QString OutProperty::GetBitName()
{
    return BitName;
}

void OutProperty::SetBitName(QString bit)
{
    BitName = bit;
    sendval();
}

QString OutProperty::getXmlString()
{
    return "";
}

void OutProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str = QString("%1 %2 %3\n%4").arg(commandString).arg(BitName).arg(value).arg(note);
    emit ValueChange(str);
}

void OutProperty::copyPropertyChild(LibProperyBase *inPro)
{
    OutProperty *in=qobject_cast<OutProperty*>(inPro);
    ID =in->ID;  
    value = in->value;
    note = in->note;
    commandString = in->commandString;

    BitName =in->BitName;
    setID(ID);
}
