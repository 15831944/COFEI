#include "flywidget.h"
#include <QMessageBox>
#include "mainwindow.h"
FlyWidget::FlyWidget(QWidget *parent) :
    QWidget(parent)
{
    pe = parent;
    m_pSettingMenu = NULL;
    m_isClicked = false;
    m_isMove = false;
    m_isEnter = false;
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::ToolTip);
    m_OldPos = QPoint(pe->width()-50,pe->height()-50);
    m_pTimer = new QTimer(this);
    connect(m_pTimer,&QTimer::timeout,this,&FlyWidget::TimerUpdate);
    CreateBtn();

    InitData();
    InitUI();
    InitConnect();
    setAttribute(Qt::WA_TranslucentBackground); 
}
void FlyWidget::TimerUpdate()
{
    m_pTimer->stop();
    m_isClicked = false;
    m_isEnter = false;
    InitUI();
}

void FlyWidget::SwitchAuthor(const QString &autor)
{
    if(autor.compare("root") == 0)
    {
        m_pMainUiBtn->setEnabled(true);
        m_pDebugUiBtn->setEnabled(true);
        m_pMonitorUiBtn->setEnabled(true);
        m_pConfigUiBtn->setEnabled(true);
        m_pShowUiBtn->setEnabled(true);
    }
    else if(autor.compare("admin") == 0)
    {
        m_pMainUiBtn->setEnabled(true);
        m_pDebugUiBtn->setEnabled(true);
        m_pMonitorUiBtn->setEnabled(true);
        m_pConfigUiBtn->setEnabled(false);
        m_pShowUiBtn->setEnabled(true);
    }
    else
    {
        m_pMainUiBtn->setEnabled(true);
        m_pDebugUiBtn->setEnabled(false);
        m_pMonitorUiBtn->setEnabled(true);
        m_pConfigUiBtn->setEnabled(false);
        m_pShowUiBtn->setEnabled(true);
    }
}

void FlyWidget::CreateBtn()
{
    QGridLayout *box = new QGridLayout(this);

    MainWindow * p = (MainWindow*)parentWidget();
    m_pMainUiBtn = new PushButton(this);
    m_pMainUiBtn->setToolTip("主界面");
    m_pMainUiBtn->getIconPath(":/images/skin/ball/mainui-0.png");
    connect(m_pMainUiBtn, SIGNAL(clicked()), p, SLOT(ShowMainDockWindow()));
    box->addWidget(m_pMainUiBtn,0,1);

    m_pDebugUiBtn = new PushButton(this);
    m_pDebugUiBtn->setToolTip("调试界面");
    m_pDebugUiBtn->getIconPath(":/images/skin/ball/debug-0.png");
    connect(m_pDebugUiBtn, SIGNAL(clicked()), p, SLOT(ShowDebugDockWindow()));
    box->addWidget(m_pDebugUiBtn,1,0);


    m_pMonitorUiBtn = new PushButton(this);
    m_pMonitorUiBtn->setToolTip("监视");
    m_pMonitorUiBtn->getIconPath(":/images/skin/ball/monitor-0.png");
    connect(m_pMonitorUiBtn, SIGNAL(clicked()), p, SLOT(ShowMonitorDockWindow()));
    box->addWidget(m_pMonitorUiBtn,1,2);

    m_pConfigUiBtn = new PushButton(this);
    m_pConfigUiBtn->setToolTip("参数");
    m_pConfigUiBtn->getIconPath(":/images/skin/ball/other-0.png");
    connect(m_pConfigUiBtn, SIGNAL(clicked()), p, SLOT(ShowConfigDockWindow()));
    box->addWidget(m_pConfigUiBtn,2,1);

    m_pShowUiBtn = new PushButton(this);
    m_pShowUiBtn->setToolTip("显示");
    m_pShowUiBtn->getIconPath(":/images/show");
    connect(m_pShowUiBtn, SIGNAL(clicked()), p, SLOT(ShowPhotoDockWindow()));
    box->addWidget(m_pShowUiBtn,1,1);

    box->setAlignment(Qt::AlignCenter);
    box->setHorizontalSpacing(35);
    box->setVerticalSpacing(35);
    this->setLayout(box);
}

void FlyWidget::InitData()
{
    m_Pixmap.load(QString(":/images/skin/ball/Hustler_007.png"));
    m_PixmapChange.load(QString(":/images/skin/ball/fly.png"));
    return;
}

void FlyWidget::InitUI()
{
    if(!m_isClicked)
    {
        m_pMainUiBtn->setVisible(false);
        m_pDebugUiBtn->setVisible(false);
        m_pMonitorUiBtn->setVisible(false);
        m_pConfigUiBtn->setVisible(false);
        m_pShowUiBtn->setVisible(false);
        resize(m_Pixmap.size());
        setMask(m_Pixmap.mask());
        move(m_OldPos);
    }
    else
    {
        m_pMainUiBtn->setVisible(true);
        m_pDebugUiBtn->setVisible(true);
        m_pMonitorUiBtn->setVisible(true);
        m_pConfigUiBtn->setVisible(true);
        m_pShowUiBtn->setVisible(true);
        resize(m_PixmapChange.size());
        setMask(m_PixmapChange.mask());
        move(pe->width()/5*2,pe->height()/5*2);

    }
}

void FlyWidget::InitConnect()
{

}

void FlyWidget::SetMenu(QMenu *menu)
{
    m_pSettingMenu = menu;
}

void FlyWidget::mousePressEvent(QMouseEvent *event)
{
    //按住左键可以托动窗口，按下右键关闭程序
    if(event->button() == Qt::LeftButton)
    {
        m_isMove = false;
        m_CurrentPos = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void FlyWidget::mouseReleaseEvent(QMouseEvent *event)
{
     if(event->button() == Qt::LeftButton)
     {
         if(!m_isMove)
         {
             if(!m_isClicked)
             {
                 m_isClicked = true;
                 m_OldPos = QCursor::pos()-m_CurrentPos;
                 InitUI();
             }
             else
             {
                 m_isClicked = false;
                 InitUI();
             }
         }
     }
}

void FlyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton)
    {
        if((event->globalPos() - m_CurrentPos).x() < 60 || (event->globalPos() - m_CurrentPos).y() < 50)
            return;
        m_isMove = true;
        move(event->globalPos() - m_CurrentPos);
        event->accept();
    }
}

void FlyWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    if(!m_isClicked)
        painter.drawPixmap(0, 0, m_Pixmap);
    else
       painter.drawPixmap(0, 0, m_PixmapChange);
}

void FlyWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    //鼠标离开窗口时是普通的指针
    setCursor(Qt::ArrowCursor);
    if(m_isClicked)
    {
        if(m_isEnter)
            m_pTimer->start(300);
        else
            m_pTimer->start(6000);
    }
}

void FlyWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    //鼠标留在窗口上时是一个手指
    setCursor(Qt::PointingHandCursor);
    if(m_isClicked)
        m_isEnter = true;
    m_pTimer->stop();
}
