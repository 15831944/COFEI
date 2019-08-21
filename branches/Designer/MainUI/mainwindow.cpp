#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <CoGraphicsItems/GraphicsItems.h>
#include <CoGraphicsItems/Graphics/Button.h>
#include <CoGraphicsItems/AttributeInstance.h>
#include <CoGraphicsItems/CoProxyItem/TouchPP.h>
#include <DynamicLib/LoadFileManager/LoadFileManager.h>
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <QProcess>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QThread>
#include <QStandardPaths>
#include <DycLibBase/LogManager/logmanager.h>
#include <MyComm_Shared.h>
#include <MyComm_Shared.h>
#include <TeachingDevice/share_value.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("MainUI"));
    InitVariable();
    m_GridLayout = new QGridLayout(ui->centralWidget);

    QMap<QString,CoreLog::QLogger *> logMap = CoreLog::QLoggerManager::getInstance()->getLogInstanceMap();
    for(auto it = logMap.begin();it != logMap.end();++it )
    {
        connect(it.value(),&CoreLog::QLogger::LogUpdateUI,this,&MainWindow::ShowLog);
    }
    QWidget *ShowWidget = CreateShowArea(ui->centralWidget);
    m_GridLayout->addWidget(ShowWidget,0,1);
    TItemFactory::SetItemFactory(new CoGraphicsItems());
    InitExcData();
    //固定画布显示区域的尺寸
    if(m_pCurScene)
        ShowWidget->setFixedSize(m_pView->width()+20,m_pView->height()+20);
    else
        this->showMaximized();
    CreatMenu();

}
void MainWindow::InitVariable()
{
    m_Teachmodel = false;
    m_CurTeachPPID = "";
    m_isWritexml = true;
    m_isPlay = true;
    m_ORGClicked = false;
    m_isEmStop = false;
    m_isNorStop = true;
    m_isReset = false;
    m_pView = NULL;
    m_pCurScene = NULL;
    m_pDBTimer    = new QTimer(this);
    ConfigData::GetInstance()->productTotal = 200000;
    ConfigData::GetInstance()->productTempTotal = 200000;
    connect(m_pDBTimer,&QTimer::timeout,this,&MainWindow::HandleDB);
    m_pEcTimer = new QTimer(this);
    connect(m_pEcTimer,&QTimer::timeout,this,&MainWindow::HandleEcInputData);
    m_mainFrame = new MainFrame();
    connect(m_mainFrame,&MainFrame::ReSetORGRunOK,this,&MainWindow::ReSetORGRunOK);    
    //autoframe = new AutoFlowFrame();
    m_pThreadObj = new ThreadObject();
    connect(m_pThreadObj,&ThreadObject::ErrorEvent,this,&MainWindow::HandleErrorEvent);


    QTimer *timerDF = new QTimer(this);
    connect(timerDF,&QTimer::timeout,this,&MainWindow::TimerDeleteFiles);
    timerDF->start(1200000);//


    m_pThread = new QThread(this);

    m_logEdit = new QLineEdit(this);
    m_logEdit->setStyleSheet(" QLineEdit{ color: red }");
    m_logEdit->setFont(QFont("黑体",12));

    m_comlabel = new QLabel(this);
    m_comlabel->setStyleSheet("QLabel{border-image: url(:/ico/image/error.png);}");
    m_comlabel->setFixedSize(30,30);
    ui->statusBar->addPermanentWidget(m_logEdit);
    ui->statusBar->addPermanentWidget(m_comlabel);
}
QWidget *MainWindow::CreateShowArea(QWidget *parent)
{
    QWidget *ShowWidget = new QWidget(parent);
    QVBoxLayout *pShowVbox = new QVBoxLayout();
    ShowWidget->setLayout(pShowVbox);
    ShowWidget->setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:15px;"));
    //Create Canvas
    QScrollArea *pShowCanvasArea = new QScrollArea();
    m_pView = new CoView(pShowCanvasArea);
    pShowCanvasArea->setWidget(m_pView);
    pShowVbox->addWidget(pShowCanvasArea);
    return ShowWidget;
}
void MainWindow::ShowLog(QString strmsg)
{
    if(strmsg.contains("[Error]"))
    {
        m_logEdit->setText(strmsg);
        emit SendChartData(1,strmsg,0,0);
    }
    else if(strmsg.contains("[Warning]"))
    {
        m_logEdit->setText(strmsg);
        emit SendChartData(2,strmsg,0,0);
    }
    else
    {
        if(strmsg.contains("Processdata cycle start ......"))
        {
//            HandleItems(1);
//            ConfigData::GetInstance()->L_state = 1;
//            BaseAxisOperate::SetAllAxisReset("Reset");
//            m_pDBTimer->start(1000);
//            m_pEcTimer->start(200);
//            m_mainFrame->SafeThreadTOSatrt();
//            connect(SafeThread::GetInstance(),&SafeThread::SafeSignals,this,&MainWindow::SafeSigs);
//            autoframe->Start();
        }
        else if(strmsg.contains("Haredword is disconnect"))
        {
            HandleItems(1);
            m_pDBTimer->start(1000);
        }
    }
}
void MainWindow::InitExcData()
{
    m_pThreadObj->moveToThread(m_pThread);
    connect(m_pThread,&QThread::started,m_pThreadObj,&ThreadObject::AnalyDB);
    m_pThread->start();
    DBInstanceManager::getDBInstance()->ReadVarDataFromDB();//先打开数据库
    OpenUIXML();
    m_pThread->quit();
    m_pThread->wait();
    ConfigData::GetInstance()->iSpeed = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_SPEED"].Initvalue;
    ConfigData::GetInstance()->iAccel = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_ACC"].Initvalue;
    ConfigData::GetInstance()->iDccel = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_DEC"].Initvalue;
    ConfigData::GetInstance()->speedRate = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap["BUILT_IN_SPEEDRATE"].Initvalue;
    //示教器运行函数对象,新增示教器运行对象代码
    m_TeachObj = new TeachDeviceRunLib();
    m_TeachThread = new QThread(this);
    m_TeachObj->moveToThread(m_TeachThread);
    connect(m_TeachThread,&QThread::started,m_TeachObj,&TeachDeviceRunLib::RunJoints);

    check_Direction = new CheckDirection();
    check_Thread = new QThread(this);
    check_Direction->moveToThread(check_Thread);
    connect(this,&MainWindow::Being_check,check_Direction,&CheckDirection::Direction_judgement);
    connect(check_Thread,&QThread::finished,check_Direction,&QObject::deleteLater);
    check_Thread->start();
    Being_check();

    get_value = new CheckABbitThread();
    get_thread = new QThread();
    get_value->moveToThread(get_thread);
    connect(this,&MainWindow::Being_get,get_value,&CheckABbitThread::Get_check_value);
    connect(get_thread,&QThread::finished,get_value,&QObject::deleteLater);
    get_thread->start();
    Being_get();
}
void MainWindow::OpenUIXML()
{
    QString saveDirName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/参数配置";
    TXmlReader mXml;
    if(!mXml.Open(saveDirName+"/UI配置.xml"))
    {
        CreateMsgBox(QMessageBox::Warning,tr("初始化"), tr("打开或解析错误"));
        return;
    }
    //解析界面图形
    m_ProStru.Clear();
    bool isGridLine = false;
    m_pCurScene = mXml.AnalyScene(m_ProStru.sceneMap,m_ProStru.flowMap,isGridLine);
    if(m_pCurScene)
    {
        QTransform transform = m_pView->transform();
        transform.reset();
        int rotate = mXml.AnalyViewRotate();
        transform.rotate(rotate);
        m_pView->setTransform(transform);
        m_pView->SetRotate(rotate);

        m_ProStru.viewWidth = m_pCurScene->GetWidth();
        m_ProStru.viewHeight = m_pCurScene->GetHeight();
        //m_pView->setFixedSize(m_ProStru.viewWidth,m_ProStru.viewHeight);
        if(rotate == 0 || rotate == 180)
            m_pView->setFixedSize(m_ProStru.viewWidth,m_ProStru.viewHeight);
        else if(rotate == 90 || rotate == -90)
            m_pView->setFixedSize(m_ProStru.viewHeight,m_ProStru.viewWidth);

        m_pView->setScene(m_pCurScene);
        UpdateUI();
    }
    QList<QString> list;
    if(LoadFileManager::GetInstance()->GetErrorState(list))
    {
        foreach (QString strError, list)
        {
            ShowLog(strError);
        }
    }
    QMap<QString,bool> temp;
    temp.insert("EmStopFlag",false);
    temp.insert("StopFlag",true);
    temp.insert("ResetFlag",false);
    temp.insert("StartFlag",true);
    auto items = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map;
    for(auto item = items.begin();item != items.end();++item)
    {
        if(item.key() != "总状态")
        {
            m_SafeMap.insert(item.key(),temp);
        }
    }
    QByteArray ba = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlNetworkStru.portName.toLatin1();
    InOutPutData::GetInstance()->ifName = new char[ba.length()];
    strcpy(InOutPutData::GetInstance()->ifName, ba.data());
    if(QString(InOutPutData::GetInstance()->ifName) != "TEST")
    {
        if(InOutPutData::GetInstance()->inPutNum > 0 && InOutPutData::GetInstance()->outPutNum > 0 )
        {
            memset(InOutPutData::GetInstance()->inPutData,0,InOutPutData::GetInstance()->inPutNum);
            memset(InOutPutData::GetInstance()->outPutData,0,InOutPutData::GetInstance()->outPutNum);
            m_ec.Start();
            m_pDBTimer->start(1000);
        }
        else
        {
            CoreLog::QLog_Error(defaultLogModule,QString("硬件参数设置有误，没有找到In/Output数据！"));
            //HandleErrorEvent(0,tr("硬件参数设置有误，没有找到In/Output数据！"));
        }
    }
    else
         CoreLog::QLog_Info(defaultLogModule,QString("Haredword is disconnect"));
}
void MainWindow::UpdateUI()
{
    // 解析完成,添加Canvas动作
    foreach(CoScene *Scene, m_ProStru.sceneMap.values())
    {
        QList<CoItemEx *>ItemList;
        Scene->GetItemsList(ItemList);
        foreach(CoItemEx *Item, ItemList)
        {
            Item->SetMoveAble(false);
            Item->SetDragAble(false);
            Item->SetSelectAble(false);
            Item->setEnabled(false);
            Item->StartAuto();
            CoWidgets *cowg = qobject_cast<CoWidgets *>(Item);
            if(cowg)
            {
                TButton *btn = qobject_cast<TButton *>(Item);
                if(btn)
                {
                    if(!btn->GetScreen().isEmpty())
                        connect(btn, &TButton::mSwithScene,this, &MainWindow::SwithScene);
                    if(btn->GetOpFunc() > CoWidgets::Function_Free)
                    {
                        connect(btn, &TButton::mSendTo,this, &MainWindow::SetFuntion);
                        if(m_FunctionBtnMap.contains(btn->GetOpFunc()))
                        {
                            m_FunctionBtnMap[btn->GetOpFunc()].insert(btn->GetId(),cowg);
                        }
                        else
                        {
                            QMap<int,CoWidgets*> temp = QMap<int,CoWidgets*>();
                            temp.insert(btn->GetId(),cowg);
                            m_FunctionBtnMap.insert(btn->GetOpFunc(),temp);
                        }
                    }
                }
                else
                {
                    connect(this, &MainWindow::SendChartData,cowg, &CoWidgets::SendTo);
                }
            }
        }
        QList<CoProxyWidget *> widgwts;
        Scene->GetItemsList(widgwts);
        foreach(CoProxyWidget *Item, widgwts)
        {
            Item->SetMoveAble(false);
            Item->SetDragAble(false);
            Item->SetSelectAble(false);

            TouchPP *pp = qobject_cast<TouchPP*>(Item);
            if(pp)
            {
                connect(pp,&TouchPP::SendHandModeEvent,this,&MainWindow::HandModeChanged);
                connect(pp,&TouchPP::SendHandParamEvent,this,&MainWindow::HandModeParamChanged);
                connect(pp,&TouchPP::SendHandModeState,this,&MainWindow::HandModeStateChanged);
            }
        }
    }
}

void MainWindow::CreateMsgBox(QMessageBox::Icon ico, const QString strTitle, const QString strMsg)
{
    QMessageBox msgBox(ico,strTitle,strMsg);
    msgBox.setStyleSheet("QPushButton {"
                         "background-color:#89AFDE;"" border-style: outset;"
                         " border-width: 1px;"" border-radius: 5px;"" border-color: beige;"
                         " font:14px;"" min-width: 5em;"" min-height:1.5em;"
                         "}"
                         "QLabel { min-height:3em;font:14px;}"
    );
    msgBox.exec();
}

void MainWindow::SafeSigs(const QString strKey, const QString strType ,const QString info)
{
    if(info.contains("急停退出"))
    {
        CoreLog::QLog_Warning(defaultLogModule, info);
        return;
    }
    if(!InOutPutData::GetInstance()->isRunning)
    {
        return;
    }
    if(strKey == "总状态")
    {
        if(strType == "EmStopFlag")
        {
//            if(!m_isEmStop)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                EmrgStop(CoWidgets::Function_EmrgStop);
            }
        }
        else if(strType == "StopFlag" && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            if(!m_isNorStop)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                Stop(CoWidgets::Function_Stop);
            }
        }
        else if(strType == "ResetFlag" && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            if((!m_isReset)&&(!m_Teachmodel))
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                ResetEvent(CoWidgets::Function_Reset);
            }
        }
        else if(strType == "ORGFlag" && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            if((!m_ORGClicked)&&(!m_Teachmodel))
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                ORGEvent(CoWidgets::Function_ORG);
            }
        }
        else if(strType == "StartFlag" && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            //任一子流程处于急停时,总启动无效
            QMap<QString,QMap<QString,bool>> flowMap = SafeThread::GetInstance()->m_FlowIOStateMap;
            for(auto flow = flowMap.begin(); flow != flowMap.end(); ++flow)
            {
                if(flowMap[flow.key()]["EmStopFlag"])
                    return;
            }
            //if(SafeThread::GetInstance()->GetAlarmLevel() > 3)
            {
                if((!m_isPlay)&&(!m_Teachmodel))
                {
                    CoreLog::QLog_Warning(defaultLogModule, info);
                    Play(CoWidgets::Function_Start);
                }
            }
        }
    }
    else
    {
        if(SafeThread::GetInstance()->m_EmStopFlag)
            return;
        m_isEmStop = false;
        if(strType == "EmStopFlag")
        {
//            if(!m_SafeMap[strKey][strType])
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["ResetFlag"] = false;
                m_SafeMap[strKey]["StartFlag"] = false;
                m_SafeMap[strKey]["StopFlag"] = true;
                m_mainFrame->Stop(strKey);
            }
        }
        else if((strType == "StopFlag")&&(!(SafeThread::GetInstance()->m_FlowIOStateMap[strKey]["EmStopFlag"])))
        {
            if(!m_SafeMap[strKey][strType])
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["StartFlag"] = true;
                m_SafeMap[strKey]["ResetFlag"] = true;
                m_SafeMap[strKey]["EmStopFlag"] = false;
                m_mainFrame->NorStop(strKey);
            }
        }
        else if((strType == "ResetFlag")&&(!(SafeThread::GetInstance()->m_FlowIOStateMap[strKey]["EmStopFlag"])))
        {
            if((!m_SafeMap[strKey][strType])&&(!m_Teachmodel))
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["StartFlag"] = true;
                m_SafeMap[strKey]["StopFlag"] = true;
                m_SafeMap[strKey]["EmStopFlag"] = false;
                m_mainFrame->ResetEvent(strKey);
            }
        }
        else if((strType == "StartFlag")&&(!(SafeThread::GetInstance()->m_FlowIOStateMap[strKey]["EmStopFlag"])))
        {
            if((!m_SafeMap[strKey][strType])&&(!m_Teachmodel))
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["EmStopFlag"] = false;
                m_SafeMap[strKey]["StopFlag"] = false;
                m_SafeMap[strKey]["ResetFlag"] = true;
                m_mainFrame->NorPlay(strKey);
            }
        }
    }
}

void MainWindow::ReSetORGRunOK(const QString strkey,uchar uc, bool result)
{
    if(uc == 0)//那么为复位的结果
    {
        if(result)
        {
            if(strkey == "总复位")
            {
                for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
                {
                    item.value()["EmStopFlag"] = false;
                    item.value()["StopFlag"] = true;
                    item.value()["ResetFlag"] = false;
                    item.value()["StartFlag"] = false;
                }
                m_isReset = false;
                m_isPlay = false;
            }
            else
            {
                m_SafeMap[strkey]["StartFlag"] = false;
                m_SafeMap[strkey]["ResetFlag"] = false;
            }
             CoreLog::QLog_Warning(defaultLogModule,QString("%1复位流程成功!").arg(strkey));
             //qDebug() << QString("%1复位流程成功!").arg(strkey);
        }
        else
        {
            if(strkey == "总复位")
            {
                for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
                {
                    item.value()["EmStopFlag"] = false;
                    item.value()["StopFlag"] = true;
                    item.value()["ResetFlag"] = true;
                    item.value()["StartFlag"] = true;
                }
                m_isReset = false;
            }
            else
            {
                m_SafeMap[strkey]["ResetFlag"] = false;
                m_SafeMap[strkey]["StartFlag"] = true;
            }
            CoreLog::QLog_Error(defaultLogModule,QString("%1复位流程失败!").arg(strkey));
            //qDebug() <<QString("%1复位流程失败!").arg(strkey);
        }
    }
    else if(uc == 1)//回原点完成
    {
        m_ORGClicked = false;
        if(result)
        {
            m_isReset = false;
            m_isPlay = false;
            CoreLog::QLog_Warning(defaultLogModule,QString("回原点流程成功!"));
            //qDebug() << QString("回原点流程成功!");
        }
        else
        {
            CoreLog::QLog_Error(defaultLogModule,QString("回原点流程失败!"));
            //qDebug() << QString("回原点流程失败!");
        }
    }
    else if(uc == 3)//停止流程结束
    {
        m_SafeMap[strkey]["StopFlag"] = true;
        m_SafeMap[strkey]["StartFlag"] = false;
        m_SafeMap[strkey]["ResetFlag"] = false;
    }
    m_isWritexml = true;
}

bool DeleteFile(QString filename){
    QFile fi;
    fi.remove(filename);
    return true;
}
int DateDiff(QDateTime old ,QDateTime cur){
 return old.daysTo(cur);
}

void MainWindow::TimerDeleteFiles()
{
    QString logPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/logs";
    QDir dir(logPath);
    if(!dir.exists())
        return;
    dir.setFilter(QDir::Files | QDir::NoSymLinks);//设置类型过滤器，此处表示只为文件格式
    QStringList filters;
    filters<<QString("Test_*.log");
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    if(list.size() < 100)
        return;
    QDateTime currtime =QDateTime::currentDateTime();
    for (int i = 0; i < list.size(); ++i)//size 返回文件字节大小
    {
        QFileInfo fileInfo = list.at(i);//文件信息
        QString name = fileInfo.fileName();
        QString strdate = name.mid(5,8);
        QStringList dateList = strdate.split("_");
        if(dateList.size() >= 3)
            strdate = "20"+dateList[2]+dateList[1]+dateList[0];
        else
        {
            DeleteFile(fileInfo.absoluteFilePath());
            continue;
        }
        QDateTime timeold = QDateTime::fromString(strdate, "yyyyMMdd");
        if(DateDiff(timeold,currtime) > 1)//大于1天的时候进行清除相关的按钮
            DeleteFile(fileInfo.absoluteFilePath());
    }
}
void MainWindow::HandleDB()
{
    if(InOutPutData::GetInstance()->isRunning)
    {
        QString stylestr  = m_comlabel->styleSheet();
        if(!stylestr.contains("normal.png"))
        {
            m_comlabel->setStyleSheet("QLabel{border-image: url(:/ico/image/normal.png);}");            
            HandleItems(1);
            ConfigData::GetInstance()->L_state = 1;
            BaseAxisOperate::SetAllAxisReset("Reset");
            m_pEcTimer->start(200);
            m_mainFrame->SafeThreadTOSatrt();
            connect(SafeThread::GetInstance(),&SafeThread::SafeSignals,this,&MainWindow::SafeSigs);
           // autoframe->Start();
        }
    }
    else{
        QString stylestr  = m_comlabel->styleSheet();
        if(!stylestr.contains("error.png"))
        {
            if(m_mainFrame)
            {
             m_mainFrame->Stop();
            }
             m_pDBTimer->stop();
          // autoframe->Stop();
            m_comlabel->setStyleSheet("QLabel{border-image: url(:/ico/image/error.png);}");
        }
    }
    m_pThreadObj->SaveRealTimeDB();
}

void MainWindow::HandleEcInputData()
{
    if(InOutPutData::GetInstance()->inPutNum > 0)
    {
        for(auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.begin();
            item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end();++item)
        {
            int ibytePos = item.value().iBytePos;
            QString ModelType = item.value().modelType;
            if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
            {
                auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
                for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
                {
                    int ibyteNum = it->byteNum;
                    if(it->key == "Statusworld")
                    {
                        uchar ucFault =  BaseAxisOperate::CheckAxisFault(3,ibyteNum,ibytePos);
                        uchar ucReady = BaseCalcFun::GetBitValue(0,ibyteNum,ibytePos);
                        QString strFault = item.key() +tr("_Fault");
                        QString strReady = item.key() +tr("_ReadySw");
                        if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strFault))
                            DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strFault].inputvalue = ucFault;
                        if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strReady))
                            DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strReady].inputvalue = ucReady;

                    }
                    else if(it->key == "Errorcode")
                    {
                        int errorCode = BaseAxisOperate::GetAxisErrorCode(it->type,ibyteNum,ibytePos);
                        QString strError = item.key() +tr("_Errorcode");
                        if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strError))
                            DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strError].inputvalue = errorCode;
                    }
                    else if(it->key == "Curpos")
                    {
                        int curPos = BaseAxisOperate::GetAxisCurPos(it->type,ibyteNum,ibytePos);
                        QString strCurpos = item.key() +tr("_Curpos");
                        if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strCurpos))
                            DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strCurpos].inputvalue = curPos;
                    }
                    else if(it->key == "Curtq")
                    {
                        int curTq = BaseAxisOperate::GetAxisCurPos(it->type,ibyteNum,ibytePos);//获取实时扭矩
                        QString strCurtq = item.key() +tr("_Curtq");
                        if(DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap.contains(strCurtq))
                            DBInstanceManager::getDBInstance()->DBReaTimInfoStru.reatiminfomap[strCurtq].inputvalue = curTq;
                    }
                    ibytePos += ibyteNum;
                }
            }
        }
    }    
}
void MainWindow::PowerOff()
{
    QProcess::execute(QString("poweroff"));
}

void MainWindow::Reboot()
{
    QProcess::execute(QString("reboot"));
}

void MainWindow::CloseUI()
{
    this->close();
}

void MainWindow::Play(const int )
{
    m_isReset = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isNorStop = false;
    m_isPlay = true;
    m_ORGClicked = true;
    m_mainFrame->Start();
    if(m_pEcTimer->isActive())
        m_pEcTimer->stop();
    m_logEdit->setText(tr(""));
    for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
    {
        item.value()["EmStopFlag"] = false;
        item.value()["StopFlag"] = false;
        item.value()["ResetFlag"] = true;
        item.value()["StartFlag"] = true;
    }

   // if(AttributeInstance::GetInstance()->Power != 3)
      //  m_curPower = AttributeInstance::GetInstance()->Power;
   // AttributeInstance::GetInstance()->Power = 3;
    QApplication::processEvents(QEventLoop::AllEvents,100);
}

void MainWindow::Stop(const int)
{
    m_isNorStop = true;
    m_isReset = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isPlay = true;
    m_ORGClicked = true;
   // if(m_curPower != -1)
      //  AttributeInstance::GetInstance()->Power = m_curPower;
    m_mainFrame->NorStop();
    QApplication::processEvents(QEventLoop::AllEvents,100);
}

void MainWindow::EmrgStop(const int i)
{
    m_isEmStop = true;
    m_isPlay = false;
    m_isWritexml = true;
    m_isReset = false;
    m_isNorStop = true;
    m_ORGClicked = false;
    m_mainFrame->ResumeEvent();
    m_mainFrame->Stop();
  //  if(m_curPower != -1)
       // AttributeInstance::GetInstance()->Power = m_curPower;
    if(!m_pEcTimer->isActive())
        m_pEcTimer->start(300);

    for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
    {
        item.value()["EmStopFlag"] = true;
        item.value()["StopFlag"] = true;
        item.value()["ResetFlag"] = false;
        item.value()["StartFlag"] = false;
    }
    QMap<int ,QMap<int,CoWidgets*>>::const_iterator item = m_FunctionBtnMap.begin();
    while (item != m_FunctionBtnMap.end() && i != item.key())
    {
        QMap<int, CoWidgets*> widgetsMap = item.value();
        foreach (CoWidgets* widget, widgetsMap)
        {
            TButton *btn = qobject_cast<TButton*>(widget);
            if(btn)
            {
                btn->SetBackGroundColor(widget->GetBackColor().at(0));
                btn->SetPressed(false);
                btn->update();
            }
        }
        ++item;
    }
}

void MainWindow::ResetEvent(const int)
{
    m_isReset = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isNorStop = true;
    m_isPlay = true;
    m_ORGClicked = true;
    m_mainFrame->ResetEvent();
    QApplication::processEvents(QEventLoop::AllEvents,100);
}

void MainWindow::ORGEvent(const int)
{
    m_ORGClicked = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isNorStop = true;
    m_isReset = true;
    m_isPlay = true;

    m_mainFrame->ORGEvent();
    QApplication::processEvents(QEventLoop::AllEvents,100);
}

void MainWindow::SuspendOrResume(const int type)
{
    m_ORGClicked = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isNorStop = true;
    m_isReset = true;
    m_isPlay = true;
 //   if(m_curPower != -1)
        //AttributeInstance::GetInstance()->Power = m_curPower;
    if(type == CoWidgets::Function_Suspend)
    {
        m_mainFrame->Pause();
    }
    else if(type == CoWidgets::Function_Resume)
    {
        m_mainFrame->Resume();
    }
    QApplication::processEvents(QEventLoop::AllEvents,100);
}

void MainWindow::HandleItems(const ushort us)
{
    foreach(CoScene *Scene, m_ProStru.sceneMap.values())
    {
        QList<CoItemEx *>ItemList;
        Scene->GetItemsList(ItemList);
        foreach(CoItemEx *Item, ItemList)
        {
            if(us == 0)
            {
                Item->setEnabled(false);
                Item->StartAuto(0);
            }
            else
            {
                Item->setEnabled(true);
                Item->StartAuto(1);
            }
        }
    }
}

void MainWindow::SwithScene(const QString & sceneName, const QString & )
{
    if(m_pCurScene->GetName() != sceneName)
    {
        m_pCurScene = m_ProStru.sceneMap[sceneName];
        m_pView->setScene(m_pCurScene);
    }
}

void MainWindow::SetFuntion(int type, QString )
{
    if(type <= 7 && type > 0)
    {
        if(!InOutPutData::GetInstance()->isRunning)
        {
            return;
        }
    }
    //CoWidgets *cowg = qobject_c  ast<CoWidgets *>(sender());
    switch (type)
    {
    case CoWidgets::Function_Start:
        if(!(SafeThread::GetInstance()->m_EmStopFlag))
        {
            //任一子流程处于急停时,总启动无效
            QMap<QString,QMap<QString,bool>> flowMap = SafeThread::GetInstance()->m_FlowIOStateMap;
            for(auto flow = flowMap.begin(); flow != flowMap.end(); ++flow)
            {
                if(flowMap[flow.key()]["EmStopFlag"])
                    return;
            }

            if((!m_isPlay)&&(!m_Teachmodel))
            {
                CoreLog::QLog_Warning(defaultLogModule, "总启动流程执行...");
                Play(type);
            }
        }
        break;
    case CoWidgets::Function_Suspend:
        SuspendOrResume(type);
        break;
    case CoWidgets::Function_Resume:
        SuspendOrResume(type);
        break;
    case CoWidgets::Function_Stop:
        if(!m_isNorStop && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            CoreLog::QLog_Warning(defaultLogModule, "总停止流程执行");
            Stop(type);
        }
        break;
    case CoWidgets::Function_EmrgStop:
    {
        CoreLog::QLog_Warning(defaultLogModule, "总急停执行");
        EmrgStop(type);
    }
        break;
    case CoWidgets::Function_Reset:
        if(!m_isReset && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            CoreLog::QLog_Warning(defaultLogModule, "总复位流程执行");
            ResetEvent(type);
        }
        break;
    case CoWidgets::Function_ORG:
        if(!m_ORGClicked && !SafeThread::GetInstance()->m_EmStopFlag)
        {
            CoreLog::QLog_Warning(defaultLogModule, "回原点流程执行");
            ORGEvent(type);
        }
        break;
    case CoWidgets::Function_Close:
        CloseUI();
        break;
    case CoWidgets::Function_PowerOff:
        PowerOff();
        break;
    case CoWidgets::Function_Reboot:
        Reboot();
        break;
    default:
        break;
    }
}

void MainWindow::HandleErrorEvent(const ushort us, const QString & str)
{
    QString strTitle = "NoError";
    QMessageBox::Icon ico = QMessageBox::NoIcon;
    if(0 == us)
    {
        strTitle = "错误";
        ico = QMessageBox::Critical;
        HandleItems(us);
    }
    else if(1 == us)
    {
        strTitle = "警告";
        ico = QMessageBox::Warning;
    }
    CreateMsgBox(ico,strTitle, str);
}
void MainWindow::HandModeChanged(char mode)
{
    //开启/退出手轮模式
    if(!m_TeachThread&&m_TeachObj)
    {
        return;
    }
    QVector<QString> ppidvec;
    switch (mode) {
    case 0://退出手轮模式
    {
        m_TeachObj->Stop();
        m_TeachThread->quit();
        m_TeachThread->wait();
        m_Teachmodel = false;
        if(m_CurTeachPPID != "")
        {
            DBInstanceManager::getDBInstance()->m_rmutex.lock();
            if(DBInstanceManager::getDBInstance()->DBVarStru.qpMap.contains(m_CurTeachPPID))
            {
                QString axisinfo = QString(DBInstanceManager::getDBInstance()->DBVarStru.qpMap[m_CurTeachPPID].axisidinfo);
                DBInstanceManager::getDBInstance()->m_rmutex.unlock();
                QStringList strlist = axisinfo.split(";");
                for(int i = 0; i < strlist.size(); ++i)
                {
                    QString str = strlist[0];
                    QStringList axisidinfo = str.split(",");
                    if(axisidinfo.size() == 2)
                    {
                        if(!ppidvec.contains(axisidinfo[0]))
                        {
                            ppidvec.append(axisidinfo[0]);
                        }
                    }
                }
            }
            DBInstanceManager::getDBInstance()->m_rmutex.unlock();
        }
        if(ppidvec.size()>0)
        {
            m_TeachObj->InitJointsBytePosParam(ppidvec);
            m_TeachObj->SetJointsModel(ppidvec,"1");
        }
        // 设置其他控件可用
        foreach (auto item, m_pCurScene->items()) {
            if(!item->isWidget())
            {
                item->setEnabled(true);
            }
        }
    }
        break;
    case 1://开启手轮模式
    {
        if(!m_TeachThread->isRunning())
        {
            m_TeachThread->start();
        }
        else{
            m_TeachObj->Stop();
            m_TeachThread->quit();
            m_TeachThread->wait();
            m_TeachThread->start();
        }
        m_Teachmodel = true;

        //设置当前画布其他控件不可用
        foreach (auto item, m_pCurScene->items()) {
            if(!item->isWidget())
            {
                item->setEnabled(false);
            }
        }
    }
        break;
    default:
        break;
    }

}

void MainWindow::HandModeParamChanged(const QString param)
{
    //手轮模式下 轴，倍率改变
    if(m_TeachThread&&m_TeachObj)
    {
        m_TeachObj->Stop();
        m_TeachThread->quit();
        m_TeachThread->wait();
        QStringList jointinfolist =  param.split(",");
        if(jointinfolist.size() == 3)
        {
            QVector<QString> JointsVec;
            m_CurTeachPPID = jointinfolist[0];
            JointsVec.append(jointinfolist[1]);
            m_TeachObj->SetRunParam(JointsVec,jointinfolist[2].toInt());
            m_TeachObj->InitJointsBytePosParam(JointsVec);
            m_TeachObj->SetJointS_ON(JointsVec);
            m_TeachThread->start();//手轮模式下切换参数才有效
        }
    }
}

void MainWindow::HandModeStateChanged(const char state)
{
    //手轮模式下  状态改变，-1 驱动器故障；0 手轮停止；1 手轮运行
    if(!(m_TeachThread&&m_TeachObj))
    {
        return;
    }
    switch (state) {
    case -1:
        break;
    case 0:
    {
        m_TeachObj->Stop();
        m_TeachThread->quit();
        m_TeachThread->wait();

        //设置当前画布其他控件不可用
        foreach (auto item, m_pCurScene->items()) {
            if(!item->isWidget())
            {
                item->setEnabled(false);
            }
        }
    }
        break;
    case 1:
    {
        if(!m_TeachThread->isRunning())
        {
            m_TeachThread->start();
        }

        // 设置其他控件可用
        foreach (auto item, m_pCurScene->items()) {
            if(!item->isWidget())
            {
                item->setEnabled(true);
            }
        }
    }
        break;
    default:
        break;
    }

}

void MainWindow::CreatMenu()
{
    bool istcp = false;
    for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.begin();
        it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.end();++it)
    {
        if(it.value().serType == "客户端")
        {
            istcp = true;
            break;
        }
    }
    if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigxmlMqttMap.size() != 0 || istcp)
    {

        QMenu * main_menu = new QMenu(this);
        main_menu = menuBar()->addMenu(QString::fromLocal8Bit("通讯连接"));
        if(istcp)
        {
            QAction *connect_action = new QAction();
            connect_action->setText("TCP重新连接");
            connect_action->setStatusTip("TCP Connect Server again");
            connect_action->setShortcut(QKeySequence("Ctrl+8"));
            connect(connect_action,&QAction::triggered,this,&MainWindow::Tcp_Connect);
            main_menu->addAction(connect_action);
        }
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigxmlMqttMap.size() != 0)
        {
            QAction *mqttconnect_action = new QAction();
            mqttconnect_action->setText("MQtt-订阅");
            mqttconnect_action->setStatusTip("重新订阅");
            mqttconnect_action->setShortcut(QKeySequence("Ctrl+9"));
            connect(mqttconnect_action,&QAction::triggered,this,&MainWindow::Mqtt_Connect);
            main_menu->addAction(mqttconnect_action);
        }
    }
}

void MainWindow::Tcp_Connect()
{
    if(SafeThread::GetInstance()->GetAlarmLevel() != 17)
    {
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.end();++it)
        {
            if(it.value().serType == "客户端")
            {
                if(!MyComm_Shared::GetInstance()->m_TcpMap.contains(it.key()))
                {
                    Communication * com = new Communication();
                    MyComm_Shared::GetInstance()->m_TcpMap.insert(it.key(),com);
                }
                if(MyComm_Shared::GetInstance()->m_TcpMap[it.key()])
                {
                    MyComm_Shared::GetInstance()->m_TcpMap[it.key()]->socket = nullptr;
                    if(!MyComm_Shared::GetInstance()->m_TcpMap[it.key()]->Client(it.value().ipAdd,it.value().portName.toInt()))
                    {
                        QMessageBox::warning(this,"Warning","重新连接失败");
                    }
                }
                else
                    QMessageBox::warning(this,"Warning","没有检测到有掉线的客户端");
            }
        }
    }   
}

void MainWindow::Mqtt_Connect()
{
    for(auto itm = MyComm_Shared::GetInstance()->m_MqttMap.begin();itm != MyComm_Shared::GetInstance()->m_MqttMap.end();itm++)
    {
        if(!itm.value()->Subscribe())
        {
            QMessageBox::warning(this,"Warning","QMtt Subscribe failed!");
        }
    }
}
MainWindow::~MainWindow()
{
    if(m_TeachThread)
    {
        m_TeachObj->Stop();
        m_TeachThread->quit();
        m_TeachThread->wait();
        delete m_TeachThread;
        m_TeachThread = NULL;
    }
    if(check_Thread)
    {
        ShareClass::GetIstance()->m_getvalue_Stop = false;
        ShareClass::GetIstance()->m_direction_Stop = false;
        check_Direction->m_check_over = false;

        check_Thread->quit();
        check_Thread->wait();

        get_thread->quit();
        get_thread->wait();

    }
    m_pThread->quit();
    m_pThread->wait();
    delete m_pThread;
    m_pThread = NULL;

    m_pDBTimer->stop();
    delete m_pDBTimer;
    m_pDBTimer  = NULL;

    m_pEcTimer->stop();
    delete m_pEcTimer;
    m_pEcTimer  = NULL;
    if(InOutPutData::GetInstance()->outPutNum > 0)
    {
        QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
        memset(InOutPutData::GetInstance()->outPutData,0,InOutPutData::GetInstance()->outPutNum);
    }
    QThread::msleep(20);
    SafeThread::GetInstance()->CloseInstance();
    m_ec.Stop();
    QThread::msleep(300);
    CoreLog::QLoggerManager::getInstance()->closeLogger();
    LoadFileManager::GetInstance()->CloseInstance();
    if(m_pView)
        delete m_pView;
    m_pView = NULL;

    if(m_pCurScene)
        delete m_pCurScene;
    m_pCurScene = NULL;

    if(m_mainFrame)
    {
        m_mainFrame->Stop();
        delete m_mainFrame;
        m_mainFrame = NULL;
    }

    delete m_pThreadObj;
    m_pThreadObj = NULL;
    delete ui;
}

