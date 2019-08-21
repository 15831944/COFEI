#ifndef LABELPROPERTY_H
#define LABELPROPERTY_H
#include "../libproperybase.h"

class LabelProperty :public LibProperyBase
{
    Q_OBJECT

    Q_PROPERTY(QString Label READ getLabel WRITE setLabel)
     Q_PROPERTY(QString note READ getNote WRITE setNote)
public:
    LabelProperty();
    QString Label;

    QString getLabel();
    void setLabel(QString label);

    QString getXmlString();
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*) Q_DECL_OVERRIDE;
};

#endif // LABELPROPERTY_H
