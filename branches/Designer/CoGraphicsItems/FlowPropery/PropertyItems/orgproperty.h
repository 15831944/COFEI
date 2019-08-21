#ifndef ORGPROPERTY_H
#define ORGPROPERTY_H

#include "../libproperybase.h"

class ORGProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString timeout READ getTimeout WRITE setTimeout)

    Q_PROPERTY(QStringList Axis READ getAxis WRITE setAxis)
    Q_PROPERTY(int AxisNum READ getAxisNum WRITE setAxisNum)

    Q_PROPERTY(QMap<QString,QVariant> AxisInfo READ getAxisInfo WRITE setAxisInfo)
    Q_PROPERTY(int AxisInfoNum READ getAxisInfoNum WRITE setAxisInfoNum)

public:
    ORGProperty();

    QString getTimeout();
    void setTimeout(QString);

    QStringList getAxis();
    void setAxis(QStringList);

    int getAxisNum();
    void setAxisNum(int);

    QMap<QString,QVariant> getAxisInfo();
    void setAxisInfo(QMap<QString,QVariant>);

    int getAxisInfoNum();
    void setAxisInfoNum(int);

    QString timeout;
    QStringList Axis;
    int AxisNum;

    static QMap<QString,QVariant> AxisInfo;
    static int AxisInfoNum;
    static QStringList AxisInfoString;
    static QStringList AxisInfoStringDefalut;
private:
   virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // ORGPROPERTY_H
