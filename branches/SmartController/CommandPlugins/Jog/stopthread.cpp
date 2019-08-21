#include "stopthread.h"
StopThread::StopThread()
{
    bytepos = -1;
}

void StopThread::run()
{
    BaseAxisOperate::SetAxisStop(2,bytepos);
}
