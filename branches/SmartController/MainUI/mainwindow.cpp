#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QTimer>
#include <QDateTime>
#include <time.h>
#include <QThread>
#include "qcwindicatorlamp.h"
#include "pushbutton.h"
#include "monitorform.h"
#include "configform.h"
#include "showphotoform.h"
#include <DycLibBase/LogManager/logmanager.h>
#include <QTime>
#include <QDebug>
#include <QRadioButton>
#include <QSpinBox>
#include <QSettings>
#include <MyGlobal.h>
#include <QCheckBox>
#include <QLabel>
#include <QValueAxis>
#include <QSqlDatabase>
#include <MyConfig_Shared>
#include <MyComm_Shared.h>
#include <DynamicLib/LoadFileManager/LoadFileManager.h>
#include <DycLibBase/XmlManager/xmlmanager.h>
#include <databasemanager.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "customlansiui.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/Exe.ico"));
    m_font.setPointSize(12);
    m_font.setFamily("黑体");
    this->setFont(m_font);
    m_isPlay = true;
    m_ORGClicked = false;
    m_isEmStop = true;
    m_isNorStop = true;
    m_isReset = true;
    m_isWritexml = true;
    m_glw = NULL;
    QString path = QApplication::applicationDirPath() + "/COFEI.db";
    DataBaseManager::GetInstance()->OpenDb(path);
    m_isLogStop = false;
//    thread = new QThread(this);
//    this->moveToThread(thread);
//    QObject::connect(thread, SIGNAL(started()), this, SLOT(LogUpdate()));
//    thread->start();

    QMap<QString,CoreLog::QLogger *> logMap = CoreLog::QLoggerManager::getInstance()->getLogInstanceMap();
    for(auto it = logMap.begin();it != logMap.end();++it )
    {
        connect(it.value(),&CoreLog::QLogger::LogUpdateUI,this,&MainWindow::ShowLog);
    }
    //Initialization variable
    initVariable();
    setStyleSheet(tr("background-color:rgb(35,35,35);color:white;"));

    m_mainFrame = new MainFrame();
    connect(this,&MainWindow::UpdateSwSerxmlEvent,m_mainFrame,&MainFrame::UpdateSwSerXML,Qt::QueuedConnection);
    connect(m_mainFrame,&MainFrame::UpdateRunTime,this,&MainWindow::ShowRunTime);
    connect(m_mainFrame,&MainFrame::ReSetORGRunOK,this,&MainWindow::ReSetORGRunOK);
    //m_mainFrame->Init();
    //fix windows size
    QDesktopWidget *deskWgt = QApplication::desktop();
    QRect screenRect = deskWgt->screenGeometry();
    screenWidth = screenRect.width() -65;
    screenHeight = screenRect.height() -25;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(screenWidth, screenHeight);
    this->move(65,25);
    //add flywidget
    showFlyWidget();

    //Layout ui todo...
    initLayout();
    TimerDeleteFiles();
}

void MainWindow::UpdateUi(QMap<QString,QVector<UIXML_STRU>> uiMap)
{
    if(!CreateCommunicator())
        return;
    uixmlMap = uiMap;
    QMap<QString,bool> temp;
    temp.insert("EmStopFlag",false);
    temp.insert("StopFlag",true);
    temp.insert("ResetFlag",false);
    temp.insert("StartFlag",false);
    auto items = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SAFETYCONFIG_Map.swXml_SAFESTATE_Map;
    for(auto item = items.begin();item != items.end();++item)
    {
        if(item.key() != "总状态")
        {
            m_SafeMap.insert(item.key(),temp);
        }
    }

    initShowConfigDockWidget();
    initShowDebugDockWidget();
    initShowMonitorDockWidget();
    initShowPhotoDockWidget();

    QByteArray ba = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlNetworkStru.portName.toLatin1();
    InOutPutData::GetInstance()->ifName = new char[ba.length()];
    strcpy(InOutPutData::GetInstance()->ifName, ba.data());
    if(InOutPutData::GetInstance()->inPutNum > 0 && InOutPutData::GetInstance()->outPutNum > 0)
    {
        memset(InOutPutData::GetInstance()->inPutData,0,InOutPutData::GetInstance()->inPutNum);
        memset(InOutPutData::GetInstance()->outPutData,0,InOutPutData::GetInstance()->outPutNum);
        m_ec.Start();
    }
    else
    {
        CoreLog::QLog_Warning(defaultLogModule,QString("硬件参数设置有误，没有找到In/Output数据！"));
    }
}
void MainWindow::ShowLog(QString strmsg)
{
    static bool isAddcount = true;
    if(m_listWidget != NULL)
    {
        if(m_listWidget->count() >= 500)
            m_listWidget->clear();
        QListWidgetItem *item = new QListWidgetItem(strmsg);
        if(strmsg.contains("[Info]"))
        {
            if(strmsg.contains("Processdata cycle start ......"))
            {
                m_mainFrame->SafeThreadTOSatrt();
                connect(SafeThread::GetInstance(),&SafeThread::SafeSignals,this,&MainWindow::SafeSigs);
                OpenUISetORGBtnTrue();//打开界面没有错误执行开启回原点按钮
            }
            else if(strmsg.contains("使用次数正常,加") && isAddcount)
            {
                isAddcount = false;
                SafeThread::GetInstance()->SetAlarmLevel(17);
            }
        }
        else if(strmsg.contains("[Error]"))
        {
            if(m_isPlay && m_ORGClicked)
            {
                CheckErrorInRuning();//用于启动之后判断ｅｒｒｏｒ
            }
            logEdit->setText(strmsg);
            item->setTextColor(Qt::red);
            if(m_listErrorWidget != NULL)
            {
                QListWidgetItem *itemerror = new QListWidgetItem(strmsg);
                itemerror->setTextColor(Qt::red);
                m_listErrorWidget->insertItem(0,itemerror);
            }
        }
        else if(strmsg.contains("[Warning]"))
        {
            item->setTextColor(Qt::yellow);
            if(strmsg.contains("指令 LOG"))
            {
                logEdit->setText(strmsg.mid(strmsg.indexOf("LOG")));
            }
            else if(strmsg.contains("使用次数已达上限！请及时更换") && !isAddcount)
            {
                isAddcount = true;
                SafeThread::GetInstance()->SetAlarmLevel(10);
            }

        }
        m_listWidget->insertItem(0,item);
    }
}
void MainWindow::LogUpdate(const QString strmsg)
{
    if(m_listWidget != NULL)
    {
        QListWidgetItem *item = new QListWidgetItem(strmsg);
        if(strmsg.contains("[Error]"))
        {
            logEdit->setText(strmsg);
            item->setTextColor(Qt::red);
        }
        else if(strmsg.contains("[Warning]"))
        {
            item->setTextColor(Qt::yellow);
        }
        m_listWidget->insertItem(0,item);
    }
//    QMap<QString,CoreLog::QLogger *> logMap = CoreLog::QLoggerManager::getInstance()->getLogInstanceMap();
//    if(logMap.size() < 1)
//        return;
//    m_isLogStop = false;
//    bool isAddcount = true;
//    cpu_set_t mask;
//    CPU_ZERO(&mask);
//    CPU_SET(1, &mask);
//    if (pthread_setaffinity_np(pthread_self(), sizeof(mask),&mask) < 0)
//    {
//        return;
//    }
//    while(!m_isLogStop)
//    {
//        QString strmsg;
//        for(auto it = logMap.begin();it != logMap.end();++it)
//        {
//            if(it.value()->m_strMsgQue.count() > 0)
//            {
//                strmsg = it.value()->m_strMsgQue.dequeue();
//                if(m_listWidget != NULL)
//                {
//                    if(m_listWidget->count() >= 500)
//                        m_listWidget->clear();
//                    QListWidgetItem *item = new QListWidgetItem(strmsg);
//                    if(strmsg.contains("Processdata cycle start ......"))
//                    {
//                        m_mainFrame->SafeThreadTOSatrt();
//                        connect(SafeThread::GetInstance(),&SafeThread::SafeSignals,this,&MainWindow::SafeSigs);
//                        OpenUISetORGButTrue();//打开界面没有错误执行开启回原点按钮
//                    }
//                    else if(strmsg.contains("[Error]"))
//                    {
//                        logEdit->setText(strmsg);
//                        item->setTextColor(Qt::red);
//                        if(m_isPlay && m_ORGClicked)
//                        {
//                            checkplaybuttonisok();//用于启动之后判断ｅｒｒｏｒ
//                        }
//                    }
//                    else if(strmsg.contains("[Warning]"))
//                    {
//                        item->setTextColor(Qt::yellow);
//                        if(strmsg.contains("使用次数已达上限！请及时更换") && !isAddcount)
//                        {
//                            isAddcount = true;
//                            SafeThread::GetInstance()->SetAlarmLevel(10);
//                        }
//                    }
//                    else if(strmsg.contains("使用次数正常,加") && isAddcount)
//                    {
//                        isAddcount = false;
//                        SafeThread::GetInstance()->SetAlarmLevel(17);
//                    }
//                    m_listWidget->insertItem(0,item);
//                }
//            }
//            QThread::usleep(500);
//            QApplication::processEvents();
//        }
//        QThread::msleep(5);
//        QApplication::processEvents();
//    }
}
void MainWindow::SafeSigs(const QString strKey, const QString strType ,const QString info)
{
    if(strKey == "总状态")
    {
        if(strType == "EmStopFlag")
        {
            if(!m_isEmStop)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_isEmStop = true;
                Stop();
            }
        }
        else if(strType == "StopFlag")
        {
            if(!m_isNorStop)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_isNorStop = true;
                NorStop();
            }
        }
        else if(strType == "ResetFlag")
        {
            if(!m_isReset)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_isReset = true;
                ResetEvent();
            }
        }
        else if(strType == "ORGFlag")
        {
            if(!m_ORGClicked)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_ORGClicked = true;
                ORGEvent();
            }
        }
        else if(strType == "StartFlag")
        {
            if(!m_isPlay)
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_isPlay = true;
                Play();
            }
        }
    }
    else
    {
        m_isEmStop = false;
        if(strType == "EmStopFlag")
        {
            if(!m_SafeMap[strKey][strType])
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["ResetFlag"] = false;
                m_SafeMap[strKey]["StartFlag"] = false;
                m_SafeMap[strKey]["StopFlag"] = true;
                m_mainFrame->Stop(strKey);
            }
        }
        else if(strType == "StopFlag")
        {
            if(!m_SafeMap[strKey][strType])
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["EmStopFlag"] = false;
                m_mainFrame->NorStop(strKey);
            }
        }
        else if(strType == "ResetFlag")
        {
            if(!m_SafeMap[strKey][strType])
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["StartFlag"] = true;
                m_SafeMap[strKey]["StopFlag"] = true;
                m_SafeMap[strKey]["EmStopFlag"] = false;
                m_mainFrame->ResetEvent(strKey);
            }
        }
        else if(strType == "StartFlag")
        {
            if(!m_SafeMap[strKey][strType])
            {
                CoreLog::QLog_Warning(defaultLogModule, info);
                m_SafeMap[strKey][strType] = true;
                m_SafeMap[strKey]["EmStopFlag"] = false;
                m_SafeMap[strKey]["StopFlag"] = false;
                m_SafeMap[strKey]["ResetFlag"] = true;
                m_mainFrame->NorPlay(strKey);
                emit SendUIEnableEvent(false);
            }
        }
    }
}
void MainWindow::initVariable()
{
    showMainDockWidget = new QDockWidget(this);
    showDebugDockWidget = new QDockWidget(this);
    showMonitorDockWidget = new QDockWidget(this);
    showConfigDockWidget = new QDockWidget(this);
    showPhotoDockWidget = new QDockWidget(this);
}
void MainWindow::showFlyWidget()
{
    m_FlyWidget = new FlyWidget(this);
    connect(this,&MainWindow::SwitchAuthor,m_FlyWidget,&FlyWidget::SwitchAuthor);
    m_FlyWidget->move(screenWidth - 60,screenHeight - 60);
    m_FlyWidget->show();
    QString autor = "user";
    emit SwitchAuthor(autor);
}

void MainWindow::OpenUISetORGBtnTrue()//初始化判断回原点是否可用程序只执行一次
{
    ButtonProtectMap["回原点"]->setEnabled(true);
}

void MainWindow::CheckErrorInRuning()
{
    //表示play之后界面就出现Error的情况
    if(!InOutPutData::GetInstance()->isRunning)
    {
        CoreLog::QLog_Warning(defaultLogModule,QString("EC有错误,开始执行急停流程!"));
        Stop();
    }
    m_isPlay = true;
    m_isEmStop = false;
    m_isNorStop = true;
    m_isReset = true;
    ButtonProtectMap["启动"]->setEnabled(false);
    ButtonProtectMap["复位"]->setEnabled(false);
    ButtonProtectMap["回原点"]->setEnabled(false);
    ButtonProtectMap["停止"]->setEnabled(true);
    ButtonProtectMap["暂停"]->setEnabled(false);
    if(ButtonProtectMap["暂停"]->text()=="恢 复")
    {
        ThreadPR();
    }
}

void MainWindow::NorStop()
{
    m_isEmStop = false;
    m_mainFrame->NorStop();
}

void MainWindow::initLayout()
{
    initToolBars();
    initDockwidget();
    initStatusBars();
}
void MainWindow::initStatusBars()
{
    QLabel *tempLabel = new QLabel(this);
    tempLabel->setStyleSheet(" QLabel{ color: green }");
    tempLabel->setText("Ready");
    tempLabel->setFont(m_font);
    ui->statusBar->addWidget(tempLabel);

    logEdit = new QLineEdit(this);
    logEdit->setStyleSheet(" QLineEdit{ color: red }");
    logEdit->setFont(m_font);
    ui->statusBar->addPermanentWidget(logEdit);

    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet(" QLabel{ color: green }");
    timeLabel->setFont(m_font);
    ui->statusBar->addPermanentWidget(timeLabel);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(1000);
    QTimer *timerDF = new QTimer(this);
    connect(timerDF,SIGNAL(timeout()),this,SLOT(TimerDeleteFiles()));
    timerDF->start(1200000);
}
void MainWindow::timerUpDate()
{
    //获取系统现在的时间
    QDateTime time = QDateTime::currentDateTime();
    //设置系统时间显示格式
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    //在标签上显示时间
    timeLabel->setText(str);
    Counter->setText(QString("%1").arg(ConfigData::GetInstance()->productCount));
    CounterT->setText(QString("%1").arg(ConfigData::GetInstance()->productTolCount));
    QList<QString> listError;
    if(LoadFileManager::GetInstance()->GetErrorState(listError))
    {
        foreach (QString strError, listError) {
            LogUpdate(strError);

        }
    }
    else
    {
        UpdateUi(ConfigData::GetInstance()->xmlMap.uiXml.uiXmlMap);
    }
    static bool isemStop = true;
    if(m_ORGClicked)
    {
        if(SafeThread::GetInstance()->m_EmStopFlag )
        {
            if(isemStop)
            {
                isemStop = false;
                emit SendUIEnableEvent(false);
            }
        }
        else
        {
            if(!isemStop)
            {
                isemStop = true;
                emit SendUIEnableEvent(true);
            }
        }
    }
}
bool MainWindow::CreateCommunicator()
{
    if(ConfigData::GetInstance()->pluginMap.contains("串口通信"))
    {
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlComportMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlComportMap.end();++it)
        {
            Communication *com = new Communication();
            if(!com->OpenPort(it.value().portName,it.value().baudRate.toInt(),8,0,1))
            {
                CoreLog::QLog_Error(defaultLogModule,QString("串口%1打开失败，请确认之后，再重新启动程序！").arg(it.value().portName));
                return false;
            }
            MyComm_Shared::GetInstance()->m_CommMap.insert(it.key(),com);
        }
    }
    if(ConfigData::GetInstance()->pluginMap.contains("网络通信"))
    {
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlIntnetMap.end();++it)
        {
            Communication *com = new Communication();
            if(it.value().serType == "服务器")
            {
                com->Server(8888,MyComm_Shared::GetInstance()->Tcp_peer_name);
            }
            else if(it.value().serType == "客户端")
            {
                if(!com->Client(it.value().ipAdd,it.value().portName.toInt()))
                {
                    CoreLog::QLog_Error(defaultLogModule,QString("客户端%1打开失败，请确认之后，再重新启动程序！").arg(it.key()));
                    return false;
                }
            }
            MyComm_Shared::GetInstance()->m_TcpMap.insert(it.key(),com);
        }
    }
    return true;
}
void MainWindow::TimerDeleteFiles()
{
    ErgodicLogDir();
    ErgodicTorqueDir();
}
void MainWindow::ErgodicLogDir()
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
         if(DateDiff(timeold,currtime) > 7)
             DeleteFile(fileInfo.absoluteFilePath());
     }
}

void MainWindow::ErgodicTorqueDir()
{
    QString logPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹/Data/Torque";
    QDir dir(logPath);
    if(!dir.exists())
        return;
    dir.setFilter(QDir::Files | QDir::NoSymLinks);//设置类型过滤器，此处表示只为文件格式
    QStringList filters;
    filters<<QString("*.txt");
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    if(list.size() < 100)
        return;
    QMap<QString,QString> tempMap;
    for (int i = 0; i < list.size(); ++i)//size 返回文件字节大小
    {
        QFileInfo fileInfo = list.at(i);//文件信息
        QString name = fileInfo.fileName();
        QString strdate = name.replace(".txt","");
        strdate = strdate.replace("__","_");
        QStringList dateList = strdate.split("_");
        if(dateList.size() >= 6)
            strdate = "20"+dateList[2]+dateList[1]+dateList[0]+dateList[3]+dateList[4]+dateList[5];
        else
        {
            DeleteFile(fileInfo.absoluteFilePath());
            continue;
        }
        tempMap.insert(strdate,fileInfo.absoluteFilePath());
    }
    auto it = tempMap.cend()-100;
    while(it != NULL)
    {
        DeleteFile(it.value());
        it--;
    }
}
int MainWindow::DateDiff(QDateTime dtStart, QDateTime dtEnd)
{
    return dtStart.daysTo(dtEnd);
}

bool MainWindow::DeleteFile(const QString filePath)
{
    QFile fi;
    fi.remove(filePath);
    return true;
}
void MainWindow::Play()
{
    m_isPlay = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isNorStop = false;
    m_ORGClicked = true;
    m_isReset = true;
    for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
    {
        item.value()["EmStopFlag"] = false;
        item.value()["StopFlag"] = false;
        item.value()["ResetFlag"] = true;
        item.value()["StartFlag"] = true;
    }
    SafeThread::GetInstance()->m_ResetFlag = false;
    logEdit->setText("");
    m_Splinechart->axisY()->setRange(0,ConfigData::GetInstance()->productTotal);
    //一旦设置启动之后设置以下按钮不可用
    ButtonProtectMap["启动"]->setEnabled(false);
    ButtonProtectMap["复位"]->setEnabled(false);
    ButtonProtectMap["回原点"]->setEnabled(false);
    ButtonProtectMap["暂停"]->setEnabled(true);
    ButtonProtectMap["停止"]->setEnabled(true);
    if (SingleCycle->isChecked())
        ConfigData::GetInstance()->productTempTotal = 1;
    SingleCycle->setEnabled(false);
    m_mainFrame->Start();
    emit SendUIEnableEvent(false);
}

void MainWindow::Stop()
{
    m_isPlay = false;
    m_isWritexml = true;
    m_isEmStop = true;
    m_isReset = false;
    m_isNorStop = true;
    m_ORGClicked = true;
    for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
    {
        item.value()["EmStopFlag"] = true;
        item.value()["StopFlag"] = true;
        item.value()["ResetFlag"] = false;
        item.value()["StartFlag"] = false;
    }
    //停止之后设置除了暂停意外的按钮所有按钮有效
    ButtonProtectMap["暂停"]->setEnabled(false);
    ButtonProtectMap["启动"]->setEnabled(true);
    ButtonProtectMap["复位"]->setEnabled(true);
    ButtonProtectMap["回原点"]->setEnabled(true);
    SingleCycle->setEnabled(true);
    m_mainFrame->Stop();
    if(ButtonProtectMap["暂停"]->text()=="恢 复")
    {
        ThreadPR();
    }
    emit SendUIEnableEvent(true);
}

void MainWindow::ResetEvent()
{
    emit SendUIEnableEvent(false);
    m_isWritexml = false;
    m_isEmStop = false;
    m_isNorStop = true;
    logEdit->setText("");
    m_isPlay = true;
    m_ORGClicked = true;
    //点击复位之后需要处理的命令需要的逻辑处理不可以再点击复位或者其他按钮待商量
    ButtonProtectMap["复位"]->setEnabled(false);
    ButtonProtectMap["回原点"]->setEnabled(false);
    ButtonProtectMap["启动"]->setEnabled(false);
    ButtonProtectMap["暂停"]->setEnabled(false);
    ButtonProtectMap["停止"]->setEnabled(true);
    m_mainFrame->ResetEvent();
}
void MainWindow::ORGEvent()
{
    m_ORGClicked = true;
    m_isWritexml = false;
    m_isEmStop = false;
    m_isPlay = true;
    logEdit->setText("");
    emit SendUIEnableEvent(false);
    BaseAxisOperate::SetAllAxisReset("Reset");
    ButtonProtectMap["复位"]->setEnabled(false);
    ButtonProtectMap["回原点"]->setEnabled(false);
    ButtonProtectMap["启动"]->setEnabled(false);
    ButtonProtectMap["暂停"]->setEnabled(false);
    ButtonProtectMap["停止"]->setEnabled(true);
    m_mainFrame->ORGEvent();

}
void MainWindow::AuthorEvent()
{
    for(int i = 0 ; i < ui->mainToolBar->actions().count(); ++i)
    {
        if(ui->mainToolBar->actions()[i]->text() == "登 录")
        {
            UserLogin login;

            if(login.exec() == QDialog::Accepted)
            {
                ui->mainToolBar->actions()[i]->setIcon(QIcon(":/images/userlock"));
                ui->mainToolBar->actions()[i]->setText("注 销");
                if(login.m_puser->text() == "root")
                {
                      QString autor = "root";
                      emit SwitchAuthor(autor);
                }
                else if(login.m_puser->text() == "admin")
                {
                    QString autor = "admin";
                    emit SwitchAuthor(autor);
                }
            }


        }
        else if(ui->mainToolBar->actions()[i]->text() == "注 销")
        {
            ui->mainToolBar->actions()[i]->setText("登 录");
            ui->mainToolBar->actions()[i]->setIcon(QIcon(":/images/userunlock"));
            ShowMainDockWindow();
            QString autor = "user";
            emit SwitchAuthor(autor);
        }
    }
}

void MainWindow::ShowRunTime(double du)
{
    static double dtime = 0.0;
    static int iNum = 1;
    static int ipoint = 1;
    dtime += du;
    m_Splinechart->setTitle(QString("产量统计/循环周期([%1 s/%2 s])").arg(QString::number(du, 'f', 3))\
                            .arg(QString::number(dtime/ConfigData::GetInstance()->productCount, 'f', 3)));
    if(m_Splineserie != NULL)
    {
        int dt = dtime/60;
        if(dt/2 == ipoint)
        {
            ++ipoint;
            m_Splineserie->append(dtime/3600,ConfigData::GetInstance()->productCount);
        }
        if(dtime/3600 >= iNum *8)
        {
            m_Splineserie->clear();
            m_Splinechart->axisX()->setRange(iNum *8,(iNum +1) *8);
            iNum += 1;
        }
    }

}

void MainWindow::clearCounter()
{
    QString  str="0";
    Counter->setText(str);
    ConfigData::GetInstance()->productCount = 0;
}


void MainWindow::ThreadPR()
{
    for(int i = 0 ; i < ui->mainToolBar->actions().count(); ++i)
    {
        if(ui->mainToolBar->actions()[i]->text() == "暂 停")
        {
            ui->mainToolBar->actions()[i]->setEnabled(false);
            m_mainFrame->Pause();
            ui->mainToolBar->actions()[i]->setEnabled(true);
            ui->mainToolBar->actions()[i]->setText("恢 复");
            ui->mainToolBar->actions()[i]->setIcon(QIcon(":/images/skin/icons/light/appbar.control.resume.png"));
        }
        else if(ui->mainToolBar->actions()[i]->text() == "恢 复")
        {
            m_mainFrame->Resume();
            ui->mainToolBar->actions()[i]->setText("暂 停");
            ui->mainToolBar->actions()[i]->setIcon(QIcon(":/images/skin/icons/light/appbar.control.pause.png"));
        }
    }
}


void MainWindow::CloseUI()
{
    emit ChangeMonitorTimerStatus(true);
    emit ChangeDebugTimerStatus(true);
    CoreLog::QLog_Info("Test","Close App!");
    SafeThread::GetInstance()->m_EmStopFlag = true;
    QSettings *configini = new QSettings("Cowain","MainUI");
    configini->setValue("Config/Tote",ConfigData::GetInstance()->productTolCount);
//    QProcess *poc = new QProcess;
//    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
//    QString str = "chattr -R +ai " + path;
//    poc->start(str);
    if(InOutPutData::GetInstance()->outPutNum > 0)
    {
        QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
        memset(InOutPutData::GetInstance()->outPutData,0,InOutPutData::GetInstance()->outPutNum);
    }
    QThread::msleep(20);
    SafeThread::GetInstance()->CloseInstance();
    SafeThread::GetInstance()->wait();
    m_ec.Stop();
    m_isLogStop = true;
    //thread->quit();
    //thread->wait();
    if(m_mainFrame != NULL)
    {
        delete m_mainFrame;
        m_mainFrame = NULL;
    }
    if(m_glw != NULL)
    {
        delete m_glw;
        m_glw = NULL;
    }
    this->close();
}
void MainWindow::initToolBars()
{
    //对需要处理的ｍａｐ进行插入存储
    ButtonProtectMap.clear();
    QFont ft;
    ft.setPointSize(12);
    ui->menuBar->setFont(ft);
    QAction *closeAct = new QAction(QIcon(":/images/skin/icons/light/appbar.close.png"),QObject::tr("关 闭"), this);
    connect(closeAct, SIGNAL(triggered()), this, SLOT(CloseUI()));

    QAction *playAct = new QAction(QIcon(":/images/skin/icons/light/appbar.control.play.png"),QObject::tr("启 动"), this);
    connect(playAct, SIGNAL(triggered()), this, SLOT(Play()));
    playAct->setEnabled(false);
    ButtonProtectMap.insert("启动",playAct);
    QAction *resumeAct = new QAction(QIcon(":/images/skin/icons/light/appbar.control.pause.png"),QObject::tr("暂 停"), this);
    connect(resumeAct, SIGNAL(triggered()), this, SLOT(ThreadPR()));
    resumeAct->setEnabled(false);
    resumeAct->setVisible(false);
    ButtonProtectMap.insert("暂停",resumeAct);
    QAction *stopAct = new QAction(QIcon(":/images/skin/icons/light/appbar.control.stop.png"),QObject::tr("停 止"), this);
    connect(stopAct, SIGNAL(triggered()), this, SLOT(Stop()));
    stopAct->setEnabled(false);
    ButtonProtectMap.insert("停止",stopAct);
    QAction *ResetAct = new QAction(QIcon(":/images/Reset"),QObject::tr("复 位"), this);
    connect(ResetAct, SIGNAL(triggered()), this, SLOT(ResetEvent()));
    ResetAct->setEnabled(false);
    ButtonProtectMap.insert("复位",ResetAct);
    QAction *ORGAct = new QAction(QIcon(":/images/origin"),QObject::tr("回原点"), this);
    connect(ORGAct, SIGNAL(triggered()), this, SLOT(ORGEvent()));
    ORGAct->setEnabled(false);
    ButtonProtectMap.insert("回原点",ORGAct);
    QAction *AuthorAct = new QAction(QIcon(":/images/userunlock"),QObject::tr("登 录"), this);
    connect(AuthorAct, SIGNAL(triggered()), this, SLOT(AuthorEvent()));

    closeAct->setFont(ft);
    playAct->setFont(ft);
    resumeAct->setFont(ft);
    stopAct->setFont(ft);
    ResetAct->setFont(ft);
    ORGAct->setFont(ft);
    AuthorAct->setFont(ft);

    QWidget *spacer = new QWidget(this);
    QHBoxLayout *hbox = new QHBoxLayout(spacer);
    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpinBox *spb = new QSpinBox(this);
    connect(spb,SIGNAL(valueChanged(int)),this,SLOT(ChangeSpeedRate(int)));
    spb->setRange(1,100);
    spb->setValue(20);
    ft.setPointSize(13);
    spb->setFont(ft);
    spb->setFixedWidth(100);

    //产量计数器
    Counter = new QLineEdit(this);
    //connect(,SIGNAL(),this,SLOT(DefineText()));//当text改变时，发出 触发槽函数
    Counter->setText("000");
    Counter->setReadOnly(true);
    Counter->setFont(ft);
    Counter->setFixedWidth(100);
    Counter->setToolTip("单次计数");

    CounterT = new QLineEdit(this);
    CounterT->setText("001");
    CounterT->setReadOnly(true);
    CounterT->setFont(ft);
    CounterT->setFixedWidth(100);
    CounterT->setToolTip("总计数");

    SigDebug = new QCheckBox("调试",this);
    connect(SigDebug,SIGNAL(clicked()),this,SLOT(OnSingleDebugClicked()));
    SigDebug->setFont(ft);
    SigDebug->setFixedWidth(100);
    //单循环/多循环选择
    SingleCycle =new QCheckBox("单循环",this);
    //SingleCycle->setStyleSheet("QCheckBox {spacing: 5px;} QCheckBox::indicator{border:1px solid white;border-radius:2px;width:15px;height:15px;}");
    connect(SingleCycle,SIGNAL(clicked()),this,SLOT(OnSingleCycleClicked()));
    SingleCycle->setFont(ft);
    SingleCycle->setFixedWidth(100);

    //右键点击计数清零按钮添加产量计数中去
    QAction *clear = new QAction("计数清零",this);
    clear->setFont(ft);
    connect(clear, SIGNAL(triggered()), this, SLOT(clearCounter()));
    Counter->addAction(clear);
    Counter->setContextMenuPolicy(Qt::ActionsContextMenu);

    hbox->addWidget(SigDebug);
    hbox->addWidget(SingleCycle);
    hbox->addWidget(Counter);
    hbox->addWidget(CounterT);
    hbox->addWidget(spb);
    hbox->setAlignment(Qt::AlignRight);

    ui->mainToolBar->addAction(closeAct);
    ui->mainToolBar->addAction(playAct);
    ui->mainToolBar->addAction(resumeAct);
    ui->mainToolBar->addAction(stopAct);
    ui->mainToolBar->addAction(ResetAct);
    ui->mainToolBar->addAction(ORGAct);
    ui->mainToolBar->addAction(AuthorAct);
    ui->mainToolBar->addWidget(spacer);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->mainToolBar->setStyleSheet(tr("background-color:rgb(50,50,50);color:white;"));
}

void MainWindow::initDockwidget()
{
    initShowMainDockWidget();
}
void MainWindow::initShowMainDockWidget()
{
    showMainDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    showMainDockWidget->setFixedWidth(screenWidth-5);
    showMainDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    QWidget *widget = new QWidget(this);
    QGridLayout *baseLayout = new QGridLayout();

    //QVBoxLayout *vboxLansi = new QVBoxLayout();
    //CustomLanSiUI *cusLSUI = new CustomLanSiUI();
   // QLabel * labelLansi = new QLabel();
    //labelLansi->setText("蓝思基础信息");
    //labelLansi->setFont(m_font);
    //labelLansi->setFixedHeight(25);
    //vboxLansi->addWidget(labelLansi);
    //vboxLansi->addWidget(cusLSUI);
    //connect(this,&MainWindow::SendUIEnableEvent,cusLSUI,&CustomLanSiUI::GetUIEnableSig);
    //cusLSUI->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;border-radius:15px;"));
    //baseLayout->addLayout(vboxLansi, 1, 0);
    //create charts
    QVBoxLayout *vboxchart = new QVBoxLayout();
    QChartView *chartView;
    chartView = new QChartView(createLineChart());
    chartView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet(tr("background-color:rgb(35,35,35);color:white;"));

    QLabel * labelchart = new QLabel();
    labelchart->setText("曲线显示");
    labelchart->setFont(m_font);
    labelchart->setFixedHeight(25);
    vboxchart->addWidget(labelchart);
    vboxchart->addWidget(chartView);
    baseLayout->addLayout(vboxchart, 0, 0);

    m_charts << chartView;

    QVBoxLayout *vboxinfo = new QVBoxLayout();
    m_listWidget=new QListWidget(this);
    QLabel * label = new QLabel();
    label->setText("日志信息");
    label->setFont(m_font);
    label->setFixedHeight(25);
    vboxinfo->addWidget(label);
    vboxinfo->addWidget(m_listWidget);
    m_listWidget->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    m_listWidget->setFont(m_font);
    baseLayout->addLayout(vboxinfo, 1, 1);

    m_listErrorWidget=new QListWidget(this);
    QVBoxLayout *vboxerror = new QVBoxLayout();
    QLabel * labelerror = new QLabel();
    labelerror->setText("故障信息");
    labelerror->setFont(m_font);
    labelerror->setFixedHeight(25);
    vboxerror->addWidget(labelerror);
    vboxerror->addWidget(m_listErrorWidget);
    m_listErrorWidget->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    m_listErrorWidget->setFont(m_font);
    baseLayout->addLayout(vboxerror, 0, 1);

    widget->setLayout(baseLayout);
    showMainDockWidget->setWidget(widget);

    this->addDockWidget(Qt::RightDockWidgetArea, showMainDockWidget);
}
void MainWindow::initShowDebugDockWidget()
{
    showDebugDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    showDebugDockWidget->setMinimumWidth(screenWidth-5);
    showDebugDockWidget->setMinimumHeight(screenHeight-125);
    //showDebugDockWidget->setWindowTitle(tr("Show debug"));
    showDebugDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);

    debugForm *debugform = new debugForm(uixmlMap["DebugUI"],showDebugDockWidget);
    connect(debugform,&debugForm::SetAxisSerConfig,this,&MainWindow::WriteSwSerXml);
    connect(this,&MainWindow::SendUIEnableEvent,debugform,&debugForm::GetUIEnableSig);
    connect(this,&MainWindow::ChangeDebugTimerStatus,debugform,&debugForm::ChangeTimerSig);
    showDebugDockWidget->setWidget(debugform);
    showDebugDockWidget->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, showDebugDockWidget);
}
void MainWindow::WriteSwSerXml(QMap<QString,SWCONFIGXML_QPOSIT_STRU> temp1,
                               QMap<QString,SWCONFIGXML_LCSP_STRU> temp2)
{
    //emit UpdateSwSerxmlEvent(temp1,temp2);
    if(m_isWritexml)
        m_mainFrame->UpdateSwSerXML(temp1,temp2);
    return;
}
void MainWindow::ReSetORGRunOK(const QString strkey,uchar uc, bool result)
{
    if(uc == 0)//那么为复位的结果
    {
        if(result)
        {
            if(strkey == "总状态")
            {
                ButtonProtectMap["回原点"]->setEnabled(true);
                ButtonProtectMap["启动"]->setEnabled(true);
                ButtonProtectMap["复位"]->setEnabled(true);
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
            }

             CoreLog::QLog_Info(defaultLogModule,QString("%1复位流程成功!").arg(strkey));
        }
        else
        {
            if(strkey == "总状态")
            {
                ButtonProtectMap["复位"]->setEnabled(true);
                ButtonProtectMap["停止"]->setEnabled(false);
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
            }
            CoreLog::QLog_Error(defaultLogModule,QString("%1复位流程失败!").arg(strkey));
        }   
    }
    else if(uc == 1)//那么为回原点结果
    {
        m_ORGClicked = false;
        if(result==true)//表示回原点成功
        {
            ButtonProtectMap["启动"]->setEnabled(true);
            ButtonProtectMap["复位"]->setEnabled(true);
            m_isReset = false;
            m_isPlay = false;
            CoreLog::QLog_Info(defaultLogModule,QString("回原点流程成功!"));
        }
        else
        {
            CoreLog::QLog_Error(defaultLogModule,QString("回原点流程失败!"));
        }
        ButtonProtectMap["回原点"]->setEnabled(true);
        ButtonProtectMap["停止"]->setEnabled(false);
    }
    else if(uc == 2)//那么为NorStop结果
    {
        if(strkey == "总状态")
        {
            m_isReset = false;
            m_isPlay = false;
            m_isEmStop = false;
            m_ORGClicked = true;
            for(auto item = m_SafeMap.begin();item != m_SafeMap.end();++item)
            {
                item.value()["EmStopFlag"] = false;
                item.value()["StopFlag"] = true;
                item.value()["ResetFlag"] = false;
                item.value()["StartFlag"] = false;
            }
            //停止之后设置除了暂停意外的按钮所有按钮有效
            ButtonProtectMap["暂停"]->setEnabled(false);
            ButtonProtectMap["启动"]->setEnabled(true);
            ButtonProtectMap["复位"]->setEnabled(true);
            ButtonProtectMap["回原点"]->setEnabled(true);
            SingleCycle->setEnabled(true);
            if(ButtonProtectMap["暂停"]->text()=="恢 复")
            {
                ThreadPR();
            }
        }
        else
        {
            m_SafeMap[strkey]["ResetFlag"] = false;
            m_SafeMap[strkey]["StartFlag"] = false;
        }
    }
    m_isWritexml = true;
    emit SendUIEnableEvent(true);
}

void MainWindow::initShowMonitorDockWidget()
{
    showMonitorDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    showMonitorDockWidget->setMinimumWidth(screenWidth-5);
    showMonitorDockWidget->setMinimumHeight(screenHeight-125);
    //showMonitorDockWidget->setWindowTitle(tr("Show Monitor"));
    showMonitorDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    monitorForm *monitorform = new monitorForm(uixmlMap["MonitorUI"],showMonitorDockWidget);
    connect(this,&MainWindow::SendUIEnableEvent,monitorform,&monitorForm::GetUIEnableSig);
    connect(this,&MainWindow::ChangeMonitorTimerStatus,monitorform,&monitorForm::TimerStatus);
    showMonitorDockWidget->setWidget(monitorform);
    showMonitorDockWidget->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, showMonitorDockWidget);
}
void MainWindow::initShowConfigDockWidget()
{
    showConfigDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    showConfigDockWidget->setMinimumWidth(screenWidth-5);
    showConfigDockWidget->setMinimumHeight(screenHeight-125);
    //showConfigDockWidget->setWindowTitle(tr("Show Config"));
    showConfigDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    ConfigForm *configform = new ConfigForm(uixmlMap["ConfigUI"],showConfigDockWidget);
    showConfigDockWidget->setWidget(configform);
    showConfigDockWidget->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, showConfigDockWidget);
}
void MainWindow::initShowPhotoDockWidget()
{
    showPhotoDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    showPhotoDockWidget->setMinimumWidth(screenWidth-5);
    showPhotoDockWidget->setMinimumHeight(screenHeight-125);
    //showPhotoDockWidget->setWindowTitle(tr("Show Photo"));
    showPhotoDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    ShowPhotoForm *showphotoform = new ShowPhotoForm(uixmlMap["ConfigUI"],showPhotoDockWidget);
    showPhotoDockWidget->setWidget(showphotoform);
    showPhotoDockWidget->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, showPhotoDockWidget);
}
MainWindow::~MainWindow()
{
    QThread::msleep(50);
    CoreLog::QLoggerManager::getInstance()->closeLogger();
    DataBaseManager::GetInstance()->CloseDb();
    delete ui;

}

void MainWindow::ShowMainDockWindow()
{
    showMainDockWidget->show();
    showDebugDockWidget->hide();
    showMonitorDockWidget->hide();
    showConfigDockWidget->hide();
    showPhotoDockWidget->hide();
    emit ChangeMonitorTimerStatus(true);
    emit ChangeDebugTimerStatus(true);
}
void MainWindow::ShowDebugDockWindow()
{
    showDebugDockWidget->show();
    showMainDockWidget->hide();
    showMonitorDockWidget->hide();
    showConfigDockWidget->hide();
    showPhotoDockWidget->hide();
    emit ChangeMonitorTimerStatus(true);
    emit ChangeDebugTimerStatus(false);
}
void MainWindow::ShowMonitorDockWindow()
{
    showMonitorDockWidget->show();
    showMainDockWidget->hide();
    showDebugDockWidget->hide();
    showConfigDockWidget->hide();
    showPhotoDockWidget->hide();
    emit ChangeMonitorTimerStatus(false);
    emit ChangeDebugTimerStatus(true);
}
void MainWindow::ShowConfigDockWindow()
{
    showConfigDockWidget->show();
    showMonitorDockWidget->hide();
    showMainDockWidget->hide();
    showDebugDockWidget->hide();
    showPhotoDockWidget->hide();
    emit ChangeMonitorTimerStatus(true);
    emit ChangeDebugTimerStatus(true);
}
void MainWindow::ShowPhotoDockWindow()
{
    showPhotoDockWidget->show();
    showMonitorDockWidget->hide();
    showMainDockWidget->hide();
    showDebugDockWidget->hide();
    showConfigDockWidget->hide();
    emit ChangeMonitorTimerStatus(true);
    emit ChangeDebugTimerStatus(true);
}

void MainWindow::receiveMessage(QString message)
{
    if(message.compare("COFEI R&D") == 0)
    {
        this->close();
    }
}
QChart *MainWindow::createBarChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < 5; i++) {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (int j(10);j<20;j++)
            *set << j;
        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    return chart;
}
QChart *MainWindow::createLineChart()
{
    m_Splinechart = new QChart();
    m_Splinechart->setTheme(QChart::ChartThemeDark);
    m_Splinechart->setTitleFont(m_font);
    m_Splinechart->setTitle("产量统计 / 循环周期( s)");

    m_Splineserie = new QLineSeries;
    m_Splineserie->append(0.0,0.0);
    m_Splinechart->addSeries(m_Splineserie);

    m_Splinechart->createDefaultAxes();
    QValueAxis * pxaxis = new QValueAxis;
    QValueAxis * pyaxis = new QValueAxis;
    m_Splinechart->legend()->hide();
    m_Splinechart->axisY()->setRange(0,ConfigData::GetInstance()->productTotal);
    m_Splinechart->axisX()->setRange(0,8);
    m_Splinechart->setAxisY(pyaxis,m_Splineserie);
    m_Splinechart->axisY()->setTitleText("产量(N)");
    m_Splinechart->setAxisX(pxaxis,m_Splineserie);

    m_Splinechart->axisX()->setTitleText("Time(h)");
    pxaxis->setTickCount(4);
    pxaxis->setMinorTickCount(3);
    //chart->axisX()->setGridLineColor(Qt::red);
    //chart->axisY()->setGridLineColor(Qt::blue);
    m_Splinechart->setAcceptHoverEvents(true);
    return m_Splinechart;
}
void MainWindow::ChangeSpeedRate(int ivalue)
{
    ConfigData::GetInstance()->speedRate = ivalue;
}

void MainWindow::OnSingleCycleClicked()
{
    if (SingleCycle->isChecked())
        ConfigData::GetInstance()->productTempTotal = 1;
    else
        ConfigData::GetInstance()->productTempTotal = ConfigData::GetInstance()->productTotal;

}

void MainWindow::OnSingleDebugClicked()
{
    if (SigDebug->isChecked())
        ConfigData::GetInstance()->L_state = 1;
    else
        ConfigData::GetInstance()->L_state = 0;
}


void MainWindow::on_actionVersion_Information_triggered()
{
    if(m_glw == NULL)
        m_glw = new MyGLWidget(20);
    m_glw->SetWidgetSize(screenWidth,screenHeight);
    m_glw->showNormal();

}
