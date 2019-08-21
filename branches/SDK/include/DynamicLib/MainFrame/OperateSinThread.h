#ifndef OperateSinThread_H
#define OperateSinThread_H

#include <QObject>
#include <QMutex>
#include "customstruct.h"
class OperateSinThread : public QObject
{
    Q_OBJECT
public:
    OperateSinThread();
signals:
    void    FaultEvent();
public slots:
    void    Run();
    void    ThreadRun(const QString);
    void    SetFlowKey(const QString);
    void    SetFlowMap(QMap<int, QMap<QString,FLOWTXT_STRU>>);
    void    ThreadIsFault();
    void    EMStopFlow();
    void    ReSetFlow();
public:
    bool    m_stopFlag;
private slots:
    void    EMStopFlowSlot();
private:
    void    ExeFuntion(EXE_PARAM, int&, FunModuleInterface** );
    int     HandleIfFunction(const IFInfo_STRU&, const QString );
    int     HandleIfErrorFun(const QString&);
    void    ModifedCurIndex(QMap<int,QMap<QString,FLOWTXT_STRU>>::iterator &);
private:
    void    GetPluginInstances();
    void    ExeCallFunc(const QString , int &,QMap<int,QMap<QString,FLOWTXT_STRU>>);
private:
    QMutex                                  *m_mutex;
    int                                     m_currentRunIndex;
    bool                                    m_threadFault;
    int                                     m_callCurrentRunIndex;
    int                                     m_forNum;
    bool                                    m_ifJump;
    QMap<QString,int>                       m_ifCountMap;
    QMap<QString,int>                       m_IDsMap;
    FunModuleInterface                      *m_fun;
    QMap<int,QMap<QString,FLOWTXT_STRU>>    m_flowCmdMap;
    QString                                 m_flowName;
    QMap<QString,FunModuleInterface*>       m_pluginMap;
    //QMap<QString,QPair<FlowDispatcher*,QThread*>> m_OpThreadMap;
};

#endif // OperateSinThread_H
