#ifndef TCPWPROPERTY_H
#define TCPWPROPERTY_H
#include "../libproperybase.h"

class TCPWProperty: public LibProperyBase
{
    Q_OBJECT

    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString netID READ getNetID WRITE setNetID)

    Q_PROPERTY(QString dataSave READ getDataSave WRITE setDataSave)

    Q_PROPERTY(QString IP READ getIP WRITE setIP)

    Q_PROPERTY(QString data READ getData WRITE setData)
public:
    TCPWProperty();

    QString netID;
    QString dataSave;
    QString IP;
    QString data;

    QString getNetID();
    void setNetID(QString val);

    QString getDataSave();
    void setDataSave(QString val);

    QString getIP();
    void setIP(QString val);

    QString getData();
    void setData(QString val);


private:
   virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
    // LibProperyBase interface
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // TCPWPROPERTY_H
