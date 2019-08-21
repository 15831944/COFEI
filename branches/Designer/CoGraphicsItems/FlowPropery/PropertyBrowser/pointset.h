#ifndef POINTSET_H
#define POINTSET_H

#include<QObject>
#include<QStringList>
#include<QDebug>
#include<QDataStream>


class PointSet
{
public:
    PointSet();
    PointSet(const PointSet&);
    ~PointSet();
    void append(QString);
    void remove();
public:
    QStringList value;
    int select;
    friend QDebug operator<<(QDebug out,const PointSet &data);
    friend QDataStream &operator <<(QDataStream &out,const PointSet &data);
    friend QDataStream &operator >>(QDataStream &in, PointSet &data);

};
Q_DECLARE_METATYPE(PointSet)
#endif // POINTSET_H
