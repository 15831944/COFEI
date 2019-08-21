#ifndef LOUT_H
#define LOUT_H
#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <funmoduleinterface.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
class Lout:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Lout();
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
    virtual bool ReadEvent(uchar*)
    {
        return true;
    }
private:
    QString m_moduleName;
    INOUTPUT_STRU _ps;
};

#endif // LOUT_H
