#include "libproperybase.h"

#include<QMetaObject>
#include<QMetaProperty>
#include "../FlowPropery/PropertyBrowser/objectcontroller.h"
#include"PropertyBrowser/pointset.h"
LibProperyBase::LibProperyBase()
{

    ID="-1";
    value=0;
    note="";
    commandString="";
}


void LibProperyBase::sendval(QString val)
{
    Q_UNUSED(val)
    QString str=commandString+" "+ID;
    str=str+"\n"+note;
    emit  ValueChange(str);
}

void LibProperyBase::setID(QString val)
{

    ID=val;
    this->sendval();
}


QString LibProperyBase::getID()
{
    return ID;
}

QString LibProperyBase::getNote()
{
    return note;
}

void LibProperyBase::setNote(QString val)
{
    note=val;
    this->sendval();
}

int LibProperyBase::getValue()
{
    return value;
}

void LibProperyBase::setValue(int val)
{
    value=val;
}

QString LibProperyBase::getXmlString()
{
    return "";
}


void LibProperyBase::copyPropertyChild(LibProperyBase *val)
{
    Q_UNUSED(val)
}

bool LibProperyBase::SaveXml(QDomDocument *doc)
{
    Q_UNUSED(doc)
    return true;
}

PointSet LibProperyBase::getPointName()
{
    return pointName;
}

void LibProperyBase::setPointName(PointSet val)
{
    pointName.select=val.select;
    pointName.value=val.value;
    sendval();
}

void LibProperyBase::copyProperty(LibProperyBase *val)
{
    copyPropertyChild(val);
    setID(ID);
}

QDataStream &operator <<(QDataStream &out, LibProperyBase &data)
{
    QMetaObject const *obj=data.metaObject();
    for(int i=obj->propertyOffset();i<obj->propertyCount();i++)
    {
       QMetaProperty pt=obj->property(i);
       QVariant val=data.property(pt.name());
       if(val.canConvert(ObjectController::userRegister_Point))
       {
           PointSet pt=val.value<PointSet>();
           out<<pt.select;
           int cnt=pt.value.count();
           out<<cnt;
           foreach (QString item, pt.value) {
               out<<item;
           }
           continue;
       }
       out<<val;
    }
    return out;
}

QDataStream &operator >>(QDataStream &in, LibProperyBase &data)
{
    QMetaObject const *obj=data.metaObject();
    for(int i=obj->propertyOffset();i<obj->propertyCount();i++)
    {
       QMetaProperty pt=obj->property(i);

       QVariant val=data.property(pt.name());
       if(val.canConvert(ObjectController::userRegister_Point))
       {
           PointSet ps=val.value<PointSet>();
           in>>ps.select;
           int cnt;
           in>>cnt;

           for(int i=0;i<cnt;++i)
           {
               QString val;
               in>>val;
               ps.append(val);
           }
           data.setProperty(pt.name(),QVariant::fromValue(ps));
        continue;
       }
       in>>val;
       data.setProperty(pt.name(),val);
    }
    return in;
}


