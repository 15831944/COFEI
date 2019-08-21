#ifndef SHARE_VALUE_H
#define SHARE_VALUE_H
#include <QMutex>
class ShareClass
{
public:
    static ShareClass* GetIstance()
    {
        static ShareClass Instance;
        return &Instance;
    }
public:
    volatile bool m_direction;
    volatile bool m_ready = false;
    QMutex        m_mutex;
    volatile int  m_cruntValue = 0;
    bool          m_threadstop;
    bool          m_direction_Stop;
    bool          m_getvalue_Stop;
    //xin hao
    QString       A_IO_Address;
    QString       B_IO_Address;
};
#endif // SHARE_VALUE_H
