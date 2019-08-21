
#ifndef ADDCOUNT_H
#define ADDCOUNT_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlTableModel>
#include <databasemanager.h>
#include <QStringList>
#include <QFile>
#include <QTextStream>
class AddCount:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    AddCount();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU ps) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    virtual FunModuleInterface *Copy();
public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }
public:
    QStringList key;
    QStringList value;
private:
    void SaveInfo(QString info,QString str);
    void SaveTxt(QString path,QString info);
private:
    QString m_moduleName;
    QString Count;
    QString path_ok;
    QString path_ng;


};

#endif // ADDCOUNT_H
