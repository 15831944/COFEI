#include "myORGthread.h"

MyORGThread::MyORGThread()
{
    m_orgLib = new PackLib;
    m_result = 1;
}
void MyORGThread::run()
{
    m_result = m_orgLib->RunAxis();
}
void MyORGThread::SetParam(SWCONFIGXML_ORG_AXIS_STRU orgAxis)
{
    m_orgLib->SetParam(orgAxis);
}

void MyORGThread::Stop(bool isNor)
{
    m_orgLib->Stop(isNor);
}
