#include "TouchPP.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QScrollArea>
#include <QGraphicsProxyWidget>
#include <QTreeWidgetItemIterator>
#include "CoGraphicsItems/AttributeInstance.h"
#include "DycLibBase/XmlManager/xmlmanager.h"
#include "AttributeWidget.h"
#include "HandMode.h"
#include <QStandardPaths>
#include "AxisOperater.h"
#include "NewPPDialog.h"
TouchPP::TouchPP(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth, const QColor &LineColor,
                 const QColor &BackColor, CoScene *Scene):
CoProxyWidget(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_touchMode = TouchMode::ppMode;
    SetType(CoItem::Mde_Axis_dug);
    QWidget *widget = new QWidget();
    widget->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setContentsMargins(1,1,1,1);
    widget->setLayout(hbox);
    m_treeWidget = CreateTreeWidget(widget);
    QScrollArea *pArea = new QScrollArea(widget);
    pArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pArea->setStyleSheet(tr("background-color:rgb(69, 187, 217);color:white;border-radius:15px;"));
    m_showWidget = CreateShowWidget(pArea);
    pArea->setWidget(m_showWidget);
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->setContentsMargins(1,1,1,1);
    QHBoxLayout *hbox1 = new QHBoxLayout();
    hbox1->setContentsMargins(1,3,1,1);
    m_posInfo = new QLabel(widget);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::lightGray);
    m_posInfo->setPalette(pa);
    m_posInfo->setFont(QFont("黑体",11));
    m_posInfo->setFixedSize(90,32);
    QPushButton *btnHand = new QPushButton("手轮模式",widget);
    btnHand->setFixedSize(65,32);
    connect(btnHand,&QPushButton::clicked,this,&TouchPP::ButtonClicked);
    QPushButton *btn = new QPushButton("示教",widget);
    connect(btn,&QPushButton::clicked,this,&TouchPP::ButtonClicked);
    btn->setFixedSize(45,32);
    m_runBtn = new QPushButton("运行",widget);
    connect(m_runBtn,&QPushButton::clicked,this,&TouchPP::ButtonClicked);
    m_runBtn->setFixedSize(45,32);
    m_stopBtn = new QPushButton("停止",widget);
    connect(m_stopBtn,&QPushButton::clicked,this,&TouchPP::ButtonClicked);
    m_stopBtn->setFixedSize(45,32);
    m_debugInfo = new QTextEdit(widget);
    m_debugInfo->setTextColor(Qt::lightGray);
    m_debugInfo->setFont(QFont("黑体",10));
    m_debugInfo->setFixedSize(100,32);
    hbox1->addWidget(m_posInfo);
    hbox1->addWidget(btnHand);
    hbox1->addSpacing(10);
    hbox1->addWidget(btn);
    hbox1->addWidget(m_runBtn);
    hbox1->addWidget(m_stopBtn);
    hbox1->addWidget(m_debugInfo);
    vBox->addLayout(hbox1);
    vBox->addWidget(pArea);
    hbox->addWidget(m_treeWidget);
    hbox->addLayout(vBox);
    if(Scene)
    {
        QGraphicsProxyWidget *proxyWidget = Scene->addWidget(widget);
        QGraphicsGridLayout *pLayout = new QGraphicsGridLayout();
        pLayout->addItem(proxyWidget,0,0);
        pLayout->setContentsMargins(1,1,1,1);
        this->setLayout(pLayout);
        resize(Scene->width(),Scene->height());
        setGeometry(0,0,Scene->width(),Scene->height()-80);
    }
    MoveEnable(false);
    setZValue(10);
    m_PPPlugin = ConfigData::GetInstance()->pluginMap.contains("G00") ? ConfigData::GetInstance()->pluginMap["G00"]->Copy():NULL;
    connect(this,&TouchPP::SendUpdateXMLEvent,this,&TouchPP::UpdateSwSerXML);
}

MyTree* TouchPP::CreateTreeWidget(QWidget *parent)
{
    MyTree *treeWidget = new MyTree(parent);
    treeWidget->setFixedWidth(353);
    treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(treeWidget,&MyTree::ItemClicking,this,&TouchPP::TreeDoubleClicked);
    connect(treeWidget,&MyTree::UpdatePP,this,&TouchPP::UpdatePP);
    connect(treeWidget,&MyTree::SavePP,this,&TouchPP::SavePP);
    connect(treeWidget,&MyTree::AddPP,this,&TouchPP::AddPP);
    connect(treeWidget,&MyTree::DeletePP,this,&TouchPP::DeletePP);
    QStringList head;
    head << "P点" << "轴名称" << "当前位置";
    QTreeWidgetItem *HeadItem = new QTreeWidgetItem(head);
    treeWidget->setHeaderItem(HeadItem);
    treeWidget->setColumnCount(3);
    for(int i = 0 ; i < 3 ; ++i)
    {
        HeadItem->setFont(i,QFont("黑体",13));
        HeadItem->setBackgroundColor(i,Qt::blue);
    }
    treeWidget->setColumnWidth(0, 100);  //设置列宽
    treeWidget->setColumnWidth(1, 80);
    treeWidget->setColumnWidth(2, 250);
    UpdateTree(treeWidget);
    return treeWidget;
}

void TouchPP::TreeDoubleClicked(QTreeWidgetItem *item, int)
{
    if(NULL == item)
        return;
    if(item->text(0).isEmpty())
        return;
    QLayoutItem *Litem;
    while((Litem = m_pGrid->takeAt(0)) != 0)
    {
        if(Litem->widget()){
            delete Litem->widget();
        }
        delete Litem;
    }

    m_posInfo->setText(item->text(0));
    if(m_touchMode == TouchMode::ppMode)
    {
        m_AxisMap.clear();
        QStringList list = item->text(1).split(",");
        for(int i = 0;i < list.size();++i)
        {
            ConfigData::GetInstance()->m_mutex.lock();
            if(!ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(list[i]))
            {
                ConfigData::GetInstance()->m_mutex.unlock();
                continue ;
            }
            QString startstr = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[list[i]].swLimitStart;
            QString endstr = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[list[i]].swLimitEnd;
            ConfigData::GetInstance()->m_mutex.unlock();
            AxisOperater *aop = new AxisOperater();
            aop->m_pointName = item->text(0);
            if(!GetSelectAble())
                aop->SetPower(GetPower());
            else
                aop->SetPower(-1);
           aop->SetAxisName(list[i],QMap<QString,PROTECT_EVENT_STRU>());
           aop->SetAxisRanage(startstr.toDouble(),endstr.toDouble());

            m_pGrid->addWidget(aop,i,0);
            m_AxisMap.insert(list[i],aop);
        }
    }
    else if(m_touchMode == TouchMode::handMode)
    {
        HandMode *hand = new HandMode();
        hand->setObjectName("handmode");
        if(!GetSelectAble())
            hand->SetPower(GetPower());
        else
            hand->SetPower(-1);
        hand->setPosInfo(tr("%1;%2;%3").arg(item->text(0)).arg(item->text(1)).arg(item->text(2)));
        hand->InitValue();
        connect(hand,&HandMode::SendHandParamEvent,this,&TouchPP::SendHandParamEvent);
        connect(hand,&HandMode::SendStateEvent,this,&TouchPP::SendHandModeState);
        connect(hand,&HandMode::SendStateEvent,this,[=](const char state, const QString errorCode){
            if(state == -1)
                m_debugInfo->setText(tr("errorCode:%1 驱动器故障").arg(errorCode));
                });

        m_pGrid->addWidget(hand,0,0);
    }
}

void TouchPP::AddPP()
{
    NewPPDialog *np = new NewPPDialog();
    connect(np,&NewPPDialog::SendPP,this,&TouchPP::AddPPToTree);
    QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> HWConfig = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap;
    np->SetHWParm(HWConfig);
    np->setModal(false);
    np->exec();
}

void TouchPP::AddPPToTree(SWCONFIGXML_QPOSIT_STRU pp)
{
    //数据库新增点位变量
    //数据库写入时,先检查是否重名,若重名,则返回false.其他错误,均报"重名"错误
    if(!DBInstanceManager::getDBInstance()->AddWritePPVarToDB(pp))
    {
        NewPPDialog *np = qobject_cast<NewPPDialog*>(sender());
        np->SetIsNew(false);
        return;
    }

    QTreeWidgetItem *TreeItem = new QTreeWidgetItem(m_treeWidget);
    QStringList axisList = pp.axisMap.keys();
    QString axis = axisList.join(",");
    QStringList posList;
    for(int i = 0; i < axisList.size(); i++)
    {
        posList.append("0");
        QTreeWidgetItem *child = new QTreeWidgetItem();
        child->setText(1,axisList.at(i));
        child->setText(2,"0");
        TreeItem->addChild(child);
    }
    QString pos = posList.join(",");
    TreeItem->setText(0,pp.id);
    TreeItem->setText(1,axis);
    TreeItem->setText(2,pos);
    TreeItem->setToolTip(0,pp.id);
    TreeItem->setToolTip(1,axis);
    TreeItem->setToolTip(2,pos);
    QMap<QString,QString> AxisMap;
    if(DBInstanceManager::getDBInstance()->DBVarStru.qpMap.contains(pp.id))
    {
        if(!QString(DBInstanceManager::getDBInstance()->DBVarStru.qpMap[pp.id].axisidinfo).isEmpty())
        {
            QStringList axisinfolist = QString(DBInstanceManager::getDBInstance()->DBVarStru.qpMap[pp.id].axisidinfo).split(";");
            for(int i = 0; i < axisinfolist.size();++i)
            {
                QString  axisvar = axisinfolist[0];
                QStringList axisvarlist = axisvar.split(":");
                if(axisvarlist.size() == 2)
                {
                    QString axisname = axisvarlist[0];
                    QStringList reglist = QString(axisvarlist[1]).split(",");
                    if(reglist.size() >= 2)
                    {
                        if(!AxisMap.contains(axisname))
                        {
                            AxisMap.insert(axisname,reglist[0]);
                        }
                    }
                }
            }
        }
    }
    if(AxisMap.size() > 0)
    {
        if(!m_PPMap.contains(pp.id))
        {
            m_PPMap.insert(pp.id,AxisMap);
        }
        else{
            m_PPMap[pp.id] = AxisMap;
        }
    }
}

void TouchPP::DeletePP()
{
    QList<QTreeWidgetItem*>items = m_treeWidget->selectedItems();
    QList<SWCONFIGXML_QPOSIT_STRU> posList;
    posList.clear();
    foreach (QTreeWidgetItem* item, items)
    {
        if(item)
        {
            // 构建p点结构
            SWCONFIGXML_QPOSIT_STRU pos;
            pos.id = item->text(0);
            QMap<QString ,AXISINFO_STRU> axisMap;
            axisMap.clear();
            QStringList axises = item->text(1).split(",");
            foreach (QString axis, axises) {
                AXISINFO_STRU info;
                axisMap.insert(axis,info);
            }
            pos.axisMap = axisMap;
            pos.axisNum = axises.size();
            posList.append(pos);

            // 删除树节点
            delete item;
            item = NULL;
        }
    }
    // 数据库删除变量
    DBInstanceManager::getDBInstance()->DelWritePPVarToDB(posList);

}

void TouchPP::UpdateSwSerXML()
{
    XmlManager xml;
    QString path=QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹"+"/参数配置/软件参数配置.xml";
    xml.WriteSWSerXml(path,m_qpMap,QMap<QString,_SWCONFIGXML_LCSP_STRU>());
    ConfigData::GetInstance()->m_mutex.lock();
    ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map = m_qpMap;
    ConfigData::GetInstance()->m_mutex.unlock();
}

void TouchPP::UpdatePP()
{
    UpdateTree(m_treeWidget);
}

void TouchPP::SavePP()
{
    m_qpMap.clear();
    for(auto it = DBInstanceManager::getDBInstance()->DBVarStru.qpMap.begin();
        it != DBInstanceManager::getDBInstance()->DBVarStru.qpMap.end(); ++it)
    {
        SWCONFIGXML_QPOSIT_STRU QPstru;
        QPstru.id = it.value().pkey;
        QPstru.note = it.value().note;
        QPstru.acc = it.value().acc;
        QPstru.dec = it.value().dec;
        QPstru.timeOut = it.value().timeOut;
        QPstru.speed = 100;
        QPstru.delayMsec = it.value().delay;
        QStringList list = QString(it.value().axisidinfo).split(";");
        QPstru.axisNum = list.size();
        foreach (QString str, list) {
            if(str.isEmpty() || !str.contains(":")||!str.contains(","))
                continue;
            QStringList list = str.split(":");
            QString posstr = list[1];
            QStringList poslist = posstr.split(",");
            if( poslist.size() < 2)
               {
                continue;
               }
                AXISINFO_STRU axisStru;
                axisStru.speed =poslist[1].toFloat();
                if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(poslist[0]))
                    axisStru.pos = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[poslist[0]].Initvalue;
                if( poslist.size() == 3)
                {
                    if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(poslist[2]))
                    {
                        axisStru.offsetPos = poslist[2];
                    }
                }
            QPstru.axisMap.insert(list[0],axisStru);
        }
        m_qpMap.insert(it.key(),QPstru);
    }
    emit SendUpdateXMLEvent();
}

void TouchPP::ButtonClicked()
{
    if(AttributeInstance::GetInstance()->Power > GetPower() || GetSelectAble() )
        return;
    QPushButton *qBtn = qobject_cast<QPushButton*>(sender());
    QString text = qBtn->text();
    if(text == "示教")
    {
        QTreeWidgetItemIterator it(m_treeWidget);
        while (*it)
        {
            if(!(*it)->text(0).isEmpty())
            {
                if((*it)->text(0) == m_posInfo->text())
                {
                    QString poss = "";
                    QString ids = "";
                    if(m_touchMode == TouchMode::ppMode)
                    {
                        int icount = 0;
                        for(auto ior = m_AxisMap.begin(); ior != m_AxisMap.end();)
                        {
                            QString strId = ior.key();
                            double pos = ior.value()->GetTraPos();
                            QString varName = m_PPMap[m_posInfo->text()][strId];
                            DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[varName].Initvalue = pos;
                            ids += strId;
                            poss += QString::number(pos);
                            if(m_AxisMap.size() > 1)
                            {
                                (*it)->child(icount)->setText(1,ior.key());
                                (*it)->child(icount++)->setText(2,QString::number(pos));
                            }
                            ++ior;
                            if(ior != m_AxisMap.end())
                            {
                                ids += ",";
                                poss += ",";
                            }
                        }
                        (*it)->setText(1,ids);
                    }
                    else if(m_touchMode == TouchMode::handMode)
                    {
                        QWidget* wg = m_pGrid->itemAt(0)->widget();
                        {
                            if(wg->objectName() == "handmode")
                            {
                                HandMode* hm = qobject_cast<HandMode*>(wg);
                                if(hm)
                                {
                                    poss = hm->GetCurPos();
                                    QStringList list = poss.split(",");
                                    if(list.size() > 1)
                                    {
                                        for(int i = 0; i < list.size();i++)
                                            (*it)->child(i)->setText(2,list.at(i));
                                    }
                                }
                            }
                        }
                    }
                    (*it)->setText(2,poss);
                    break;
                }
            }
            ++it;
        }
    }
    else if(text == "运行")
    {
        QString ppID = m_posInfo->text();
        SWCONFIGXML_QPOSIT_STRU qpStru;
        if(ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map.contains(ppID))
        {
            qpStru = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.swXml_QP_Map[ppID];
        }
        //        qpStru.id = "PPDebug";
        //        qpStru.speed = 100;
        //        qpStru.acc = 100;
        //        qpStru.dec = 100;
        //        qpStru.delayMsec = 5;
        //        qpStru.timeOut = 60000;
        //        qpStru.axisNum = m_AxisMap.size();
        //        for(auto ior = m_AxisMap.begin(); ior != m_AxisMap.end();++ior)
        //        {
        //            QString axisName = ior.key();
        //            uint Traspeed = ior.value()->GetTraSpeed()/2 ;
        //            int Trapos = ior.value()->GetTraPos();
        //            AXISINFO_STRU asixStru;
        //            asixStru.pos = Trapos;
        //            asixStru.speed = Traspeed;
        //            qpStru.axisMap[axisName] = asixStru;
        //        }
        //m_debugInfo
        if(m_PPPlugin)
        {
            m_debugInfo->setPlainText("执行P点...");
            m_PPPlugin->Initialization();
            SWCONFIGXML_STRU ps;
            ps.swXml_QP_Stru = qpStru;
            m_PPPlugin->SetModuleParam(ps);
            m_PPPlugin->RunModule();
            QString strMsg;
            m_PPPlugin->GetExcResult(strMsg);
            m_PPPlugin->StopModule();
            m_debugInfo->setPlainText(strMsg);
        }
    }
    else if(text == "停止")
    {
        if(m_PPPlugin)
        {
            m_PPPlugin->isTerminate = true;
            m_debugInfo->setPlainText("停止执行P点.");
        }
    }
    else if(text == "手轮模式")
    {
        if(NULL == m_treeWidget->currentItem())
            return;
        qBtn->setText("退出手轮");
        m_runBtn->setEnabled(false);
        m_stopBtn->setEnabled(false);
        m_touchMode = TouchMode::handMode;
        TreeDoubleClicked(m_treeWidget->currentItem(),0);
        emit SendHandModeEvent(TouchMode::handMode);
    }
    else if(text == "退出手轮")
    {
        if(NULL == m_treeWidget->currentItem())
            return;
        qBtn->setText("手轮模式");
        m_runBtn->setEnabled(true);
        m_stopBtn->setEnabled(true);
        m_touchMode = TouchMode::ppMode;
        TreeDoubleClicked(m_treeWidget->currentItem(),0);
        emit SendHandModeEvent(TouchMode::ppMode);
    }
}

void TouchPP::UpdateTree(QTreeWidget *treeWidget)
{
    if(treeWidget == NULL)
        return;
    treeWidget->clear();
    for(auto item = DBInstanceManager::getDBInstance()->DBVarStru.qpMap.begin();
        item != DBInstanceManager::getDBInstance()->DBVarStru.qpMap.end();++item)
    {
        QStringList head;
        head << item.key();
        QMap<QString,QString> tempMap;
        if(!QString(item.value().axisidinfo).isEmpty())
        {
            QTreeWidgetItem *TreeItem = new QTreeWidgetItem(treeWidget,head);
            QString axisIds = "";
            QString axisPos = "";
            QStringList list = QString(item.value().axisidinfo).split(";");
            int len = list.size();
            int icount = 0;
            foreach (QString str, list)
            {
                if(str.isEmpty() || !str.contains(":"))
                    continue;
                icount++;
                QStringList list1 = str.split(":");
                double pos = -1.0;
                /***/
                QString regspeedstr = list1[1];
                QStringList reglist = regspeedstr.split(",");
                QString regname  = "";
                if(reglist.size() < 2)//位置 ,百分比 ,offset name
                {
                    continue;
                }
                regname = reglist[0];
                if(DBInstanceManager::getDBInstance()->DBVarStru.regnumMap.contains(reglist[0]))
                    pos = DBInstanceManager::getDBInstance()->DBVarStru.regnumMap[reglist[0]].Initvalue;
                tempMap[list1[0]] = reglist[0];
                axisIds += list1[0] ;
                axisPos += QString::number(pos);
                if(icount < len)
                {
                    axisIds += ",";
                    axisPos += ",";
                }
                if(len > 1)
                {
                    QStringList chlid;
                    chlid << "" << list1[0] << QString::number(pos);
                    QTreeWidgetItem *childItem = new QTreeWidgetItem(chlid);
                    TreeItem->addChild(childItem);
                }
            }
            TreeItem->setText(1,axisIds);
            TreeItem->setText(2,axisPos);
            TreeItem->setToolTip(1,axisIds);
            TreeItem->setToolTip(2,axisPos);
        }
        m_PPMap.insert(item.key(),tempMap);
    }
    QTreeWidgetItem *Item = new QTreeWidgetItem(treeWidget,QStringList());
    Item->setExpanded(true);
}
QWidget *TouchPP::CreateShowWidget(QWidget *parent)
{
    QWidget * pWidget = new QWidget(parent);
    m_pGrid = new QGridLayout();
    m_pGrid->setContentsMargins(1,1,1,1);
    m_pGrid->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_pGrid->setSizeConstraint(QLayout::SetFixedSize);
    pWidget->setLayout(m_pGrid);
    return pWidget;
}

CoProxyWidget *TouchPP::Copy()
{
    return NULL;
}

void TouchPP::Copy(TouchPP *From)
{
    Q_UNUSED(From);
}

void TouchPP::SetAttributeEvent(const int type, const QString value, uchar)
{
    switch (type)
    {
    case CoAttributeWidget::Power:
        SetPower(value.toInt());
        break;
    default:
        break;
    }
}

MyTree::MyTree(QWidget *parent):
    QTreeWidget(parent)
{

}

void MyTree::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    QTreeWidgetItem  *curItem =  this->itemAt(pos);
    if (curItem == NULL)
        return;
    emit ItemClicking(curItem,indexAt(event->pos()).column());
}

QMenu *MyTree::CreateContextMenu()
{
    QMenu *menu = new QMenu();
    menu->setFont(QFont("黑体",12));
    QAction *add = new QAction(QObject::tr("新建P点"));
    connect(add, &QAction::triggered, this, &MyTree::AddPP);
    QAction *del = new QAction(QObject::tr("删除P点"));
    connect(del, &QAction::triggered, this, &MyTree::DeletePP);
    QAction *upd = new QAction(QObject::tr("刷新P点"));
    connect(upd, &QAction::triggered, this, &MyTree::UpdatePP);
    QAction *save = new QAction(QObject::tr("保存PP"));
    connect(save, &QAction::triggered, this, &MyTree::SavePP);
    menu->addAction(add);
    menu->addAction(del);
    menu->addAction(upd);
    menu->addAction(save);
    return menu;
}
void MyTree::contextMenuEvent(QContextMenuEvent *event)
{
//    QPoint pos = event->pos();
//    QTreeWidgetItem  *curItem =  this->itemAt(pos);
//    if (curItem == NULL)
//    {
//        return;
//    }
    QMenu  *menu = CreateContextMenu();
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
    event->accept();
}
