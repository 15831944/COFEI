#ifndef OPERATEMAINFLOW_H
#define OPERATEMAINFLOW_H
#include "safethread.h"
#include "customstruct.h"
#include <QMutex>
#include <QWaitCondition>
class OperateThread;
class FlowOperater :public QObject
{
    Q_OBJECT
public:
    FlowOperater();
signals:
    void    FaultEvent();
public slots:
    void    Run(const QString, QMap<QString,FunModuleInterface*>);
    void    SetFlowKey(const QString);
    void    SetFlowMap(QMap<int, QMap<QString,FLOWTXT_STRU>>);
    void    ThreadIsFault();
    void    EMStopFlow();
    void    StopFlow();
    void    PauseFlow();
    void    ResumeFlow();
    void    ReSetFlow();
public:
    bool    m_stopFlag;
    bool    m_pause;
private slots:
    void    EMStopFlowSlot();
private:
    void    ExeFuntion(EXE_PARAM, int&, FunModuleInterface** );
    int     HandleIfFunction(const IFInfo_STRU&, const QString );
    int     HandleIfErrorFun(const QString&);
    QString ReadTxt(QString path);
    void    ModifedCurIndex(QMap<int,QMap<QString,FLOWTXT_STRU>>::iterator &);
    void    ExeCallFunc(const QString , int &,QMap<int,QMap<QString,FLOWTXT_STRU>>);
private:
    QMutex                                  *m_mutex;
    QMutex                                  *m_PauseMutex;
    QWaitCondition                          m_PauseCondition;
    int                                     m_currentRunIndex;
    bool                                    m_threadFault;
    int                                     m_callCurrentRunIndex;
    int                                     m_forNum;
    bool                                    m_ifJump;
    QMap<QString,int>                       m_ifCountMap;
    QMap<QString,int>                       m_IDsMap;
    bool                                    m_isSPLPause;
    FunModuleInterface                      *m_fun;
    QMap<int,QMap<QString,FLOWTXT_STRU>>    m_flowCmdMap;
    QMap<QString, FunModuleInterface *>     m_puglinMap;
    QString                                 m_flowName;
    QMap<QString,QPair<OperateThread*,QThread*>> m_OpThreadMap;
};

#endif // OPERATEMAINFLOW_H
