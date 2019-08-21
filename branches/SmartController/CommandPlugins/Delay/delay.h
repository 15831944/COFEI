#ifndef DELAY_H
#define DELAY_H

#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <funmoduleinterface.h>

class Delay:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Delay();
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
    QString sTimeOut;
    bool m_isDigit;
};

#endif // DELAY_H
