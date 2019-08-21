#ifndef JSPROPERTY_H
#define JSPROPERTY_H
#include "../libproperybase.h"

class JSProperty :public LibProperyBase
{
    Q_OBJECT

    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString scriptName READ getScriptName WRITE setScriptName)

    Q_PROPERTY(QString dataSave READ getDataSave WRITE setDataSave)

    Q_PROPERTY(QString type READ getType WRITE setType)

    Q_PROPERTY(QString typeID READ getTypeID WRITE setTypeID)
public:
    JSProperty();
    QString scriptName;
    QString dataSave;
    QString type;
    QString typeID;

    QString getScriptName();
    void setScriptName(QString);

    QString getDataSave();
    void setDataSave(QString val);

    QString getType();
    void setType(QString);

    QString getTypeID();
    void setTypeID(QString);
private:
   virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*inPro);
    // LibProperyBase interface
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // JSPROPERTY_H
