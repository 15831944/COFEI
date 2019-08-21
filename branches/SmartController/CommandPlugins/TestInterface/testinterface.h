#ifndef TESTINTERFACE_H
#define TESTINTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <funmoduleinterface.h>
#include<QStringList>
#include <MyGlobal.h>
using namespace CoreGlobal;
class TestInterface:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    TestInterface();
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
    bool CheckBasePoint();
    bool CheckMovePoint();
    static int AnalysisIOStringS(QMap<QString,uchar> &ValueMap);
private:
    QString m_moduleName;
    SWCONFIGXML_CY_STRU _ps;
    bool isInitStatOk;
    int m_iError;
};

#endif // TESTINTERFACE_H
