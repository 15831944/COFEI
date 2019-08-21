#ifndef PACKLIB_H
#define PACKLIB_H

#include "packlib_global.h"
#include <MyGlobal.h>
using namespace CoreGlobal;
class PACKLIBSHARED_EXPORT PackLib
{
public:
    PackLib();
public:
    void SetParam(SWCONFIGXML_ORG_AXIS_STRU);
    int RunAxis();
    void Stop(bool isNor = true);
private:
    SWCONFIGXML_ORG_AXIS_STRU _ps;
    bool AxisInit;
    bool isTerminate;
    int m_StatusworldPos;
    uchar m_StatusworldNum;
    int m_INputModelPos;
    int m_CurPos;
    uchar m_CurNum;
    int m_ErrorPos;
    uchar m_ErrorNum;

    int m_ContrlworldPos;
    uchar m_ContrlworldNum;
    int m_ModelPos;
    uchar m_ModelNum;
    int m_TraposPos;
    uchar m_TraposNum;
    int m_TraspeedPos;
    uchar m_TraspeedNum;
    int m_AccelPos;
    uchar m_AccelNum;
    int m_DecelPos;
    uchar m_DecelNum;
    int swLimitEnd;
    int swLimitStart;
    QString hwLimitStart;
    QString hwLimitEnd;
    uint iAcc;
    uint iDcc;

private:
    int GetResult(int itimeout,bool isSatrt,uchar uc);
    bool SetAxisOp();
    bool SetAxisOverParam();
    bool SetAxisBackParam();
    bool ChangeAxisParam();
    bool CheckAxisInfo();
    void SetAxisStart();
    void SetAxisChangeimmediately(uchar uc);
    void SetAxisStop();
    void ChangeAsixModel();
    int ApproachORG();
};

#endif // PACKLIB_H
