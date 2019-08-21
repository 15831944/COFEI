#ifndef INPROPERTY_H
#define INPROPERTY_H
#include "../libproperybase.h"

class InProperty:public LibProperyBase
{
    Q_OBJECT
public:
    InProperty();
    //Q_PROPERTY(QString ID READ getID WRITE setID)
    Q_PROPERTY(SWITCHODER SwitchOder READ readSwitchOder WRITE setSwitchOder)
    Q_PROPERTY(QString BitName READ GetBitName WRITE SetBitName)
    Q_PROPERTY(int value READ getValue() WRITE setValue)
    Q_PROPERTY(QString note READ getNote WRITE setNote)

public:
    enum SWITCHODER{
        IN,
        OUT,
        LIN,
        LOUT,
        MOUT,
        MIN
    };
    Q_ENUM(SWITCHODER)
private:
    QString BitName;

    SWITCHODER SwitchOder;
public:

    QString GetBitName();
    void SetBitName(QString bit);
    QString getXmlString();
    void setSwitchOder(SWITCHODER swoder);
    SWITCHODER readSwitchOder();
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*) Q_DECL_OVERRIDE;
};

#endif // INPROPERTY_H
