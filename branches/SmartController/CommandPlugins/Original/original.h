#ifndef ORIGINAL_H
#define ORIGINAL_H
#include <QApplication>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include "myORGthread.h"
#include <QString>
class Original:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Original();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(SWCONFIGXML_STRU) ;
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
    QString m_moduleName;
    SWCONFIGXML_ORG_OP_STRU _ps;
    bool isInitStatOk;
    QMap<QString,MyORGThread *> m_ORGLibMap;
    uchar *tempOutputData;

    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
private:
    void StopAxis();
    int  GetContrlWorldPos(QString strKey);
    int  GetStatusworldPos(QString strKey);
    bool SetAxisInit(const QVector<int> &tempVec);
    bool CheckAxisInit();
};

#endif // ORIGINAL_H
