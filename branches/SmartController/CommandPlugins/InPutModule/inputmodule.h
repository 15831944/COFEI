#ifndef INPUTMODULE_H
#define INPUTMODULE_H
#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <QApplication>
#include <funmoduleinterface.h>
class  InPutModule :public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    InPutModule();
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
    int CheckIOBit();
private:
    QString m_moduleName;
    INOUTPUT_STRU _ps;
    uchar m_ioPos;
    ushort m_ioValue;
    bool m_CommOk;
};

#endif // INPUTMODULE_H
