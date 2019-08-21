
#ifndef LTIMER_H
#define LTIMER_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>

class LTimer:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    LTimer();
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
    TIMERInfo_STRU _ps;
    float m_timeuse;
    struct timeval tpStart,tpEnd;
    QString m_TimerId;
};

#endif // LTIMER_H
