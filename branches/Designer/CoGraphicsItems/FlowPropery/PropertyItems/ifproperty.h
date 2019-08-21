#ifndef IFPROPERTY_H
#define IFPROPERTY_H
#include "../libproperybase.h"

class IFProperty : public LibProperyBase
{
    Q_OBJECT


    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(IFCONDITION resultType READ getResultType WRITE setResultType)

     Q_PROPERTY(IFCONDITIONTYPE conditionType READ getConditionType WRITE setConditionType)

     Q_PROPERTY(QString conditionValue READ getConditionValue WRITE setConditionValue)
public:

    IFProperty();
     virtual QString getXmlString() Q_DECL_OVERRIDE;

    enum IFCONDITION
    {
        Stop=1,
        Jump,
        clear

    };
    Q_ENUM(IFCONDITION)

    enum IFCONDITIONTYPE
    {
        Data=1,
        Count
    };
    Q_ENUM(IFCONDITIONTYPE)

    IFCONDITION getResultType();
    void setResultType(IFCONDITION val);


    IFCONDITIONTYPE getConditionType();
    void setConditionType(IFCONDITIONTYPE val);

    QString getConditionValue();
    void setConditionValue(QString val);


private:
    IFCONDITION resultType;

    QString getLabelName();
    void setLabelName(QString val);
    QString labelName;

    IFCONDITIONTYPE conditionType;
    QString conditionValue;
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);

    // LibProperyBase interface
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // IFPROPERTY_H
