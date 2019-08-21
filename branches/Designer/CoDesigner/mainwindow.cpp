#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QList>
#include <QFileDialog>
#include "Treewidget.h"
#include "Dialog/NewScene/DialogNewScene.h"
#include "Dialog/NewProject/DialogNewProject.h"
#include <CoGraphics/Undo/UndoAddDel.h>
#include <CoGraphics/Undo/UndoMoveDrag.h>
#include <CoWidgetBox/WidgetBox.h>
#include <CoGraphicsItems/AttributeFactory.h>
#include <DycLibBase/LogManager/logmanager.h>
#include <MyComm_Shared.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("COFEI设计程序"));
    m_pThread = NULL;
    InitVariable();
    CreateActions();
    InitStatusBars();
    InitMenus();
    InitToolBars();
    QMap<QString,CoreLog::QLogger *> logMap = CoreLog::QLoggerManager::getInstance()->getLogInstanceMap();
    for(auto it = logMap.begin();it != logMap.end();++it )
    {
        connect(it.value(),&CoreLog::QLogger::LogUpdateUI,this,&MainWindow::ShowLog);
    }
    m_GridLayout = new QGridLayout(ui->centralWidget);

    m_TaskWidget = CreateTaskArea(ui->centralWidget);
    m_GridLayout->addWidget(m_TaskWidget,0,0);

    QWidget *ShowWidget = CreateShowArea(ui->centralWidget);
    m_GridLayout->addWidget(ShowWidget,0,1);

    m_AttrWidget = CreateAttributeArea(ui->centralWidget);
    m_GridLayout->addWidget(m_AttrWidget,0,2);
    m_TaskBar = CreateTaskBar(this);
    m_pTimer = CreateTimer(this);

    TItemFactory::SetItemFactory(new CoGraphicsItems());
    saveflowscene = new ItemSave;
}
void MainWindow::ShowLog(QString strmsg)
{
    if(strmsg.contains("[Error]"))
    {
        m_Note->setText(strmsg);
    }
    else
    {
        if(strmsg.contains("Processdata cycle start ......"))
        {
            BaseAxisOperate::SetAllAxisReset("Reset");
            foreach (PROJECT_STRU proStru, m_SceneAttrMap.values())
            {
                proStru.SetDebugMode(true);
            }
            m_Note->setText(strmsg);
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
void MainWindow::InitVariable()
{
    m_SceneId = 0;
    m_ItemId = 0;
    m_SceneHost = 2;
    m_zMin = 0;
    m_zMax = 0;
    m_isHideAttrWidget = false;
    m_isHideTaskWidget = false;
    m_pDrawSelected = NULL;
    m_pCurScene = NULL;
    m_IsSaveXml = true;
}

void MainWindow::CreateActions()
{
    m_ActFileNew = AddFileAction(tr("新建项目(&N)..."));
    m_ActFileNew->setShortcut(QKeySequence::New);
    connect(m_ActFileNew, SIGNAL(triggered()), this, SLOT(CreateNewProject()));

    m_ActFileOpen = AddFileAction(tr("打开工程(&O)"));
    m_ActFileOpen->setShortcut(QKeySequence::Open);
    connect(m_ActFileOpen, SIGNAL(triggered()), this, SLOT(OpenProject()));
    m_ActFileSave = AddFileAction(tr("保存工程(&S)"));
    m_ActFileSave->setShortcut(QKeySequence::Save);
    connect(m_ActFileSave, SIGNAL(triggered()), this, SLOT(SaveXml()));
    m_ActFileSaveAs = AddFileAction(tr("工程另存(&F)"));
    m_ActFileSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(m_ActFileSaveAs, SIGNAL(triggered()), this, SLOT(SaveXmlAs()));
    m_ActFileClose = AddFileAction(tr("关闭程序"));
    connect(m_ActFileClose, SIGNAL(triggered()), this, SLOT(close()));
    /////////////////////////////////////////////////
    m_ActEditAll = AddFileAction(tr("全选(&A)"));
    m_ActEditAll->setShortcut(QKeySequence::SelectAll);
    connect(m_ActEditAll, SIGNAL(triggered()), this, SLOT(SelectAllItems()));
    m_ActEditCut = AddEditAction(tr("剪切(&X)"), tr(":/images/edit/Cut.png"), tr(""));
    m_ActEditCut->setShortcut(QKeySequence::Cut);
    connect(m_ActEditCut, SIGNAL(triggered()), this, SLOT(CutItem()));
    m_ActEditCopy = AddEditAction(tr("复制(&C)"), tr(":/images/edit/Copy.png"), tr(""));
    m_ActEditCopy->setShortcut(QKeySequence::Copy);
    connect(m_ActEditCopy, SIGNAL(triggered()), this, SLOT(CopyItem()));
    m_ActEditPaste = AddEditAction(tr("粘贴(&P)"), tr(":/images/edit/Paste.png"), tr(""));
    m_ActEditPaste->setShortcut(QKeySequence::Paste);
    connect(m_ActEditPaste, SIGNAL(triggered()), this, SLOT(PasteItem()));

    m_UndoStack = new QUndoStack(this);
    m_ActUndo = m_UndoStack->createUndoAction(this, tr("撤销(&U)"));
    m_ActUndo->setIcon(QIcon(tr(":/images/edit/Undo.png")));
    m_ActUndo->setShortcut(QKeySequence::Undo);
    m_ActRedo = m_UndoStack->createRedoAction(this, tr("重做(&R)"));
    m_ActRedo->setIcon(QIcon(tr(":/images/edit/Redo.png")));
    m_ActRedo->setShortcut(QKeySequence::Redo);

    m_ActEditDel = AddEditAction(tr("删除(&D)"), tr(":/images/edit/Del.png"), tr(""));
    m_ActEditDel->setShortcut(QKeySequence::Delete);
    connect(m_ActEditDel, SIGNAL(triggered()), this, SLOT(DeleteItem()));

    /////////////////////////////////////////////
    m_ActGraphMerge = AddEditAction(tr("合并(&M)"), tr(":/images/graph/Merge.png"), tr(""));
    connect(m_ActGraphMerge, SIGNAL(triggered()), this, SLOT(MergeItems()));
    m_ActGraphSplit = AddEditAction(tr("分裂(&S)"), tr(":/images/graph/Merge.png"), tr(""));
    connect(m_ActGraphSplit, SIGNAL(triggered()), this, SLOT(SplitItems()));
    m_ActAlignLeft = AddEditAction(tr("左对齐(&L)"), tr(":/images/graph/AlignLeft.png"), tr(""));
    connect(m_ActAlignLeft, SIGNAL(triggered()), this, SLOT(AlignLeft()));
    m_ActAlignRight = AddEditAction(tr("右对齐(&R)"), tr(":/images/graph/AlignRight.png"), tr(""));
    connect(m_ActAlignRight, SIGNAL(triggered()), this, SLOT(AlignRight()));
    m_ActAlignTop = AddEditAction(tr("顶对齐(&T)"), tr(":/images/graph/AlignTop.png"), tr(""));
    connect(m_ActAlignTop, SIGNAL(triggered()), this, SLOT(AlignTop()));
    m_ActAlignBottom = AddEditAction(tr("底对齐(&B)"), tr(":/images/graph/AlignBottom.png"), tr(""));
    connect(m_ActAlignBottom, SIGNAL(triggered()), this, SLOT(AlignBottom()));
    m_ActEditHWEqual = AddEditAction(tr("等大"), tr(":/images/graph/size.png"), tr(""));
    connect(m_ActEditHWEqual, SIGNAL(triggered()), this, SLOT(AlignHWEqual()));
    m_ActEditHEqual = AddEditAction(tr("等高(&H)"), tr(":/images/graph/hight.png"), tr(""));
    connect(m_ActEditHEqual, SIGNAL(triggered()), this, SLOT(AlignHEqual()));
    m_ActEditWEqual = AddEditAction(tr("等宽(&W)"), tr(":/images/graph/width.png"), tr(""));
    connect(m_ActEditWEqual, SIGNAL(triggered()), this, SLOT(AlignWEqual()));
    m_ActMoveUp = AddEditAction(tr("上移"), tr(":/images/graph/Up.png"), tr(""));
    m_ActMoveUp->setShortcut(QKeySequence::MoveToPreviousLine);
    connect(m_ActMoveUp, SIGNAL(triggered()), this, SLOT(Up()));
    m_ActMoveDown = AddEditAction(tr("下移"), tr(":/images/graph/Down.png"), tr(""));
    m_ActMoveDown->setShortcut(QKeySequence::MoveToNextLine);
    connect(m_ActMoveDown, SIGNAL(triggered()), this, SLOT(Down()));
    m_ActMoveLeft = AddEditAction(tr("左移"), tr(":/images/graph/Left.png"), tr(""));
    m_ActMoveLeft->setShortcut(QKeySequence::MoveToPreviousChar);
    connect(m_ActMoveLeft, SIGNAL(triggered()), this, SLOT(Left()));
    m_ActMoveRight = AddEditAction(tr("右移"), tr(":/images/graph/Right.png"), tr(""));
    m_ActMoveRight->setShortcut(QKeySequence::MoveToNextChar);
    connect(m_ActMoveRight, SIGNAL(triggered()), this, SLOT(Right()));

    /////////////////////////////////////////////////////////////
    m_ActZoomOut = AddFileAction(tr("放大"));
    connect(m_ActZoomOut, SIGNAL(triggered()), this, SLOT(ZoomOut()));
    m_ActZoomIn = AddFileAction(tr("缩小"));
    connect(m_ActZoomIn, SIGNAL(triggered()), this, SLOT(ZoomIn()));
    m_ActZoomNormal = AddFileAction(tr("正常"));
    connect(m_ActZoomNormal, SIGNAL(triggered()), this, SLOT(ZoomNormal()));
    m_ActGrid = AddFileAction(tr("网格线"));
    m_ActGrid->setCheckable(true);
    m_ActGrid->setChecked(false);
    connect(m_ActGrid, SIGNAL(triggered()), this, SLOT(SetGridLine()));
    m_ActDebug = AddFileAction(tr("调试模式"));
    m_ActDebug->setCheckable(true);
    m_ActDebug->setChecked(false);
    connect(m_ActDebug, SIGNAL(triggered()), this, SLOT(SetDebugMode()));
    m_ActSetHost = AddFileAction(tr("设为主画面(&H)"));
    connect(m_ActSetHost, SIGNAL(triggered()), this, SLOT(SetHost()));
    ///////////////////////////////////////////////////////
    m_ActAbout = AddFileAction(tr("关于(&A)"));
    //////////////////////////////////////////////////////
    m_ActEditHWEqual->setEnabled(false);
    m_ActEditHEqual->setEnabled(false);
    m_ActEditWEqual->setEnabled(false);
    m_ActAlignTop->setEnabled(false);
    m_ActAlignLeft->setEnabled(false);
    m_ActAlignRight->setEnabled(false);
    m_ActAlignBottom->setEnabled(false);
}

CoAction *MainWindow::AddFileAction(QString _Name)
{
    CoAction *Action = new CoAction(_Name, this);
    Action->setFont(QFont("黑体",11));
    return Action;
}

CoAction *MainWindow::AddEditAction(QString _Name, QString _IconDir, QString _ShutCut)
{
    CoAction *Action = new CoAction(_Name, this);
    Action->setIcon(QIcon(_IconDir));
    Action->m_Type = CoItem::Base;
    Action->setFont(QFont("黑体",11));
    if(_ShutCut.length() > 0)
    {
        Action->setShortcut(_ShutCut);
    }

    return Action;
}

void MainWindow::InitMenus()
{
    ui->menuBar->setFont(QFont("黑体",12));
    QMenu* mFileMenu = ui->menuBar->addMenu(tr("文件(&F)"));
    mFileMenu->addAction(m_ActFileNew);
    mFileMenu->addSeparator();
    mFileMenu->addAction(m_ActFileOpen);
    mFileMenu->addAction(m_ActFileSave);
    mFileMenu->addAction(m_ActFileSaveAs);
    mFileMenu->addSeparator();
    mFileMenu->addAction(m_ActFileClose);

    QMenu* mEditMenu = ui->menuBar->addMenu(tr("编辑(&E)"));
    mEditMenu->addAction(m_ActEditAll);
    mEditMenu->addSeparator();
    mEditMenu->addAction(m_ActEditCut);
    mEditMenu->addAction(m_ActEditCopy);
    mEditMenu->addAction(m_ActEditPaste);
    mEditMenu->addAction(m_ActEditDel);
    mEditMenu->addSeparator();
    mEditMenu->addAction(m_ActUndo);
    mEditMenu->addAction(m_ActRedo);

    QMenu* mGraphicsMenu = ui->menuBar->addMenu(tr("图形(&G)"));
    mGraphicsMenu->addAction(m_ActGraphMerge);
    mGraphicsMenu->addAction(m_ActGraphSplit);
    mGraphicsMenu->addSeparator();
    mGraphicsMenu->addAction(m_ActAlignLeft);
    mGraphicsMenu->addAction(m_ActAlignRight);
    mGraphicsMenu->addAction(m_ActAlignTop);
    mGraphicsMenu->addAction(m_ActAlignBottom);
    mGraphicsMenu->addSeparator();
    mGraphicsMenu->addAction(m_ActEditHEqual);
    mGraphicsMenu->addAction(m_ActEditWEqual);
    mGraphicsMenu->addAction(m_ActEditHWEqual);
    mGraphicsMenu->addSeparator();
    mGraphicsMenu->addAction(m_ActMoveUp);
    mGraphicsMenu->addAction(m_ActMoveDown);
    mGraphicsMenu->addAction(m_ActMoveLeft);
    mGraphicsMenu->addAction(m_ActMoveRight);

    QMenu* mWindowMenu = ui->menuBar->addMenu(tr("窗口(&U)"));
    mWindowMenu->addAction(m_ActZoomOut);
    mWindowMenu->addAction(m_ActZoomIn);
    mWindowMenu->addAction(m_ActZoomNormal);
    mWindowMenu->addSeparator();
    mWindowMenu->addAction(m_ActGrid);
    mWindowMenu->addSeparator();
    mWindowMenu->addAction(m_ActDebug);
    mWindowMenu->addAction(m_ActSetHost);
    mWindowMenu->addSeparator();

    QMenu* mAboutMenu = ui->menuBar->addMenu(tr("关于(&H)"));
    mAboutMenu->addAction(m_ActAbout);
}

void MainWindow::InitToolBars()
{
    ui->mainToolBar->addAction(m_ActFileNew);
    ui->mainToolBar->addAction(m_ActFileOpen);
    ui->mainToolBar->addAction(m_ActFileSave);
    ui->mainToolBar->addAction(m_ActFileSaveAs);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(m_ActAlignTop);
    ui->mainToolBar->addAction(m_ActAlignLeft);
    ui->mainToolBar->addAction(m_ActAlignRight);
    ui->mainToolBar->addAction(m_ActAlignBottom);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(m_ActEditHEqual);
    ui->mainToolBar->addAction(m_ActEditWEqual);
    ui->mainToolBar->addAction(m_ActEditHWEqual);
    ui->mainToolBar->addSeparator();
}

void MainWindow::InitStatusBars()
{
    QLabel* mLabelLineStyleShow = TBase::Label(tr("      样式"), tr("mLabelLineStyleShow"));
    QComboBox* mComboBoxLineStyle = TBase::LineStyleComboBox();
    mComboBoxLineStyle->setCurrentIndex(1);
    m_LineStyle = Qt::SolidLine;
    connect(mComboBoxLineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(LineStyleChanged(int)));

    QLabel* mLabelLineWidthShow = TBase::Label(tr("宽度"), tr("mLabelLineWidthShow"));
    m_LabelLineWidth     = TBase::Label(tr(""), tr("mLabelLineWidth"), 50, 22);

    QSpinBox* mSpinBoxLineWidth   = TBase::SpinBox(tr("mSpinBoxLineWidth"), 1, 100);
    m_LineWidth = 1;
    LineWidthChanged(m_LineWidth);
    connect(mSpinBoxLineWidth, SIGNAL(valueChanged(int)), this, SLOT(LineWidthChanged(int)));

    QLabel* mLabelLineColorShow = TBase::Label(tr("线条"), tr("mLabelLineColorShow"));
    m_LabelLineColor     = TBase::Label(tr(""), tr("mLabelLineColor"), 50, 22);

    m_LineColor = Qt::white;
    TBase::ColorLabel(m_LabelLineColor, m_LineColor);
    QPushButton* mButtonLineColor    = TBase::Button(tr("浏览"), tr("mButtonLineColor"));
    connect(mButtonLineColor, SIGNAL(clicked()), this, SLOT(UpdateLineColor()));

    QLabel* mLabelBackColorShow = TBase::Label(tr("背景"), tr("mLabelBackColorShow"));
    m_LabelBackColor     = TBase::Label(tr(""), tr("mLabelBackColor"), 50, 22);

    m_BackColor = Qt::black;
    TBase::ColorLabel(m_LabelBackColor, m_BackColor);
    QPushButton* mButtonBackColor    = TBase::Button(tr("浏览"), tr("mButtonLineColor"));
    connect(mButtonBackColor, SIGNAL(clicked()), this, SLOT(UpdateBackColor()));

    m_Note = TBase::Label(tr("Ready."), tr("mNote"),200,22);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::darkGreen);
    m_Note->setPalette(pa);
    ui->statusBar->addWidget(mLabelLineStyleShow);
    ui->statusBar->addWidget(mComboBoxLineStyle);
    ui->statusBar->addWidget(mLabelLineWidthShow);
    ui->statusBar->addWidget(m_LabelLineWidth);
    ui->statusBar->addWidget(mSpinBoxLineWidth);
    ui->statusBar->addWidget(mLabelLineColorShow);
    ui->statusBar->addWidget(m_LabelLineColor);
    ui->statusBar->addWidget(mButtonLineColor);
    ui->statusBar->addWidget(mLabelBackColorShow);
    ui->statusBar->addWidget(m_LabelBackColor);
    ui->statusBar->addWidget(mButtonBackColor);
    ui->statusBar->addPermanentWidget(m_Note);
}

QWidget *MainWindow::CreateTaskArea(QWidget *parent)
{
    QWidget * taskWidget = new QWidget(parent);
    taskWidget->setFixedWidth(230);
    taskWidget->setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:15px;"));
    CoWidgetBox *taskBox = new CoWidgetBox(taskWidget);
    connect(taskBox,&CoWidgetBox::TransferHideEvent,this,&MainWindow::HandleTaskHideEvent);
    QVBoxLayout *vbox = new QVBoxLayout();
    taskWidget->setLayout(vbox);
    vbox->addWidget(taskBox);

    CoTreeWidget *treeWidget = new CoTreeWidget();
    treeWidget->setObjectName("CoTreeWidget");
    taskBox->AddWidget(treeWidget);
    connect(treeWidget,&CoTreeWidget::SendNewCanvasEvent,this,&MainWindow::CreateNewCanvas);
    connect(this,&MainWindow::SendNewCanvasEnd,treeWidget,&CoTreeWidget::AddCanvasItem);
    connect(treeWidget,&CoTreeWidget::SendNewProjectEvent,this,&MainWindow::CreateNewProject);
    connect(this,&MainWindow::SendNewProjectEnd,treeWidget,&CoTreeWidget::AddProjectItem);

    connect(treeWidget,&CoTreeWidget::SendDelCanvasEvent,this,&MainWindow::DeleteCanvas);
    connect(this,&MainWindow::SendDelCanvasEnd,treeWidget,&CoTreeWidget::DeleteCanvasItem);
    connect(treeWidget,&CoTreeWidget::SendDelProjectEvent,this,&MainWindow::DeleteProject);
    connect(this,&MainWindow::SendDelProjectEnd,treeWidget,&CoTreeWidget::DeleteProjectItem);
    connect(treeWidget,&CoTreeWidget::SendChangeTreeNodeEvent,this,&MainWindow::ChangeTreeNode);
    connect(treeWidget,&CoTreeWidget::SendRenameCanvasEvent,this,&MainWindow::RenameCanvas);
    connect(treeWidget,&CoTreeWidget::SendRenameProjectEvent,this,&MainWindow::RenameProject);

    connect(this,&MainWindow::SendTreeCurItem,treeWidget,&CoTreeWidget::SetCurItem);

    return taskWidget;
}

QWidget *MainWindow::CreateShowArea(QWidget *parent)
{
    QWidget *ShowWidget = new QWidget(parent);
    m_pShowVbox = new QVBoxLayout();
    ShowWidget->setLayout(m_pShowVbox);
    ShowWidget->setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:15px;"));
    InitShowWidget();
    return ShowWidget;
}

void MainWindow::InitShowWidget()
{
    //Create Canvas
    m_pShowCanvasArea = new QScrollArea();
    m_pView = new CoView();
    connect(m_pView, SIGNAL(MousePress(int,int)), this, SLOT(ViewClicked(int,int)));
    m_pShowCanvasArea->setWidget(m_pView);

    //Create DataWidget
    m_pShowDataArea = new QScrollArea();
    m_pDataManagerView = new DataManagerForm();
    QHBoxLayout *hbox = new QHBoxLayout;
    m_pShowDataArea->setLayout(hbox);
    hbox->addWidget(m_pDataManagerView);
    //m_pDataManagerView->resize(800,800);
    m_pShowDataArea->hide();
    //Create 权限管理用户
    m_pShowAuthorityArea = new QScrollArea();
    QHBoxLayout *hlay = new QHBoxLayout;
    m_pShowAuthorityArea->setLayout(hlay);
    m_pDataManagerPerView = new databasepermiss();
    hlay->addWidget(m_pDataManagerPerView);
    m_pShowAuthorityArea->hide();

    //Create DeviceWidget
    m_pShowDeviceArea = new QScrollArea();
    m_hardView = new Hardwarefrom();
    m_pShowDeviceArea->setWidget(m_hardView);
    m_pShowDeviceArea->hide();

    m_pShowVbox->addWidget(m_pShowCanvasArea);
    m_pShowVbox->addWidget(m_pShowAuthorityArea);
    m_pShowVbox->addWidget(m_pShowDataArea);
    m_pShowVbox->addWidget(m_pShowDeviceArea);
}

void MainWindow::ViewClicked(int x, int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    if(NULL == m_pCurScene)
    {
        if(NULL == m_pDrawSelected)
        {
            CreateMsgBox(QMessageBox::Warning,tr("提示"),tr("请选择一个绘图工具，再进行绘图"));
        }
        else
        {
            CreateMsgBox(QMessageBox::Warning,tr("提示"),tr("请新建一个画布，再进行绘图"));
        }
    }
}

QWidget *MainWindow::CreateAttributeArea(QWidget *parent)
{
    QWidget * attrteWidget = new QWidget(parent);
    attrteWidget->setFixedWidth(300);
    attrteWidget->setStyleSheet(tr("background-color:rgb(63, 147, 168);color:white;border-radius:15px;"));
    QVBoxLayout *vbox = new QVBoxLayout();
    attrteWidget->setLayout(vbox);
    m_pAttrBaseDlg = new CoAttributeBox(attrteWidget);
    connect(m_pAttrBaseDlg,&CoAttributeBox::TransferHideEvent,this,&MainWindow::HandleAttrHideEvent);
    vbox->addWidget(m_pAttrBaseDlg);

    return attrteWidget;
}

TaskBar *MainWindow::CreateTaskBar(QWidget *parent)
{
    TaskBar *taskBar = new TaskBar(parent);
    taskBar->hide();
    connect(taskBar,&TaskBar::SendBarItemClick,this,&MainWindow::TaskBarItemClicked);
    return taskBar;
}

QTimer *MainWindow::CreateTimer(QWidget *parent)
{
    QTimer *timer = new QTimer(parent);
    connect(timer,&QTimer::timeout,this,&MainWindow::EnableEditAction);
    timer->start(500);
    return timer;
}

MainWindow::~MainWindow()
{
    m_pTimer->stop();
    if(m_pThread != NULL)
    {
        m_pThread->quit();
        m_pThread->wait();
        delete m_pThread;
        m_pThread = NULL;
    }
    CoreLog::QLoggerManager::getInstance()->closeLogger();
    delete ui;
}

void MainWindow::CreateNewCanvas(const QString& proName, const int parentName)
{
    DialogNewScene *NewSceneShow = new DialogNewScene(this);
    if(NewSceneShow)
    {
        QString tempProName = proName;
        if(parentName == CoTreeWidget::Canvas)
        {
            NewSceneShow->SetBackColor(Qt::white);
            NewSceneShow->SetNameList(m_SceneAttrMap[tempProName].sceneMap.keys());
        }
        else
        {
//            QString tempProName = proName;
            if(parentName == CoTreeWidget::Reset)
            {
                NewSceneShow->SetName(tr("总复位"));
            }
            else if(parentName == CoTreeWidget::ChlidReset)
            {
                NewSceneShow->SetName(tr("子复位"));
                QStringList nameList = proName.split(",");
                tempProName = nameList[1];
            }
            else if(parentName == CoTreeWidget::FlowThread)
            {
                QStringList nameList = proName.split("#");
                if(nameList.size() >= 2)
                {  QString nameproname = nameList[0];
                    QStringList tempName = nameproname.split(",");
                    tempProName = tempName[1];
                }
            }
            else if(parentName == CoTreeWidget::Org)
            {
                NewSceneShow->SetName(tr("回原点"));
            }
            NewSceneShow->SetNameList(m_SceneAttrMap[tempProName].flowMap.keys());

        }

        NewSceneShow->exec();
        if(NewSceneShow->GetName().contains(",")|| NewSceneShow->GetName().contains(";")||NewSceneShow->GetName().contains("#"))
        {
            QMessageBox::information(NULL,tr("警告"),tr("画布名字不允许包含特殊字符,请重新输入!"));
            delete NewSceneShow;
            NewSceneShow = NULL;
            return;
        }
        if(NewSceneShow->mSave)
        {
            int height = m_SceneAttrMap[tempProName].viewHeight;
            int width = m_SceneAttrMap[tempProName].viewWidth;
            CoScene *AddScene = CreateNewScene(++m_SceneId, tempProName,NewSceneShow->GetName(),
                                               NewSceneShow->GetBackColor(),parentName,0,0,width ,height);
            if(AddScene)
            {
                if(parentName == CoTreeWidget::Canvas)
                {
                    //m_pView->setFixedSize(m_SceneAttrMap[proName].viewWidth,m_SceneAttrMap[proName].viewHeight);
                    AddScene->setSceneRect(0,0,m_SceneAttrMap[proName].viewWidth,m_SceneAttrMap[proName].viewHeight);
                    m_SceneAttrMap[proName].sceneMap.insert(NewSceneShow->GetName(),AddScene);
                    AttributeInstance::GetInstance()->canvasMap[proName].append(AddScene->GetName());
                }
                else
                {
                    //m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
                    //AddScene->setSceneRect(0,0,m_pView->rect().width(),m_pView->rect().height());
                    //重置流程图画布的尺寸，铺满center区域
                    AddScene->setSceneRect(0,0,m_pView->size().width(),m_pView->size().height()*2);
                    AddScene->ResizeEvent(false);
                    if(parentName >= CoTreeWidget::Flow &&parentName <= CoTreeWidget::Org)
                    {
                        FLOW_STRU flowstru;
                        flowstru.SceneMap.insert(NewSceneShow->GetName(), AddScene);
                        m_SceneAttrMap[proName].flowMap.insert(NewSceneShow->GetName(),flowstru);
                        AttributeInstance::GetInstance()->flowMap[proName].append(AddScene->GetName());
                    }
                    else if(parentName == CoTreeWidget::OrgThread)
                    {
                        m_SceneAttrMap[proName].flowMap["回原点"].SceneMap.insert(NewSceneShow->GetName(), AddScene);
                    }
                    else if(parentName == CoTreeWidget::ResetThread)
                    {
                        m_SceneAttrMap[proName].flowMap["总复位"].SceneMap.insert(NewSceneShow->GetName(), AddScene);
                    }
                    else
                    {
                        QStringList nameList ;
                        if(parentName == CoTreeWidget::FlowThread)//获取当前节点下所有的名字
                        {
                            QStringList nameListall = proName.split("#");
                            if(nameListall.size() >= 2)
                            {
                                QString allchildname = nameListall[1];
                                QStringList pronamelist = allchildname.split(";");
                                for( int i = 0; i < pronamelist.size();++i)
                                {
                                    if(pronamelist[i]== NewSceneShow->GetName())
                                    {
                                        delete NewSceneShow;
                                        NewSceneShow = NULL;
                                        return;
                                    }
                                }
                                QString proname = nameListall[0];
                                nameList = proname.split(",");
                            }
                        }
                        else{
                            nameList = proName.split(",");
                        }
                        if(nameList.size() == 2)
                        {
                            m_SceneAttrMap[nameList[1]].flowMap[nameList[0]].SceneMap.insert(NewSceneShow->GetName(), AddScene);
                        }
                    }
                }
                m_pView->setScene(AddScene);
                m_pCurScene = AddScene;
                ChangeShowWidget(CoTreeWidget::Canvas);
            }
        }
        delete NewSceneShow;
        NewSceneShow = NULL;
    }
}

void MainWindow::CreateNewProject()
{
    DialogNewProject *NewProject = new DialogNewProject(this);
    if(NewProject)
    {
        NewProject->SetNameList(m_SceneAttrMap.keys());
        NewProject->SetHeight(600);
        NewProject->SetWidth(800);
        NewProject->exec();
        if(NewProject->mSave)
        {
            int viewHeight = NewProject->GetHeight();
            int viewWidth = NewProject->GetWidth();
            PROJECT_STRU projectStru;
            projectStru.viewHeight = viewHeight;
            projectStru.viewWidth  = viewWidth;
            AttributeInstance::GetInstance()->canvasMap.insert(NewProject->GetName(),QStringList());
            AttributeInstance::GetInstance()->flowMap.insert(NewProject->GetName(),QStringList());
            emit SendNewProjectEnd(NewProject->GetName());
            ChangeShowWidget(CoTreeWidget::Project);
            QString saveDirName = QCoreApplication::applicationDirPath() + "/" + NewProject->GetName();
            IsDirExist(saveDirName);
            m_hardView->m_path = saveDirName+"/"+"硬件配置.xml";
            m_hardView->CreatXmlRoot();
            m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
            projectStru.safeScene = CreateNewScene(++m_SceneId, NewProject->GetName(),tr("安全配置"),QColor(10,10,10),
                                                   CoTreeWidget::SafeArea,0,0,m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
            m_TaskBar->move(m_AttrWidget->x()-50,100);
            m_SceneAttrMap.insert(NewProject->GetName(),projectStru);
            m_IsSaveXml = true;
        }
        delete NewProject;
        NewProject = NULL;
    }
}

void MainWindow::DeleteCanvas(const QString & Proname,const QString & Cvasname,const int parentName)
{
    QString strnote = QString("删除项目 %1 指定页面 %2，将丢失页面上所有图形，且不可恢复，是否继续删除").arg(Proname).arg(Cvasname);
    if(QMessageBox::Yes != CreateMsgBox(QMessageBox::Information,tr("提示"),strnote,QMessageBox::Yes|QMessageBox::No))
    {
        return;
    }
    if(m_SceneAttrMap.contains(Proname))
    {
        if(parentName == CoTreeWidget::Canvas)
        {
            if(m_SceneAttrMap[Proname].sceneMap.contains(Cvasname))
            {
                if(m_SceneAttrMap[Proname].sceneMap[Cvasname] == m_pCurScene)
                {
                    if(m_SceneHost == m_pCurScene->GetId())
                    {
                        m_SceneHost = 0;
                    }
                    delete m_pCurScene;
                    m_pCurScene = 0;
                }

                m_SceneAttrMap[Proname].sceneMap.remove(Cvasname);
                AttributeInstance::GetInstance()->canvasMap[Proname].removeOne(Cvasname);
            }
        }
        else if(parentName >= CoTreeWidget::Flow && parentName <= CoTreeWidget::Org)
        {
            if(m_SceneAttrMap[Proname].flowMap.contains(Cvasname))
            {
                if(m_SceneAttrMap[Proname].flowMap[Cvasname].SceneMap[Cvasname] == m_pCurScene)
                {
                    if(m_SceneHost == m_pCurScene->GetId())
                    {
                        m_SceneHost = 0;
                    }
                    delete m_pCurScene;
                    m_pCurScene = 0;
                }
                m_SceneAttrMap[Proname].flowMap.remove(Cvasname);
                AttributeInstance::GetInstance()->flowMap[Proname].removeOne(Cvasname);
            }
        }
        else if(parentName == CoTreeWidget::OrgThread)
        {
            if(m_SceneAttrMap[Proname].flowMap["回原点"].SceneMap[Cvasname] == m_pCurScene)
            {
                if(m_SceneHost == m_pCurScene->GetId())
                {
                    m_SceneHost = 0;
                }
                delete m_pCurScene;
                m_pCurScene = 0;
            }
            m_SceneAttrMap[Proname].flowMap["回原点"].SceneMap.remove(Cvasname);
        }
        else if(parentName == CoTreeWidget::ResetThread)
        {
            if(m_SceneAttrMap[Proname].flowMap["总复位"].SceneMap[Cvasname] == m_pCurScene)
            {
                if(m_SceneHost == m_pCurScene->GetId())
                {
                    m_SceneHost = 0;
                }
                delete m_pCurScene;
                m_pCurScene = 0;
            }
            m_SceneAttrMap[Proname].flowMap["总复位"].SceneMap.remove(Cvasname);

        }
        else if(parentName == CoTreeWidget::FlowThread)
        {
            QStringList nameList = Cvasname.split(",");
            if(m_SceneAttrMap[Proname].flowMap[nameList[1]].SceneMap[nameList[0]] == m_pCurScene)
            {
                if(m_SceneHost == m_pCurScene->GetId())
                {
                    m_SceneHost = 0;
                }
                delete m_pCurScene;
                m_pCurScene = 0;
            }
            m_SceneAttrMap[Proname].flowMap[nameList[1]].SceneMap.remove(nameList[0]);
        }
        else if(parentName == CoTreeWidget::ChlidReset)
        {
            if(m_SceneAttrMap[Proname].flowMap[Cvasname].SceneMap["子复位"] == m_pCurScene)
            {
                if(m_SceneHost == m_pCurScene->GetId())
                {
                    m_SceneHost = 0;
                }
                delete m_pCurScene;
                m_pCurScene = 0;
            }
            m_SceneAttrMap[Proname].flowMap[Cvasname].SceneMap.remove("子复位");
        }
        emit SendDelCanvasEnd();
    }
}

void MainWindow::ChangeTreeNode(const QString &Proname, const QString &chlidname,int iNodename)
{

    if(iNodename == CoTreeWidget::Canvas)
    {
        if(m_SceneAttrMap.contains(Proname))
        {
            if(m_SceneAttrMap[Proname].sceneMap.contains(chlidname))
            {
                m_pCurScene = m_SceneAttrMap[Proname].sceneMap[chlidname];
                //m_pView->setFixedSize(m_SceneAttrMap[Proname].viewWidth,m_SceneAttrMap[Proname].viewHeight);
                m_pView->setScene(m_pCurScene);
                ChangeShowWidget(iNodename);
            }
        }
    }
    if(iNodename >= CoTreeWidget::Flow && iNodename <= CoTreeWidget::Org)
    {
        if(m_SceneAttrMap.contains(Proname))
        {
            if(m_SceneAttrMap[Proname].flowMap.contains(chlidname))
            {
                m_pCurScene = m_SceneAttrMap[Proname].flowMap[chlidname].SceneMap[chlidname];
                //m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
                m_pView->setScene(m_pCurScene);
                ChangeShowWidget(iNodename);
            }
        }
    }
    if(iNodename == CoTreeWidget::FlowThread || iNodename == CoTreeWidget::ChlidReset)
    {
        QStringList nameList = Proname.split(",");
        if(m_SceneAttrMap.contains(nameList[1]))
        {
            if(m_SceneAttrMap[nameList[1]].flowMap.contains(nameList[0]))
            {
                m_pCurScene = m_SceneAttrMap[nameList[1]].flowMap[nameList[0]].SceneMap[chlidname];
                //m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
                m_pView->setScene(m_pCurScene);
                ChangeShowWidget(iNodename);
            }
        }
    }
    if(iNodename == CoTreeWidget::ResetThread)
    {
        if(m_SceneAttrMap.contains(Proname))
        {
            if(m_SceneAttrMap[Proname].flowMap.contains("总复位"))
            {
                m_pCurScene = m_SceneAttrMap[Proname].flowMap["总复位"].SceneMap[chlidname];
                //m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
                m_pView->setScene(m_pCurScene);
                ChangeShowWidget(iNodename);
            }
        }
    }
    if(iNodename == CoTreeWidget::OrgThread)
    {
        if(m_SceneAttrMap.contains(Proname))
        {
            if(m_SceneAttrMap[Proname].flowMap.contains("回原点"))
            {
                m_pCurScene = m_SceneAttrMap[Proname].flowMap["回原点"].SceneMap[chlidname];
                //m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
                m_pView->setScene(m_pCurScene);
                ChangeShowWidget(iNodename);
            }

        }
    }
    else if(iNodename == CoTreeWidget::DataArea)
    {
        ChangeShowWidget(iNodename);
        m_pAttrBaseDlg->AddWidget(m_pDataManagerView->GetAttriArea());
    }
    else if(iNodename == CoTreeWidget::Authority)
    {
        ChangeShowWidget(iNodename);
        m_pAttrBaseDlg->AddWidget(m_pDataManagerPerView->GetPerAttrArea());
    }
    else if(iNodename == CoTreeWidget::Devices)
    {
        ChangeShowWidget(iNodename);
        m_pAttrBaseDlg->AddWidget(m_hardView->GetAttrArea());
    }
    else if(iNodename == CoTreeWidget::SafeArea)
    {
        if(m_SceneAttrMap[Proname].safeScene)
        {
            m_pCurScene = m_SceneAttrMap[Proname].safeScene;
            //m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
            m_pView->setScene(m_pCurScene);
            ChangeShowWidget(iNodename);
        }
    }
}

void MainWindow::DeleteProject(const QString & name)
{
    if(m_SceneAttrMap.contains(name))
    {
        QString strnote = QString("删除指定项目 %1，将丢失项目上所有图形和数据，且不可恢复，是否继续删除").arg(name);
        if(QMessageBox::Yes != CreateMsgBox(QMessageBox::Information,tr("提示"),strnote,QMessageBox::Yes|QMessageBox::No))
        {
            return;
        }

        bool iscur = false;
        if(m_SceneAttrMap[name].sceneMap.values().contains(m_pCurScene))
        {
            iscur = true;
        }
        if(!iscur)
        {
            for(auto item = m_SceneAttrMap[name].flowMap.begin();item != m_SceneAttrMap[name].flowMap.end();++item)
            {
                if(item.value().SceneMap.values().contains(m_pCurScene))
                {
                    iscur = true;
                    break;
                }
            }
        }
        if(iscur)
        {
            delete m_pCurScene;
            m_pCurScene = 0;
        }
        m_SceneAttrMap.remove(name);
        emit SendDelProjectEnd();

        AttributeInstance::GetInstance()->canvasMap.remove(name);
        AttributeInstance::GetInstance()->flowMap.remove(name);
    }

}

void MainWindow::RenameProject(const QString &oldName, const QString &newName)
{
    if(oldName.compare(newName) == 0)
    {
        return;
    }
    PROJECT_STRU projectStru;
    if(m_SceneAttrMap.contains(oldName))
    {
        projectStru = m_SceneAttrMap[oldName];
        projectStru.ChangeProName(newName);
        m_SceneAttrMap.remove(oldName);
        m_SceneAttrMap.insert(newName,projectStru);
        QStringList list = AttributeInstance::GetInstance()->canvasMap[oldName];
        AttributeInstance::GetInstance()->canvasMap.remove(oldName);
        AttributeInstance::GetInstance()->canvasMap.insert(newName,list);

        list = AttributeInstance::GetInstance()->flowMap[oldName];
        AttributeInstance::GetInstance()->flowMap.remove(oldName);
        AttributeInstance::GetInstance()->flowMap.insert(newName,list);
    }
}

void MainWindow::RenameCanvas(const QString &proName,const QString &oldName, const QString &newName ,QString flowName)
{
    if(oldName.compare(newName) == 0)
    {
        return;
    }
    if(m_SceneAttrMap.contains(proName))
    {
        if(m_SceneAttrMap[proName].sceneMap.contains(oldName))
        {
            CoScene* sceneStru = m_SceneAttrMap[proName].sceneMap[oldName];
            sceneStru->SetName(newName);
            m_SceneAttrMap[proName].sceneMap.insert(newName,sceneStru);
            m_SceneAttrMap[proName].sceneMap.remove(oldName);

            AttributeInstance::GetInstance()->canvasMap[proName].removeOne(oldName);
            AttributeInstance::GetInstance()->canvasMap[proName].append(newName);
        }
        else if(m_SceneAttrMap[proName].flowMap.contains(flowName))
        {
            CoScene* sceneStru = NULL;

            sceneStru = m_SceneAttrMap[proName].flowMap[flowName].SceneMap[oldName];
            sceneStru->SetName(newName);
            m_SceneAttrMap[proName].flowMap[flowName].SceneMap.insert(newName,sceneStru);
            m_SceneAttrMap[proName].flowMap[flowName].SceneMap.remove(oldName);
            if(flowName.compare(oldName) == 0)
            {
                FLOW_STRU flowStru = m_SceneAttrMap[proName].flowMap[flowName];
                m_SceneAttrMap[proName].flowMap.insert(newName,flowStru);
                m_SceneAttrMap[proName].flowMap.remove(flowName);
            }
        }
    }
}

void MainWindow::ChangeShowWidget(int swName)
{
    switch (swName)
    {
    case CoTreeWidget::Canvas:
    case CoTreeWidget::Flow:
    case CoTreeWidget::Reset:
    case CoTreeWidget::Org:
    case CoTreeWidget::FlowThread:
    case CoTreeWidget::ChlidReset:
    case CoTreeWidget::ResetThread:
    case CoTreeWidget::OrgThread:
    case CoTreeWidget::SafeArea:
        m_pShowCanvasArea->show();
        m_pShowDataArea->hide();
        m_pShowDeviceArea->hide();
        m_pShowAuthorityArea->hide();
        m_TaskBar->show();
        break;
    case CoTreeWidget::DataArea:
        m_pShowCanvasArea->hide();
        m_pShowDataArea->show();
        m_pShowDeviceArea->hide();
        m_pShowAuthorityArea->hide();
        m_TaskBar->hide();
        break;
    case CoTreeWidget::Authority:
        m_pShowCanvasArea->hide();
        m_pShowDataArea->hide();
        m_pShowDeviceArea->hide();
        m_pShowAuthorityArea->show();
        m_TaskBar->hide();
        break;
    case CoTreeWidget::Devices:
        m_pShowCanvasArea->hide();
        m_pShowDataArea->hide();
        m_pShowDeviceArea->show();
        m_pShowAuthorityArea->hide();
        m_TaskBar->hide();
        break;
    default:
        m_TaskBar->hide();
        break;
    }
}

CoScene* MainWindow::CreateNewScene(int SceneId, QString proName, QString Name, QColor BackColor,
                                    int SceneType,double PosX, double PosY, double Width, double Height)
{
    CoScene *AddScene = new CoScene(PosX, PosY, Width, Height);
    AddScene->SetId(SceneId);
    AddScene->SetProjectName(proName);
    AddScene->SetName(Name);
    AddScene->SetBackColor(BackColor);
    AddScene->ResizeEvent(m_ActGrid->isChecked());
    SetCoScene(AddScene,SceneType);

    return AddScene;
}

void MainWindow::SetCoScene(CoScene *Scene,int SceneType)
{
    if(NULL == Scene)
    {
        return;
    }
    connect(Scene, SIGNAL(mDropFinsh(double, double, double, double)),
            this, SLOT(DrawItem(double, double, double, double)));

    connect(Scene, SIGNAL(mClickItem(const QString&,const QString&,const int,const int,CoItemEx*)),
            this, SLOT(ClickItemForAttrWidget(const QString&,const QString&,const int,const int,CoItemEx*)));

    connect(Scene, SIGNAL(mClickWidget(const QString&,const QString&,const int,const int,CoProxyWidget*)),
            this, SLOT(ClickWidgetForAttrWidget(const QString&,const QString&,const int,const int,CoProxyWidget*)));

    connect(Scene, SIGNAL(mMoveItemList(CoScene*,QList<CoItem*>,QList<CoProxyWidget*>,double,double,double,double)),
            this, SLOT(MoveItemList(CoScene*,QList<CoItem*>,QList<CoProxyWidget*>,double,double,double,double)));

    emit SendNewCanvasEnd(Scene->GetName(),SceneType);
}

void MainWindow::DrawItem(double StartX, double StartY, double StopX, double StopY)
{
    CoItem * Item = NULL;
    CoProxyWidget *Widget = NULL;
    if(m_pCurScene && NULL != m_pDrawSelected)
    {
        if(StopX-StartX < 80 &&  StopY -StartY < 60)
        {
            StopX = StartX + 80;
            StopY = StartY + 60;
        }
        if(m_pCurScene->GetName().compare(tr("安全配置")) == 0)
        {
            if(m_pDrawSelected->m_Type != CoItem::Warnlamp &&
                    m_pDrawSelected->m_Type != CoItem::Mde_Safe_Sig &&
                    m_pDrawSelected->m_Type != CoItem::SafeState)
            {
                return;
            }

        }
        if(m_pDrawSelected->m_Type <= CoItem::B_3)
        {         
            Item = TItemFactory::CreateItem(m_pDrawSelected->m_Type,
                                      StartX, StartY, StopX, StopY,
                                      m_LineStyle, m_LineWidth, m_LineColor, m_BackColor);
            if(NULL == Item)
            {
                return;
            }
            Item->SetProName(m_pCurScene->GetProjectName());
        }
        else
        {
            Widget = TItemFactory::CreateProxyWidget(m_pDrawSelected->m_Type,
                                                                  StartX, StartY, StopX, StopY,m_LineStyle,
                                                                  m_LineWidth, m_LineColor, m_BackColor,m_pCurScene);
            if(NULL == Widget)
            {
                return;
            }
            Widget->SetProName(m_pCurScene->GetProjectName());

        }
    }
    AddItemToScene(Item,Widget);
}

void MainWindow::AddItemToScene(CoItem *Item, CoProxyWidget *Widget)
{
    if(NULL == m_pCurScene)
    {
        return;
    }
    QList<CoItem *> ItemList = QList<CoItem *>();
    if(Item)
    {
        SetItem(Item);
        ItemList.push_back(Item);
    }
    QList<CoProxyWidget *> WidgetList = QList<CoProxyWidget *>();
    if(Widget)
    {
        WidgetList.push_back(Widget);
    }

    AddItemList(m_pCurScene, ItemList,WidgetList);
}

void MainWindow::SetItem(CoItem *Item)
{
    if(NULL == Item)
    {
        return;
    }
    connect(Item, &CoItem::mDrop,this, &MainWindow::ItemDrop);
    connect(Item, &CoItem::mSwithScene,this, &MainWindow::SwithScene);
}

void MainWindow::ItemDrop(CoItem *Item, double StartDx, double StartDy, double StopDx, double StopDy)
{
    if(NULL == m_pCurScene || NULL == Item)
    {
        return;
    }

    CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
    m_UndoStack->push(Undo);
    Undo->SetScene(m_pCurScene);
    Undo->AddItem(Item);
    Undo->SetVector(StartDx, StartDy, StopDx, StopDy);
}

void MainWindow::SwithScene(const QString & sceneName, const QString & proName)
{
    if(m_pCurScene->GetProjectName() != proName)
    {
        m_pCurScene = m_SceneAttrMap[proName].sceneMap[sceneName];
        m_pView->setScene(m_pCurScene);
    }
    else
    {
        if(m_pCurScene->GetName() != sceneName)
        {
            m_pCurScene = m_SceneAttrMap[proName].sceneMap[sceneName];
            m_pView->setScene(m_pCurScene);
        }
    }
}

void MainWindow::AddItemList(CoScene *Scene, QList<CoItem *> &ItemList, QList<CoProxyWidget *> WidgetList)
{
    if(NULL == Scene || (ItemList.isEmpty() && WidgetList.isEmpty()))
    {
        return;
    }

//    Scene->clearSelection();
//    QList<CoItem *> list;
//    Scene->GetItemsList(list);
//    int id = 0;
//    foreach (CoItem * Item, list)
//    {
//       if(id < Item->GetId())
//           id = Item->GetId();
//    }
//    QList<CoProxyWidget *> listwg;
//    Scene->GetItemsList(listwg);
//    foreach (CoProxyWidget * Item, listwg)
//    {
//       if(id < Item->GetId())
//           id = Item->GetId();
//    }
    foreach(CoItem *Item, ItemList)
    {
        Item->setSelected(true);
        Item->SetId(++m_ItemId);
    }
    foreach(CoProxyWidget *Item, WidgetList)
    {
        Item->setSelected(true);
        Item->SetId(++m_ItemId);
    }

    CoUndoAddDel *Undo = new CoUndoAddDel(Scene, ItemList);
    Undo->AddProxyWidgets(WidgetList);
    connect(Undo,&CoUndoAddDel::SendAddDelEvent,this,&MainWindow::HandleAddDelEvent);
    Undo->SetOperation(CoUndoAddDel::OPERATION_ADD);
    // push时，执行一次，完成添加
    m_UndoStack->push(Undo);
}

void MainWindow::AddItemAttrToMap(CoItem *Item)
{
    BASE_STRU * attrStru = NULL;
    uchar uc = 0;
    switch (Item->GetType()) {
    case CoItem::Button:
        attrStru = new BTNATTR_STRU();
        break;
    default:
        break;
    }
    if(attrStru != NULL)
    {
        if(uc == 0)
        {
            attrStru->id = Item->GetId();
            attrStru->type = Item->GetType();
            attrStru->power = Item->GetPower();
            //            m_SceneAttrMap[m_pCurScene->GetProjectName()].sceneMap[m_pCurScene->GetName()].AttrMap.insert(Item->GetId(),attrStru);
        }
        else
        {
            attrStru->id = Item->GetId();
            attrStru->type = Item->GetType();
            //            m_SceneAttrMap[m_pCurScene->GetProjectName()].flowMap[m_pCurScene->GetName()].AttrMap.insert(Item->GetId(),attrStru);
        }
    }

}
void MainWindow::DelItemAttrFromMap(CoItem *)
{
    if(m_pCurScene == NULL) return;
    //    if(Item->GetType() >= 300)
    //        m_SceneAttrMap[m_pCurScene->GetProjectName()].flowMap[m_pCurScene->GetName()].AttrMap.remove(Item->GetId());
    //    else
    //    {
    //        m_SceneAttrMap[m_pCurScene->GetProjectName()].sceneMap[m_pCurScene->GetName()].AttrMap.remove(Item->GetId());
    //    }
}
void MainWindow::TaskBarItemClicked(CoAction* mAction)
{
    m_pCurScene->SetMode(CoScene::InsertItem);
    if(NULL != m_pDrawSelected)
    {
        m_pDrawSelected->setChecked(false);
    }
    if(mAction == m_pDrawSelected)
    {
        m_pDrawSelected = NULL;
    }
    else
    {
        m_pDrawSelected = mAction;
    }
    static bool isFlowArrow = false;
    if(mAction->m_Type == CoItem::FlowArrow)
    {
        m_pCurScene->SetMode(CoScene::InsertArrow);
        QList<CoItem*> list;
        m_pCurScene->GetItemsList(list);
        foreach (CoItem *Item, list)
        {
            Item->SetDragAble(false);
            Item->SetMoveAble(false);
        }
        isFlowArrow = true;
    }
    else
    {
        if(isFlowArrow)
        {
            isFlowArrow = false;
            QList<CoItem*> list;
            m_pCurScene->GetItemsList(list);
            foreach (CoItem *Item, list)
            {
                Item->SetDragAble(true);
                Item->SetMoveAble(true);
            }
        }
    }
}

void MainWindow::HandleAttrHideEvent(bool isHide)
{
    m_isHideAttrWidget = isHide;
    m_AttrWidget->setVisible(!m_isHideAttrWidget);
}

void MainWindow::HandleTaskHideEvent(bool isHide)
{
    m_isHideTaskWidget = isHide;
    m_TaskWidget->setVisible(!m_isHideTaskWidget);
}

void MainWindow::ClickItemForAttrWidget(const QString &, const QString &, const int type, const int ,CoItemEx* Item)
{
    if(!m_isHideAttrWidget)
    {
        CoAttributeWidget *attr = AttributeFactory::CreateAttributeWidget(type,Item);
        SetAttrWidget(attr);
        m_pAttrBaseDlg->AddWidget(attr);
    }
}

void MainWindow::ClickWidgetForAttrWidget(const QString &, const QString &, const int type, const int, CoProxyWidget *Item)
{
    if(!m_isHideAttrWidget)
    {
        CoAttributeWidget *attr = AttributeFactory::CreateAttributeWidget(type,Item);
        SetAttrWidget(attr);
        m_pAttrBaseDlg->AddWidget(attr);
    }
}

void MainWindow::SetAttrWidget(CoAttributeWidget * attb)
{
    if(attb == NULL)
        return;
    connect(attb,&CoAttributeWidget::SendTextEditedEvent,this,&MainWindow::SetAttribute);
}

void MainWindow::SetAttribute(uchar type,int name, const QString &strContent)
{
    Q_UNUSED(type)
    Q_UNUSED(name)
    Q_UNUSED(strContent)
    //    CoAttributeWidget *attr = qobject_cast<CoAttributeWidget*>(sender());
    //    QString proN = m_pCurScene->GetProjectName();
    //    QString cvsN = m_pCurScene->GetName();
    //    int id = attr->GetId();

}

void MainWindow::MoveItemList(CoScene *Scene, QList<CoItem *> ItemList,
                              QList<CoProxyWidget*> WidgetList,double StartX, double StartY, double StopX, double StopY)
{
    if(NULL == Scene || (ItemList.length() <= 0 && WidgetList.length() <= 0))
    {
        return;
    }

    CoUndoMoveDrag *Undo = new CoUndoMoveDrag(Scene, ItemList);
    Undo->AddProxyWidgets(WidgetList);
    m_UndoStack->push(Undo);

    double dx = StopX - StartX;
    double dy = StopY - StartY;
    Undo->SetVector(dx, dy, dx, dy);
}

void MainWindow::LineStyleChanged(int Style)
{
    m_LineStyle = Qt::PenStyle(Style);
}

void MainWindow::LineWidthChanged(int Width)
{
    m_LineWidth = Width;
    TBase::ColorLabel(m_LabelLineWidth, Qt::black, m_LabelLineWidth->width(), m_LineWidth);
}

void MainWindow::UpdateLineColor()
{
    if(TBase::ChooseColor(m_LineColor, this))
    {
        TBase::ColorLabel(m_LabelLineColor, m_LineColor);
    }
}

void MainWindow::UpdateBackColor()
{
    if(TBase::ChooseColor(m_BackColor, this))
    {
        TBase::ColorLabel(m_LabelBackColor, m_BackColor);
    }
}

void MainWindow::OpenProject()
{
    QString FileDir = QFileDialog::getExistingDirectory(this, tr("打开"),
                                                        QApplication::applicationDirPath());

    if(tr("") == FileDir)
    {
        return;
    }
    QString proName = FileDir.right(FileDir.length() - FileDir.lastIndexOf("/") - 1);
    emit SendTreeCurItem(proName,1);
    DeleteProject(proName);
    emit SendNewProjectEnd(proName);
    TXmlReader mXml;
    if(!mXml.Open(FileDir+"/UI配置.xml"))
    {
        CreateMsgBox(QMessageBox::Warning,tr("初始化"),tr("打开或解析错误"));
        return;
    }
    //解析界面图形
    m_ProStru.Clear();
    mXml.AnalyFlowChart(FileDir+"/scene.item");
    bool isGridLine = false;
    m_pCurScene = mXml.AnalyScene(m_ProStru.sceneMap,m_ProStru.flowMap,isGridLine);
    m_ActGrid->setChecked(isGridLine);
    AttributeInstance::GetInstance()->canvasMap.insert(proName,QStringList());
    m_hardView->LoadHwXml(FileDir+"/硬件配置.xml");
    m_hardView->m_path = FileDir+"/硬件配置.xml";
    if(m_pCurScene)
    {
        m_SceneHost = m_pCurScene->GetId();
        if(m_ProStru.sceneMap.contains("安全配置"))
        {
            m_ProStru.safeScene = m_ProStru.sceneMap["安全配置"];
            m_ProStru.sceneMap.remove("安全配置");
            SetCoScene(m_ProStru.safeScene,CoTreeWidget::SafeArea);
            GetCurSceneItemId(m_ProStru.safeScene);
        }
        QTransform transform = m_pView->transform();
        transform.reset();
        int rotate = mXml.AnalyViewRotate();
        transform.rotate(rotate);
        m_pView->setTransform(transform);
        m_pView->SetRotate(rotate);

        m_ProStru.viewWidth = m_pCurScene->GetWidth();
        m_ProStru.viewHeight = m_pCurScene->GetHeight();
        //m_pView->setFixedSize(m_ProStru.viewWidth,m_ProStru.viewHeight);
        m_pView->setFixedSize(m_pShowCanvasArea->width(),m_pShowCanvasArea->height());
        m_pView->setScene(m_pCurScene);

        m_SceneAttrMap.insert(proName,m_ProStru);
        emit SendTreeCurItem(m_pCurScene->GetName(),0);
        UpdateAfterOpenFile(proName);
        m_TaskBar->move(m_AttrWidget->x()-50,100);
        emit SendTreeCurItem(m_pCurScene->GetName(),0);
    }
    m_IsSaveXml = true;
    return;

}
void MainWindow::UpdateAfterOpenFile(const QString &proName)
{
    // 解析完成,添加Canvas动作
    foreach(CoScene *Scene, m_SceneAttrMap[proName].sceneMap.values())
    {
        SetCoScene(Scene,CoTreeWidget::Canvas);
        AttributeInstance::GetInstance()->canvasMap[proName].append(Scene->GetName());
        GetCurSceneItemId(Scene);
    }

    for(auto item = m_SceneAttrMap[proName].flowMap.begin(); item != m_SceneAttrMap[proName].flowMap.end();++item)
    {
        if(item.key().compare("总复位") == 0)
        {
            CreateFlowScene(item.key(),CoTreeWidget::Reset,item.value().SceneMap,
                            CoTreeWidget::ResetThread);
        }
        else if(item.key().compare("回原点") == 0)
        {
            CreateFlowScene(item.key(),CoTreeWidget::Org,item.value().SceneMap,
                            CoTreeWidget::OrgThread);
        }
        else
        {
            CreateFlowScene(item.key(),CoTreeWidget::Flow,item.value().SceneMap,
                            CoTreeWidget::FlowThread);
             AttributeInstance::GetInstance()->flowMap[proName].append(item.key());
        }
    }
}
void MainWindow::CreateFlowScene(const QString flowName, int flowtype, const QMap<QString, CoScene *> threads, int threadType)
{
    if(!threads.contains(flowName))
        return;
    emit SendTreeCurItem("流程图",1);
    CoScene *flow = threads[flowName];
    SetCoScene(flow,flowtype);
    GetCurSceneItemId(flow);

    if(threads.contains("子复位"))
    {
        emit SendTreeCurItem(flow->GetName(),1);
        SetCoScene(threads["子复位"],CoTreeWidget::ChlidReset);
        GetCurSceneItemId(threads["子复位"]);
    }
    for(auto it = threads.begin();it != threads.end();++it)
    {
        if(it.key().compare(flowName) == 0||
           it.key().compare("子复位") ==0 )
            continue;
        emit SendTreeCurItem(flow->GetName(),1);
        SetCoScene(it.value(),threadType);
        GetCurSceneItemId(it.value());
    }
}

void MainWindow::SaveXml()//保存xml的问题
{
    m_IsSaveXml = true;
    QString strDir = "";
    for(auto it = m_SceneAttrMap.begin();it != m_SceneAttrMap.end();++it)
    {
        strDir += "工程 ";
        if(!SaveToFile(it.key()))
        {
            strDir += it.key() + " 保存失败! ";
            m_IsSaveXml = false;
        }
        else
        {
            strDir += it.key() + " 已保存成功! ";
        }
    }
    if(!strDir.isEmpty())
        m_Note->setText(strDir);
}

void MainWindow::SaveXmlAs()
{

}

void MainWindow::SelectAllItems()
{
    if(NULL == m_pCurScene)
    {
        return;
    }
    QGraphicsItem *mItem = NULL;
    m_pCurScene->SetItemsSelected(mItem);
}

void MainWindow::CutItem()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    m_pCurScene->GetItemsSelected(m_CopyList);

    foreach (CoItem *mItem, m_CopyList)
    {
        m_pCurScene->removeItem(mItem);
    }
}

void MainWindow::CopyItem()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    m_pCurScene->GetItemsSelected(m_CopyList);
}

void MainWindow::PasteItem()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    CopyItemListToScene(m_CopyList);
}

void MainWindow::CopyItemListToScene(QList<CoItem *> &ItemList)
{
    //
    QList<QGraphicsItem*> items = m_pCurScene->items();
    foreach (QGraphicsItem* item, items) {
        item->setSelected(false);
    }

    QList<CoItem *> List;

    foreach(CoItem *Item, ItemList)
    {
        List.push_back(CopyItem(Item));
        Item->setSelected(false);
        Item->m_isfirstSelected = false;
    }

    AddItemList(m_pCurScene, List);
}

CoItem* MainWindow::CopyItem(CoItem *SourceItem)
{
    if(NULL == SourceItem || m_pCurScene == NULL)
    {
        return NULL;
    }

    CoItem *Item = SourceItem->Copy();
    QList<CoItem *> ItemList;
    m_pCurScene->GetItemsList(ItemList);
    if(ItemList.size() > 0)
    {
        int W = Item->GetStopX() - Item->GetStartX();
        int H = Item->GetStopY() - Item->GetStartY();
        Item->SetStartPos(Item->GetStartX() + W/2, Item->GetStartY() + H/2);
        Item->SetStopPos(Item->GetStopX() + W/2, Item->GetStopY() + H/2);
    }
    SetItem(Item);

    return Item;
}

void MainWindow::DeleteItem()
{
    if(NULL == m_pCurScene)
    {
        return;
    }
    QList<CoItem *> items;
    m_pCurScene->DeleteItemsSelected(items);
    QList<CoProxyWidget *> widgwts;
    m_pCurScene->GetItemsSelected(widgwts);
    if(widgwts.size() > 0 || items.size() > 0)
    {
        CoUndoAddDel *Undo = new CoUndoAddDel(m_pCurScene, items);
        Undo->AddProxyWidgets(widgwts);
        connect(Undo,&CoUndoAddDel::SendAddDelEvent,this,&MainWindow::HandleAddDelEvent);
        Undo->SetOperation(CoUndoAddDel::OPERATION_DEL);
        m_UndoStack->push(Undo);
    }

}

void MainWindow::MergeItems()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<QGraphicsItem *> ItemsList;
    m_pCurScene->GetItemsSelected(ItemsList);

    if(ItemsList.size() <= 1)
    {
        return;
    }
    QGraphicsItemGroup *pGroup = m_pCurScene->createItemGroup(ItemsList);

    // 设置 group 可选中、可移动
    pGroup->setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    pGroup->setHandlesChildEvents(false);

}

void MainWindow::SplitItems()
{
    if(NULL == m_pCurScene)
    {
        return;
    }
    QList<QGraphicsItemGroup *> ItemsList;
    m_pCurScene->GetItemsSelected(ItemsList);
    foreach (QGraphicsItemGroup* pGroup, ItemsList) {
        m_pCurScene->destroyItemGroup(pGroup);
    }

}

void MainWindow::UpItems(CoItem *Item)
{
    Item->SetZValue(++m_zMax);
}

void MainWindow::AlignLeft()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    double startX = 9999;
    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            startX = Item->GetCurrentStartX();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(startX < 9999)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                Undo->SetScene(m_pCurScene);
                Undo->AddItem(Item);
                // 设置坐标差值，X方向
                Undo->SetVector(startX - Item->GetCurrentStartX(), 0, startX - Item->GetCurrentStartX(), 0);
                m_UndoStack->push(Undo);
            }
        }
    }
}

void MainWindow::AlignRight()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    double stopX = 0;
    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            stopX = Item->GetCurrentStopX();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(stopX > 0)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                Undo->SetScene(m_pCurScene);
                Undo->AddItem(Item);
                // 设置坐标差值，X方向
                Undo->SetVector(stopX - Item->GetCurrentStopX(), 0, stopX - Item->GetCurrentStopX(), 0);
                m_UndoStack->push(Undo);
            }
        }
    }
}

void MainWindow::AlignTop()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    double startY = 9999;

    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            startY = Item->GetCurrentStartY();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(startY < 9999)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                Undo->SetScene(m_pCurScene);
                Undo->AddItem(Item);
                // 设置坐标差值，Y方向
                Undo->SetVector(0, startY - Item->GetCurrentStartY(), 0, startY - Item->GetCurrentStartY());
                m_UndoStack->push(Undo);
            }
        }
    }
}

void MainWindow::AlignBottom()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    double stopY = 0;
    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            stopY = Item->GetCurrentStopY();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(stopY > 0)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                Undo->SetScene(m_pCurScene);
                Undo->AddItem(Item);
                // 设置坐标差值，Y方向
                Undo->SetVector(0, stopY - Item->GetCurrentStopY(), 0, stopY - Item->GetCurrentStopY());
                m_UndoStack->push(Undo);
            }
        }
    }
}

void MainWindow::AlignHWEqual()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    int W = 0;
    int H = 0;
    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            W = Item->GetStopX() - Item->GetStartX();
            H = Item->GetStopY() - Item->GetStartY();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(W != 0 && H != 0)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                int wDiff = W - (Item->GetStopX() - Item->GetStartX());
                int hDiff = H - (Item->GetStopY() - Item->GetStartY());

                if( wDiff != 0 || hDiff != 0)
                {
                    CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                    Undo->SetScene(m_pCurScene);
                    Undo->AddItem(Item);
                    // 设置坐标差值，XY方向
                    Undo->SetVector(0, 0, wDiff, hDiff);
                    m_UndoStack->push(Undo);
                }
            }
        }
    }
    m_pView->update();
}

void MainWindow::AlignHEqual()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    int H = 0;
    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            H = Item->GetStopY() - Item->GetStartY();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(H != 0)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                int hDiff = H - (Item->GetStopY() - Item->GetStartY());
                if(hDiff != 0)
                {
                    CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                    Undo->SetScene(m_pCurScene);
                    Undo->AddItem(Item);
                    // 设置坐标差值，Y方向
                    Undo->SetVector(0, 0, 0, hDiff);
                    m_UndoStack->push(Undo);
                }
            }
        }
    }
    m_pView->update();
}

void MainWindow::AlignWEqual()
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItemEx *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);
    int W = 0;
    foreach(CoItemEx *Item, ItemList)
    {
        if(Item->m_isfirstSelected)
        {
            W = Item->GetStopX() - Item->GetStartX();
            break;
        }
    }
    foreach(CoItemEx *Item, ItemList)
    {
        if(W != 0)
        {
            if(!Item->m_isfirstSelected)  //对齐标准项不需要移动
            {
                int wDiff = W - (Item->GetStopX() - Item->GetStartX());

                if( wDiff != 0)
                {
                    CoUndoMoveDrag *Undo = new CoUndoMoveDrag();
                    Undo->SetScene(m_pCurScene);
                    Undo->AddItem(Item);
                    // 设置坐标差值，X方向
                    Undo->SetVector(0, 0, wDiff, 0);
                    m_UndoStack->push(Undo);
                }
            }
        }
    }
    m_pView->update();
}

void MainWindow::Up()
{
    MoveItems(0, -1, 0, -1);
}

void MainWindow::Down()
{
    MoveItems(0, 1, 0, 1);
}

void MainWindow::Left()
{
    MoveItems(-1, 0, -1, 0);
}

void MainWindow::Right()
{
    MoveItems(1, 0, 1, 0);
}

void MainWindow::MoveItems(double Dx1, double Dy1, double Dx2, double Dy2)
{
    if(NULL == m_pCurScene)
    {
        return;
    }

    QList<CoItem *> ItemList;
    m_pCurScene->GetItemsSelected(ItemList);

    foreach(CoItem *Item, ItemList)
    {
        Item->MoveTo(Item->GetStartX() + Dx1, Item->GetStartY() + Dy1,
                     Item->GetStopX() + Dx2, Item->GetStopY() + Dy2);
    }
    QList<CoProxyWidget *> WidgetList;
    m_pCurScene->GetItemsSelected(WidgetList);
    foreach(CoProxyWidget *Item, WidgetList)
    {
        Item->MoveTo(Item->GetStartX() + Dx1, Item->GetStartY() + Dy1,
                      Item->GetStopX() + Dx2, Item->GetStopY() + Dy2);
    }
}

void MainWindow::EnableEditAction()
{
    if(m_pCurScene == NULL)
        return;
    QList<CoItem *> List;
    m_pCurScene->GetItemsSelected(List);
    if(List.size() > 1)
    {
        m_ActEditHWEqual->setEnabled(true);
        m_ActEditHEqual->setEnabled(true);
        m_ActEditWEqual->setEnabled(true);
        m_ActAlignTop->setEnabled(true);
        m_ActAlignLeft->setEnabled(true);
        m_ActAlignRight->setEnabled(true);
        m_ActAlignBottom->setEnabled(true);
    }
    else
    {
        m_ActEditHWEqual->setEnabled(false);
        m_ActEditHEqual->setEnabled(false);
        m_ActEditWEqual->setEnabled(false);
        m_ActAlignTop->setEnabled(false);
        m_ActAlignLeft->setEnabled(false);
        m_ActAlignRight->setEnabled(false);
        m_ActAlignBottom->setEnabled(false);
    }
}

void MainWindow::HandleAddDelEvent(int operat, QList<CoItem *> list)
{
    if(CoUndoAddDel::OPERATION_ADD == operat)
    {
        foreach (CoItem *item, list)
        {
            AddItemAttrToMap(item);
        }
    }
    else
    {
        foreach (CoItem *item, list)
        {
            DelItemAttrFromMap(item);
        }
    }
}

void MainWindow::ThreadReadXml()
{

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_isHideAttrWidget)
        {
            QPoint point = frameGeometry().bottomRight() - event->globalPos();
            if(point.x() <= 10)
            {
                m_isHideAttrWidget = false;
                m_AttrWidget->setVisible(!m_isHideAttrWidget);
            }
        }
        if(m_isHideTaskWidget)
        {
            QPoint point = event->globalPos() - frameGeometry().bottomLeft() ;
            if(point.x() <= 10)
            {
                m_isHideTaskWidget = false;
                m_TaskWidget->setVisible(!m_isHideTaskWidget);
            }
        }
        event->accept();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        CoAction *mAction = new CoAction("释放");
        mAction->m_Type = CoItem::Base;
        m_pDrawSelected = mAction;
    }
}



bool MainWindow::SaveSWXML(QString ProName, QString path)
{
    QList<TFlowBase*> Allflowbase;
    QList<TFlowBase *> FlowChartlist ;
    if(m_SceneAttrMap[ProName].flowMap.size() ==0)//Project don't contain flowScene
    {
        return true;
    }
    for(auto it = m_SceneAttrMap[ProName].flowMap.begin();it != m_SceneAttrMap[ProName].flowMap.end();++it)
    {
        for(auto ito = it.value().SceneMap.begin();ito != it.value().SceneMap.end() ;++ito)
        {
            FlowChartlist.clear();
            ito.value()->GetItemsList(FlowChartlist);
            foreach (TFlowBase* folwbs, FlowChartlist) {
                Allflowbase.push_back(folwbs);
            }
        }

    }
    QFile *file = new QFile();
    file->setFileName(path);
    if(!file->exists())
    {
        file->open(QIODevice::WriteOnly);
        QXmlStreamWriter wr(file);
        wr.setAutoFormatting(true);
        wr.writeStartDocument();
        wr.writeStartElement("Data");
        wr.writeStartElement("程序配置");

        wr.writeStartElement("启动按钮配置");
        wr.writeEndElement();

        wr.writeStartElement("安全配置");
        wr.writeEndElement();

        wr.writeStartElement("计时集合");
        wr.writeEndElement();

        wr.writeStartElement("等待指令集合");
        wr.writeEndElement();

        wr.writeStartElement("保护事件配置");
        wr.writeEndElement();

        wr.writeStartElement("汽缸集合");
        wr.writeEndElement();

        wr.writeStartElement("条件判断");
        wr.writeEndElement();

        wr.writeStartElement("通信部分");
        wr.writeEndElement();

        wr.writeStartElement("数据保存");
        wr.writeEndElement();

        wr.writeStartElement("数据上传");
        wr.writeEndElement();

        wr.writeStartElement("运动部分");
        wr.writeStartElement("励磁");
        //
        wr.writeStartElement("输出");
        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","1");
        wr.writeEndElement();

        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","2");
        wr.writeEndElement();

        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","0");
        wr.writeEndElement();

        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","3");
        wr.writeEndElement();

        wr.writeEndElement();
        //
        wr.writeStartElement("输入");
        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","0");
        wr.writeEndElement();

        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","1");
        wr.writeEndElement();

        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","1");
        wr.writeAttribute("bit","2");
        wr.writeEndElement();

        wr.writeStartElement("比特");
        wr.writeAttribute("vaule","0");
        wr.writeAttribute("bit","6");
        wr.writeEndElement();
        wr.writeEndElement();
        //
        wr.writeEndElement();
        wr.writeStartElement("扭矩模式");
        wr.writeEndElement();
        wr.writeStartElement("寸动");
        wr.writeEndElement();
        wr.writeEndElement();
        wr.writeStartElement("托盘指令");
        wr.writeEndElement();

        wr.writeStartElement("运动");
        wr.writeEndElement();

        wr.writeEndElement();
        wr.writeEndElement();
        wr.writeEndDocument();

        file->close();
    }
    if(!file->open(QIODevice::ReadOnly))
    {
        file->close();
        return false;
    }
    QTextStream vReadStream(file);
    QTextCodec *vCodec = QTextCodec::codecForName("UTF-8");
    vReadStream.setCodec(vCodec);
    QString vXmlDataStr = vReadStream.readAll();

    file->close();
    QDomDocument doc;
    if(!doc.setContent(vXmlDataStr))
    {
        file->close();
        return false;
    }
    if(false == file->open(QIODevice::WriteOnly))
        return false;
    bool isok = false;
    for(int i = 0 ;i < Allflowbase.size();++i)
    {
        isok = Allflowbase[i]->PropertyBase->SaveXml(&doc);
        if(isok == false)
        {
            return false;
        }

    }
    QTextStream ts(file);
    ts.reset();
    ts.setCodec("utf-8");
    doc.save(ts, 4, QDomNode::EncodingFromTextStream);
    file->close();
    return isok;
}

bool MainWindow::IsEnd(TFlowBase *p_Fchart)
{
    QList<CoArrow*> ArrowList =  p_Fchart->GetOutputArrows();
    if(ArrowList.size() > 0)
    {
        bool isEnd = false;
        foreach (CoArrow* arrow, ArrowList)
        {
            CoFlowChart * flowchar = arrow->GetEndItem();
            if(flowchar->GetType() == CoItem::FlowEnd)
                isEnd = true;
        }
        return isEnd;
    }
    return true;
}



TFlowBase *MainWindow::GetStartChart(CoScene *Scene)
{
    TFlowBase *flowchar = NULL;
    QList<CoFlowChart *> FlowChartlist ;
    Scene->GetItemsList(FlowChartlist);
    if(FlowChartlist.size() < 3)  //小于 3 个图元
    {
        return flowchar;
    }
    for(int i = 0 ;i < FlowChartlist.size();++i)
    {
        flowchar = dynamic_cast<TFlowBase*>(FlowChartlist[i]) ;
        if(flowchar->GetType() == CoItem::FlowStart) //start flowchart
        {
            return flowchar;
        }
    }
    return NULL;
}

bool MainWindow::writeTxt(char *str, QString filename)
{
    QFile file(filename);
    bool isok = file.open(QIODevice::WriteOnly);
    if(isok)
    {
        file.write(str);
    }
    else
        return false;
    file.close();
    return true;
}

void MainWindow::writeTxtAppend(char *str, QString filename)
{
    QFile file(filename);
    bool isok = file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str);
    }
    file.close();
}
// 子流程
void MainWindow::write(QMap<QString, QMap<int, QString> > &FlowTxtMap,QString path)
{
    for(auto it = FlowTxtMap.begin();it != FlowTxtMap.end();++it)
    {
        char *cr=NULL;
        QByteArray buff;
        for(auto ito = it.value().begin();ito != it.value().end();++ito)
        {
            buff += (QString("%1,%2\n").arg(ito.key()).arg(ito.value())).toUtf8();

        }
        cr = buff.data();
        writeTxt(cr,path +"/" + it.key() + ".txt");
    }
}


QString MainWindow::GetCode(QString str)
{
    QString code = "";
    if(str == "")
        return code;
    code = str.section('\n',0,0);
    return code;

}

bool MainWindow::SaveFlowScene(QString ProName,QString path)
{
    QMap<QString,QMap<QString,QList<TFlowBase*> > >proData;
    proData.clear();
    QMap<QString,QList<TFlowBase*> > childProData;

    QList<TFlowBase *> FlowChartlist ;
    //-------fixed by korol-------
    for(auto it = m_SceneAttrMap[ProName].flowMap.begin();it != m_SceneAttrMap[ProName].flowMap.end();++it)
    {
        childProData.clear();
        for(auto ito = it.value().SceneMap.begin();ito != it.value().SceneMap.end() ;++ito)
        {
            FlowChartlist.clear();
            ito.value()->GetItemsList(FlowChartlist);
            childProData.insert(ito.key(),FlowChartlist);
        }
        proData.insert(it.key(),childProData);
    }

    return(saveflowscene->saveItem_v1(proData,path));

    }

void MainWindow::CreateHWxml(QString path)
{
    QFile *file = new QFile();
    file->setFileName(path);
    if(!file->exists())
    {
        file->open(QIODevice::WriteOnly);
        QXmlStreamWriter wr(file);
        wr.setAutoFormatting(true);
        wr.writeStartDocument();
        wr.writeStartElement("data");
        wr.writeStartElement("硬件配置");

        wr.writeStartElement("网口配置");
        wr.writeEndElement();

        wr.writeStartElement("串口配置");
        wr.writeEndElement();

        wr.writeStartElement("网络配置");
        wr.writeEndElement();

        wr.writeStartElement("伺服控制模式");
        wr.writeEndElement();

        wr.writeStartElement("OUTPUT");
        wr.writeEndElement();

        wr.writeStartElement("INPUT");
        wr.writeEndElement();

        wr.writeStartElement("坐标系配置");
        wr.writeEndElement();

        wr.writeEndElement();
        wr.writeEndElement();
        wr.writeEndDocument();

        file->close();
    }
}
void MainWindow::ZoomOut()
{
    if(m_pView)
    {
        m_pView->Zoom(m_pView->GetZoom()+5);
    }
}

void MainWindow::ZoomIn()
{
    if(m_pView)
    {
        m_pView->Zoom(m_pView->GetZoom()-5);
    }
}

void MainWindow::ZoomNormal()
{
    if(m_pView)
    {
        m_pView->SetZoom(0);
    }
}

void MainWindow::SetGridLine()
{
    bool isGrid = m_ActGrid->isChecked();
    foreach (PROJECT_STRU proStru, m_SceneAttrMap.values())
    {
        proStru.SetGrid(isGrid);
    }
}

void MainWindow::SetDebugMode()
{
    bool isDebug = m_ActDebug->isChecked();
    if(isDebug)
    {
        foreach (PROJECT_STRU proStru, m_SceneAttrMap.values())
        {
            proStru.SetDebugMode(true);
        }
//        OpenXml();
//        if(!CreateCommunicator())
//            return;
//        //todo 1.设置网口名;2.设置I/O字节数组;3.打开EtherCat;4.等待EC打开成功.
//        QByteArray ba = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlNetworkStru.portName.toLatin1();
//        InOutPutData::GetInstance()->ifName = new char[ba.length()];
//        strcpy(InOutPutData::GetInstance()->ifName, ba.data());
//        if(InOutPutData::GetInstance()->inPutNum > 0 && InOutPutData::GetInstance()->outPutNum > 0)
//        {
//            memset(InOutPutData::GetInstance()->inPutData,0,InOutPutData::GetInstance()->inPutNum);
//            memset(InOutPutData::GetInstance()->outPutData,0,InOutPutData::GetInstance()->outPutNum);
//            m_etherCat.Start();
//        }
//        else
//        {
//            CoreLog::QLog_Error(defaultLogModule,QString("硬件参数设置有误，没有找到In/Output数据！"));
//        }
    }
    else
    {
        m_etherCat.Stop();
        foreach (PROJECT_STRU proStru, m_SceneAttrMap.values())
        {
            proStru.SetDebugMode(isDebug);
        }
    }

}

void MainWindow::SetHost()
{
    if(m_pCurScene)
    {
        m_SceneHost = m_pCurScene->GetId();
    }
}


bool MainWindow::GetScenCode(CoScene *Scene,QVector<QString> &TxtVec,QString &Err)
{
    if(Scene == NULL)
    {
        return true;
    }
    TxtVec.clear();
    Err = "";
    QList<CoFlowChart *> FlowChartlist ;
    Scene->GetItemsList(FlowChartlist);
    if(FlowChartlist.size() < 3)  //小于 3 个图元
    {
        Err = "Flow_main Miswrite \n Char Count < 3!";
        return false;
    }
    TFlowBase *flowchar = NULL;
    bool startflow = false;
    bool endflow = false;
    for(int i = 0 ;i < FlowChartlist.size();++i)
    {
        if(FlowChartlist[i]->GetType() == CoItem::FlowStart) //start flowchart
        {
            flowchar = dynamic_cast<TFlowBase*> (FlowChartlist[i]);
            startflow = true;
            break;
        }
    }
    if(!startflow)
    {
        Err = "Check the End or Start exists or not!";
        return false;
    }
    QList<CoArrow*>OutputArrow = flowchar->GetOutputArrows();
    if(OutputArrow.size() == 0)
    {
        Err = "Flow_main Miswrite !";
        return false;
    }
    for(int j = 0; j < OutputArrow.size();++j)
    {
        TFlowBase *Tchart = dynamic_cast<TFlowBase*>(OutputArrow[j]->GetEndItem());
        if(Tchart->GetType() == CoItem::Flowflow)//流程
        {
            //FlowVec.push_back(GetCode(Tchart->Code));
        }
        else  //指令块，后再无流程只有线程,
        {
            if(!(Tchart->GetType() == CoItem::IF && GetCode(Tchart->Code).contains("Jump")))
            {
                TxtVec.push_back(GetCode(Tchart->Code));
            }
            flowchar = Tchart;
        }
    }
    while(!IsEnd(flowchar))
    {
        bool ifsb = false;
        // Get chart
        QList<CoArrow*> ArrowList =  flowchar->GetOutputArrows();

        if(flowchar->GetType() == CoItem::IF)
        {
            if(ArrowList.size() >2)
            {
                Err = "IF Miswrite Number of lines > 2 or < 1!";
                return false;
            }
            else if(ArrowList.size() == 2)
            {
                {
                    if(ArrowList[0]->GetYNPolicy() == ArrowList[1]->GetYNPolicy())
                    {
                        Err = "IF Miswrite Jump 指令\n两条连线极性不可以一样！";
                        return false;
                    }
                    for(int i = 0;i <ArrowList.size();++i) //NoPolicy frist!
                    {
                        if(ArrowList[i]->GetYNPolicy() == false)
                        {
                            TFlowBase * ifchart = dynamic_cast<TFlowBase*>(ArrowList[i]->GetEndItem());
                            if(ifchart->GetType() == CoItem::IDs)//NO 属性必须指向label
                            {
                                 TxtVec.push_back(GetCode(flowchar->Code)+GetCode(ifchart->Code));
                            }
                            else
                            {
                                Err = "IF 指令NO属性只能指向Label！\n 请检查流程图";
                                return false;
                            }
                        }
                    }
                    for(int k = 0;k <ArrowList.size();++k) //yes policy
                    {
                        if(ArrowList[k]->GetYNPolicy() == true)
                        {
                            flowchar = dynamic_cast<TFlowBase*>(ArrowList[k]->GetEndItem());
                            if(false == (flowchar->GetType() == CoItem::IF && flowchar->Code.contains("Jump")))
                            {
                                TxtVec.push_back(GetCode(flowchar->Code));
                            }
                            ifsb = true;
                        }
                    }

                }

            }
            else
            {
                flowchar = dynamic_cast<TFlowBase*>(ArrowList[0]->GetEndItem());
                TxtVec.push_back(GetCode(flowchar->Code));
                ifsb = true;
            }
            if(false == ifsb)//if contains next flowitems (no yespolicy)
            {
                Err = "IF Miswrite No YesPolicy!";
                return false;
            }
        }
        else
        {
             for(int j = 0; j < ArrowList.size();++j)
             {
                flowchar = dynamic_cast<TFlowBase*>(ArrowList[j]->GetEndItem());
                if(!(GetCode(flowchar->Code).contains("IF")&&GetCode(flowchar->Code).contains("Jump")))
                {
                    TxtVec.push_back(GetCode(flowchar->Code));
                }
             }
        }


    }
    return true;
}

bool MainWindow::WriteFlowScene(QString path, QVector<QString>flow)
{

    QByteArray buff ;
    for(int i = 0;i < flow.size();++i)
    {
        buff += (QString("%1,").arg(i).toLatin1() + flow[i].toUtf8() + '\n');
    }
    char *cr = buff.data();
    return writeTxt(cr,path);
}

void MainWindow::MakeDir(QDir &dir,const QString path)
{
    if(!dir.exists(path))
    {
        dir.mkdir(path);
    }
}
///
/// \brief MainWindow::HandelFlowScene_v1
/// \param PorStu
/// \param path
/// \param Err
/// \return
///
bool MainWindow::HandelFlowScene_v1(QString PorStu, QString path, QString &Err)
{
    QString flowpath = path +"/";
    QDir dir;
    MakeDir(dir,flowpath);
    //先解析复位总复位
    QString resetPath = flowpath + "/" +"总复位/";
    MakeDir(dir,resetPath);
    QString resetThreadPath = resetPath + "线程/";
     MakeDir(dir,resetThreadPath);
    QVector<QString> TxtVec;
    for(auto it = m_SceneAttrMap[PorStu].flowMap.begin(); it != m_SceneAttrMap[PorStu].flowMap.end();++it)
    {
        if(it.key() =="总复位")
        {
           for(auto ir = it.value().SceneMap.begin();ir != it.value().SceneMap.end();++ir)
           {
               if(ir.key() == "总复位")
               {
                   if(GetScenCode(ir.value(),TxtVec,Err))
                   {
                       WriteFlowScene(resetPath+"总复位.txt",TxtVec);
                       TxtVec.clear();
                   }
                   else
                       return false;
               }
               else//复位线程
               {
                   if(GetScenCode(ir.value(),TxtVec,Err))
                   {
                       WriteFlowScene(resetThreadPath+ir.key()+".txt",TxtVec);
                       TxtVec.clear();
                   }
                   else
                       return false;
               }
           }
        }
        else if (it.key() == "回原点")
        {
            QString homePath = flowpath + "/" +"回原点/";
            MakeDir(dir,homePath);
            QString homeThreadPath = homePath + "线程/";
            MakeDir(dir,homeThreadPath);
            for(auto ito = it.value().SceneMap.begin();ito != it.value().SceneMap.end();++ito)
            {
                if(ito.key() == "回原点")
                {
                    if(GetScenCode(ito.value(),TxtVec,Err))
                    {
                        WriteFlowScene(homePath+"回原点.txt",TxtVec);
                        TxtVec.clear();
                    }
                    else
                        return false;
                }
                else//复位线程
                {
                    if(GetScenCode(ito.value(),TxtVec,Err))
                    {
                        WriteFlowScene(homeThreadPath+ito.key()+".txt",TxtVec);
                        TxtVec.clear();
                    }
                    else
                        return false;
                }
            }

        }
        else
        {
            QString partPath = flowpath + "/" +"流程/";
            MakeDir(dir,partPath);
            QString partdial = partPath + it.key() + "/";
            MakeDir(dir,partdial);
            QString partThreadPath = partdial + "线程/";
            MakeDir(dir,partThreadPath);
            for(auto ic = it.value().SceneMap.begin();ic != it.value().SceneMap.end();++ic)
            {
                if(ic.key() == it.key()||ic.key() == "子复位")
                {
                    if(GetScenCode(ic.value(),TxtVec,Err))
                    {
                        WriteFlowScene(partdial+ic.key()+".txt",TxtVec);
                        TxtVec.clear();
                    }
                    else
                        return false;
                }
                else//流程线程
                {
                    if(GetScenCode(ic.value(),TxtVec,Err))
                    {
                        WriteFlowScene(partThreadPath+ic.key()+".txt",TxtVec);
                        TxtVec.clear();
                    }
                    else
                        return false;
                }
            }
        }
    }
    return true;
}
int MainWindow::CreateMsgBox(QMessageBox::Icon ico, const QString strTitle,
                             const QString strMsg,QMessageBox::StandardButtons buttons)
{
    QMessageBox msgBox(ico,strTitle,strMsg,buttons,this);
    msgBox.setStyleSheet("QPushButton {"
                         "background-color:#89AFDE;"" border-style: outset;"
                         " border-width: 1px;"" border-radius: 5px;"" border-color: beige;"
                         " font:14px;"" min-width: 6em;"" min-height:1.5em;"
                         "}"
                         "QLabel { min-height:3em;font:14px;}"
                         );
    return msgBox.exec();
}

void MainWindow::GetCurSceneItemId(CoScene *Scene)
{
    if(Scene->GetId() > m_SceneId)
    {
        m_SceneId = Scene->GetId();
    }
    QList<CoItem *>ItemList;
    Scene->GetItemsList(ItemList);
    QList<CoProxyWidget *> WidgetList;
    Scene->GetItemsList(WidgetList);
    foreach(CoItem *Item, ItemList)
    {
        if(Item->GetId() > m_ItemId)
            m_ItemId = Item->GetId();
        SetItem(Item);
    }
    foreach(CoProxyWidget *widget, WidgetList)
    {
        if(widget->GetId() > m_ItemId)
            m_ItemId = widget->GetId();
    }
}

void MainWindow::OpenXml()
{
    if(m_pCurScene)
    {
        XmlManager xml;
        QString path = QCoreApplication::applicationDirPath() + "/" + m_pCurScene->GetProjectName();
        QString ConfigPath = path+"/硬件配置.xml";
        QString str = xml.ReadHWConfigXml(ConfigPath,ConfigData::GetInstance()->xmlMap.hwConfigXml);
        if(!str.isEmpty())
            CoreLog::QLog_Error(defaultLogModule,str);
        int iNum = 0;
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwIOMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwIOMap.end();it++)
        {
            if(it.value().strType == "EC")
                iNum += it.value().iByteNum;
            ConfigData::GetInstance()->hwIOMap.insert(it.key(),it.value());
        }
        for(auto it2 = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.begin();
            it2 != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.end();it2++)
        {
            iNum += it2.value().iByteNum;

        }
        if(iNum > 0)
        {
            InOutPutData::GetInstance()->inPutNum = iNum;
            InOutPutData::GetInstance()->inPutData = new uchar[iNum];
            memset(InOutPutData::GetInstance()->inPutData,0,iNum);
        }
        iNum = 0;
        for(auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwIOMap.begin();
            it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwIOMap.end();it++)
        {
            if(it.value().strType == "EC")
                iNum += it.value().iByteNum;
            ConfigData::GetInstance()->hwIOMap.insert(it.key(),it.value());
        }
        for(auto it2 = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.begin();
            it2 != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.end();it2++)
        {
            iNum += it2.value().iByteNum;

        }
        if(iNum > 0)
        {
            InOutPutData::GetInstance()->outPutNum = iNum;
            InOutPutData::GetInstance()->outPutData = new uchar[iNum];
            memset(InOutPutData::GetInstance()->outPutData,0,iNum);
        }
    }
}
void MainWindow::closeEvent(QCloseEvent *ev)
{ 
    int result = CreateMsgBox(QMessageBox::Warning,tr("警告"),tr("画布数据是否已保存?"),
                              QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == result)
    {
        ev->accept();
    }
    else
    {
        ev->ignore();
    }
}


bool MainWindow::IsDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
        return true;
    }
    else
    {
        bool ok = dir.mkdir(fullPath);
        return ok;
    }
    return false;
}
bool MainWindow::SaveToFile(const QString ProName)
{
    QString saveDirName = QCoreApplication::applicationDirPath() + "/" + ProName;
    if(!IsDirExist(saveDirName))
        return false;
    if(0 == m_SceneHost)
        return false;
    TXmlWriter mXml;

    mXml.AddHead();
    // 添加正文
    QDomElement mXmlRootScada = mXml.AddRoot(tr("Data"));

    // scene
    QDomElement mXmlScenes = mXml.AddChild(mXmlRootScada, tr("Scenes"));
    mXmlScenes.setAttribute(tr("Name"), ProName);
    mXmlScenes.setAttribute(tr("Host"), m_SceneHost);
    mXmlScenes.setAttribute(tr("GridLine"), m_ActGrid->isChecked());
    mXmlScenes.setAttribute(tr("Width"), m_SceneAttrMap[ProName].viewWidth);
    mXmlScenes.setAttribute(tr("Height"), m_SceneAttrMap[ProName].viewHeight);

    //保存view的旋转属性
    int rotate = 0;
    float m11 = m_pView->transform().m11();
    float m12 = m_pView->transform().m12();
    float m21 = m_pView->transform().m21();
    float m22 = m_pView->transform().m22();
    if(m11 == 1 && m12 == 0 && m21 == 0 && m22 == 1)
        rotate = 0;
    else if(m11 == 0 && m12 == 1 && m21 == -1 && m22 == 0)
        rotate = 90;
    else if(m11 == 0 && m12 == -1 && m21 == 1 && m22 == 0)
        rotate = -90;
    else if(m11 == -1 && m12 == 0 && m21 == 0 && m22 == -1)
        rotate = 180;
    mXmlScenes.setAttribute(tr("Rotate"),rotate);
    if(m_SceneAttrMap[ProName].sceneMap.size() > 0)
    {
        QDomElement mXmlCanvas = mXml.AddChild(mXmlScenes, tr("Canvas"));
        foreach (CoScene *AddScene, m_SceneAttrMap[ProName].sceneMap.values())
        {
            QDomElement mXmlScene = mXml.AddChild(mXmlCanvas, tr("scene"));
            mXml.AddItems(mXmlScene, AddScene);
        }
    }
    if(m_SceneAttrMap[ProName].safeScene)
    {
        QDomElement mXmlSafe = mXml.AddChild(mXmlScenes, tr("SafeConfig"));
        mXml.AddItems(mXmlSafe, m_SceneAttrMap[ProName].safeScene);
    }
    if(m_SceneAttrMap[ProName].flowMap.size() > 0)
    {
        if(false == SaveFlowScene(ProName,saveDirName+"/scene.item"))
            return false;
        // Flow flowMap
        QDomElement mXmlFlow = mXml.AddChild(mXmlScenes, tr("FlowChart"));
        for(auto item = m_SceneAttrMap[ProName].flowMap.begin();
            item != m_SceneAttrMap[ProName].flowMap.end();++item)
        {
            QString flowName = item.key();
            QDomElement mXmlScene = mXml.AddChild(mXmlFlow, tr("scene"));
            QDomElement mXmlThreads = mXml.AddChild(mXmlScene, tr("Threads"));
            for(auto it = item.value().SceneMap.begin();it != item.value().SceneMap.end();++it)
            {
                QDomElement mXmlTemp;
                if(it.key().compare(flowName) == 0)
                {
                    mXmlTemp = mXmlScene;
                }
                else if(it.key().compare("子复位") == 0)
                {
                    QDomElement mXmlChildReset = mXml.AddChild(mXmlScene, tr("childReset"));
                    mXmlTemp = mXmlChildReset;
                }
                else
                {
                    QDomElement mXmlThread = mXml.AddChild(mXmlThreads, tr("thread"));
                    mXmlTemp = mXmlThread;
                }
                mXmlTemp.setAttribute(tr("Id"), it.value()->GetId());
                mXmlTemp.setAttribute(tr("Name"), it.value()->GetName());
                mXmlTemp.setAttribute(tr("BackColor"), it.value()->GetBackColor().name(QColor::HexArgb));
                mXmlTemp.setAttribute(tr("Width"), it.value()->GetWidth());
                mXmlTemp.setAttribute(tr("Height"), it.value()->GetHeight());
                QList<CoArrow *>ItemList;
                it.value()->GetItemsList(ItemList);
                QDomElement XmlItem = mXml.AddChild(mXmlTemp, tr("Items"));
                foreach(CoArrow *Item, ItemList)
                {
                    mXml.AddArrow(XmlItem, Item);
                }
            }

        }
    }
    if(!mXml.Save(saveDirName+"/UI配置.xml"))
    {
        CreateMsgBox(QMessageBox::Warning,tr("保存"),tr("保存失败，请重试"));
    }
    else{
        CreateMsgBox(QMessageBox::Warning,tr("保存"),tr("UI配置.xml保存成功!"));
    }
    if(!SaveSWXML(ProName,saveDirName+"/软件参数配置.xml"))
    {
        CreateMsgBox(QMessageBox::Warning,tr("保存"),tr("软件配置保存失败，请重试"));
    }
    else{
        CreateMsgBox(QMessageBox::Warning,tr("保存"),tr("软件参数配置.xml保存成功!"));
    }
    QString err = "";
    if(false == HandelFlowScene_v1(ProName,saveDirName,err))
    {
        CreateMsgBox(QMessageBox::Warning,tr("保存"),err);
    }
    else{
        CreateMsgBox(QMessageBox::Warning,tr("保存"),tr("流程图文件保存成功!"));
    }

    return true;
}
