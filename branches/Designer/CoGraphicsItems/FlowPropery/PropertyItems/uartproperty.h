#ifndef UARTPROPERTY_H
#define UARTPROPERTY_H
#include "../libproperybase.h"

#include <QObject>

class UartProperty : public LibProperyBase
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID WRITE setID )
    Q_PROPERTY(QString note READ getNote WRITE setNote)
    Q_PROPERTY(QString SeriportID READ getSeriportID WRITE setSeriportID)
    Q_PROPERTY(QString SavePath READ getSavePath WRITE setSavePath)
    Q_PROPERTY(QString DataCompare READ getDataCompare WRITE setDataCompare)
    Q_PROPERTY(QString Start READ getCheckStr WRITE setCheckStr)
    Q_PROPERTY(QString End READ getEndStr WRITE setEndStr)
    Q_PROPERTY(UARTCOMMAND CommandStr READ getCommandStr WRITE setCommandStr)

public:
    UartProperty();
    enum UARTCOMMAND{
        RS_R,
        RS_W
    };
    Q_ENUM(UARTCOMMAND)
public:
    QString getSeriportID();
    void setSeriportID(QString id);

    QString getSavePath();
    void setSavePath(QString path);

    QString getDataCompare();
    void setDataCompare(QString data);

    QString getCheckStr();
    void setCheckStr(QString str);

    QString getEndStr();
    void setEndStr(QString str);

    UARTCOMMAND getCommandStr();
    void setCommandStr(UARTCOMMAND command);

    bool SaveXml(QDomDocument *doc);
private:
    QString SeriportID;
    QString SavePath;
    QString DataCompare;
    QString Start;
    QString End;
    UARTCOMMAND CommandStr;
private:
    virtual void sendval(QString val="") Q_DECL_OVERRIDE;
    virtual void copyPropertyChild(LibProperyBase*);
};

#endif // UARTPROPERTY_H
