#ifndef ENDPROPERTY_H
#define ENDPROPERTY_H
#include "../libproperybase.h"

class EndProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

   Q_PROPERTY(QString note READ getNote WRITE setNote)
public:
    EndProperty();
     virtual QString getXmlString() Q_DECL_OVERRIDE;
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // ENDPROPERTY_H
