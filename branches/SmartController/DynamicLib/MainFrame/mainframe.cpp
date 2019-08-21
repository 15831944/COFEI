#include "mainframe.h"
#include <QMessageBox>
#include "FlowOperater.h"
#include "FlowDispatcher.h"
#include <QStandardPaths>
#include <sys/sysinfo.h>
#include <QDebug>
#include <DycLibBase/LogManager/logmanager.h>
#include <QCoreApplication>
MainFrame::MainFrame()
{
    m_ObjectMap.clear();
}

void MainFrame::Start()
{
    StartFlow();//防止执行其他流程启动时候进行急停之后亮插件报错导致等级15的问题,子启动之后,再次点击总启动会报错
    SafeThread::GetInstance()->SetFlowAlarmLevel(17);

}

void MainFrame::StartFlow()
{
    //int icore = get_nprocs_conf();
    //int iNum = 0;
    for(auto item = ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap.begin();
        item != ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap.end();++item)
    {
        //if((iNum++) > icore)
        //    break;
        if(item.key().compare("回原点") == 0 || item.key().compare("总复位") == 0)
            continue;
        QString flowName = item.key();
        CreateFlowDispatcher(flowName);
        if(!m_ObjectMap[flowName].second->isRunning())
        {
            m_ObjectMap[flowName].first->SetFlowName(flowName);
            m_ObjectMap[flowName].first->SetLogTitle(QString("流程[%1] ").arg(flowName));
            m_ObjectMap[flowName].first->SetFlowMode(true);
            m_ObjectMap[flowName].first->SetFlowType(2);
            m_ObjectMap[flowName].first->SetFlowCmdMap(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[flowName].flowCmdMap);
            m_ObjectMap[flowName].second->start();
        }
        QThread::msleep(5);
    }
}

void MainFrame::StartFlow(const QString strKey)
{
    if(!ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap.contains(strKey))
    {
        return;
    }
     CreateFlowDispatcher(strKey);
    if(!m_ObjectMap[strKey].second->isRunning())
    {
        m_ObjectMap[strKey].first->SetFlowName(strKey);
        m_ObjectMap[strKey].first->SetLogTitle(QString("流程[%1] ").arg(strKey));
        m_ObjectMap[strKey].first->SetFlowMode(true);
        m_ObjectMap[strKey].first->SetFlowType(2);
        m_ObjectMap[strKey].first->SetFlowCmdMap(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[strKey].flowCmdMap);
        m_ObjectMap[strKey].second->start();
    }
}

bool MainFrame::CreateFlowDispatcher(const QString strKey)
{
    if(!m_ObjectMap.contains(strKey))
    {
        QThread *thred = new QThread();
        FlowDispatcher *pflow = new FlowDispatcher();
        QPair<FlowDispatcher*,QThread*> pair;
        pair.first = pflow;
        pair.second = thred;
        m_ObjectMap.insert(strKey,pair);
        pflow->moveToThread(thred);
        QObject::connect(thred, &QThread::started, pflow, &FlowDispatcher::Run,Qt::QueuedConnection);
        QObject::connect(pflow, &FlowDispatcher::RunResult, this, &MainFrame::GetRunResult,Qt::QueuedConnection);
    }
    else
    {
        m_ObjectMap[strKey].first->EmStopSlot();
        m_ObjectMap[strKey].second->quit();
        m_ObjectMap[strKey].second->wait();
    }
    return true;
}

void MainFrame::Stop()
{
    //    SafeThread::GetInstance()->SetAlarmLevel(16);
    SafeThread::GetInstance()->m_StopFlag = true;
    for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    {
        item.value().first->EmStopSlot();
        item.value().first->SetFlowType(2);
    }
    for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    {
        item.value().second->quit();
        item.value().second->wait();
    }
    CoreLog::QLog_Info(defaultLogModule,tr("全部程序立即停止流程结束"));
//    qDebug() << tr("全部程序立即停止流程结束");
}
void MainFrame::NorPlay(const QString strKey)
{
    //SafeThread::GetInstance()->SetAlarmLevel(17,strKey);
    SafeThread::GetInstance()->SetFlowAlarmLevel(17);

    StartFlow(strKey);

}
void MainFrame::NorStop()
{
    SafeThread::GetInstance()->SetFlowAlarmLevel(3);
    for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    {
        item.value().first->StopSlot();
        item.value().first->SetFlowType(3);
    }
    // for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    // {
    //        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    //  item.value().second->quit();
    //  item.value().second->wait();
    // }
    // CoreLog::QLog_Info(defaultLogModule,"全部程序停止流程结束");
    // qDebug()<<"停止流程退出:全部程序停止流程结束";
    //emit ReSetORGRunOK("总状态",2,true);
}
void MainFrame::NorStop(const QString strKey)
{
    SafeThread::GetInstance()->SetFlowAlarmLevel(3);
    if(m_ObjectMap.contains(strKey))
    {
        m_ObjectMap[strKey].first->StopSlot();
        m_ObjectMap[strKey].first->SetFlowType(3);
        // QCoreApplication::processEvents(QEventLoop::AllEvents,100);
        //  m_ObjectMap[strKey].second->quit();
        // m_ObjectMap[strKey].second->wait();
    }
    // CoreLog::QLog_Info(defaultLogModule,QString("程序 %1 停止流程结束").arg(strKey));
    // emit ReSetORGRunOK(strKey,2,true);
}

void MainFrame::Stop(const QString strKey)
{
    //    SafeThread::GetInstance()->SetAlarmLevel(16);
    if(m_ObjectMap.contains(strKey))
    {
        m_ObjectMap[strKey].first->EmStopSlot();
        m_ObjectMap[strKey].first->SetFlowType(2);
        m_ObjectMap[strKey].second->quit();
        m_ObjectMap[strKey].second->wait();
        CoreLog::QLog_Info(defaultLogModule,QString("程序 %1 立即停止流程结束").arg(strKey));
//        qDebug() << QString("程序 %1 立即停止流程结束").arg(strKey);
    }
}

void MainFrame::Pause()
{ 
    SafeThread::GetInstance()->SetFlowAlarmLevel(16);
    for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    {
        item.value().first->PauseSlot();
    }
}
void MainFrame::Resume()
{
    SafeThread::GetInstance()->SetFlowAlarmLevel(17);
    ResumeEvent();
}

void MainFrame::ResumeEvent()
{
    for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    {
        item.value().first->ResumeSlot();
    }
}

void MainFrame::ResetEvent()
{    
    SafeThread::GetInstance()->m_StopFlag = false;
    QString strKey = "#Reset#";
    CreateFlowDispatcher(strKey);
    for(auto item = m_ObjectMap.begin();item != m_ObjectMap.end();++item)
    {
        item.value().first->ResetSlot();
    }
    if(!m_ObjectMap[strKey].second->isRunning())
    {
        m_ObjectMap[strKey].first->SetFlowName("总复位");
        m_ObjectMap[strKey].first->SetFlowMode(false);
        m_ObjectMap[strKey].first->SetFlowType(0);
        m_ObjectMap[strKey].first->SetLogTitle("总复位程序");
        m_ObjectMap[strKey].first->SetFlowCmdMap(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap["总复位"].flowCmdMap);
        m_ObjectMap[strKey].second->start();
    }
    SafeThread::GetInstance()->SetFlowAlarmLevel(4);
}
void MainFrame::ResetEvent(const QString strKey)
{
    if(!ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap.contains(strKey))
        return;
    CreateFlowDispatcher(strKey);
    if(m_ObjectMap.contains(strKey))
    {
        m_ObjectMap[strKey].first->ResetSlot();
    }
    if(!m_ObjectMap[strKey].second->isRunning())
    {
        m_ObjectMap[strKey].first->SetFlowName(strKey);
        m_ObjectMap[strKey].first->SetFlowMode(false);
        m_ObjectMap[strKey].first->SetFlowType(0);
        m_ObjectMap[strKey].first->SetLogTitle(QString("%1复位程序").arg(strKey));
        m_ObjectMap[strKey].first->SetFlowCmdMap(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap[strKey].ResetCmdMap);
        m_ObjectMap[strKey].second->start();
    }
    SafeThread::GetInstance()->SetFlowAlarmLevel(4);
}
void MainFrame::ORGEvent()
{ 
    SafeThread::GetInstance()->SetFlowAlarmLevel(6);
    QString strKey = "#ORG#";
    if(m_ObjectMap.contains(strKey))
    {
        m_ObjectMap[strKey].first->ResetSlot();
    }
    CreateFlowDispatcher(strKey);
    if(!m_ObjectMap[strKey].second->isRunning())
    {
        m_ObjectMap[strKey].first->SetFlowName("回原点");
        m_ObjectMap[strKey].first->SetFlowMode(false);
        m_ObjectMap[strKey].first->SetFlowType(1);
        m_ObjectMap[strKey].first->SetLogTitle(tr("回原点程序"));
        m_ObjectMap[strKey].first->SetFlowCmdMap(ConfigData::GetInstance()->xmlMap.flowTxt.FlowMap["回原点"].flowCmdMap);
        m_ObjectMap[strKey].second->start();
    }
}
void MainFrame::SafeThreadTOSatrt()
{
    SafeThread::GetInstance()->InitSafeVariable();
    SafeThread::GetInstance()->SetFlowAlarmLevel(16);
}

MainFrame::~MainFrame()
{
    Stop();
    QThread::msleep(10);
    ConfigData::GetInstance()->m_mutex.unlock();
    ConfigData::GetInstance()->childPluginMap.clear();
    ConfigData::GetInstance()->hwIOMap.clear();
    ConfigData::GetInstance()->pluginMap.clear();
}

void MainFrame::UpdateSwSerXML(QMap<QString,SWCONFIGXML_QPOSIT_STRU> ,QMap<QString,SWCONFIGXML_LCSP_STRU> )
{
    //    XmlManager xml;
    //    QString path=QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹"+"/参数配置/软件参数配置.xml";
    //    xml.WriteSWSerXml(path,temp,temp2);
}

void MainFrame::GetRunTime(double d)
{
    emit UpdateRunTime(d);
}

void MainFrame::GetRunResult(const QString str,uchar uc, bool isok)
{
    if((uc == 1) || (uc == 0))  //复位,回原点完成,信号等级切为16(Ready),执行结束需要把线程退出
    {
        if(isok)
         {   if(str == "总复位")
            {
                if(m_ObjectMap.contains("#Reset#"))
                {
                    if(m_ObjectMap["#Reset#"].second->isRunning())
                    {
                        m_ObjectMap["#Reset#"].second->quit();
                        m_ObjectMap["#Reset#"].second->wait();
                    }
                }
            }
            else if(str == "回原点")
            {
                if(m_ObjectMap.contains("#ORG#"))
                {
                    if(m_ObjectMap["#ORG#"].second->isRunning())
                    {
                        m_ObjectMap["#ORG#"].second->quit();
                        m_ObjectMap["#ORG#"].second->wait();
                    }
                }
            }
            else{
                if(m_ObjectMap.contains(str))
                {
                    if(m_ObjectMap[str].second->isRunning())
                    {
                        m_ObjectMap[str].second->quit();
                        m_ObjectMap[str].second->wait();
                    }
                }
            }
            SafeThread::GetInstance()->SetFlowAlarmLevel(16);
        }
    }
    else if(uc == 3)//表示停止流程过程//总状态或者子状态
    {
        if(m_ObjectMap.contains(str))
        {
            m_ObjectMap[str].second->quit();
            m_ObjectMap[str].second->wait();
        }
        if(isok)
        {
            CoreLog::QLog_Warning(defaultLogModule,QString("%1停止流程退出成功!").arg(str));
//            qDebug() << QString("%1停止流程退出成功!").arg(str);
        }
        else
        {
            CoreLog::QLog_Warning(defaultLogModule,QString("%1流程停止失败!").arg(str));
//            qDebug() << QString("%1流程停止失败!").arg(str);
        }
        bool on_runing = false;
        for(auto item = m_ObjectMap.begin(); item != m_ObjectMap.end(); ++item)
        {
            if(m_ObjectMap[item.key()].second->isRunning())
            {
                on_runing = true;
                break;
            }
        }
        if(!on_runing) //代表所有停止流程都结束了
        {
            if(SafeThread::GetInstance()->GetFlowAlarmLevel() == 3)
            {
                qDebug()<<" 停完了设置等级16:";
                SafeThread::GetInstance()->SetFlowAlarmLevel(16);
            }
        }
    }
    else if(uc == 4)//暂停类型
    {
        if(m_ObjectMap.contains(str))//对应的流程都停下了
        {
            m_ObjectMap[str].second->quit();
            m_ObjectMap[str].second->wait();
        }
    }
    emit ReSetORGRunOK(str,uc,isok);
}
