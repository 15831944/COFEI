#include "loutproperty.h"

LoutProperty::LoutProperty()
{
    ID = "-1";
    value = -1;
    note = "";
    commandString = "LOUT";

    BitName = "";
}

QString LoutProperty::GetBitName()
{
    return BitName;
}

void LoutProperty::SetBitName(QString bit)
{
    BitName = bit;
    sendval();
}

QString LoutProperty::getXmlString()
{
    return "";
}

void LoutProperty::sendval(QString val)
{
   Q_UNUSED(val)
    QString str = QString("%1 %2 %3\n%4").arg(commandString).arg(BitName).arg(value).arg(note);
    emit ValueChange(str);
}


void LoutProperty::copyPropertyChild(LibProperyBase *inPro)
{
    LoutProperty *in=qobject_cast<LoutProperty*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;
    commandString = in->commandString;

    BitName =in->BitName;
    setID(ID);
}
