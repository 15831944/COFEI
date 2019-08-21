
#ifndef RELATIVEMOVE_H
#define RELATIVEMOVE_H
#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <funmoduleinterface.h>
#include <MyGlobal.h>
#include <QDebug>
#include <QPair>
using namespace CoreGlobal;
class AsixStopThread : public QThread
{
    Q_OBJECT
public:
    AsixStopThread(){}
    ~AsixStopThread(){}
protected:
    void run()
    {
        BaseCalcFun::SetBitValue(2,m_ibytePos,m_ivalue);
    }
public:
    void SetAsixData(int ibytePos,char c)
    {
        m_ibytePos = ibytePos;
        m_ivalue = c;
    }
private:
    int m_ibytePos;
    char m_ivalue;
};
class RelativeMove:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    RelativeMove();
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
    //AsixStopThread m_asixStopThread;
    ///
    /// \brief _ps
    ///快速定位参数结构体
    SWCONFIGXML_QPOSIT_STRU _ps;
    //初始化是否ok的标志，错误直接退出
    bool m_isInitStatOk;
    QMap<int,QMap<QString,uchar>> m_ContrlworldMap;
    QVector<int> m_CWPosVec;
    //控制字的位置
    int m_ContrlWorldPos;
    //状态字的位置
    int m_StatusworldPos;
    uchar *tempOutputData;

    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOFFSON;
private:
    int  GetContrlWorldPos(QString strKey);
    int  GetStatusworldPos(QString strKey);
    int  GetModelData(QString strKey);
    int  GetInputData(QString strKey,int &ireturn,int &curPos);
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool JudgeSWLimit(QString strKey,int CurPos,QString &strMsg);
    void StopAxis();
    bool SetAxisInit(const QVector<int> &tempVec);
    bool CheckAxisInit();
    void SetAxisParm();
    void RunAxis();
    void ResetAllAxis(const QVector<int> &tempVec);
    void RelativeChange();
};

#endif // RELATIVEMOVE_H
