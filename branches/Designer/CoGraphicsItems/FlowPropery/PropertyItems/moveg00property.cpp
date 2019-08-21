#include "moveg00property.h"

MoveG00Property::MoveG00Property()
{
    ID="-1";
    note="";
    commandString="G00";


}

QString MoveG00Property::getXmlString()
{
    return "";
}

//Text Show
void MoveG00Property::sendval(QString val)
{
    Q_UNUSED(val)
    if(pointName.value.size() > pointName.select )
    {
        QString str=commandString+" "+((pointName.value.count()>0)?pointName.value.at(pointName.select):"");
        str=str+"\n"+note;
        emit  ValueChange(str);
    }
}


void MoveG00Property::copyPropertyChild(LibProperyBase *inPro)
{
    MoveG00Property *in=qobject_cast<MoveG00Property*>(inPro);
    ID =in->ID;

    note = in->note;

    setID(ID);
}
