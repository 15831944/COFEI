#include "monitorform.h"
#include <QTabWidget>
#include <MyGlobal.h>
#include <QSqlError>
#include <QSqlRecord>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define ADJ(x,y)  x>y?x:y
QWidget *monitorForm::pe = NULL;
monitorForm::monitorForm(QVector<UIXML_STRU> vec,QWidget *parent) : QWidget(parent)
{
    //初始化数据库
    DataBaseManager::GetInstance()->CreateDBTable("create table lifeManagement(ID varchar(100) primary key, addCount int, info varchar(100), maxCount int, actCount varchar(100), UserName varchar(100), type varchar(100))");
    DataBaseManager::GetInstance()->CreateDBTable("create table BurnIn(ID varchar(50) primary key, Timesec varchar(100), status varchar(50))");
    tabmodel = new QSqlTableModel(this,DataBaseManager::GetInstance()->GetDataBase());//重写data函数是字体居中
    tabmodel->setEditStrategy(My_Talbview::OnManualSubmit);//设置tabview可以手动改
    tabmodel->setTable("lifeManagement");
    tabmodel->select();
    m_dbBurninMap.clear();
    pe = parent;
    m_uivec = vec;
    m_irow = 0;
    m_icolumn = 0;
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(TimerUpInputData()));
    //m_pVTimer = new QTimer(this);
    //connect(m_pVTimer,SIGNAL(timeout()),this,SLOT(TimerUpVData()));
//    m_pTabTimer = new QTimer(this);
//    connect(m_pTabTimer,SIGNAL(timeout()),this,SLOT(TimerUpTabData()));
    CreateUI();
    TimerStatus(true);
   // DiffCurTimeToDb(0);
  //  m_pVTimer->start(1000);
}

monitorForm::~monitorForm()
{
    //m_pVTimer->stop();
    m_pTimer->stop();
    //m_pTabTimer->stop();
    file->close();
}

QMenu *monitorForm::CreatQmenu()
{
    QMenu *menu = new QMenu(this);
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建"));
    connect(add,&QAction::triggered,this,&monitorForm::AddData);
    QAction *update = new QAction(QObject::tr("更新"));
    connect(update,&QAction::triggered,this,&monitorForm::UpdateData);
    QAction *del = new QAction(QObject::tr("删除"));
    connect(del,&QAction::triggered,this,&monitorForm::DeleteData);
    QAction *save = new QAction(QObject::tr("保存"));
    connect(save,&QAction::triggered,this,&monitorForm::Save);
    menu->addAction(add);
    menu->addAction(update);
    menu->addAction(del);
    menu->addAction(save);
    return menu;
}

void monitorForm::AddData()
{
    QSqlRecord record = tabmodel->record();
    int row = tabmodel->rowCount();
    tabmodel->insertRecord(row,record);
}

void monitorForm::UpdateData()
{
    if(tabmodel != NULL)
    {
        tabmodel->select();   //更新tabview
    }
}

void monitorForm::DeleteData()
{
    int rowCount = tabview->currentIndex().row();
    bool mydelect = CreateMessagebox("提示","你确定删除当前行吗？\n 当前行所有数据将清空！");
    if(mydelect)
    {
        tabmodel->removeRow(rowCount);
        tabmodel->submitAll();
    }
    else
    {
        tabmodel->revertAll();
    }

}

bool monitorForm::CreateMessagebox(QString title, QString msg)
{
    QDialog logbox;
    logbox.setStyleSheet("background-color:rgb(35,35,35);border-radius:15px;");
    logbox.resize(310,300);
    logbox.setWindowTitle(title);
    QVBoxLayout *vbox = new QVBoxLayout(&logbox);
    QHBoxLayout *hbox = new QHBoxLayout();
    QLabel *message = new QLabel(&logbox);
    QFont ft;
    ft.setPointSize(12);
    message->setFixedSize(300,200);
    message->setText(msg);
    message->setFont(ft);
    message->setStyleSheet("background-color:rgb(35,35,35);color:white");
    message->setAlignment(Qt::AlignCenter);
    vbox->addWidget(message);
    QPushButton *ok = new QPushButton(&logbox);
    ok->setText("确定");
    ok->setFont(ft);
    ok->setStyleSheet("background-color:rgb(35,35,35);color:white;border-radius:15px;");
    connect(ok,&QPushButton::clicked,&logbox,&QDialog::accept);
    QPushButton *clean = new QPushButton(&logbox);
    clean->setStyleSheet("background-color:rgb(35,35,35);color:white;border-radius:15px;");
    clean->setText("取消");
    clean->setFont(ft);
    connect(clean,&QPushButton::clicked,&logbox,&QDialog::reject);
    hbox->addWidget(ok);
    hbox->addWidget(clean);
    vbox->addLayout(hbox);
    logbox.setLayout(vbox);
    if(logbox.exec() == QDialog::Accepted)
        return true;
    return false;
}

void monitorForm::Save()
{
    tabmodel->submitAll();
}

void monitorForm::contextMenuEvent(QContextMenuEvent *event)
{
    if(tabWig->currentIndex() == 2)
    {
        QMenu *menu = CreatQmenu();
        menu->exec(QCursor::pos());
        event->accept();
    }
    else
        return;
}
void monitorForm::CreateUI()
{
     tabWig = new QTabWidget(this);
     tabWig->setGeometry(0,0,pe->width(),pe->height()-32);
     connect(tabWig,&QTabWidget::currentChanged,this,&monitorForm::ChangedTab);
    //Create IO
    QScrollArea *pAreaIO= new QScrollArea(this);
    QWidget *wgIO = new QWidget(this);
    QHBoxLayout *boxIO = new QHBoxLayout(this);
    wgIO->setLayout(boxIO);
    pAreaIO->setWidget(wgIO);//这里设置滚动窗口
    wgIO->setGeometry(0,0,pe->width()-10,pe->height());
   // pAreaIO->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWig->addTab(pAreaIO,"IO部分");

    //Create Server
    QScrollArea *pAreaSer= new QScrollArea(this);
    QWidget *wgSer = new QWidget(this);
    QVBoxLayout *boxSer = new QVBoxLayout(this);
    wgSer->setLayout(boxSer);
    wgSer->setGeometry(0,0,pe->width(),pe->height());
    pAreaSer->setWidget(wgSer);//这里设置滚动窗口
    tabWig->addTab(pAreaSer,"轴信息");
    //----------------begin form korlo---------------
    //Create 寿命管理
    QScrollArea *pAreaCut = new QScrollArea(this);
    QWidget *wgCut = new QWidget(this);
    QVBoxLayout *boxCut = new QVBoxLayout(this);
    wgCut->setLayout(boxCut);
    wgCut->setGeometry(0,0,pe->width(),pe->height());
    pAreaCut->setWidget(wgCut);//设置滚动窗口
    tabWig->addTab(pAreaCut,"信息管理");
    QFont font;
    font.setPointSize(11);
    QFont font1;
    font1.setPointSize(13);
    font1.setFamily("黑体");
    //设置表头
    tabmodel->setHeaderData(0,Qt::Horizontal,"ID");
    tabmodel->setHeaderData(1,Qt::Horizontal,"计数/次");
    tabmodel->setHeaderData(2,Qt::Horizontal,"定义");
    tabmodel->setHeaderData(3,Qt::Horizontal,"额定值");
    tabmodel->setHeaderData(4,Qt::Horizontal,"实际值");
    tabmodel->setHeaderData(5,Qt::Horizontal,"权限级别");
    tabmodel->setHeaderData(6,Qt::Horizontal,"操作类型");

    tabview = new QTableView(wgCut);
    tabview->setModel(tabmodel);//链接在前 设置属性在后
    tabview->verticalHeader()->setDefaultSectionSize(30);
    tabview->setGeometry(0,5,pe->width()-10,pe->height());
    //设置各列的宽度
    tabview->setColumnWidth(0,100);
    tabview->setColumnWidth(1,100);
    tabview->setColumnWidth(2,300);
    tabview->setColumnWidth(3,150);
    tabview->setColumnWidth(4,150);
    tabview->setColumnWidth(5,150);
    tabview->setColumnWidth(6,150);
    tabview->setFont(font);
    //tabview->setContextMenuPolicy(Qt::CustomContextMenu);
    tabview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabview->verticalHeader()->hide();//影藏默认的那个列
    boxCut->addWidget(tabview);
    tabWig->setFont(font1);

    //----------------end form korlo---------------

//    QScrollArea *pAreaPrescript= new QScrollArea(this);
//    QWidget *wgPrescript = new QWidget(this);
//    QVBoxLayout *boxPrescript = new QVBoxLayout(this);
//    wgPrescript->setLayout(boxPrescript);
//    wgPrescript->setGeometry(0,0,pe->width(),pe->height());
//    pAreaPrescript->setWidget(wgPrescript);//这里设置滚动窗口

//    QStringList headerlist;
//    headerlist << "名称" << "类型" << "额定值" << "当前值" <<"注释";
//    PrescriptTabWidget *ptw = new PrescriptTabWidget(headerlist,wgPrescript);
//    boxPrescript->addWidget(ptw);
//    tabWig->addTab(pAreaPrescript,"配方信息");

    QScrollArea *pAreaP= new QScrollArea(this);
    QWidget *wgP = new QWidget(this);
    QVBoxLayout *boxP = new QVBoxLayout(this);
    wgP->setLayout(boxP);
    if(100*16 > pe->width())
        wgP->setGeometry(0,0,100*16,pe->height());
    else
        wgP->setGeometry(0,0,pe->width(),pe->height());
    pAreaP->setWidget(wgP);//这里设置滚动窗口
    QSqlQuery query;
    query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT ID,Timesec,status FROM BurnIn");

    while(query.next())
    {
        QString strkey = query.value("ID").toString();
        DbBurnin bn;
        bn.timesec = query.value("Timesec").toUInt();
        bn.status = query.value("status").toInt();
        m_dbBurninMap.insert(strkey,bn);
    }
    auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_PalletInfo.begin();
    {
        QString filePath = "/home/." + item.key();
        file = new QFile(filePath);
        file->open(QIODevice::ReadWrite);
        stream = new QDataStream(file);
        MODELIOSTRU model;
        model.lable->setFont(font1);
        static int imodel = 0;
        if((++imodel) * pe->width()/3 >= pe->width()-20)
        {
            wgP->setGeometry(0,0,(imodel) *pe->width()/3,pe->height()-10);
        }
        model.lable->setText(item.key());
        boxP->addWidget(model.wgModel);
        int i = 0;
        QStringList namelist;
        namelist<< "ID" << "Timesec" << "status";
        m_irow = item.value().palletDimension["row"];
        m_icolumn = item.value().palletDimension["column"];
        for(i = 0; i < m_irow ;++i)
        {
            for(int j  = 0; j < m_icolumn ;++j)
            {
                PushButton *pbtn = new PushButton();
                connect(pbtn,&PushButton::RClicked,this,&monitorForm::Rclicking);

                pbtn->setFixedSize(32,32);
                pbtn->setFont(font1);
                pbtn->setStatusImageNum(1);
                m_pBtnVec.append(pbtn);
                model.baseLayoutOut->addWidget(pbtn,i,j);
                QString skey = QString("%1x%2").arg(i+1, 2, 10, QChar('0')).arg(j+1, 2, 10, QChar('0'));
                pbtn->setToolTip(skey);
                if(!m_dbBurninMap.contains(skey))
                {
                    QStringList vlist;
                    vlist << skey << QString("%1").arg(0) << QString("%1").arg(0x08);
                    DataBaseManager::GetInstance()->ExcInsertDb("BurnIn",namelist,vlist);
                    pbtn->getIconPath(":/images/pass");
                    DbBurnin bn;
                    bn.timesec = 0;
                    bn.status = 0x08;
                    m_dbBurninMap.insert(skey,bn);
                }
            }
        }
        int iheight = i * 32;
        if(iheight > pe->height()-10)
        {
            model.reWGsize(iheight);
            wgP->setMinimumHeight(iheight);
        }
    }
    pAreaP->setVisible(false);
    //tabWig->addTab(pAreaP,"变量信息");

    //add 指示灯
    QMap<QString,MODELIOSTRU> tempIOMap = QMap<QString,MODELIOSTRU>();
    QMap<QString,MODELSTRU> tempSerMap = QMap<QString,MODELSTRU>();
    QMap<QString,int> tempIn;
    QMap<QString,int> tempout;
    for(int i(0);i < m_uivec.count();++i)
    {
        if(m_uivec[i].className == "Lamp")
        {
            if(!tempIOMap.contains(m_uivec[i].model)&&(m_uivec[i].signalType == "IO"))
            {
                MODELIOSTRU model;
                model.lable->setFont(font1);
                static int imodel = 0;
                if((++imodel) * pe->width()/3 >= pe->width()-20)
                {
                    wgIO->setGeometry(0,0,(imodel) *pe->width()/3,pe->height()-10);
                }
                model.lable->setText(QString("模组:%1").arg(m_uivec[i].model));
                boxIO->addWidget(model.wgModel);

                tempIOMap.insert(m_uivec[i].model,model);
                tempIn.insert(m_uivec[i].model,0);
                tempout.insert(m_uivec[i].model,0);
            }
            else if(!tempSerMap.contains(m_uivec[i].model)&&m_uivec[i].signalType == "SER")
            {
                MODELSTRU model;
                model.lable->setFont(font1);
                static int imodel = 0;
                if((++imodel) * pe->height()/2 > pe->height())
                    wgSer->setGeometry(0,0,pe->width(),imodel*pe->height()/2);
                model.lable->setText(QString("模式:%1").arg(m_uivec[i].model));
                model.wgIn->setToolTip("伺服输入部分");
                model.wgOut->setToolTip("伺服输出部分");
                boxSer->addWidget(model.wgModel);
                tempSerMap.insert(m_uivec[i].model,model);
            }

            if(m_uivec[i].signalType == "IO")
            {  
                QWidget *w = new QWidget();
                w->setFixedSize(pe->width()/3-10,m_uivec[i].csize.height());
                QcwIndicatorLamp *btn = new QcwIndicatorLamp(w);
                btn->SetClickEnable(false);
                btn->setFixedSize(m_uivec[i].csize);

                QLabel *l = new QLabel(w);
                l->setAlignment(Qt::AlignTop);
                l->move(btn->width()+10,-2);
                l->setFont(font);
                btn->setToolTip(m_uivec[i].strText);
                l->setText(m_uivec[i].strText);
                static int iE = 0;
                //static int iQ = 0;

                if(m_uivec[i].address.toUpper().indexOf("E") != -1)
                {
                    //static uchar ic = 0;
                    tempIOMap[m_uivec[i].model].baseLayoutOut->addWidget(w,tempIn[m_uivec[i].model]++,0);
                    ++iE;
                }
//                else
//                {
//                    //static uchar ic = 0;
//                    tempIOMap[m_uivec[i].model].baseLayoutOut->addWidget(w,tempout[m_uivec[i].model]++,1);
//                    ++iQ;
//                }

                //int iheight = (ADJ(iE,iQ)) * (m_uivec[i].csize.height());
                int iheight = iE * (m_uivec[i].csize.height());
                if(iheight > pe->height()-10)
                {
                    tempIOMap[m_uivec[i].model].reWGsize(iheight);
                     wgIO->setMinimumHeight(iheight);
                }
                m_LampIOMap.insert(m_uivec[i].address,btn);
            }
            else if(m_uivec[i].signalType == "SER")
            {
                auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap;

                QMap<QString,QVector<QcwIndicatorLamp*>> templampMap;
                templampMap.clear();
                if(item.contains(m_uivec[i].address))
                {
                    QString strType = item[m_uivec[i].address].modelType;
                    if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(strType))
                    {
                        auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[strType]["INPUT"];
                        QVector<QcwIndicatorLamp*> templampVec;
                        for(auto ior = it.byteInfoVec.begin();ior != it.byteInfoVec.end();++ior)
                        {
                            auto it1 = ior->bitInfoMap.begin();
                            if(it1 == ior->bitInfoMap.end())
                                continue;
                            static uchar ir = 1;
                            uchar ic = 1;
                            uchar ititel = 1;
                            QLabel *laxis = new QLabel();
                            laxis->setAlignment(Qt::AlignHCenter);
                            laxis->setFixedSize(80,20);
                            laxis->setFont(font);
                            laxis->setText(m_uivec[i].address);
                            tempSerMap[m_uivec[i].model].baseLayoutIn->addWidget(laxis,ir,0);
                            for(;it1 != ior->bitInfoMap.end();++it1)
                            {
                                QString name = it1.value().name;
                                if(tempSerMap[m_uivec[i].model].isInfirst)
                                {
                                    QLabel *l = new QLabel();
                                    l->setAlignment(Qt::AlignLeft);
                                    l->setFixedSize(120,20);
                                    l->setFont(font);
                                    l->setText(name);
                                    tempSerMap[m_uivec[i].model].baseLayoutIn->addWidget(l,0,ititel++);
                                }

                                QcwIndicatorLamp *btn = new QcwIndicatorLamp();
                                btn->SetClickEnable(false);
                                btn->setFixedSize(m_uivec[i].csize);
                                btn->setToolTip(name);

                                tempSerMap[m_uivec[i].model].baseLayoutIn->addWidget(btn,ir,ic++);
                                templampVec.append(btn);
                            }
                            tempSerMap[m_uivec[i].model].isInfirst = false;
                            ++ir;
                        }
                        templampMap.insert("INPUT",templampVec);
                    }
                }
                auto item1 = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap;
                if(item1.contains(m_uivec[i].address))
                {
                    QString strType = item[m_uivec[i].address].modelType;
                    if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(strType))
                    {
                        auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[strType]["OUTPUT"];
                        QVector<QcwIndicatorLamp*> templampVec;
                        for(auto ior = it.byteInfoVec.begin();ior != it.byteInfoVec.end();++ior)
                        {
                            auto it1 = ior->bitInfoMap.begin();
                            if(it1 == ior->bitInfoMap.end())
                                continue;
                            static uchar ir = 1;
                            uchar j = 1;
                            uchar ititel = 1;
                            QLabel *laxis = new QLabel();
                            laxis->setAlignment(Qt::AlignHCenter);
                            laxis->setFixedSize(80,20);
                            laxis->setFont(font);
                            laxis->setText(m_uivec[i].address);
                            tempSerMap[m_uivec[i].model].baseLayoutOut->addWidget(laxis,ir,0);
                            //static bool isfirst = true;
                            for(;it1 != ior->bitInfoMap.end();++it1)
                            {
                                QString name = it1.value().name;
                                if(tempSerMap[m_uivec[i].model].isOutfirst)
                                {
                                    QLabel *l = new QLabel();
                                    l->setAlignment(Qt::AlignLeft);
                                    l->setFixedSize(120,20);
                                    l->setFont(font);
                                    l->setText(name);
                                    tempSerMap[m_uivec[i].model].baseLayoutOut->addWidget(l,0,ititel++);
                                }
                                QcwIndicatorLamp *btn = new QcwIndicatorLamp();
                                btn->SetClickEnable(false);
                                btn->setFixedSize(m_uivec[i].csize);

                                btn->setToolTip(name);

                                tempSerMap[m_uivec[i].model].baseLayoutOut->addWidget(btn,ir,j++);
                                templampVec.append(btn);
                            }
                            tempSerMap[m_uivec[i].model].isOutfirst = false;
                            ++ir;
                        }
                        templampMap.insert("OUTPUT",templampVec);
                    }
                }
                if(!m_LampSERMap.contains(m_uivec[i].address))
                    m_LampSERMap.insert(m_uivec[i].address,templampMap);
            }
        }
    }
}

void monitorForm::TimerUpInputData()
{
    QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
    for(auto it = m_LampIOMap.begin();it != m_LampIOMap.end();++it)
    {
        uchar uc = 0;
        //ShowAddressData(it.key(),uc);
        if(it.key().contains("LE"))
            BaseCalcFun::Analysis_L(it.key(),uc);
        else if(it.key().contains("E"))
            BaseCalcFun::AnalysisIOString(it.key(),uc);
        if(uc == 0)
            it.value()->SetAlarm(false);
        else
            it.value()->SetAlarm(true);
    }
    for(auto item = m_LampSERMap.begin();item != m_LampSERMap.end();++item)
    {
        QMap<QString,QVector<uchar>> bitMap = QMap<QString,QVector<uchar>>();
        ShowAddressData(item.key(),bitMap);
        auto tempMap = item.value();
        for(auto it = bitMap.begin();it != bitMap.end();++it)
        {
            if(tempMap.contains(it.key()))
            {
                auto vec = tempMap[it.key()];
                for(int i = 0;i < vec.count();++i)
                {
                    uchar uc = it.value()[i];
                    if(uc == 0)
                        vec[i]->SetAlarm(false);
                    else
                        vec[i]->SetAlarm(true);
                }
            }
        }
    }
}
void monitorForm::TimerStatus(bool isStop)
{
    if(isStop)
    {
        m_pTimer->stop();
    }
    else
    {
        if(tabWig->currentIndex() != 3)
            m_pTimer->start(300);     
    }
}

void monitorForm::GetUIEnableSig(bool isenable)
{
//    if(isenable)
//    {
//        m_pTabTimer->start(500);
//    }
//    else
//    {
//        m_pTabTimer->stop();
//    }
}
int monitorForm::GetRTCTime()
{
    int fd, retval;
    struct rtc_time rtc_tm;
    fd = open("/dev/rtc", O_RDONLY);
    if (fd == -1) {
        return fd;
    }

    /* Read the RTC time/date */
    retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
    if (retval == -1) {
          return retval;
    }
    ::close(fd);
    //to UTC time
    struct tm t;
    memset(&t,0,sizeof(t));
    t.tm_year = rtc_tm.tm_year +1900;
    t.tm_mon = rtc_tm.tm_mon+1;
    t.tm_mday = rtc_tm.tm_mday;
    t.tm_hour = rtc_tm.tm_hour;
    t.tm_min = rtc_tm.tm_min;
    t.tm_sec = rtc_tm.tm_sec;

    return timegm(&t);
//    if(tt != -1)
//    {
//        struct tm* t2 = &t;
//        *t2 = *localtime(&tt);
//        char* ds = (char*) malloc(24);
//        memset(ds, 0, 24);
//        sprintf(ds,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",t2->tm_year,t2->tm_mon,
//                t2->tm_mday, t2->tm_hour, t2->tm_min,t2->tm_sec);
//        return ds;
//    }
    //     return NULL;
}

void monitorForm::UpdateDb(const QString strName, QStringList vlist)
{
    QMutexLocker lock(&m_mutex);
    QString exp = QString("ID = '%1'").arg(strName);
    QStringList namelist;
    namelist<< "ID" << "Timesec" << "status";
    DataBaseManager::GetInstance()->ExcUpdateDb("BurnIn",namelist,vlist,exp);
}
void monitorForm::DiffCurTimeToDb(const uchar uc)
{
    int irtc = GetRTCTime();
    for(auto item = m_dbBurninMap.begin(); item != m_dbBurninMap.end();++item)
    {
        QStringList list = item.key().split("x");
        int x = list[0].toInt();
        int y = list[1].toInt();
        int xy = (x-1)*m_icolumn + y -1;

        if((item.value().status&0x10) == 0x10)
        {
            m_pBtnVec[xy]->getIconPath(":/images/unuse");
        }
        else if((item.value().status&0x40) == 0x40)
        {
            if(uc == 0)
            {
                item.value().status = 0x08;
                m_pBtnVec[xy]->getIconPath(":/images/pass");
                QStringList vlist;
                vlist << QString("%1").arg(item.key()) << QString("%1").arg(0) << QString("%1").arg(item.value().status);
                UpdateDb(item.key(),vlist);
            }
            else
            {
                m_pBtnVec[xy]->getIconPath(":/images/clear");
            }
        }
        else if((item.value().status&0x20) == 0x20)
        {
            m_pBtnVec[xy]->getIconPath(":/images/init");
        }
        else if((item.value().status&0x02) == 0x02)
        {
            int diff = irtc - item.value().timesec;
            if(diff > ConfigData::GetInstance()->m_cusTime)
            {
                item.value().status = 0x08;
                m_pBtnVec[xy]->getIconPath(":/images/pass");
                QStringList vlist;
                vlist << QString("%1").arg(item.key()) << QString("%1").arg(0) << QString("%1").arg(item.value().status);
                UpdateDb(item.key(),vlist);
            }
            else
            {
                int imin = diff /60;
                int isec = diff %60;
                m_pBtnVec[xy]->setText(QString("%1:%2").arg(imin).arg(isec));
                m_pBtnVec[xy]->getIconPath(":/images/start");
            }
        }
        else if((item.value().status&0x08) == 0x08)
        {
            m_pBtnVec[xy]->getIconPath(":/images/pass");
        }
        else if((item.value().status&0x01) == 0x01)
        {
            m_pBtnVec[xy]->getIconPath(":/images/ready");
        }
    }
}
void monitorForm::TimerUpVData()
{
    if (!file->size())
        return ;
    if ( !file->seek(0) )
        return;
    int x = 0;
    int y = 0;
    int z = 0;
    (*stream) >> x >> y >> z;
    uchar uc = 0;
    BaseCalcFun::Analysis_L("LE15.15",uc);
    QString strkey = QString("%1x%2").arg(x, 2, 10, QChar('0')).arg(y, 2, 10, QChar('0'));
    auto item = m_dbBurninMap.find(strkey);
    if(item != m_dbBurninMap.end())
    {
        file->seek(0);
        x = 0;
        y = 0;
        z = 0;
        (*stream) << x << y << z;
        file->flush();
        if((item.value().status&0x01) == 0x01)
        {
            uchar uc1 = 0;
            BaseCalcFun::Analysis_L("LE15.13",uc1);
            if(uc1 == 1)
            {
                item.value().status = 0x20;
            }
            else
            {
                if(uc == 1)
                {
                    item.value().status = 0x40;
                }
                else
                {
                    item.value().status = 0x02;
                }
            }
            item.value().timesec = GetRTCTime();
            QStringList vlist;
            vlist << QString("%1").arg(item.key()) << QString("%1").arg(item.value().timesec) << QString("%1").arg(item.value().status);
            UpdateDb(item.key(),vlist);
        }
        else if((item.value().status&0x08) == 0x08 )
        {
            item.value().status = 0x01;
            QStringList vlist;
            vlist << QString("%1").arg(item.key()) << QString("%1").arg(0) << QString("%1").arg(item.value().status);
            UpdateDb(item.key(),vlist);
        }
        else if((item.value().status&0x20) == 0x20)
        {
            item.value().status = 0x01;
            QStringList vlist;
            vlist << QString("%1").arg(item.key()) << QString("%1").arg(0) << QString("%1").arg(item.value().status);
            UpdateDb(item.key(),vlist);
        }
    }
    DiffCurTimeToDb(uc);
    update();
}

void monitorForm::TimerUpTabData()
{
    if(tabWig->currentIndex() == 2)
    {
        UpdateData();
    }
}
void monitorForm::ChangedTab(int index)
{
    if(index == 3)
    {
        m_pTimer->stop();
    }
    else
    {
        m_pTimer->start(300);
    }
}

void monitorForm::Rclicking(bool isunused)
{
    //add update db
     PushButton *qBtn = qobject_cast<PushButton*>(sender());
     QString skey = qBtn->toolTip();
     auto item = m_dbBurninMap.find(skey);
     if((item.value().status&0x08) == 0x08 && isunused)
     {
         item.value().status = 0x10;
         QStringList vlist;
         vlist << skey << QString("%1").arg(0) << QString("%1").arg(item.value().status);
         UpdateDb(item.key(),vlist);
         qBtn->getIconPath(":/images/unuse");
     }
     else if((item.value().status&0x10) == 0x10 && !isunused)
     {
         item.value().status = 0x08;
         QStringList vlist;
         vlist << skey << QString("%1").arg(0) << QString("%1").arg(item.value().status);
         UpdateDb(item.key(),vlist);
         qBtn->getIconPath(":/images/pass");
     }
}
int monitorForm::ShowAddressData(QString str,QMap<QString,QVector<uchar>> &bitMap)
{
    int iresult = 0;
    auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap;
    if(item.contains(str))
    {
        int iBytePos = item[str].iBytePos;
        QString strModelType = item[str].modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(strModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[strModelType]["INPUT"];
            QVector<uchar> tempbitVec;
            tempbitVec.clear();
            for(auto ior  = it.byteInfoVec.begin();ior != it.byteInfoVec.end();++ior)
            {
                int ibyteNum = ior->byteNum;
                for(auto it1 = ior->bitInfoMap.begin();it1 != ior->bitInfoMap.end();++it1)
                {
                    int bitNum = it1.value().bit;
                    char c = CoreGlobal::BaseCalcFun::GetBitValue(bitNum,ibyteNum,iBytePos);
                    tempbitVec.append(c);
                }
                iBytePos += ibyteNum;
            }
            bitMap.insert("INPUT",tempbitVec);
        }
    }
    auto item1 = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap;
    if(item1.contains(str))
    {
        int iBytePos = item1[str].iBytePos;
        QString strModelType = item[str].modelType;
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(strModelType))
        {
            auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[strModelType]["OUTPUT"];
            QVector<uchar> tempbitVec;
            tempbitVec.clear();
            for(auto ior  = it.byteInfoVec.begin();ior != it.byteInfoVec.end();++ior)
            {
                int ibyteNum = ior->byteNum;
                for(auto it1 = ior->bitInfoMap.begin();it1 != ior->bitInfoMap.end();++it1)
                {
                    int bitNum = it1.value().bit;
                    uchar c = CoreGlobal::BaseCalcFun::GetOutBitValue(bitNum,ibyteNum,iBytePos);
                    tempbitVec.append(c);
                }
                iBytePos += ibyteNum;
            }
            bitMap.insert("OUTPUT",tempbitVec);
        }
    }

    return iresult;
}
