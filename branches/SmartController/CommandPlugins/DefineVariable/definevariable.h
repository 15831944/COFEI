
#ifndef DEFINEVARIABLE_H
#define DEFINEVARIABLE_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>

class DefineVariable:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    DefineVariable();
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
    bool isInitStatOk;
    int imodel;
    QMap<QString,uchar> m_dbBurninMap;
    bool CheckTPV();
    void QueryDb();
};

#endif // DEFINEVARIABLE_H
