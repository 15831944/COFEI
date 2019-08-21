#ifndef LIBPROPERYBASE_H
#define LIBPROPERYBASE_H

#include <QFile>
#include <QDomDocument>
#include<QDataStream>
#include<QTextStream>
#include"PropertyBrowser/pointset.h"
class LibProperyBase:public QObject
{
Q_OBJECT
public:
    LibProperyBase();

protected:
    virtual void sendval(QString val="");
public:
    virtual QString getID();
    virtual void setID(QString val);

    virtual QString getNote();
    virtual void setNote(QString);

    virtual int getValue();
    virtual void setValue(int);

    virtual QString getXmlString();

    virtual void copyPropertyChild(LibProperyBase*);
    virtual bool SaveXml(QDomDocument *doc);

    virtual PointSet getPointName();
    virtual void setPointName(PointSet);

    PointSet pointName;
signals:
    void ValueChange(QString val);

protected:
    QString ID;
    QString note;
    QString commandString;
    int value;

public:
    void copyProperty(LibProperyBase*);
    friend QDataStream& operator <<(QDataStream &out, LibProperyBase &data);
    friend QDataStream& operator >>(QDataStream &in, LibProperyBase &data);
};



#endif // LIBPROPERYBASE_H
