#ifndef MOVEG101PROPERTY_H
#define MOVEG101PROPERTY_H
#include "../libproperybase.h"

class MoveG101Property : public LibProperyBase
{
    Q_OBJECT
  //  Q_PROPERTY(QString ID READ getID WRITE setID )
    Q_PROPERTY(QString pointname READ getPointname WRITE setPointName)
    Q_PROPERTY(QString note READ getNote WRITE setNote)
    Q_PROPERTY(int timeout READ getTimeOut WRITE setTimeout)
    Q_PROPERTY(int dly READ getdly WRITE setdly)
    Q_PROPERTY(int Velocitypercent READ getVelocitypercent WRITE setVelocitypercent)
    Q_PROPERTY(int acc READ getacc WRITE setacc)
    Q_PROPERTY(int dcc READ getdcc WRITE setdcc)
    Q_PROPERTY(int AxisNum READ getAxisNum WRITE setAxisNum)
    Q_PROPERTY(QString AxisInfo READ getAxisInfo WRITE setAxisInfo)
    Q_PROPERTY(int AxisVltypert READ getAxisVltypert WRITE setAxisVltypert)
    Q_PROPERTY(int tq READ gettq WRITE settq)





public:
    MoveG101Property();
    virtual QString getXmlString() Q_DECL_OVERRIDE;
    bool SaveXml(QDomDocument *doc);

    QString getPointname();
    void setPointName(QString);

    int getTimeOut();
    void setTimeout(int tme);

    int getdly();
    void setdly(int dlyval);

    int getVelocitypercent();
    void setVelocitypercent(int velocitypcent);

    int getacc();
    void setacc(int ac);

    int getdcc();
    void setdcc(int dc);

    int getAxisNum();
    void setAxisNum(int axisnum);

    QString getAxisInfo();
    void setAxisInfo(QString axininfo);

    int getAxisVltypert();
    void setAxisVltypert(int Avp);

    int gettq();
    void settq(int tqval);

private:
    QString pointname;
    int timeout;
    int dly;
    int Velocitypercent;
    int acc;
    int dcc;
    int AxisNum;
    QString AxisInfo;
    int AxisVltypert;
    int tq;
private:
     virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // MOVEG101PROPERTY_H
