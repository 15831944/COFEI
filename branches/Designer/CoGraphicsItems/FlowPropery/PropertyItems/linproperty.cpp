#include "linproperty.h"

LinProperty::LinProperty()
{
     ID = "-1";
     value = -1;
     note = "";
     commandString = "LIN";

     BitName = "";
}


QString LinProperty::GetBitName()
{
    return BitName;
}

void LinProperty::SetBitName(QString bit)
{
    BitName = bit;
    sendval();
}


QString LinProperty::getXmlString()
{
    return "";
}

void LinProperty::sendval(QString val)
{
  Q_UNUSED(val)
    QString str = QString("%1 %2 %3\n%4").arg(commandString).arg(BitName).arg(value).arg(note);
    emit ValueChange(str);
}

void LinProperty::copyPropertyChild(LibProperyBase *inPro)
{
    LinProperty *in=qobject_cast<LinProperty*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;
    commandString = in->commandString;

    BitName=in->BitName;
    setID(ID);
}
