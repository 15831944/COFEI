#ifndef TORQUE_H
#define TORQUE_H
#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <funmoduleinterface.h>
#include <DycLibBase/TorqueChart/torquechart.h>
#include <QElapsedTimer>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
class Torque:public QObject,FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    Torque();
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
    QString m_moduleName;
   SWCONFIGXML_QTORQUE_STRU _ps;
    //初始化是否ok的标志，错误直接退出
    bool m_isInitStatOk;
    QMap<int,QMap<QString,uchar>> m_ContrlworldMap;
    //控制字的位置
    int m_ContrlWorldPos;
    //状态字的位置
    int m_StatusworldPos;
    //Dang Qian Zhou
    QString m_axis;
    //SAVE PATH
    QString temppath;
    //Bit State
    QString bitstate;
private:
    int  GetContrlWorldPos(QString strKey);
    int  GetStatusworldPos(QString strKey);
    int  GetInputData(QString strKey,int &ireturn,int &curPos);
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool JudgeSWLimit(QString strKey,int CurPos,QString &strMsg);
    void StopAxis(QString strKey);
    bool SetAxisInit(QString strKey);
    void SetAxisParm(QString strKey,TORQUE_INFO_STRU axisInfoStru);
    void RunAxis(QString strKey);
    void SavePoint(QString filename, QStringList data);
    void ResetStartState(int val);
};


#endif // TORQUE_H
