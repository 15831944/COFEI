#ifndef JOGPROPERTY_H
#define JOGPROPERTY_H

#include <QObject>
#include "../libproperybase.h"

class JogProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(int timeout READ GetTimeout WRITE SetTimeout)
    Q_PROPERTY(int stupdly READ GetStupdly WRITE SetStupdly)
    Q_PROPERTY(int add READ GetAdd WRITE SetAdd)
    Q_PROPERTY(int dcc READ GetDcc WRITE SetDcc)
    Q_PROPERTY(QString axisId READ GetAxis WRITE SetAxis)
    Q_PROPERTY(int Spend READ GetSpend WRITE SetSpend)
    Q_PROPERTY(QString BitName READ GetBitName WRITE SetBitName)
    Q_PROPERTY(int value READ getValue WRITE setValue)
    Q_PROPERTY(int Maxpos READ getDirection WRITE setDirection)



public:
    JogProperty();
private:
   virtual void sendval(QString val="") Q_DECL_OVERRIDE;
   virtual void copyPropertyChild(LibProperyBase*);
public:
    void SetTimeout(int timout);
    int GetTimeout();

    void SetStupdly(int dly);
    int GetStupdly();

    void SetAdd(int add_i);
    int GetAdd();

    void SetDcc(int dcc_i);
    int GetDcc();

    void SetAxis(QString axisid);
    QString GetAxis();

    void SetSpend(int spend_i);
    int GetSpend();

    void SetBitName(QString bitstr);
    QString GetBitName();

    void setDirection(int dec);
    int getDirection();
private:
    int timeout;
    int stupdly;
    int add;
    int dcc;
    QString axisId;
    int Spend;
    QString BitName;
    int Maxpos;

public:
    bool SaveXml(QDomDocument *doc);
};

#endif // JOGPROPERTY_H
