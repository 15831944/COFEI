#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "mainframe_global.h"
#include <Comm.h>
#include <QList>
#include "customstruct.h"
#include "safethread.h"
class FlowDispatcher;
#define ChildFlowNum    16
class MAINFRAMESHARED_EXPORT MainFrame :public QObject
{
    Q_OBJECT
public:
    MainFrame();
    ~MainFrame();

public:
    void Start();
    void Stop();
    void Stop(const QString strKey);
    void NorPlay(const QString strKey);
    void NorStop();
    void NorStop(const QString strKey);
    void Pause();
    void Resume();
    void ResumeEvent();
    void ResetEvent();
    void ResetEvent(const QString strKey);
    void ORGEvent();
    void SafeThreadTOSatrt();

signals:
    void UpdateUi(QMap<QString,QVector<UIXML_STRU>> uiMap);
    void UpdateSwQPXMLEvent(QMap<QString,QMap<QString,SWCONFIGXML_QPOSIT_STRU>>);
    void UpdateRunTime(double);
    void ReSetORGRunOK(const QString,uchar ,bool);
private:
    QMap<QString,QPair<FlowDispatcher*,QThread*>> m_ObjectMap;
private:
    void StartFlow();
    void StartFlow(const QString strKey);
    bool CreateFlowDispatcher(const QString strKey);
public slots:
       void UpdateSwSerXML(QMap<QString,SWCONFIGXML_QPOSIT_STRU>,QMap<QString,SWCONFIGXML_LCSP_STRU>);
       void GetRunTime(double d);
       void GetRunResult(const QString str, uchar, bool);
};

#endif // MAINFRAME_H
