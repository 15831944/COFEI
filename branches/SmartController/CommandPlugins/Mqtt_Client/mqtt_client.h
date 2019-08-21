
#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>

class Mqtt_Client:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Mqtt_Client();
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
    QString get_send_message(QString path);
    QString m_moduleName;
    SWCONFIGXML_MQTT_DATA_STRU _ps;
    QString temppath;

};

#endif // MQTT_CLIENT_H
