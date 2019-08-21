#ifndef FLOWDISPATCHER_H
#define FLOWDISPATCHER_H
#include <QObject>
#include <funmoduleinterface.h>
#include <QMap>
class FlowOperater;
class FlowDispatcher :public QObject
{
    Q_OBJECT
public:
    FlowDispatcher();
signals:
    void RunResult(const QString,uchar ,bool);
public slots:
    void SetFlowName(const QString);
    void SetLogTitle(const QString);
    void SetFlowCmdMap(const QMap<int,QMap<QString,FLOWTXT_STRU>>);
    void SetFlowMode(bool);
    void SetFlowType(const uchar uc);
    void Run();

    void EmStopSlot();
    void StopSlot();
    void PauseSlot();
    void ResumeSlot();
    void ResetSlot();
private:
    void GetPluginInstances();
private:
    QMap<QString,FunModuleInterface*>       m_pluginMap;
    QString                                 m_flowName;
    FlowOperater                            *m_pFlowHandle;
    bool                                    m_isLoop;
    QString                                 m_logTitle;
    uchar                                   m_flowType;


};

#endif // FLOWDISPATCHER_H
