#ifndef AXISRUNPOS_H
#define AXISRUNPOS_H
#include <MyGlobal.h>
#include <QObject>
using namespace CoreGlobal;

class AxisRunPos
{
public:
    AxisRunPos(SWCONFIGXML_QPOSIT_STRU ps);
    int RungoPos(QString &strMsg);
    void StopgoPos();
    void SetsumRunParam();

private:
    void GetInitParam();
    void GetContrlWorldPos(QString strKey);
    void GetStatusworldPos(QString strKey);
    bool SetInitaxis();
    bool CheckInit();
    void SetAxisCWBit6();
    void SetAxisParm();
    void StartAxis();
    bool JudgeHWLimit(QString strKey,QString &strMsg);
    bool JudgeSWLimit(QString strKey,int CurPos,QString &strMsg);
    void StopAxis();
    int GetInputData(QString strKey,int &ireturn,int &curPos);
    void EndAxisBit();
private:
    SWCONFIGXML_QPOSIT_STRU _ps;
    uchar *tempOutputData;
    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOFFSON;
    QVector<int> m_SWPosVec;
    QVector<int> m_CWPosVec;
    QMap<QString,int> m_ConPosMap;
    QMap<QString,int> m_StaPosMap;
    QMap<QString,int> m_OutModelPosMap;
    QMap<QString,int> m_InModelPosMap;
    QMap<QString,int> m_errorMap;
    QMap<QString,int> m_errcoderMap;
    QMap<QString,int> m_TraPosByteMap;
    QMap<QString,int> m_CurPosByteMap;
    QMap<QString,int> m_TraSpeedByteMap;
    QMap<QString,int> m_TraAccByteMap;
    QMap<QString,int> m_TraDccByteMap;
    bool m_initok;
//    QString m_type;

};

#endif // AXISRUNPOS_H
