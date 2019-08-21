#ifndef STOPTHREAD_H
#define STOPTHREAD_H
#include <QThread>
#include <QObject>
#include <MyGlobal.h>
using namespace CoreGlobal;
class StopThread : public QThread
{
    Q_OBJECT
public:
    StopThread();
protected:
    void run();
public:
    int bytepos;
};

#endif // STOPTHREAD_H
