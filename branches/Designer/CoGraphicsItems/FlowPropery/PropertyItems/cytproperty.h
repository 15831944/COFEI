#ifndef CYTPROPERTY_H
#define CYTPROPERTY_H
#include "../libproperybase.h"

class Cytproperty :public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString Note READ getNote WRITE setNote)
    Q_PROPERTY(QString ID READ getID WRITE setID)
    Q_PROPERTY(int Type READ GetKey WRITE setKey)
public:
    Cytproperty();
public:
    int Key;
public:

    int GetKey();
    void setKey(int k);

    QString getXmlString();
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*) Q_DECL_OVERRIDE;
};

#endif // CYTPROPERTY_H
