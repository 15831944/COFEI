//#-------------------------------------------------
//# 插件说明：此插件是为设备运作当中针对双按钮启动进行开发的此插件；
//# 防止操作人员进行误操作，也为设备安全运行提供保障；
//#-------------------------------------------------
#ifndef DOUBLESTART_H
#define DOUBLESTART_H
#include <QObject>
#include <QtPlugin>
#include <QApplication>
#include <funmoduleinterface.h>
//DOUBLESTARTSHARED_EXPORT
//
class  DoubleStart:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)

public:
    DoubleStart();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU ps) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    virtual FunModuleInterface* Copy();
private:

private:

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
    QString                m_moduleName;//定义插件模块的名字
    QVector<INOUTPUT_STRU> _ps;//
private:
    bool CheckButtonIOStat0();
};

#endif // DOUBLESTART_H
