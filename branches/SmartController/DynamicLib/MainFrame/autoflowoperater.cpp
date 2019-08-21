#include "autoflowoperater.h"
#include "RunPlugin.h"
#include <DycLibBase/LogManager/logmanager.h>
#include <DycLibBase/StackManager/stackmanager.h>
#include <MyEC_Shared.h>

AutoFlowOperater::AutoFlowOperater()
{
    m_callCurrentRunIndex = -1;
    m_currentRunIndex = -1;
    m_forNum = 0;
    m_ifJump = false;
    m_ifCountMap.clear();
    m_fun = NULL;
    m_stopFlag = false;
    m_isSPLPause = false;
    m_pluginMap.clear();
    GetPluginsMap();
}

void AutoFlowOperater::SetFlowKey(const QString strKey)
{
    m_flowName = strKey;
}

void AutoFlowOperater::SetFlowMap(QMap<int, QMap<QString, FLOWTXT_STRU> > temp)
{
    m_flowCmdMap.clear();
    m_flowCmdMap = temp;
    for( auto item = temp.begin();item != temp.end();++item)
    {
        foreach (QString strkey, item.value().keys())
        {
            if(strkey.contains("#"))
            {
                m_IDsMap.insert(strkey,item.key());
            }
        }
    }
}

void AutoFlowOperater::StopAutoFlow()
{
    m_stopFlag = true;
    if(m_fun != NULL)
        m_fun->isTerminate = true;
}

void AutoFlowOperater::Run()
{
    if(m_pluginMap.size() < 1)
    {
        return;
    }
     m_stopFlag = false;
    while(!m_stopFlag)
    {
       RunSingle(m_flowName,m_pluginMap);
        if(m_stopFlag)
            break;
    }
}
void AutoFlowOperater::RunSingle(const QString key, QMap<QString, FunModuleInterface *> funMap)
{
    m_pluginMap = funMap;
    auto it = m_flowCmdMap.begin();
    for(;it != m_flowCmdMap.end();)
    {
        if(m_stopFlag)
        {
            return;
        }
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
            if(m_stopFlag)
            {
                return;
            }
            QString strkey = item.next().key();
            if(strkey.contains("#"))
            {
                continue;
            }
            if(FlowData::GetInstance()->m_transMap.contains(strkey))
            {
                strkey = FlowData::GetInstance()->m_transMap[strkey];
            }
            EXE_PARAM exeParam;
            exeParam.strkey = strkey;
            exeParam.strMsg = QString("[自动扫描流程]%1执行编号%2指令 %3 ").arg(key).arg(it.key()).arg(strkey);
            exeParam.flowStru = item.value();
            int iResult = 0;
           if(m_stopFlag)
            {
                return;
            }
            ExeFuntion(exeParam,iResult,&m_fun);
            if(iResult < 0 && !m_isSPLPause)
            {
                m_stopFlag = true;
                return ;
            }
            QThread::usleep(1000);
        }
        if(!m_isSPLPause)
            ++it;
    }
     m_currentRunIndex = -1;
}

void AutoFlowOperater::ExeFuntion(EXE_PARAM exeParam,int &iresult,FunModuleInterface** pfun)
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
        ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            ExeCallFunc(strCall,iresult,ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap[strCall]);
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
    else if(strkey.compare("FOR") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            for(;m_forNum < flowStru.istatus.toInt();++m_forNum)
            {
                ExeCallFunc("For-"+strCall,iresult,ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap[strCall]);
            }
            m_forNum = 0;
        }
    }
    else if(strkey.compare("WHILE") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            StackManager * pstackmanager = new StackManager;
            if(flowStru.istatus.isEmpty())
                return ;
            while(pstackmanager->AnalysisEvent(flowStru.istatus))
            {
                ExeCallFunc("While-"+strCall,iresult,ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap[strCall]);
            }
        }
    }
    else if(strkey.compare("DO-WHILE") == 0 )
    {
        QString strCall = flowStru.id;
        if(ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            StackManager * pstackmanager = new StackManager;
            if(flowStru.istatus.isEmpty())
                return;
            do
            {
                ExeCallFunc("DoWhile-"+strCall,iresult,ConfigData::GetInstance()->xmlMap.flowTxt.autoFlowMap[m_flowName].threadCmdMap[strCall]);
            }while(pstackmanager->AnalysisEvent(flowStru.istatus));
        }
    }
    else
       {
        CoreLog::QLog_Error(defaultLogModule,str);
        iresult = -1;

    }
}
void AutoFlowOperater::ExeCallFunc(const QString strCall,int &iresult,QMap<int,QMap<QString,FLOWTXT_STRU>> temp)
{
    auto item = temp.begin();
    if(m_callCurrentRunIndex != -1)
    {
        item = temp.find(m_callCurrentRunIndex);
    }
    for(;item != temp.end();)
    {
       if(m_stopFlag)
        {
            return;
        }
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
            if(m_stopFlag)
            {
                return;
            }
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

void AutoFlowOperater::GetPluginsMap()
{
    m_pluginMap.clear();
    for(auto it = ConfigData::GetInstance()->pluginMap.begin();
        it != ConfigData::GetInstance()->pluginMap.end();++it)
    {
        m_pluginMap.insert(it.key(),it.value()->Copy());
    }
}

int AutoFlowOperater::HandleIfErrorFun(const QString &funname)
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
        StopAutoFlow();
        return -99;
    }
    else if(funname.contains("Call"))
    {
        QString strCall = funname.mid(funname.indexOf("Call")+4);
        if(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap.contains(strCall))
        {
            int iresult = -1;
            ExeCallFunc("IF-" +strCall,iresult,ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[m_flowName].threadCmdMap[strCall]);
            if(iresult < 0)
            {
                m_stopFlag = true;
                return iresult;
            }
        }
    }
    return 1;
}

int AutoFlowOperater::HandleIfFunction(const IFInfo_STRU &_info,const QString strFunction)
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

