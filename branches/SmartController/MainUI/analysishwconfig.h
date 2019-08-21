/************************************
* @brief 摘要 解析字节的位置
* @author sunny
* @date 2017/06/01
************************************/
#ifndef ANALYSISHWCONFIG_H
#define ANALYSISHWCONFIG_H

#include <MyGlobal.h>
typedef struct _BYTEINFO
{
    QString strType = "";
    uchar byteNum = 0;
    int   bytePos = -1;

}BYTEINFO;


class AnalysisHwConfig
{
public:
    static int GetContrlWorldPos(const QString &Key,QMap<QString,BITINFO_STRU> &bitInfoMap,QMap<QString,BYTEINFO> &m_byteInfoMap);
    static int GetStatusWorldPos(const QString &strKey,QMap<QString,BITINFO_STRU> &bitInfoMap,QMap<QString,BYTEINFO> &m_byteInfoMap);
};

#endif // ANALYSISHWCONFIG_H
