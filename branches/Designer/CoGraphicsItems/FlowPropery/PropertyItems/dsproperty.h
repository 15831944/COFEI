#ifndef DSPROPERTY_H
#define DSPROPERTY_H
#include "../libproperybase.h"

class DSProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(int value READ getValue() WRITE setValue)

     Q_PROPERTY(QString IN_IO_0 READ getIN_IO_0() WRITE setIN_IO_0)

     Q_PROPERTY(int IN_IO_0_Value READ getIN_IO_0_Value() WRITE setIN_IO_0_Value)

    Q_PROPERTY(QString IN_IO_1 READ getIN_IO_1() WRITE setIN_IO_1)

    Q_PROPERTY(int IN_IO_1_Value READ getIN_IO_1_Value() WRITE setIN_IO_1_Value)
public:
    DSProperty();
     virtual QString getXmlString() Q_DECL_OVERRIDE;

    QString getIN_IO_0();
    void setIN_IO_0(QString);

    int getIN_IO_0_Value();
    void setIN_IO_0_Value(int);

    QString getIN_IO_1();
    void setIN_IO_1(QString);

    int getIN_IO_1_Value();
    void setIN_IO_1_Value(int);

private:
    QString IN_IO_0;
    int IN_IO_0_Value;

    QString IN_IO_1;
    int IN_IO_1_Value;
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);

    // LibProperyBase interface
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // DSPROPERTY_H
