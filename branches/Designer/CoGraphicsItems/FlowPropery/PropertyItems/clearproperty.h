#ifndef CLEARPROPERTY_H
#define CLEARPROPERTY_H
#include "../libproperybase.h"

class ClearProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)
public:
    ClearProperty();
    virtual QString getXmlString() Q_DECL_OVERRIDE;

private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // CLEARPROPERTY_H
