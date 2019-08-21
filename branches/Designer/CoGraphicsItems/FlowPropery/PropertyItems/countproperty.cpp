#include "countproperty.h"

CountProperty::CountProperty()
{
    commandString  = "CNT";
}

void CountProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+ID;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

QString CountProperty::getID()
{
    return ID;
}

void CountProperty::setID(QString Idval)
{
    ID = Idval;
    sendval();
}

QString CountProperty::getNote()
{
    return note;
}

void CountProperty::setNote(QString note_1)
{
    note = note_1;
    sendval();
}
