#include "FlowDispatcher.h"
#include <MyConfig_Shared>
#include "FlowOperater.h"
#include <QDebug>
FlowDispatcher::FlowDispatcher()
{
    m_pFlowHandle = new FlowOperater();
    connect(m_pFlowHandle,&FlowOperater::FaultEvent,this,&FlowDispatcher::EmStopSlot,Qt::QueuedConnection);
    GetPluginInstances();
    m_isLoop = false;
    m_flowType = 255;
}

void FlowDispatcher::SetFlowName(const QString name)
{
    m_flowName = name;
    m_pFlowHandle->SetFlowKey(m_flowName);
}

void FlowDispatcher::SetLogTitle(const QString title)
{
    m_logTitle = title;
}

void FlowDispatcher::SetFlowCmdMap(const QMap<int, QMap<QString, FLOWTXT_STRU>> temp)
{
    m_pFlowHandle->SetFlowMap(temp);
}

void FlowDispatcher::SetFlowMode(bool loop)
{
    m_isLoop = loop;
}

void FlowDispatcher::SetFlowType(const uchar uc)
{
    m_flowType = uc;
}

void FlowDispatcher::Run()
{
    if(m_pluginMap.size() < 1)
    {
        return;
    }
    m_pFlowHandle->m_stopFlag = false;
    do
    {
        m_pFlowHandle->Run(m_logTitle,m_pluginMap);

        if(m_pFlowHandle->m_stopFlag)
            break;

    }while(m_isLoop);
    if(!m_isLoop)
    {
        if(!m_pFlowHandle->m_stopFlag)
        {
            emit RunResult(m_flowName,m_flowType,true);
        }
        else{
            emit RunResult(m_flowName,m_flowType,false);
        }
    }
    else {
        emit RunResult(m_flowName,m_flowType,!m_pFlowHandle->m_stopFlag);
    }

}
void FlowDispatcher::EmStopSlot()
{
    m_pFlowHandle->EMStopFlow();
}
void FlowDispatcher::StopSlot()
{
    SetFlowMode(false);
}
void FlowDispatcher::PauseSlot()
{
    m_pFlowHandle->PauseFlow();
}
void FlowDispatcher::ResumeSlot()
{
    m_pFlowHandle->ResumeFlow();
}
void FlowDispatcher::ResetSlot()
{
    m_pFlowHandle->ReSetFlow();
}
void FlowDispatcher::GetPluginInstances()
{
    m_pluginMap.clear();
    for(auto it = ConfigData::GetInstance()->pluginMap.begin();
        it != ConfigData::GetInstance()->pluginMap.end();++it)
    {
        m_pluginMap.insert(it.key(),it.value()->Copy());
    }
}
