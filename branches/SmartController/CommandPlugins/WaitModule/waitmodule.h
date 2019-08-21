
#ifndef WAITMODULE_H
#define WAITMODULE_H
#include <QObject>
#include <QApplication>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <funmoduleinterface.h>
#include <DycLibBase/StackManager/stackmanager.h>
class WaitModule:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    WaitModule();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU ps) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    virtual FunModuleInterface* Copy();
public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }

private:
    QString m_moduleName;
    SWCONFIGXML_WAIT_STRU _ps;

};

#endif // WAITMODULE_H
