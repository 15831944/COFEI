#include "OperateThread.h"
#include "OperateSinThread.h"
#include <QMutexLocker>
#include <MyConfig_Shared>
#include <QStandardPaths>
#include <QTextStream>
#include <MyGlobal.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include <DycLibBase/LogManager/logmanager.h>
#include "safethread.h"
#include "RunPlugin.h"
#include <QDebug>
using namespace CoreGlobal;
#define NSEC_PER_SEC    (1000000000)
OperateThread::OperateThread()
{
    m_currentRunIndex = -1;
    m_callCurrentRunIndex = -1;
    m_forNum = 0;
    m_ifJump = false;
    m_ifCountMap.clear();
    m_OpThreadMap.clear();
    m_fun = NULL;
    m_stopFlag = false;
    m_isSPLPause = false;
    m_threadFault = false;
    m_mutex = new QMutex;
    m_OpThreadMap.clear();
}
void OperateThread::ModifedCurIndex(QMap<int,QMap<QString,FLOWTXT_STRU>>::iterator &item)
{
    item = m_flowCmdMap.find(m_currentRunIndex);
    if(item != m_flowCmdMap.end() && m_threadFault)
    {
        while(--item != m_flowCmdMap.end())
        {
            if(!item.value().contains("THREAD"))
            {
                ++item;
                break;
            }
        }
    }
    m_threadFault = false;
}
void OperateThread::SetFlowKey(const QString strKey)
{
    m_flowName = strKey;
}

void OperateThread::SetFlowMap(QMap<int, QMap<QString, FLOWTXT_STRU> > temp)
{
    m_flowCmdMap.swap(temp);
    for( auto item = m_flowCmdMap.begin();item != m_flowCmdMap.end();++item)
    {
        foreach (QString strkey, item.value().keys())
        {
            if(strkey.contains("#"))
            {
                m_IDsMap.insert(strkey,item.key());
            }
        }
    }
    GetPluginInstances();
}
void OperateThread::GetPluginInstances()
{
    m_pluginMap.clear();
    for(auto it = ConfigData::GetInstance()->pluginMap.begin();
        it != ConfigData::GetInstance()->pluginMap.end();++it)
    {
        m_pluginMap.insert(it.key(),it.value()->Copy());
    }
}

void OperateThread::Run()
{
    if(m_flowCmdMap.size() > 0)
    {
        QString strTitle = QString("线程[%1]").arg(m_flowName);
        m_stopFlag = false;
        ThreadRun(strTitle);
    }
}
void OperateThread::ThreadRun(const QString key)
{
    auto it = m_flowCmdMap.begin();
    if(m_currentRunIndex != -1)
    {
        ModifedCurIndex(it);
    }
    for(;it != m_flowCmdMap.end();)
    {
        __QUITRUN(SafeThread::GetInstance()->m_EmStopFlag|m_stopFlag)
        m_fun = NULL;
        if(m_ifJump)
        {
            it = m_flowCmdMap.find(m_currentRunIndex);
            m_ifJump = false;
            continue;
        }
        m_isSPLPause = false;
        m_currentRunIndex = it.key();
        QMapIterator<QString,FLOWTXT_STRU> item(it.value());
        while(item.hasNext())
        {
            __QUITRUN(SafeThread::GetInstance()->m_EmStopFlag|m_stopFlag)
            m_mutex->lock();
            QString strkey = item.next().key();
            if(strkey.contains("#"))
            {
                m_mutex->unlock();
                continue;
            }
            if(FlowData::GetInstance()->m_transMap.contains(strkey))
            {
                strkey = FlowData::GetInstance()->m_transMap[strkey];
            }
            EXE_PARAM exeParam;
            exeParam.strkey = strkey;
            exeParam.strMsg = QString("%1执行编号%2指令 %3 ").arg(key).arg(it.key()).arg(strkey);
            exeParam.flowStru = item.value();
            int iResult = 0;
            __QUITRUN(SafeThread::GetInstance()->m_EmStopFlag|m_stopFlag)
            ExeFuntion(exeParam,iResult,&m_fun);
            m_mutex->unlock();
            if(iResult < 0 && !m_isSPLPause)
            {
                m_stopFlag = true;
                emit FaultEvent();
                SafeThread::GetInstance()->SetFlowAlarmLevel(15);
                return ;
            }
            QThread::usleep(1000);
        }
        if(!m_isSPLPause)
            ++it;
    }
    m_currentRunIndex = -1;
}

void OperateThread::ExeFuntion(EXE_PARAM exeParam,int &iresult,FunModuleInterface** pfun)
{
    if(iresult < 0)
    {
        return;
    }
    FLOWTXT_STRU flowStru = exeParam.flowStru;
    QString strkey = exeParam.strkey;
    QString strMsg = exeParam.strMsg;
    QString str = QString("plugin %1 is not find").arg(strkey);
    if(m_pluginMap.contains(strkey))
    {
        *pfun = m_pluginMap[strkey];
        RunPlugin::Exe(strkey,strMsg,iresult,flowStru,*pfun);
    }
    else if(strkey.compare("CALL") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            ExeCallFunc(strCall,iresult,ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strCall]);
        }
    }
    else if(strkey.compare("GoTo") == 0 )
    {

    }
    else if(strkey.compare("IF") == 0 )
    {
        IFInfo_STRU IfInfo = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_IF_Map[flowStru.id];
        int ir = HandleIfFunction(IfInfo,flowStru.istatus);
        if(ir < 0)
        {
            str = strMsg + QString("%1 %2 执行命令:%3").arg(" false! ").arg(iresult).arg(flowStru.istatus);

            if(HandleIfErrorFun(flowStru.istatus) < 0)
            {
                iresult = -99;
                CoreLog::QLog_Error(defaultLogModule, str);
            }
            else
            {
                CoreLog::QLog_Info(defaultLogModule, str);
            }
        }
        else
        {
            if(ConfigData::GetInstance()->L_state == 1)
            {
                str = strMsg + QString("%1 %2").arg(flowStru.istatus).arg(" OK!");
                CoreLog::QLog_Info(defaultLogModule,str);
            }
        }
    }
    else if(strkey.compare("THREAD") == 0 )
    {
        QString strThread = flowStru.id;
        if(!m_OpThreadMap.contains(strThread) &&
                ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strThread))
        {
            QThread *thread = new QThread;
            OperateSinThread *pchildflow = new OperateSinThread();
            pchildflow->SetFlowKey(m_flowName);
            pchildflow->SetFlowMap(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strThread]);
            QPair<OperateSinThread*,QThread*> pair;
            pair.first = pchildflow;
            pair.second = thread;
            m_OpThreadMap.insert(strThread,pair);
            pchildflow->moveToThread(thread);
            QObject::connect(thread, &QThread::started, pchildflow, &OperateSinThread::Run,Qt::QueuedConnection);
            connect(pchildflow,&OperateSinThread::FaultEvent,this,&OperateThread::ThreadIsFault,Qt::QueuedConnection);
        }
        else
        {
            m_OpThreadMap[strThread].first->EMStopFlow();
            m_OpThreadMap[strThread].second->quit();
            m_OpThreadMap[strThread].second->wait();
        }
        if(!m_OpThreadMap[strThread].second->isRunning())
            m_OpThreadMap[strThread].second->start();
    }
    else if(strkey.compare("FOR") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            for(; m_forNum < flowStru.istatus.toInt();++m_forNum)
            {
                ExeCallFunc(strCall,iresult,
                            ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strCall]);
            }
            m_forNum = 0;
        }
    }
    else if(strkey.compare("WHILE") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            StackManager * pstackmanager = new StackManager;
            if(flowStru.istatus.isEmpty())
                return ;
            while(pstackmanager->AnalysisEvent(flowStru.istatus))
            {
                ExeCallFunc(strCall,iresult,
                            ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strCall]);
            }
        }
    }
    else if(strkey.compare("DO-WHILE") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            StackManager * pstackmanager = new StackManager;
            if(flowStru.istatus.isEmpty())
                return;
            do
            {
                ExeCallFunc(strCall,iresult,
                            ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strCall]);

            }while(pstackmanager->AnalysisEvent(flowStru.istatus));
        }
    }
    else
    {
        CoreLog::QLog_Error(defaultLogModule,str);
        iresult = -1;
    }
    SafeThread::GetInstance()->m_StopFlag = false;
}
void OperateThread::ExeCallFunc(const QString strCall,int &iresult,QMap<int,QMap<QString,FLOWTXT_STRU>> temp)
{
    auto item = temp.begin();
    if(m_callCurrentRunIndex != -1)
    {
        item = temp.find(m_callCurrentRunIndex);
    }
    for(;item != temp.end();)
    {
        __QUITRUN(SafeThread::GetInstance()->m_EmStopFlag|m_stopFlag)
        QMapIterator<QString,FLOWTXT_STRU> it(item.value());
        m_callCurrentRunIndex = item.key();
        while(it.hasNext())
        {
            QString strkey = it.next().key();
            if(FlowData::GetInstance()->m_transMap.contains(strkey))
            {
                strkey = FlowData::GetInstance()->m_transMap[strkey];
            }
            EXE_PARAM tempexeParam;
            tempexeParam.strkey = strkey;
            tempexeParam.strMsg = QString("Call %1执行编号%2指令 %3 ").arg(strCall).arg(m_callCurrentRunIndex).arg(strkey);
            tempexeParam.flowStru = it.value();
            __QUITRUN(SafeThread::GetInstance()->m_EmStopFlag|m_stopFlag)
            ExeFuntion(tempexeParam,iresult,&m_fun);
            if(iresult < 0 )
            {
                m_stopFlag = true;
                return ;
            }
            QThread::usleep(1000);
        }
        ++item;
    }
    m_callCurrentRunIndex = -1;

}
void OperateThread::ThreadIsFault()
{
    EMStopFlowSlot();
    emit FaultEvent();
    SafeThread::GetInstance()->SetFlowAlarmLevel(15);
    m_threadFault = true;
}

void OperateThread::EMStopFlow()
{
    EMStopFlowSlot();
}

void OperateThread::StopFlow()
{
    m_stopFlag = true;
}

void OperateThread::PauseFlow()
{
    if(m_fun != NULL)
    {
        if(m_fun->GetModuleName().compare("IN") == 0 || m_fun->GetModuleName().compare("LIN") == 0 ||
                m_fun->GetModuleName().compare("WAIT") == 0)
        {
            m_isSPLPause = true;
            m_fun->isTerminate = true;
        }
    }
    m_mutex->lock();
}

void OperateThread::ResumeFlow()
{
    m_mutex->unlock();
}

void OperateThread::ReSetFlow()
{
    m_currentRunIndex = -1;
    m_callCurrentRunIndex = -1;
    m_forNum = 0;
    for(auto it = m_OpThreadMap.begin();it != m_OpThreadMap.end();++it)
    {
        it.value().first->ReSetFlow();
    }
}

void OperateThread::EMStopFlowSlot()
{
    m_stopFlag = true;
    m_mutex->unlock();
    if(m_fun != NULL)
        m_fun->isTerminate = true;
    for(auto it = m_OpThreadMap.begin();it != m_OpThreadMap.end();++it)
    {
        it.value().first->EMStopFlow();
        it.value().second->quit();
        it.value().second->wait();
    }
}

int OperateThread::HandleIfErrorFun(const QString &funname)
{
    if(funname.contains("Jump"))
    {
        m_ifJump = true;
        QString strkey = funname.mid(funname.indexOf("Jump")+4);
        if(m_IDsMap.contains(strkey))
            m_currentRunIndex = m_IDsMap[strkey];
    }
    else if(funname.contains("Stop"))
    {
        EMStopFlowSlot();
        return -99;
    }
    else if(funname.contains("Call"))
    {
        QString strCall = funname.mid(funname.indexOf("Call")+4);
        if(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            int iresult = -1;
            ExeCallFunc("IF-" +strCall,iresult,
                        ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strCall]);
            if(iresult < 0)
            {
                m_stopFlag = true;
                return iresult;
            }
        }
    }
    return 1;
}

int OperateThread::HandleIfFunction(const IFInfo_STRU &_info,const QString strFunction)
{
    int iresult = -1;
    if(_info.type == "数据")
    {
        StackManager stackmanager;
        if(!stackmanager.AnalysisEvent(_info.strEvent))
            return iresult;
    }
    else if(_info.type == "计数")
    {
        if(strFunction == "clear")
        {
            if(m_ifCountMap.contains(_info.id))
                m_ifCountMap[_info.id] = 0;
            return 1;
        }
        QString sdest = _info.strEvent;
        sdest = sdest.toUpper();
        int idest = 0;
        idest = sdest.toInt();
        if(m_ifCountMap.contains(_info.id))
        {
            m_ifCountMap[_info.id] += 1;
        }
        else
        {
            int inum = 1;
            m_ifCountMap.insert(_info.id,inum);
        }
        if(m_ifCountMap[_info.id] >= idest)
        {
            m_ifCountMap[_info.id] = 0;
            return iresult;
        }
    }
    return 1;
}

