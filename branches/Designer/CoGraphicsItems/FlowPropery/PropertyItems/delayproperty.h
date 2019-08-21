#ifndef DELAYPROPERTY_H
#define DELAYPROPERTY_H
#include "../libproperybase.h"

class DelayProperty : public LibProperyBase
{
    Q_OBJECT

    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString delayTime READ getDelayTime WRITE setDelayTime )
private:
    QString delayTime;
public:
    DelayProperty();
     virtual QString getXmlString() Q_DECL_OVERRIDE;

    virtual void setValue(int) Q_DECL_OVERRIDE;

    QString getDelayTime();
    void setDelayTime(QString val);
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // DELAYPROPERTY_H
