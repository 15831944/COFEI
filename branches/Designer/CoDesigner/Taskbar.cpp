#include "Taskbar.h"
#include <CoWidgetBox/TaskBar/Iconlabel.h>
#include <CoWidgetBox/TaskBar/Taskgroup.h>
#include <CoWidgetBox/TaskBar/Taskheader.h>
#include <CoGraphics/Graphics.h>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
TaskBar::TaskBar(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::Tool |Qt::CustomizeWindowHint |
                         Qt::WindowTitleHint |Qt::WindowStaysOnTopHint);

    QVBoxLayout *vbox = new QVBoxLayout();
    this->setLayout(vbox);
    QScrollArea *Area = new QScrollArea(this);
    vbox->addWidget(Area);
    m_panel = new CoTaskBar(Area);
    Area->setWidget(m_panel);

    CoTaskGroup *taskGroup = new CoTaskGroup(m_panel);
    m_panel->addWidget(taskGroup);
    taskGroup->addIconLabel(CreateIconLabel(":images/draw/Free.png","释放",CoItem::Base,taskGroup));

    CoTaskBox *drawTaskBox = new CoTaskBox(QPixmap(":/images/draw/widget.png"), "绘图工具", true, m_panel);
    m_panel->addWidget(drawTaskBox);

    QHBoxLayout *drawHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(drawHBox);
    CoIconLabel *pix = CreateIconLabel(":images/draw/Pixmap.png","图片",CoItem::Pixmap,drawTaskBox);
    drawTaskBox->addIconLabel(pix,false);
    drawHBox->addWidget(pix);
    CoIconLabel *groupbox = CreateIconLabel(":images/draw/groupbox.png","组框",CoItem::GroupBox,drawTaskBox);
    drawTaskBox->addIconLabel(groupbox,false);
    drawHBox->addWidget(groupbox);

    QHBoxLayout *chartHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(chartHBox);
    CoIconLabel *curve = CreateIconLabel(":images/draw/chart.png","曲线",CoItem::Chart,drawTaskBox);
    drawTaskBox->addIconLabel(curve,false);
    CoIconLabel *TableView = CreateIconLabel(":images/draw/Table.png","表格",CoItem::TableView,drawTaskBox);
    drawTaskBox->addIconLabel(TableView,false);
    chartHBox->addWidget(curve);
    chartHBox->addWidget(TableView);

    QHBoxLayout *widgetHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(widgetHBox);
    CoIconLabel *btn = CreateIconLabel(":images/draw/Button.png","按钮",CoItem::Button,drawTaskBox);
    drawTaskBox->addIconLabel(btn, false);
    widgetHBox->addWidget(btn);
    CoIconLabel *calBtn = CreateIconLabel(":images/draw/calibration.png","示教",CoItem::CalBtn,drawTaskBox);
    drawTaskBox->addIconLabel(calBtn, false);
    widgetHBox->addWidget(calBtn);

    widgetHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(widgetHBox);
    CoIconLabel *text = CreateIconLabel(":images/draw/TextBox.png","文本框",CoItem::TextEdit,drawTaskBox);
    drawTaskBox->addIconLabel(text, false);
    widgetHBox->addWidget(text);
    CoIconLabel *lable = CreateIconLabel(":images/draw/Lable.png","标签",CoItem::Label,drawTaskBox);
    drawTaskBox->addIconLabel(lable, false);
    widgetHBox->addWidget(lable);

    widgetHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(widgetHBox);
    CoIconLabel *iolamp = CreateIconLabel(":images/draw/Light.png","IO灯",CoItem::Iolamp,drawTaskBox);
    drawTaskBox->addIconLabel(iolamp,false);
    CoIconLabel *warnlamp = CreateIconLabel(":images/draw/WarnLamp.png","警示灯",CoItem::Warnlamp,drawTaskBox);
    drawTaskBox->addIconLabel(warnlamp,false);
    widgetHBox->addWidget(iolamp);
    widgetHBox->addWidget(warnlamp);

    widgetHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(widgetHBox);
    CoIconLabel *sigLevel = CreateIconLabel(":images/draw/SigLevel.png","信号等级",CoItem::Mde_Safe_Sig,drawTaskBox);
    drawTaskBox->addIconLabel(sigLevel,false);
    CoIconLabel *safestate = CreateIconLabel(":images/draw/safeState.png","安全状态",CoItem::SafeState,drawTaskBox);
    drawTaskBox->addIconLabel(safestate,false);
    widgetHBox->addWidget(sigLevel);
    widgetHBox->addWidget(safestate);

    widgetHBox = new QHBoxLayout();
    drawTaskBox->groupLayout()->addLayout(widgetHBox);
    CoIconLabel *gpOutput= CreateIconLabel(":images/draw/Button.png","GPIO写",CoItem::GPOutPut,drawTaskBox);
    drawTaskBox->addIconLabel(gpOutput,false);
    CoIconLabel *gpInput = CreateIconLabel(":images/draw/Light.png","GPIO读",CoItem::GPInPut,drawTaskBox);
    drawTaskBox->addIconLabel(gpInput,false);
    widgetHBox->addWidget(gpOutput);
    widgetHBox->addWidget(gpInput);

    CoTaskBox *ModuleTaskBox = new CoTaskBox(QPixmap(":/images/draw/graphics.png"), "自定义模块", true, m_panel);
    m_panel->addWidget(ModuleTaskBox);
    QHBoxLayout *ModuleHBox = new QHBoxLayout();
    ModuleTaskBox->groupLayout()->addLayout(ModuleHBox);
    CoIconLabel *module1 = CreateIconLabel(":images/draw/modules.png","PP示教模块",CoItem::Mde_Axis_dug,ModuleTaskBox);
    ModuleTaskBox->addIconLabel(curve,false);
    CoIconLabel *module2 = CreateIconLabel(":images/draw/modules.png","登录模块",CoItem::Mde_Login,ModuleTaskBox);
    ModuleTaskBox->addIconLabel(TableView,false);
    ModuleHBox->addWidget(module1);
    ModuleHBox->addWidget(module2);


    CoTaskBox *flowTaskBox = new CoTaskBox(QPixmap(":/images/Flow.png"), "基础流程图", true, m_panel);
    m_panel->addWidget(flowTaskBox);

    QHBoxLayout *flowhbox = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox);
    CoIconLabel *folw_rect = CreateIconLabel(":/images/draw/dely.ico","T",CoItem::Dely,flowTaskBox);
    flowTaskBox->addIconLabel(folw_rect);
    CoIconLabel *folw_arrow = CreateIconLabel(":/images/draw/arrow.ico","连线",CoItem::FlowArrow,flowTaskBox);
    flowTaskBox->addIconLabel(folw_arrow);
    flowhbox->addWidget(folw_rect);
    flowhbox->addWidget(folw_arrow);

    QHBoxLayout *flowhbox_1 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_1);
    CoIconLabel *folw_start = CreateIconLabel(":/images/draw/start.ico","开始",CoItem::FlowStart,flowTaskBox);
    flowTaskBox->addIconLabel(folw_start);
    CoIconLabel *folw_end = CreateIconLabel(":/images/draw/end.ico","结束",CoItem::FlowEnd,flowTaskBox);
    flowTaskBox->addIconLabel(folw_end);
    flowhbox_1->addWidget(folw_start);
    flowhbox_1->addWidget(folw_end);

    QHBoxLayout *flowhbox_2 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_2);
    //CoIconLabel *folw_process = CreateIconLabel(":/images/draw/thread.ico","线程",CoItem::FlowThread,flowTaskBox);
    CoIconLabel *flow_G101 = CreateIconLabel(":/images/draw/torque.ico","G101",CoItem::G101,flowTaskBox);
    flowTaskBox->addIconLabel(flow_G101);
    CoIconLabel *folw_G00 = CreateIconLabel(":/images/draw/g00.ico","G00",CoItem::G00,flowTaskBox);
    flowTaskBox->addIconLabel(folw_G00);
    flowhbox_2->addWidget(flow_G101);
    flowhbox_2->addWidget(folw_G00);

    QHBoxLayout *flowhbox_3 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_3);
    CoIconLabel *folw_M00 = CreateIconLabel(":/images/draw/cycling.ico","M00",CoItem::FlowSport,flowTaskBox);
    flowTaskBox->addIconLabel(folw_M00);
    CoIconLabel *folw_if = CreateIconLabel(":/images/draw/if.ico","   IF",CoItem::IF,flowTaskBox);
    flowTaskBox->addIconLabel(folw_if);
    flowhbox_3->addWidget(folw_M00);
    flowhbox_3->addWidget(folw_if);

//    QHBoxLayout *flowhbox_4 = new QHBoxLayout();
//    flowTaskBox->groupLayout()->addLayout(flowhbox_4);
//    CoIconLabel *folw_IN = CreateIconLabel(":/images/draw/inin.png","IN",CoItem::In,flowTaskBox);
//    flowTaskBox->addIconLabel(folw_IN);
//    CoIconLabel *folw_OUT = CreateIconLabel(":/images/draw/outo.png","OUT",CoItem::Out,flowTaskBox);
//    flowTaskBox->addIconLabel(folw_OUT);
//    flowhbox_4->addWidget(folw_OUT);
//    flowhbox_4->addWidget(folw_IN);

//    QHBoxLayout *flowhbox_5 = new QHBoxLayout();
//    flowTaskBox->groupLayout()->addLayout(flowhbox_5);
//    CoIconLabel *folw_LIN = CreateIconLabel(":/images/draw/LIn.png","LIN",CoItem::Lin,flowTaskBox);
//    flowTaskBox->addIconLabel(folw_LIN);
//    CoIconLabel *folw_LOUT = CreateIconLabel(":/images/draw/LOut.png","LOUT",CoItem::Lout,flowTaskBox);
//    flowTaskBox->addIconLabel(folw_LOUT);
//    flowhbox_5->addWidget(folw_LOUT);
//    flowhbox_5->addWidget(folw_LIN);

    QHBoxLayout *flowhbox_6 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_6);
    CoIconLabel *folw_flow = CreateIconLabel(":/images/draw/Process.ico","模块组",CoItem::Flowflow,flowTaskBox);
    flowTaskBox->addIconLabel(folw_flow);
    CoIconLabel *flow_label = CreateIconLabel(":/images/draw/label.png","标签",CoItem::IDs,flowTaskBox);
    flowTaskBox->addIconLabel(flow_label);
    flowhbox_6->addWidget(folw_flow);
    flowhbox_6->addWidget(flow_label);

//    QHBoxLayout *flowhbox_8 = new QHBoxLayout();
//    flowTaskBox->groupLayout()->addLayout(flowhbox_8);
//    CoIconLabel *folw_call = CreateIconLabel(":/images/draw/Process.ico","CALL",CoItem::FlowCall,flowTaskBox);
//    flowTaskBox->addIconLabel(folw_call);
   // CoIconLabel *flow_G101 = CreateIconLabel(":/images/draw/torque.ico","G101",CoItem::G101,flowTaskBox);
  //  flowTaskBox->addIconLabel(flow_G101);
   // flowhbox_8->addWidget(folw_call);
   // flowhbox_8->addWidget(flow_G101);

    QHBoxLayout *flowhbox_9 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_9);
    CoIconLabel *folw_G04= CreateIconLabel(":/images/draw/location.ico","  G04",CoItem::G04,flowTaskBox);
    flowTaskBox->addIconLabel(folw_G04);
    CoIconLabel *flow_JS = CreateIconLabel(":/images/draw/script.ico","JS",CoItem::Js,flowTaskBox);
    flowTaskBox->addIconLabel(flow_JS);
    flowhbox_9->addWidget(folw_G04);
    flowhbox_9->addWidget(flow_JS);

    QHBoxLayout *flowhbox_10 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_10);
    CoIconLabel *folw_TCPW= CreateIconLabel(":/images/draw/tcp.ico","TCPW",CoItem::Tcp_W,flowTaskBox);
    flowTaskBox->addIconLabel(folw_TCPW);
    CoIconLabel *folw_TCPR = CreateIconLabel(":/images/draw/tcp.ico","TCPR",CoItem::Tcp_R,flowTaskBox);
    flowTaskBox->addIconLabel(folw_TCPR);
    flowhbox_10->addWidget(folw_TCPW);
    flowhbox_10->addWidget(folw_TCPR);

    QHBoxLayout *flowhbox_11 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_11);
    CoIconLabel *folw_ORG= CreateIconLabel(":/images/draw/org.ico","ORG",CoItem::Org,flowTaskBox);
    flowTaskBox->addIconLabel(folw_ORG);
        CoIconLabel *folw_IN = CreateIconLabel(":/images/draw/Switch.ico"," IO ",CoItem::In,flowTaskBox);
        flowTaskBox->addIconLabel(folw_IN);
    flowhbox_11->addWidget(folw_ORG);
    flowhbox_11->addWidget(folw_IN);

    QHBoxLayout *flowhbox_12 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_12);
    CoIconLabel *folw_Wait= CreateIconLabel(":/images/draw/Wait.png"," Wait",CoItem::Wait,flowTaskBox);
    flowTaskBox->addIconLabel(folw_Wait);
        CoIconLabel *folw_Log = CreateIconLabel(":/images/draw/LOG.png"," Log ",CoItem::Log,flowTaskBox);
        flowTaskBox->addIconLabel(folw_Log);
    flowhbox_12->addWidget(folw_Wait);
    flowhbox_12->addWidget(folw_Log);


    QHBoxLayout *flowhbox_13 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_13);
    CoIconLabel *folw_Ds= CreateIconLabel(":/images/draw/doubleStart.ico"," 双启动",CoItem::Ds,flowTaskBox);
    CoIconLabel *folw_Rs= CreateIconLabel(":/images/draw/Serial.ico"," Uart",CoItem::RS_R,flowTaskBox);
    flowTaskBox->addIconLabel(folw_Ds);
    flowTaskBox->addIconLabel(folw_Rs);
    flowhbox_13->addWidget(folw_Ds);
    flowhbox_13->addWidget(folw_Rs);

    QHBoxLayout *flowhbox_14 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_14);
    CoIconLabel *folw_Cyt= CreateIconLabel(":/images/draw/cyt.ico","  计时",CoItem::Cyt,flowTaskBox);
    CoIconLabel *folw_Cnt= CreateIconLabel(":/images/draw/Count.ico","  计数",CoItem::Add,flowTaskBox);
    flowTaskBox->addIconLabel(folw_Cyt);
    flowTaskBox->addIconLabel(folw_Cnt);
    flowhbox_14->addWidget(folw_Cyt);
    flowhbox_14->addWidget(folw_Cnt);

    QHBoxLayout *flowhbox_15 = new QHBoxLayout();
    flowTaskBox->groupLayout()->addLayout(flowhbox_15);
    CoIconLabel *folw_Jog= CreateIconLabel(":/images/draw/Jog.ico","  Jog",CoItem::Jog,flowTaskBox);
    CoIconLabel *folw_B= CreateIconLabel(":/images/draw/Jog.ico","  备份",CoItem::Jog,flowTaskBox);
    flowTaskBox->addIconLabel(folw_Jog);
    flowTaskBox->addIconLabel(folw_B);
    flowhbox_15->addWidget(folw_Jog);
    flowhbox_15->addWidget(folw_B);


    m_panel->addStretch();
    m_panel->setFixedSize(240,700);
    this->setFixedSize(264,725);
    this->setWindowTitle("工具栏");
}

void TaskBar::HideTaskBox(QList<QString> tBoxNameList)
{
    QObjectList list = m_panel->children();
    foreach (QObject *obj, list)
    {
        if(obj->metaObject()->className() == QStringLiteral("CoTaskBox"))
        {
            CoTaskBox *tb = qobject_cast<CoTaskBox*>(obj);
            if(tBoxNameList.contains(tb->GetTitle()))
            {
                tb->showHide();
            }
        }
    }
}

void TaskBar::IconLabelClicked()
{
    CoIconLabel *il = qobject_cast<CoIconLabel*>(sender());
    emit SendBarItemClick(il->Action());
}

CoIconLabel *TaskBar::CreateIconLabel(const QString strPath, const QString strText, const int type, QWidget *parent)
{
    CoIconLabel *iL = new CoIconLabel(QPixmap(strPath), strText, parent);
    iL->SetActionType(type);
    if(type == CoItem::Base)
        iL->Action()->setCheckable(false);
    else
        iL->Action()->setCheckable(true);
    iL->setFixedSize(120,25);
    iL->setFont(QFont("黑体",11));
    connect(iL,&CoIconLabel::clicked,this,&TaskBar::IconLabelClicked);
    return iL;
}
