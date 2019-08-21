/*******************************************
 * @Author ying
 * @Date 2017/06/30
 * @Describe
 * 直角坐标插补模式命令插件用户只需要调用命令即可
 * 先进行公式计算得到需要的参数
 * 按照命令模式的要求实现此过程
 *
 */
#ifndef RECTCOOINTER_H
#define RECTCOOINTER_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QVector>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <DycLibBase/LCSPLib/lcsplib.h>
#include <QMutex>
#include <DycLibBase/Coordinate/coordinate.h>

//定义优先级
#define MY_PRIORITY (1) /* we use 49 as the PRREMPT_RT use 50
    as the priority of kernel tasklets 取值范围（１－９８）98为优先级最高
    and interrupt handler by default */
//定义栈的大小16M一般默认８Ｍ
#define MAX_SAFE_STACK (16*1024) /* The maximum stack size which is一般默认８KB 64位　１６kb
    guaranteed safe to access without*/
class RectCooInter:public QObject,public FunModuleInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID IBasicModule_iid )

    Q_INTERFACES(FunModuleInterface)
public:
    RectCooInter();
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
    void stack_prefault();
    typedef struct _LCSPIOTRIGGERINFO_STRU
    {
        int ctrn = -1;
        QString bitName = "";
        int stat = -1;
    }LCSPIOTRIGGERINFO_STRU;
private slots:
    void ThreadRun();
    void IOTRiggerSlot(QString,int);
    void Runiresult(const int);
private:
    QVector<QPair<int,int>> m_tempINSON;
    SWCONFIGXML_LCSP_STRU _ps;
    SWCONFIGXML_LCSP_STRU m_ps;
    Coordinate *m_swtoJ;
    QString m_moduleName;
    bool m_isInitOk;
    LCSPLib *m_LCSPlib;
    QThread *MyLCSPThread;
    QVector<QMap<QString,QVector<int>>> m_arrary;
    int m_runiresult;
    //获取控制字目标位置相关参数
    int m_iBytePos;
    int m_byteNum;
    QString m_ModelType;
    QMutex m_mutex;
private:
    void MyPriority();
    int GetStatuswordPos(QString strKey);
    int GetErrorcode(QString strKey);
    //void GetLCSPCurpos(QString strKey, int &curPos);
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool JudgeSWLimit(QString strKey,int CurPos,QString &strMsg);
    SWCONFIGXML_LCSP_STRU Getcsysaxisid(SWCONFIGXML_LCSP_STRU _ps);
public:
    void GetLCSPCurpos(QString strKey, int &curPos);
};

#endif // RECTCOOINTER_H
