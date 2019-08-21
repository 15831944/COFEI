
#ifndef RECKONBYTIME_H
#define RECKONBYTIME_H
#include <QObject>
#include <QtPlugin>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
//#include <databasemanager.h>
#include <QStringList>
#include <QDateTime>
#include <funmoduleinterface.h>

class ReckonByTime:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    ReckonByTime();
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
    DIYLOG_STRU _ps;
};

#endif // RECKONBYTIME_H
