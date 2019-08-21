
#ifndef BURNCYLINDER_H
#define BURNCYLINDER_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <MyGlobal.h>
using namespace CoreGlobal;
class BurnCylinder:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    BurnCylinder();
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
    SWCONFIGXML_CY_STRU _ps;
};

#endif // BURNCYLINDER_H
