#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <DycLibBase/PackLib/packlib.h>
#include <Comm.h>
class MyORGThread : public QThread
{
public:
    MyORGThread();
protected:
    void run();
private:
    PackLib * m_orgLib;
public:
    void SetParam(SWCONFIGXML_ORG_AXIS_STRU);
    void Stop(bool isNor = true);
    int m_result;
};

#endif // MYTHREAD_H
