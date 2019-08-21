#ifndef SUBPROPERTY_H
#define SUBPROPERTY_H
#include "../libproperybase.h"

class SubProperty : public LibProperyBase
{
    Q_OBJECT
//    Q_PROPERTY(QString ID READ getID WRITE setID )
    Q_PROPERTY(SUBFLOW FlowType READ getFlowType WRITE setFlowType )
    Q_PROPERTY(QString threadName READ getThreadName WRITE setThreadName)
    Q_PROPERTY(QString note READ getNote WRITE setNote)

public:
    enum SUBFLOW
    {
        CALL,
        THREAD,
        SUB_FLOW
    };
    Q_ENUM(SUBFLOW)
    SubProperty();
    virtual QString getXmlString() Q_DECL_OVERRIDE;

    void setThreadName(QString val);
    QString getThreadName();

    void setFlowType(SUBFLOW subflow);
    SUBFLOW getFlowType();
private:
    QString threadName;
    SUBFLOW FlowType;
   virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // SUBPROPERTY_H
