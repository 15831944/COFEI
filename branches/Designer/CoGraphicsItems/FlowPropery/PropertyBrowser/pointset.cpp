#include "pointset.h"

PointSet::PointSet()
{
    select=0;
}

PointSet::PointSet(const PointSet &old)
{
    value=old.value;
    select=old.select;
}

PointSet::~PointSet()
{

}

void PointSet::append(QString val)
{
    value.append(val);
}

void PointSet::remove()
{
    value.removeLast();
}



QDataStream &operator <<(QDataStream &out, const PointSet &data)
{
    int cnt=data.value.count();
    out<<data.select;
    out<<cnt;
    foreach (QString item, data.value) {
        out<<item;
    }
    return out;
}

QDataStream &operator >>(QDataStream &in,  PointSet &data)
{
    int cnt;
    in>>data.select;
    in>>cnt;

    for(int i=0;i<cnt;++i)
    {
        QString val;
        in>>val;
        data.append(val);
    }
    return in;
}

QDebug operator <<(QDebug out,  const PointSet &data)
{
    foreach (QString item, data.value) {
        out<<item;
    }
    return out;
}
