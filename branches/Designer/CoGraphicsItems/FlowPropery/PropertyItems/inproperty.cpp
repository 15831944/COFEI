#include "inproperty.h"
#include "QMetaEnum"
InProperty::InProperty()
{
    ID = "-1";;
    value = -1;
    note = "";
    commandString = "IN";
    SwitchOder = IN;
    BitName = "";
}

QString InProperty::GetBitName()
{
    sendval();
    return BitName;

}

void InProperty::SetBitName(QString bit)
{
    BitName = bit;
    sendval();
}

QString InProperty::getXmlString()
{
    return "";
}

void InProperty::setSwitchOder(InProperty::SWITCHODER swoder)
{
    SwitchOder = swoder;
    sendval();
}

InProperty::SWITCHODER InProperty::readSwitchOder()
{
    return SwitchOder;
}

void InProperty::sendval(QString val)
{   Q_UNUSED(val)
    QMetaEnum matem = QMetaEnum::fromType<SWITCHODER>();
    QString str = QString("%1 %2 %3 \n%5").arg(matem.valueToKey(SwitchOder)).arg(BitName).arg(value).arg(note);
    emit  ValueChange(str);
}

void InProperty::copyPropertyChild(LibProperyBase *inPro)
{
    InProperty *in=qobject_cast<InProperty*>(inPro);
    ID =in->ID;
    value = in->value;
    note = in->note;
    commandString = in->commandString;

    BitName =in->BitName;
    setID(ID);
}
