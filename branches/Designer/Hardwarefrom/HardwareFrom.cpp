#include "HardwareFrom.h"
#include <QDebug>
#include <QGridLayout>
QMap<QString,HWCONFIGXML_IO_STRU> Hardwarefrom::IoMap;
QMap<QString,HWCONFIGXML_SEROUTPUT_STRU> Hardwarefrom::AxisMap;
Hardwarefrom::Hardwarefrom()
{
    qRegisterMetaType<HWCONFIGXML_COMPORT_STRU>("HWCONFIGXML_COMPORT_STRU");
    QString stylesheetstr = "QScrollArea {background-color:transparent;border: 0.5px solid white;border-radius:2px;margin-top:6px;}";
    mp_scrollarea = new QScrollArea();
    mp_scrollarea->hide();
    //   QHBoxLayout *hbox = new QHBoxLayout(this);
    //-----------------6.4--change layout-----------
    mp_scrollarea_arm= new QScrollArea(this);
    QGroupBox *gbox = new QGroupBox(this);
    gbox->setStyleSheet("QGroupBox"
                        "{"
                        "border: 1.5px solid white;"
                        "border-radius:8px;"
                        "margin-top:6px;"
                        "}"
                        "QGroupBox:title"
                        "{"
                        "color:black;"
                        "subcontrol-origin: margin;"
                        "left: 10px;"
                        "}");
    gbox->setTitle("视觉显示");
    gbox->setFixedWidth(400);
    gbox->setFont(QFont("黑体",12));

    QLabel *piclab = new QLabel(gbox);
    piclab->move(5,45);
    piclab->setScaledContents(true);
    QPixmap *pixmap = new QPixmap(":/images/robotvision.png");
    pixmap->scaled(piclab->size(),Qt::KeepAspectRatio);
    piclab->setPixmap(*pixmap);

    QGroupBox *gbox_set = new QGroupBox(this);
    gbox_set->setStyleSheet("QGroupBox"
                            "{"
                            "border: 1.5px solid white;"
                            "border-radius:8px;"
                            "margin-top:6px;"
                            "}"
                            "QGroupBox:title"
                            "{"
                            "color:black;"
                            "subcontrol-origin: margin;"
                            "left: 10px;"
                            "}");
    gbox_set->setFont(QFont("黑体",12));
    // gbox_set->setGeometry(450,510,450,150);
    gbox_set->setTitle("网口配置");
    QPushButton *but_set = new QPushButton(gbox_set);
    but_set->setText("-------点击设置-------");
    connect(but_set,&QPushButton::clicked,this,&Hardwarefrom::SaveNetName);
    but_set->move(10,45);
    but_set->setFixedSize(350,30);
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{background: rgb(100, 160, 220);}";

    but_set->setStyleSheet(butssl);
    QFont font_set(QFont("黑体",11));
    QLabel *netlabel = new QLabel(gbox_set);
    netlabel->setFont(font_set);
    netlabel->setText("网口名称：");
    netlabel->move(10,85);
    netlabel->setFixedSize(70,30);

    p_mNetName = new QLineEdit(gbox_set);
    p_mNetName->move(90,85);
    p_mSaveTips = new QLabel(gbox_set);
    p_mSaveTips->setFont(font_set);
    p_mSaveTips->setFixedSize(300,30);
    p_mSaveTips->move(10,120);
    p_mSaveTips->setText("-----");


    mp_scrollarea_arm->setWindowTitle("坐标信息");
    mp_scrollarea_arm->setStyleSheet(stylesheetstr);
    mp_scrollarea_arm->viewport()->setStyleSheet("QSpinBox{color:black;border-radius:1px;border: 0.1px solid white;}"
                                                 "QComboBox{color:black;border-radius:1px;border: 0.1px solid white;}"
                                                 "QLineEdit{color:black;border-radius:1px;border: 0.1px solid white;}");

    m_pscrollVbox = new QVBoxLayout(mp_scrollarea_arm);
    m_pscrollVbox->addStretch();
    mp_scrollarea_arm->setLayout(m_pscrollVbox);

    QPushButton *addbut = new QPushButton(this);
    // addbut->setGeometry(950,10,250,30);
    addbut->setText("添加坐标系轴配置");
    addbut->setFixedHeight(50);
    addbut->setStyleSheet(butssl);
    connect(addbut,&QPushButton::clicked,this,&Hardwarefrom::AddCoorUI);

    p_TreeView = new QTreeView(this);
    p_TreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(p_TreeView,&QTreeView::customContextMenuRequested,this,&Hardwarefrom::ShowContextMenu);
    p_TreeView->setFixedSize(300,800);
    QGridLayout *pgridlout = new QGridLayout(this);
    model = new QStandardItemModel(p_TreeView);
    // model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("硬件信息"));
    p_TreeView->setModel(model);
    // p_TreeView->setGeometry(100,10,300,this->height()-150);//p_topwidget->width()/2-600  TreeView
    p_TreeView->setFont(QFont("黑体",12));
    p_TreeView->header()->hide();
    p_TreeView->setEditTriggers(0);
    connect(p_TreeView,&QTreeView::clicked,this,&Hardwarefrom::treeview_click);
    p_TreeView->setStyleSheet("QTreeView{border: 1px solid lightgray;}"
                              "QTreeView::item {height: 40px;border-radius: 2px;"
                              "border: 1px solid transparent;background: transparent;color: black;}"
                              "QTreeView::item:has-children {border: none;border-bottom: 1px solid lightgray;}"
                              "QTreeView::item:hover {border: 1px solid rgb(170, 190, 230);"
                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(230, 240, 250),"
                              "stop: 0.5 rgb(220, 235, 255), stop: 1.0 rgb(210, 230, 255));}"
                              "QTreeView::item:selected {border: 1px solid rgb(170, 190, 230);"
                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(230, 240, 250),"
                              "stop: 0.5 rgb(220, 235, 255), stop: 1.0 rgb(210, 230, 255));}");
    p_tcp = new  QStandardItem ("TCP_IP");
    p_server = new QStandardItem("服务器");
    p_tcp->appendRow(p_server);
    p_cline = new QStandardItem("客户端");
    p_tcp->appendRow(p_cline);
    p_tcp->setChild(1,1,new QStandardItem());
    model->appendRow(p_tcp);
    p_sreial = new QStandardItem("串口");
    model->appendRow(p_sreial);
    p_other = new QStandardItem("硬件");
    model->appendRow(p_other);
    p_arm = new QStandardItem("轴臂信息");
    model->appendRow(p_arm);

    pgridlout->addWidget(p_TreeView,0,0,4,1);

    pgridlout->addWidget(gbox,0,1,3,1);
    pgridlout->addWidget(gbox_set,3,1,1,1);
    pgridlout->addWidget(addbut,0,2,1,1);
    pgridlout->addWidget(mp_scrollarea_arm,1,2,3,1);
    this->setLayout(pgridlout);
}

Hardwarefrom::~Hardwarefrom()
{

}

QMenu *Hardwarefrom::CreatMenu()
{
    QMenu * p_menu = new QMenu(this);
    p_menu->setFont(QFont("黑体",12));
    QAction *addaxis = new QAction(QObject::tr("新增轴"));
    connect(addaxis,&QAction::triggered,this,&Hardwarefrom::AddAxis);
    QAction *addio = new QAction(QObject::tr("新增IO"));
    connect(addio,&QAction::triggered,this,&Hardwarefrom::AddIo);
    QAction *del = new QAction(QObject::tr("...."));
    p_menu->addAction(addaxis);
    p_menu->addAction(addio);
    p_menu->addAction(del);
    return p_menu;
}

QMenu *Hardwarefrom::CreatMenu_subitem()
{
    QMenu *p_menu = new QMenu(this);
    p_menu->setFont(QFont("黑体",12));
    //QAction *insert = new QAction(QObject::tr("插入"));
    QMenu *p_inset = new QMenu(QObject::tr("插入"));
    QAction *p_axis = new QAction(QObject::tr("插入轴"));
    connect(p_axis,&QAction::triggered,this,&Hardwarefrom::IsertAxis);
    QAction *p_io = new QAction(QObject::tr("插入IO"));
    connect(p_io,&QAction::triggered,this,&Hardwarefrom::IsertIO);
    p_inset->addAction(p_axis);
    p_inset->addAction(p_io);
    p_menu->addMenu(p_inset);
    //connect(insert,&QAction::triggered,this,&MainWindow::addaxis);
    QAction *delet = new QAction(QObject::tr("删除"));
    connect(delet,&QAction::triggered,this,&Hardwarefrom::deleteAxis);
    QAction *othe = new QAction(QObject::tr("...."));
    //p_menu->addAction(insert);
    p_menu->addAction(delet);
    p_menu->addAction(othe);
    return p_menu;
}

void Hardwarefrom::ShowContextMenu(const QPoint point)
{
    QModelIndex index = p_TreeView->indexAt(point);
    QString text = index.data().toString();
    if(text == tr("硬件"))
    {
        QMenu *menu = CreatMenu();
        menu->exec(QCursor::pos());
    }
    else if(index.parent().data().toString() == tr("硬件") &&
            (text.contains("IO_") || text.contains("Server_")))
    {
        QMenu *menu_sub = CreatMenu_subitem();
        menu_sub->exec(QCursor::pos());
    }
}

void Hardwarefrom::treeview_click(const QModelIndex &index)
{
    //当前单击action的行号和名称
    m_treeviewstate.row = index.row();
    m_treeviewstate.atname = index.data().toString();
    click_state = Other;
    Key =index.data().toString();
    if(index.data().toString()== "服务器" || index.data().toString() == "客户端")   //TCP设置界面
    {
        AddTcpFrom *m_p_tcpfrom = new AddTcpFrom();
        connect(m_p_tcpfrom,&AddTcpFrom::tcpparametar,this,&Hardwarefrom::HandleAddtcp);
        mp_scrollarea->setWidget(m_p_tcpfrom);   //qscrollarea 会自动delete widget

    }
    else if(index.parent().data().toString()== "服务器" || index.parent().data().toString() == "客户端")//tcp attirbut
    {
        AddTcpFrom *m_p_tcpfrom = new AddTcpFrom();
        connect(m_p_tcpfrom,&AddTcpFrom::tcpparametar,this,&Hardwarefrom::HandleAddtcpFix);
        m_p_tcpfrom->SetTcpParameter(TcpMap[Key]);
        mp_scrollarea->setWidget(m_p_tcpfrom);
    }
    else if (index.data().toString()== "硬件")
    {
        click_state = Axis;

    }
    else if(index.parent().data().toString()=="硬件")
    {
        click_state = AxisIndex;
        if(index.data().toString().contains("IO_"))
        {
            AddIo_Tofrom(IoMap[Key]);
        }
        else if(index.data().toString().contains("Server_"))
        {
            AddAxisFrom *addaxis = new AddAxisFrom();
            addaxis->SetAxisParameter(AxisMap[Key]);
            connect(addaxis,&AddAxisFrom::Submit_axisinfo,this,&Hardwarefrom::HandleAddAxisFix);
            mp_scrollarea->setWidget(addaxis);
        }
    }
    else if (index.data().toString()=="串口")
    {
        AddSerialPort *m_p_SerialPort = new AddSerialPort();
        connect(m_p_SerialPort,&AddSerialPort::Conmit_serialport,this,&Hardwarefrom::HandleAddSerialPort);
        mp_scrollarea->setWidget(m_p_SerialPort);

    }
    else if(index.parent().data().toString()=="串口")
    {
        AddSerialPort *m_p_SerialPort = new AddSerialPort();
        m_p_SerialPort->SetSerialPortParameter(SerialMap[Key]);
        connect(m_p_SerialPort,&AddSerialPort::Conmit_serialport,this,&Hardwarefrom::HandleAddSerialPortFix);
        mp_scrollarea->setWidget(m_p_SerialPort);
        click_state = AxisIndex;
    }
    else if(index.data().toString()=="轴臂信息")
    {
        AddArm *p_armfrom = new AddArm();
        connect(p_armfrom,&AddArm::Submit_armInfo,this,&Hardwarefrom::handleAddcoordUI);
        mp_scrollarea->setWidget(p_armfrom);

    }
    else if (index.parent().data().toString()=="轴臂信息")
    {
        AddArm *p_armfrom = new AddArm();
        p_armfrom->SetArmParameter(ArmMap[Key]);
        connect(p_armfrom,&AddArm::Submit_armInfo,this,&Hardwarefrom::handleAddcoordUIFix);
        mp_scrollarea->setWidget(p_armfrom);
        click_state = AxisIndex;
    }
    else
    {
        click_state = Other;
    }

}

void Hardwarefrom::AddAxis()
{
    AxisIsert = false;
    AddAxisFrom *addaxis = new AddAxisFrom();
    connect(addaxis,&AddAxisFrom::Submit_axisinfo,this,&Hardwarefrom::HandleAddAxis);
    mp_scrollarea->setWidget(addaxis);
}
///
/// \brief HardwareFrom::HandleAddAxis  p_other add items show add axisinfo
/// \param axisinfo by attribute
/// \return ture save xml ok  else save fail
///
bool Hardwarefrom::HandleAddAxis(HWCONFIGXML_SEROUTPUT_STRU axisinfo)
{
    QString oldname = Key;
    oldname = oldname.remove("Server_").remove("IO_");
    QVector<QString>axisvec;
    axisvec.clear();
    axisvec.push_back(axisinfo.id);
    DBInstanceManager::getDBInstance()->AddWriteRealTimeDataToDB(axisvec);
    QStandardItem *standitem = new QStandardItem(QString("Server_%1").arg(axisinfo.id));
    standitem->setDragEnabled(true);
    standitem->setDropEnabled(true);
    AxisMap.insert(QString("Server_%1").arg(axisinfo.id),axisinfo);
    if(AxisIsert == true)
    {
        p_other->insertRow(m_treeviewstate.row + 1,standitem);
        saveaxisinfo.DeleteAxisAndIoOrder(Key,QString("Server_%1").arg(axisinfo.id),m_path);//排序中插入
        AxisIsert = false;
//        if(Key.contains("E"))//从Input节点下找
//        {
//            saveaxisinfo.InsertAxisXML(axisinfo,"INPUT",oldname,1,m_path);
//        }
//        else
//        {
//            saveaxisinfo.InsertAxisXML(axisinfo,"OUTPUT",oldname,1,m_path);                    //Output中插入
//        }
        //return saveaxisinfo.InsertAxisXML(axisinfo,"INPUT",oldname,1,m_path);
    }
    else
    {
        p_other->appendRow(standitem);
        saveaxisinfo.SaveAxisAndIoOrder(QString("Server_%1").arg(axisinfo.id),m_path);

    }
    return (SaveHardwareInfo());
}

bool Hardwarefrom::HandleAddAxisFix(HWCONFIGXML_SEROUTPUT_STRU axisinfo)
{
    QString oldname = Key;
    oldname = oldname.remove("Server_");
    DBInstanceManager::getDBInstance()->UpdateWriteRealTimeDataToDB(oldname,axisinfo.id);
    p_other->removeRow(m_treeviewstate.row);
    QStandardItem *standitem = new QStandardItem(QString("Server_%1").arg(axisinfo.id));
    p_other->insertRow(m_treeviewstate.row,standitem);
    AxisMap.insert(standitem->index().data().toString(),axisinfo);
    saveaxisinfo.FixAxisAndIoOrder(Key,axisinfo.id,m_path);
    //saveaxisinfo.InsertAxisXML(axisinfo,"INPUT",oldname,0,m_path);
   return (SaveHardwareInfo());// 0 插入删除
}

void Hardwarefrom::IsertAxis()
{
    AxisIsert = true;
    AddAxisFrom *addaxis = new AddAxisFrom();
    connect(addaxis,&AddAxisFrom::Submit_axisinfo,this,&Hardwarefrom::HandleAddAxis);
    mp_scrollarea->setWidget(addaxis);
}

void Hardwarefrom::deleteAxis()
{
    QString str = m_treeviewstate.atname;//.remove("Server_").remove("IO_");
    str = str.remove("Server_").remove("IO_");
    QVector<QString> DeleAxis;
    DeleAxis.clear();
    if(m_treeviewstate.atname.contains("Server_"))
    {
        DeleAxis.push_back(str);
        DBInstanceManager::getDBInstance()->DelWriteRealTimeDataToDB(DeleAxis);
        saveaxisinfo.DeleteAxisAndIoOrder(m_treeviewstate.atname,"",m_path);
        saveaxisinfo.DeleteInfoXML("OUTPUT","伺服部分",str,0,m_path);
        saveaxisinfo.DeleteInfoXML("INPUT","伺服部分",str,0,m_path);
        p_other->removeRow(m_treeviewstate.row);
    }
    else if(m_treeviewstate.atname.contains("IO_"))
    {
        saveaxisinfo.DeleteAxisAndIoOrder(m_treeviewstate.atname,"",m_path);
        if(m_treeviewstate.atname.contains("Q"))
        {
             saveaxisinfo.DeleteInfoXML("OUTPUT","IO部分",str,0,m_path);
        }
        else if (m_treeviewstate.atname.contains("E"))
        {
            saveaxisinfo.DeleteInfoXML("INPUT","IO部分",str,0,m_path);
        }
        p_other->removeRow(m_treeviewstate.row);
        if(IoMap.contains(m_treeviewstate.atname))
        {
            DBInstanceManager::getDBInstance()->DelWriteIOVarToDB(str,IoMap[m_treeviewstate.atname].iByteNum);
            IoMap.remove(m_treeviewstate.atname);
        }
    }
    else if(m_treeviewstate.atname.contains("tty"))//bug
    {
        str = m_treeviewstate.atname.split(":")[0];
        saveaxisinfo.DeleteInfoXML("串口配置","串口编号",str,1,m_path);
        p_sreial->removeRow(m_treeviewstate.row);
    }
    else if(m_treeviewstate.atname.contains("轴臂"))
    {
        str = m_treeviewstate.atname.mid(3);
        saveaxisinfo.DeleteInfoXML("关节配置","关节编号",str,1,m_path);
        p_arm->removeRow(m_treeviewstate.row);
    }
}

bool Hardwarefrom::HandleAddSerialPort(HWCONFIGXML_COMPORT_STRU serialinfo)
{
    QStandardItem *standitem = new QStandardItem(QString("%1:%2").arg(serialinfo.Id).arg(serialinfo.portName));
    p_sreial->appendRow(standitem);
    SerialMap.insert(standitem->index().data().toString(),serialinfo);
    //保存xml
    return (saveaxisinfo.SaveSerialXML(serialinfo,m_path));

}

bool Hardwarefrom::HandleAddSerialPortFix(HWCONFIGXML_COMPORT_STRU serialinfo)
{
    SerialMap.insert(Key,serialinfo);
    return (saveaxisinfo.SaveSerialXML(serialinfo,m_path));
}

void Hardwarefrom::AddIo()
{
    IoIsert = false;
    AddIoFrom *addio = new AddIoFrom();
    connect(addio,SIGNAL(Submit(HWCONFIGXML_IO_STRU)),this,SLOT(HandleIo(HWCONFIGXML_IO_STRU)));
    mp_scrollarea->setWidget(addio);

}

void Hardwarefrom::AddIo_Tofrom(HWCONFIGXML_IO_STRU IoParamter)
{
    AddIoFrom *addio = new AddIoFrom();
    addio->SetIoParameter(IoParamter);
    connect(addio,SIGNAL(Modify(HWCONFIGXML_IO_STRU,HWCONFIGXML_IO_STRU)),this,SLOT(HandleIo(HWCONFIGXML_IO_STRU,HWCONFIGXML_IO_STRU)));
    mp_scrollarea->setWidget(addio);
}

bool Hardwarefrom::HandleIo(HWCONFIGXML_IO_STRU ioinfo)
{
    QStandardItem *standitem = new QStandardItem(QString("IO_%1").arg(ioinfo.id));
    IoMap.insert(QString("IO_%1").arg(ioinfo.id),ioinfo);//保存修改或新建的数据

    if(IoIsert == false)
    {
       p_other->appendRow(standitem);
       //保存xml
       saveaxisinfo.SaveAxisAndIoOrder(standitem->index().data().toString(),m_path);
    }
    else//Isert
    {
        p_other->insertRow(m_treeviewstate.row + 1,standitem);
        saveaxisinfo.DeleteAxisAndIoOrder(Key,standitem->index().data().toString(),m_path);//排序中插入
        IoIsert = false;

    }
    //数据库新增IO变量
    DBInstanceManager::getDBInstance()->AddWriteIOVarToDB(ioinfo.id,ioinfo.iByteNum);

    return(SaveHardwareInfo());
}

bool Hardwarefrom::HandleIo(HWCONFIGXML_IO_STRU pre, HWCONFIGXML_IO_STRU now)
{
    for(int i = 0; i < p_other->rowCount();i++)
    {
        QStandardItem *item = p_other->child(i);
        if(item->data(Qt::DisplayRole).toString() == QString("IO_%1").arg(pre.id))
        {
            item->setData(QVariant(QString("IO_%1").arg(now.id)),Qt::DisplayRole);
            saveaxisinfo.SaveAxisAndIoOrder(item->data().toString(),m_path);
            //数据库修改IO变量
            DBInstanceManager::getDBInstance()->UpdateWriteIOVarToDB(pre.id,pre.iByteNum,now.id,now.iByteNum);
            break;
        }
    }
    return(SaveHardwareInfo());
}

bool Hardwarefrom::HandleIoFix(HWCONFIGXML_IO_STRU ioinfo)
{
    p_other->removeRow(m_treeviewstate.row);
    QStandardItem *IO_itms = new QStandardItem(QString("IO_%1").arg(ioinfo.id));
    p_other->insertRow(m_treeviewstate.row,IO_itms);
    IoMap.insert(IO_itms->index().data().toString(),ioinfo);//保存修改或新建的数据
    //保存xml
    saveaxisinfo.FixAxisAndIoOrder(Key,ioinfo.id,m_path);
    return (SaveHardwareInfo());
}

void Hardwarefrom::IsertIO()
{
    IoIsert = true;
    AddIoFrom *addio = new AddIoFrom();
    connect(addio,SIGNAL(Submit(HWCONFIGXML_IO_STRU)),this,SLOT(HandleIo(HWCONFIGXML_IO_STRU)));
    mp_scrollarea->setWidget(addio);
}

bool Hardwarefrom::HandleAddtcp(HWCONFIGXML_INTNET_STRU TcpIp)
{
    QStandardItem *standitem =NULL;
    if(TcpIp.serType =="客户端")
    {
        standitem = new QStandardItem(QString("%1:%2").arg(TcpIp.ipAdd).arg(TcpIp.portName));
        p_cline->appendRow(standitem);
    }
    else
    {
        standitem = new QStandardItem(QString("%1:%2").arg(getHostIpAddress()).arg(TcpIp.portName));
        p_server->appendRow(standitem);
    }
    TcpMap.insert(standitem->index().data().toString(),TcpIp);
    //保存xml

    return (saveaxisinfo.SaveTcpXML(TcpIp,m_path));
}

bool Hardwarefrom::HandleAddtcpFix(HWCONFIGXML_INTNET_STRU TcpIp)
{
    QStandardItem *standitem =NULL;
    if(TcpIp.serType =="客户端")
    {
        p_cline->removeRow(m_treeviewstate.row);
        standitem = new QStandardItem(QString("%1:%2").arg(TcpIp.ipAdd).arg(TcpIp.portName));
        p_cline->appendRow(standitem);
    }
    else
    {
        p_server->removeRow(m_treeviewstate.row);
        standitem = new QStandardItem(QString("%1:%2").arg(getHostIpAddress()).arg(TcpIp.portName));
        p_server->appendRow(standitem);
    }
    TcpMap.insert(standitem->index().data().toString(),TcpIp);
    //保存xml

    return (saveaxisinfo.SaveTcpXML(TcpIp,m_path));
}

bool Hardwarefrom::handleAddcoordUI(HWCONFIGXML_ARMINFO_STRU arm)
{
    QStandardItem *Arm_itms = new QStandardItem(QString("轴臂:%1").arg(arm.Id));
    p_arm->appendRow(Arm_itms);
    ArmMap.insert(Arm_itms->index().data().toString(),arm);
    return (saveaxisinfo.SaveAirXML(arm,m_path));
}

bool Hardwarefrom::handleAddcoordUIFix(HWCONFIGXML_ARMINFO_STRU arm)
{
    p_arm->removeRow(m_treeviewstate.row);
    QStandardItem *Arm_itms = new QStandardItem(QString("轴臂:%1").arg(arm.Id));
    p_arm->appendRow(Arm_itms);
    ArmMap.insert(Arm_itms->index().data().toString(),arm);
    return(saveaxisinfo.SaveAirXML(arm,m_path));
}
///
/// \brief Hardwarefrom::AddCoorUI 添加坐标系配置
///
void Hardwarefrom::AddCoorUI()
{
    AddCoordinate *p_coorUI = new AddCoordinate();
    connect(p_coorUI,&AddCoordinate::Submit,this,&Hardwarefrom::AcoorInUI);
    mp_scrollarea->setWidget(p_coorUI);
}

void Hardwarefrom::AcoorInUI(COORDINATESTUR CoorInfo)
{
    AddCoordinateUI *CoorUi = new AddCoordinateUI(CoorInfo.coordname,
                                                  CoorInfo.limtstart,CoorInfo.limtend,
                                                  CoorInfo.contain);
    //connect for ui double click
    connect(CoorUi,&AddCoordinateUI::SendCoorTile,this,&Hardwarefrom::FixCoordinate);
    CoorMap.insert(CoorInfo.coordname,CoorInfo);
    int count = m_pscrollVbox->count();
    m_pscrollVbox->insertWidget(--count,CoorUi);

    saveaxisinfo.SaveCoordinateXML(CoorInfo,m_path,ArmMap);
}
///
/// \brief Hardwarefrom::FixCoordinate
/// \param CoorName
/// remove coordinateUI when you create new coordinateUI xml while overwrite the coordiamteInfo,but dont delete
void Hardwarefrom::FixCoordinate(QString CoorName)
{
    AddCoordinate *p_coorUI = new AddCoordinate();
    p_coorUI->SetParameter(CoorMap[CoorName]) ;
    connect(p_coorUI,&AddCoordinate::Submit,this,&Hardwarefrom::AcoorInUI);
    mp_scrollarea->setWidget(p_coorUI);
}

bool Hardwarefrom::SaveHardwareInfo()
{
    if(!saveaxisinfo.DeleteAllInputOutput(m_path))
    {
        return false;
    }
    QVector<QString>Hardware;//获取硬件排序
    Hardware = saveaxisinfo.GetAxisAndIoOder(m_path);
    //按照排列顺序依次保存xml
    foreach (QString str, Hardware) {
        if(str.contains("Server")){
            if(AxisMap.contains(str))
            {
                saveaxisinfo.SaveInfoXML(AxisMap[str],m_path);
            }
        }
        else
        {
            if(IoMap.contains(str))
            {
                saveaxisinfo.SaveIoXML(IoMap[str],m_path);
            }
        }
    }
    return true;
}

void Hardwarefrom::SaveNetName()
{
    if(saveaxisinfo.SaveNetInfo(p_mNetName->text(),m_path))
    {
        p_mSaveTips->setText(QString("网口名称：%1 更改成功！").arg(p_mNetName->text()));
    }
    else
        p_mSaveTips->setText(tr("网口名称设置失败！"));
}

bool Hardwarefrom::LoadHwXml(QString path)
{
    XmlManager XmlRead;
    HWCONFIGXML_MAP hwConfig;
    CoorVec.clear();
    XmlRead.ReadHWConfigXml(path,hwConfig);
    for(auto it = hwConfig.hwConfigXmlOutputMap.hwIOMap.begin();it != hwConfig.hwConfigXmlOutputMap.hwIOMap.end();it++)
    {
        IoMap.insert(QString("IO_%1").arg(it.value().id),it.value());
    }
    for(auto iot = hwConfig.hwConfigXmlInputMap.hwIOMap.begin();iot != hwConfig.hwConfigXmlInputMap.hwIOMap.end();iot++)
    {
        IoMap.insert(QString("IO_%1").arg(iot.value().id),iot.value());
    }
    QVector<QString>DataBaseAxisVec;
    DataBaseAxisVec.clear();
    for(auto ito = hwConfig.hwConfigXmlOutputMap.hwSerOutputMap.begin();ito != hwConfig.hwConfigXmlOutputMap.hwSerOutputMap.end();ito++)
    {
        AxisMap.insert(QString("Server_%1").arg(ito.value().id),ito.value());
        DataBaseAxisVec.push_back(ito.value().id);
    }
    DBInstanceManager::getDBInstance()->AddWriteRealTimeDataToDB(DataBaseAxisVec);//-------Load info -------
    //硬件排序显示
    QVector<QString>Hardware;
    Hardware = saveaxisinfo.GetAxisAndIoOder(path);
    foreach (QString str, Hardware) {
        if(str.contains("Server")){
                    QStandardItem *standitem = new QStandardItem(str);
                    p_other->appendRow(standitem);
        }
        else
        {
                    QStandardItem *standitem = new QStandardItem(str);
                    p_other->appendRow(standitem);
        }
    }
    for(auto ico = hwConfig.hwConfigXmlIntnetMap.begin();ico != hwConfig.hwConfigXmlIntnetMap.end();ico++)
    {
        if(ico.value().serType =="客户端")
        {
            TcpMap.insert(QString("%1:%2").arg(ico.value().ipAdd).arg(ico.value().portName),ico.value());
            QStandardItem* standitem = new QStandardItem(QString("%1:%2").arg(ico.value().ipAdd).arg(ico.value().portName));
            p_cline->appendRow(standitem);
        }
        else
        {
            TcpMap.insert(QString("%1:%2").arg(getHostIpAddress()).arg(ico.value().portName),ico.value());
            QStandardItem* standitem = new QStandardItem(QString("%1:%2").arg(getHostIpAddress()).arg(ico.value().portName));
            p_server->appendRow(standitem);
        }
    }
    for(auto iro = hwConfig.hwConfigXmlComportMap.begin();iro != hwConfig.hwConfigXmlComportMap.end();iro ++)
    {
        SerialMap.insert(QString("%1:%2").arg(iro.value().Id).arg(iro.value().portName),iro.value());
        QStandardItem *standitem = new QStandardItem(QString("%1:%2").arg(iro.value().Id).arg(iro.value().portName));
        p_sreial->appendRow(standitem);
    }
    for(auto irt = hwConfig.hwConfigXmlCoordinateMap.begin();irt != hwConfig.hwConfigXmlCoordinateMap.end();irt++)
    {
        for (auto cro = irt.value().hwArmInfoMap.begin();cro != irt.value().hwArmInfoMap.end();cro ++)
        {
            ArmMap.insert(QString("轴臂:%1").arg(cro.value().Id),cro.value());

        }
    }
    for(auto cor = hwConfig.hwConfigXmlCoordinateMap.begin();cor != hwConfig.hwConfigXmlCoordinateMap.end();cor ++)
    {
            COORDINATESTUR CorStur;
            CorStur.coordname = cor.key();
            CorStur.limtstart = cor.value().stalim;
            CorStur.limtend = cor.value().endlim;
            for(auto arm = cor.value().hwArmInfoMap.begin();arm != cor.value().hwArmInfoMap.end();arm ++)
            {
              CorStur.contain += arm.value().Id;
            }
            CoorVec.push_back(CorStur);
    }
    foreach (COORDINATESTUR crstur, CoorVec) {
        AddCoordinateUI *CoorUi = new AddCoordinateUI(crstur.coordname,
                                                      crstur.limtstart,crstur.limtend,
                                                      crstur.contain);
        //connect for ui double click
        connect(CoorUi,&AddCoordinateUI::SendCoorTile,this,&Hardwarefrom::FixCoordinate);
        CoorMap.insert(crstur.coordname,crstur);
        int count = m_pscrollVbox->count();
        m_pscrollVbox->insertWidget(--count,CoorUi);
    }
    for(auto ar = ArmMap.begin(); ar != ArmMap.end();ar++)
    {
        QStandardItem *Arm_itms = new QStandardItem(QString("轴臂:%1").arg(ar.value().Id));
        p_arm->appendRow(Arm_itms);
    }
     p_mNetName->setText(hwConfig.hwConfigXmlNetworkStru.portName);
    return true;
}


QString Hardwarefrom::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            strIpAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果没有找到，则以本地IP地址为IP
    if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return strIpAddress;
}

QMap<QString,int> Hardwarefrom::GetIoInfo()
{
    QMap<QString,int>IoVec;
    for(auto it = IoMap.begin();it != IoMap.end();it++)
    {
       IoVec.insert(it.key().mid(3),it.value().iByteNum);
    }
    return IoVec;
}

QVector<QString> Hardwarefrom::GetAxisInfo()
{
    QVector<QString>AxisVec;
    for(auto it = AxisMap.begin();it != AxisMap.end();it++)
    {
       AxisVec.push_back(it.key().mid(7));
    }
    return AxisVec;
}

void Hardwarefrom::CreatXmlRoot()
{
    saveaxisinfo.CreateXmlRoot(m_path);
}


