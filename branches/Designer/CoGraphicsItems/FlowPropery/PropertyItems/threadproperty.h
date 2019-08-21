#ifndef THREADPROPERTY_H
#define THREADPROPERTY_H
#include "../libproperybase.h"

class ThreadProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString threadName READ getThreadName WRITE setThreadName)
public:
    ThreadProperty();
     virtual QString getXmlString() Q_DECL_OVERRIDE;

    void setThreadName(QString val);
    QString getThreadName();
private:
    QString threadName;
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // THREADPROPERTY_H
