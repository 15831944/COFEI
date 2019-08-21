#include "logproperty.h"
#include<QMetaEnum>

LOGProperty::LOGProperty()
{
    ID="-1";
    note="";
    commandString="LOG";

    information="";
    typeInfo=Normal;
}

QString LOGProperty::getInformation()
{
    return information;
}

void LOGProperty::setInformation(QString val)
{
    information=val;
    sendval();
}

LOGProperty::TypeInfo LOGProperty::getTypeInfo()
{
    return typeInfo;
}

void LOGProperty::setTypeInfo(LOGProperty::TypeInfo val)
{
    typeInfo=val;
     sendval();
}

void LOGProperty::sendval(QString val)
{
    Q_UNUSED(val)

    QString str=commandString+" "+information+" "+QString::number(typeInfo,10);
    str=str+"\n"+note;

    emit  ValueChange(str);

}

void LOGProperty::copyPropertyChild(LibProperyBase * inPro)
{
    LOGProperty *in=qobject_cast<LOGProperty*>(inPro);
    ID =in->ID;
    note = in->note;

    information=in->information;
    typeInfo=in->typeInfo;
    setID(ID);
}
