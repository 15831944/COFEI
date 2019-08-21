
#ifndef PRINTLOG_H
#define PRINTLOG_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>

class PrintLog:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    PrintLog();
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
private:
    QString m_log;
    DIYLOG_STRU _ps;

};

#endif // PRINTLOG_H
