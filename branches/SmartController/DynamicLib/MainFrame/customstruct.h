#ifndef CUSTOMSTRUCT_H
#define CUSTOMSTRUCT_H
#include <funmoduleinterface.h>
#include <QMutex>
#define __QUITRUN(x)  if(x){\
                    return;}

typedef struct _EXE_PARAM
{
    FLOWTXT_STRU flowStru;
    QString strkey;
    QString strMsg;
    _EXE_PARAM()
    {
        strkey = "";
        strMsg = "";
    }
}EXE_PARAM;

class FlowData
{
private:
    FlowData()
    {
        m_stopFlag = false;
        m_transMap.clear();
        m_transMap.insert("RS_R","串口通信");
        m_transMap.insert("RS_W","串口通信");
        m_transMap.insert("TCP_R","网络通信");
        m_transMap.insert("TCP_W","网络通信");
    }

public:
    static FlowData *GetInstance()
    {
        static FlowData pInstance;

        return &pInstance;
    }
public:
    QMap<QString,QString> m_transMap;
    bool m_stopFlag;
    QMutex m_mutex;
};
#endif // CUSTOMSTRUCT_H
