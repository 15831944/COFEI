#ifndef COJS_H
#define COJS_H

#include <QObject>
#include <MyHW_Share.h>
#include <QTimer>
class JsORGThread : public QThread
{
public:
    JsORGThread();
    ~JsORGThread();
protected:
    void    run();

private:
    PackLib *m_OrgLib;
public:
    void    SetParam(SWCONFIGXML_ORG_AXIS_STRU orgAxis);
    void    Stop(bool isNor = true);
    int     m_OrgResult;
};
class CoJS : public QObject
{
    Q_OBJECT
public:
    explicit CoJS();
    ~CoJS();
signals:
    void    SendUpdateXMLEvent(QMap<QString,SWCONFIGXML_QPOSIT_STRU> ,QMap<QString,SWCONFIGXML_LCSP_STRU> );
public slots:
    QString GetFileContent(QString);
    bool    CreateFile(QString,QString);
    int     GetAxisCurPos(const QString AxisId);
    int     GetAxisCurtq(const QString AxisId);
    int     GetAxisErrorCode(const QString AxisId);
    int     GetAxisFault(const QString AxisId);
    int     GetAxisReadySw(const QString AxisId);
    void    SetSpeed(const uint);
    void    SetAcc(const uint);
    void    SetDcc(const uint);
    void    SetRatio(const uchar);
    bool    SetAxisSon(const QString, uchar uc);
    bool    SetAxisReset(const QString);
    bool    SetAxisORG(const QString);
    bool    SetAxisRun(const QString,int);
    bool    SetAxisStop(const QString);
    bool    WriteSWXml();
private slots:
    void    AxisTimer();
    void    UpdateSwSerXML(QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp,QMap<QString,SWCONFIGXML_LCSP_STRU> temp2);
private:
    void    Init();
private:
    JsORGThread *m_ORGThread;
    bool        m_isORG;
    QTimer      *m_AxisTimer;
    QString     m_AxisName;
    int         m_ContrlBytePos;
    int         m_StausBytePos;
    bool        SaveXmlIsOK;
    QMap<QString, BITINFO_STRU>     m_StausBitInfoMap;
    QMap<QString,BYTEINFO>          m_StausByteInfoMap;
    QMap<QString, BITINFO_STRU>     m_ContrlBitInfoMap;
    QMap<QString,BYTEINFO>          m_ContrlByteInfoMap;

};

#endif // COJS_H
