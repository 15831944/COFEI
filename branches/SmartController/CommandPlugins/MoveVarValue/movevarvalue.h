
#ifndef MOVEVARVALUE_H
#define MOVEVARVALUE_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QFile>
#include <QIODevice>

class MoveVarValue:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    MoveVarValue();
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
    MOVE_VAR_VALUE _ps;
    QString temppath;
    bool ReadTxt(QString path,QByteArray &buff);

};

#endif // MOVEVARVALUE_H
