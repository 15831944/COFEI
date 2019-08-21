#include "labelproperty.h"

LabelProperty::LabelProperty()
{
    ID = "-1";
    value=0;
    note = "";
    commandString = "Label";

    Label="Unnamed";
}

QString LabelProperty::getLabel()
{
    return Label;
}

void LabelProperty::setLabel(QString label)
{
    Label = label;
    sendval();
}

QString LabelProperty::getXmlString()
{
    return "";
}

void LabelProperty::sendval(QString val)
{
    Q_UNUSED(val)
    QString str = QString("#%1\n%2").arg(Label).arg(note);
    emit ValueChange(str);
}


void LabelProperty::copyPropertyChild(LibProperyBase *inPro)
{
    LabelProperty *in=qobject_cast<LabelProperty*>(inPro);
    ID =in->ID;
    note = in->note;
    commandString = in->commandString;

    Label=in->Label;
    setID(ID);
}
