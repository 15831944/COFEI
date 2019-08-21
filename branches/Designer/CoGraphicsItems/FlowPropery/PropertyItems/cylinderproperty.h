#ifndef CYLINDERPROPERTY_H
#define CYLINDERPROPERTY_H
#include "../libproperybase.h"

class CylinderProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )

    Q_PROPERTY(int value READ getValue WRITE setValue)

    Q_PROPERTY(QString note READ getNote WRITE setNote)

    Q_PROPERTY(QString softOut READ getSoftOut WRITE setSoftOut)

    Q_PROPERTY(QString originPoint READ getOriginPoint WRITE setOriginPoint)

    Q_PROPERTY(QString movePoint READ getMovePoint WRITE setMovePoint)

    Q_PROPERTY(int timeOut READ getTimeOut WRITE setTimeOut)

    Q_PROPERTY(int stepDelay READ getStepDelay WRITE setStepDelay)

    Q_PROPERTY(QString tips1 READ getTips1 WRITE setTips1)
    Q_PROPERTY(int key1 READ getKey1 WRITE setKey1 )

    Q_PROPERTY(QString tips2 READ getTips2 WRITE setTips2)
    Q_PROPERTY(int key2 READ getKey2 WRITE setKey2 )

    Q_PROPERTY(QString tips3 READ getTips3 WRITE setTips3)
    Q_PROPERTY(int key3 READ getKey3 WRITE setKey3 )

    Q_PROPERTY(QString tips4 READ getTips4 WRITE setTips4)
    Q_PROPERTY(int key4 READ getKey4 WRITE setKey4 )

    Q_PROPERTY(QString tips5 READ getTips5 WRITE setTips5)
    Q_PROPERTY(int key5 READ getKey5 WRITE setKey5 )

private:

    QString softOut;
    QString originPoint;
    QString movePoint;
    int timeOut;
    int stepDelay;

    QString tips1;
    int key1;

    QString tips2;
    int key2;

    QString tips3;
    int key3;

    QString tips4;
    int key4;

    QString tips5;
    int key5;

public:
    CylinderProperty();



    virtual QString getXmlString() Q_DECL_OVERRIDE;

    virtual void setValue(int) Q_DECL_OVERRIDE;


    QString getSoftOut();
    void setSoftOut(QString val);


    QString getOriginPoint();
    void setOriginPoint(QString val);


    QString getMovePoint();
    void setMovePoint(QString val);

    int getTimeOut();
    void setTimeOut(int val);

    int getStepDelay();
    void setStepDelay(int val);

    QString getTips1();
    void setTips1(QString val);
    int getKey1();
    void setKey1(int val);

    QString getTips2();
    void setTips2(QString val);
    int getKey2();
    void setKey2(int val);

    QString getTips3();
    void setTips3(QString val);
    int getKey3();
    void setKey3(int val);

    QString getTips4();
    void setTips4(QString val);
    int getKey4();
    void setKey4(int val);

    QString getTips5();
    void setTips5(QString val);
    int getKey5();
    void setKey5(int val);


private:
   virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);


    // LibProperyBase interface
public:
    bool SaveXml(QDomDocument *doc);
};

#endif // CYLINDERPROPERTY_H
