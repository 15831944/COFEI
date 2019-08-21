#ifndef MotionControl_H
#define MotionControl_H

#include "palletinfo.h"
#include "axisinfo.h"
#include <MyGlobal.h>

class MotionControl
{
public:
    MotionControl();
    ~MotionControl();

public:
    QMap<QString, AxisParams> *axisMap;

    //初始化是否ok的标志，错误直接退出
    bool m_isInitStatOk;
    QMap<int,QMap<QString,uchar>> m_ContrlworldMap;
    QVector<int> m_CWPosVec;

    //表示各个轴的目标位置
    QMap<QString,int> m_CWRunPosMap;
    QVector<int> m_SWPosVec;
    QVector<int> m_OutModelPosVec;
    QVector<int> m_InModelPosVec;

    //每个轴允许的偏差值
    QMap<QString, double> m_deviation;

    //控制字的位置
    int m_ContrlWorldPos;

    //状态字的位置
    int m_StatusworldPos;
    uchar *tempOutputData;
    bool isLEAxis;
    int ierror;
    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOFFSON;

public:
    bool GetExcitationParams();
    void Init();
    void Start();
    QString GetStatus();

    int  GetContrlWordPos(QString strKey);
    int  GetStatuswordPos(QString strKey);
    int  GetInModelPos(QString strKey);
    int  GetOutModelPos(QString strKey);
    int  GetInputData(QString strKey,int &ireturn,int &curPos);
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool JudgeSWLimit(QString strKey,int CurPos,QString &strMsg);
    void StopAxis();
    void ChangeAxisORGModel();
    bool SetAxisInit(const QVector<int> &tempVec);
    bool CheckAxisInit();
    void SetAxisParm();
    void RunAxis();
    void SetAxisCWBit6();

    //获取每个轴允许的偏差值
    void GetDeviation(QString str);
};

#endif // MotionControl_H
