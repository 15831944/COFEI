
#ifndef JOG_H
#define JOG_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <MyGlobal.h>
#include <QDebug>
#include <QApplication>
#include <stopthread.h>
#include "MyHW_Share.h"
using namespace CoreGlobal;
class Jog:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Jog();
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
private:
    //控制字的位置
    int m_ContrlWorldPos;
    //状态字的位置
    int m_StatusworldPos;
    QVector<int> m_CWPosVec;
    int m_AxisNum;
    uchar *tempOutputData;
    SWCONFIGXML_JOG_STRU _ps;
    bool m_isInitStatOk;
    uchar value;
    QMap<QString,StopThread*> StopMap;
     int ireturn = -1;

     QMap<QString,BITINFO_STRU> bitInfoMap;
     QMap<QString,BYTEINFO> byteInfoMap;

     QMap<QString, BITINFO_STRU> InbitInfoMap;
     QMap<QString,BYTEINFO> InbyteInfoMap;
private:
    bool SetAxisInit(const QVector<int> &tempVec);
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool CheckAxisInit();
    void SetAxisParm();
    void RunAxis();
    void ResetAllAxis(const QVector<int> &tempVec);
    void StopAxis();
};

#endif // JOG_H
