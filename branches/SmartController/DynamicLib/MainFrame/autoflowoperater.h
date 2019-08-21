#ifndef AUTOFLOWOPERATER_H
#define AUTOFLOWOPERATER_H
#include "customstruct.h"
#include <QObject>
#include <Comm.h>
#include <MyConfig_Shared>

class AutoFlowOperater :public QObject
{
     Q_OBJECT
public:
    AutoFlowOperater();
public :
    void    SetFlowKey(const QString);
    void    SetFlowMap(QMap<int, QMap<QString,FLOWTXT_STRU>>);
    void    StopAutoFlow();
    void    Run();
public:
    bool                                    m_stopFlag;
private:
    void    RunSingle(const QString, QMap<QString,FunModuleInterface*>);
    void    ExeFuntion(EXE_PARAM, int&, FunModuleInterface** );
    int     HandleIfFunction(const IFInfo_STRU&, const QString );
    int     HandleIfErrorFun(const QString&);
    void    ExeCallFunc(const QString , int &,QMap<int,QMap<QString,FLOWTXT_STRU>>);
    void    GetPluginsMap();
private:
    int                                     m_forNum;
    bool                                    m_ifJump;
    QMap<QString,int>                       m_ifCountMap;
    QMap<QString,int>                       m_IDsMap;
    bool                                    m_isSPLPause;
    FunModuleInterface                      *m_fun;
    QMap<int,QMap<QString,FLOWTXT_STRU>>    m_flowCmdMap;
    QMap<QString, FunModuleInterface *>     m_pluginMap;
    QString                                 m_flowName;
    int                                     m_callCurrentRunIndex;
    int                                     m_currentRunIndex;
};

#endif // AUTOFLOWOPERATER_H
