#ifndef LCSPLIB_H
#define LCSPLIB_H
#include <MyGlobal.h>
#include "lcsplib_global.h"
#include <QObject>
#include<QTimer>
using namespace CoreGlobal;
class LCSPLIBSHARED_EXPORT LCSPLib:public QObject
{
    Q_OBJECT
private: //关于轴的触发需要的参数情况
    typedef struct _LCSPIOTRIGGERINFO_STRU
    {
        int ctrn = -1;
        QString bitName = "";
        int stat = -1;
    }LCSPIOTRIGGERINFO_STRU;
public :
    LCSPLib();
    QVector<QMap<QString ,QVector<int>>> GetLCSPArray(const SWCONFIGXML_LCSP_STRU _ps);
    QVector<LCSPIOTRIGGERINFO_STRU> GetLCSPTriggerInfoVector(const SWCONFIGXML_LCSP_STRU _ps);
    bool InitCspAxisParam(const SWCONFIGXML_LCSP_STRU _ps);
    int SONAxis();
    int RunLCSPAxis(const SWCONFIGXML_LCSP_STRU _ps);//运行插补模式的轴
    void SOFFAxis();
    void StopLCSP(const QString);//停止轴
private:
    int f;//插补周期nｓ
    double x;//加速度阶段的位移
    double k;
    QMap<QString,int> m_CWByteMap;
    QMap<QString,int> m_TraPosByteMap;
    QMap<QString,int> m_TraSpeedByteMap;
    QMap<QString,int> m_TraModelByteMap;
    QMap<QString,int> m_TraAccByteMap;
    QMap<QString,int> m_TraDccByteMap;
    QMap<QString,int> m_SWByteMap;
    QMap<QString,int> m_CurPosByteMap;
    QMap<QString,int> m_CurModelByteMap;
    QVector<int> m_CWPosVec;
    QVector<int> m_SWPosVec;
    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOffOP;
    QVector<QString> m_AxisIdVec;
    QMutex m_mutex;
    QVector<QMap<QString,int >>m_summap;
    QMap<QString,double> m_eleratMap;
    QVector<QMap<QString,double>> m_calTrik;
    QVector<QString> m_PosMaxkey;
    QTimer *m_timer ;
    QString m_bitname;
    uchar m_value;

signals:
    void IOTRigger(QString,int);//用于触发io信息
    void ExcFinish(const int );
private:
    QVector<QMap<QString,QVector<int>>> HandleArray(const SWCONFIGXML_LCSP_STRU _ps);
    bool GetLCSPCurpos(QString strKey, int &curPos);
    bool GetlcspaxisinitParam();
    bool CheckLCSPAxisInit();
    void GetContrlWordPos(QString strKey);
    void  GetStatuswordPos(QString strKey);
    bool Getaccandctrspeed(const SWCONFIGXML_LCSP_STRU _ps,int axisNum);
    bool GetLCSPTriggerInfo(const QVector<LCSP_TRIGGERINFO_STRU> , int ,const QVector<QMap<QString, QVector<int> > >);
    bool handlesummap(QVector<QMap<QString, int>>,QVector<QMap<QString,QVector<int>>>&);
    bool Calculformula(QMap<QString,int>, QMap<QString, int>axisfrontmappos, QVector<QMap<QString, QVector<int> > > &sumlcspvec);
    int  GetMaxPosnum(QMap<QString, int> axismappos, QMap<QString, int>axisfrontmappos);
    void RunLCSPSinAxis(const QString ,QMap<QString, QVector<int> >);//运行一个轴
    void SetLCSPSUMTraPosandModel();//目标位置赋值和模式更改
private:
    void GetLCSPTraPosParam(QString strkey);
    QMap<int, LCSPLib::LCSPIOTRIGGERINFO_STRU> m_trimap;
private slots:
    void IOTri(QString bitName, int stat);
    void update();


};

#endif // LCSPLIB_H
