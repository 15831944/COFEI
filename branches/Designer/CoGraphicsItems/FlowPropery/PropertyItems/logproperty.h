#ifndef LOGPROPERTY_H
#define LOGPROPERTY_H

#include "../libproperybase.h"
class LOGProperty : public LibProperyBase
{
    Q_OBJECT

    Q_PROPERTY(QString Note READ getNote WRITE setNote)

    Q_PROPERTY(QString information READ getInformation WRITE setInformation)

    Q_PROPERTY(TypeInfo typeInfo READ getTypeInfo WRITE setTypeInfo)

public:
    LOGProperty();
    enum TypeInfo
    {
        Warning=0,
        Normal
    };
    Q_ENUM(TypeInfo)

    QString getInformation();
    void setInformation(QString);

    TypeInfo getTypeInfo();
    void setTypeInfo(TypeInfo);
private:
    QString information;

    TypeInfo typeInfo;

    virtual void sendval(QString val="") Q_DECL_OVERRIDE ;
    virtual void copyPropertyChild(LibProperyBase*) ;
};

#endif // LOGPROPERTY_H
