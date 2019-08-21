#include "cytproperty.h"

Cytproperty::Cytproperty()
{
    ID="Change it";
    note="";
    value=0;
    commandString="CYT";

    Key=0;
}

int Cytproperty::GetKey()
{
    return Key;
}

void Cytproperty::setKey(int k)
{
    Key = k;
    sendval();
}

QString Cytproperty::getXmlString()
{
    return "";
}

void Cytproperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+ID +" "+QString("%1").arg(Key);
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void Cytproperty::copyPropertyChild(LibProperyBase *inPro)
{
    Cytproperty *in=qobject_cast<Cytproperty*>(inPro);
    ID =in->ID;
    value=in->value;
    note = in->note;
    commandString = in->commandString;
    setID(ID);
}
