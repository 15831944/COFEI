#ifndef CALLPROPERTY_H
#define CALLPROPERTY_H


#include "../libproperybase.h"

class CallProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString threadName READ getThreadName WRITE setThreadName)

public:
    CallProperty();
    virtual QString getXmlString() Q_DECL_OVERRIDE;

    void setThreadName(QString val);
    QString getThreadName();
private:
    QString threadName;

    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // CALLPROPERTY_H
