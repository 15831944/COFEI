#ifndef LOUTPROPERTY_H
#define LOUTPROPERTY_H
#include "../libproperybase.h"

class LoutProperty : public LibProperyBase
{
    Q_OBJECT
public:
    LoutProperty();
    Q_PROPERTY(QString ID READ getID WRITE setID)
    Q_PROPERTY(QString BitName READ GetBitName WRITE SetBitName)
   Q_PROPERTY(int value READ getValue() WRITE setValue)
    Q_PROPERTY(QString note READ getNote WRITE setNote)
private:
    QString BitName;
public:
    QString GetBitName();
    void SetBitName(QString bit);
    QString getXmlString();

private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*) Q_DECL_OVERRIDE;
};

#endif // LOUTPROPERTY_H
