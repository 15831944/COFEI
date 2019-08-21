/************************************
* @brief 摘要 快速定位插件
* @author sunny
* @date 2017/06/01
************************************/
#ifndef RAPIDPOSITION_H
#define RAPIDPOSITION_H

#include <QObject>
#include <QtPlugin>
#include <QTimer>
#include <QThread>
#include <funmoduleinterface.h>
#include <MyGlobal.h>
#include <QDebug>
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

class RapidPosition:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    RapidPosition();

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
    virtual bool ReadEvent(uchar *)
    {
        return true;
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
    //QMap<QString,int> m_CWRunPosMap;
    QVector<int> m_SWPosVec;
    QVector<int> m_OutModelPosVec;
    QVector<int> m_InModelPosVec;
    //控制字的位置
    int m_ContrlWorldPos;
    QMap<QString,int> m_errorMap;
    //状态字的位置
    int m_StatusworldPos;
    uchar *tempOutputData;
    bool isLEAxis;
    int ierror;
    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOFFSON;
private:
    int  GetContrlWorldPos(QString strKey);
    int  GetStatusworldPos(QString strKey);
    int  GetInModelPos(QString strKey);
    int  GetOutModelPos(QString strKey);
    int  GetInputData(QString strKey,int &ireturn,int &curPos);
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool JudgeSWLimit(QString strKey,int CurPos,QString &strMsg);
    void StopAxis();
    void ChangeAxisORGModel();
    bool SetAxisInit(const QVector<int> &tempVec);
    bool CheckAxisInit();
    void SetAxisParam();
    void RunAxis();
    void SetAxisCWBit6();
    void SetModelParam();
};

#endif // RAPIDPOSITION_H
