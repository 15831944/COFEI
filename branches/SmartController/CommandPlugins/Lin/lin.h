#ifndef LIN_H
#define LIN_H
#include <QObject>
#include <QApplication>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <funmoduleinterface.h>

//LINSHARED_EXPORT
class Lin:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Lin();
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
    bool CheckIOBit();
private:
    QString m_moduleName;
    INOUTPUT_STRU _ps;
    QString str;
};


#endif // LIN_H
