#include "autoflowframe.h"

AutoFlowFrame::AutoFlowFrame()
{
  m_AutoObjectMap.clear();
}

AutoFlowFrame::~AutoFlowFrame()
{

}

void AutoFlowFrame::Start()
{
    for(auto item = ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap.begin();
        item != ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap.end();++item)
    {
        QString flowName = item.key();
        CreateAutoFlowDispatcher(flowName);
        if(!m_AutoObjectMap[flowName].second->isRunning())
        {
            m_AutoObjectMap[flowName].first->SetFlowKey(flowName);
            m_AutoObjectMap[flowName].first->SetFlowMap(ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[flowName].flowCmdMap);
            m_AutoObjectMap[flowName].second->start();
        }
        QThread::msleep(5); 
        break;//暂时只接受一个流程
    }
}

void AutoFlowFrame::Stop()
{
    for(auto item = m_AutoObjectMap.begin();item != m_AutoObjectMap.end();++item)
    {
        item.value().first->StopAutoFlow();
    }
    for(auto item = m_AutoObjectMap.begin();item != m_AutoObjectMap.end();++item)
    {
        item.value().second->quit();
        item.value().second->wait();
    }
}

bool AutoFlowFrame::CreateAutoFlowDispatcher(const QString strKey)
{
    if(!m_AutoObjectMap.contains(strKey))
    {
        QThread *thred = new QThread();
        AutoFlowOperater *pflow = new AutoFlowOperater();
        QPair<AutoFlowOperater*,QThread*> pair;
        pair.first = pflow;
        pair.second = thred;
        m_AutoObjectMap.insert(strKey,pair);
        pflow->moveToThread(thred);
        QObject::connect(thred, &QThread::started, pflow, &AutoFlowOperater::Run,Qt::QueuedConnection);
    }
    else
    {
        m_AutoObjectMap[strKey].first->StopAutoFlow();
        m_AutoObjectMap[strKey].second->quit();
        m_AutoObjectMap[strKey].second->wait();
    }
    return true;
}
