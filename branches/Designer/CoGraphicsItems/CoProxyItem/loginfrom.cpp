#include "loginfrom.h"
#include <DatabaseLib/DBInstanceManager/dbinstancemanager.h>
#include <CoGraphicsItems/AttributeInstance.h>
LoginFrom::LoginFrom(double StartX, double StartY, double StopX, double StopY, const Qt::PenStyle &LineStyle,
                     const int LineWidth, const QColor &LineColor, const QColor &BackColor, CoScene *Scene):
                       CoProxyWidget(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Mde_Login);
    this->resize(350,261);
    this->SetBackGroundColor(230,230,230,230);
    widget = new QWidget();
    widget->resize(345,255);
    widget->setWindowOpacity(0.86);
    widget->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap1(":/Images/back6.png");
    palette.setBrush(QPalette::Window, QBrush(pixmap1));
    widget->setPalette(palette);
    //widget->setStyleSheet("border-radius:15px");
    QFont font ("楷体", 13, 70);
    QLabel *loglabel = new QLabel();
    loglabel->setFixedHeight(40);
    loglabel->setFixedWidth(120);
    QPixmap pixmap(":/Images/Cow.png");
    QPixmap fitpixmap=pixmap.scaled(loglabel->width(), loglabel->height());
    loglabel->setPixmap(fitpixmap);

    QLabel * LogText = new QLabel();
    LogText->setText("欢迎登录 COFEI");
    LogText->setFont(font);

    m_pUserName = new QComboBox();
    QStringList userIdlist = DBInstanceManager::getDBInstance()->GetAllUserNamesFromDB();
    m_pUserName->addItems(userIdlist);
    m_pUserName->setFixedWidth(190);
    m_pPassWord = new QLineEdit();
    m_pPassWord->setFixedWidth(190);
    m_pPassWord->setPlaceholderText(QStringLiteral("密码"));
    m_pPassWord->setEchoMode(QLineEdit::Password);

    m_pLoginBtn = new QPushButton(m_pPassWord);
    QIcon butico(":/Images/go.ico");
    m_pLoginBtn->move(162,0);
    m_pLoginBtn->setIcon(butico);
    connect(m_pLoginBtn,&QPushButton::clicked,this,&LoginFrom::Login);

     errlab = new QLabel();//tips
     errlab->setVisible(false);
     errlab->setStyleSheet("color:red;");

     m_pExitBtn = new QPushButton();
     m_pExitBtn->setVisible(false);
     m_pExitBtn->setText("注销当前用户");
     connect(m_pExitBtn,&QPushButton::clicked,this,&LoginFrom::Logout);
     QIcon exitIco(":/Images/logout.ico");
     m_pExitBtn->setIcon(exitIco);

    m_pForget = new QLabel();
    m_pForget->setText("<u> 忘记COFEI密码？</u>");
    m_pgridLayout = new QGridLayout();
    m_pgridLayout->addWidget(loglabel,0,2,1,4,Qt::AlignCenter);
    m_pgridLayout->addWidget(LogText,2,3,1,2,Qt::AlignCenter);
    m_pgridLayout->addWidget(m_pUserName,3,1,1,6,Qt::AlignCenter);
    m_pgridLayout->addWidget(m_pPassWord,4,1,1,6,Qt::AlignCenter);
    m_pgridLayout->addWidget(m_pExitBtn,5,1,1,6,Qt::AlignCenter);
    m_pgridLayout->addWidget(errlab,6,1,1,6,Qt::AlignCenter);
    m_pgridLayout->addWidget(m_pForget,7,1,1,6,Qt::AlignCenter);
    widget->setLayout(m_pgridLayout);

    QGraphicsProxyWidget *widget_1 = Scene->addWidget(widget);
    m_pLayout = new QGraphicsGridLayout();
    m_pLayout->addItem(widget_1,0,0,10,8);
    m_pLayout->setHorizontalSpacing(10);
    m_pLayout->setVerticalSpacing(10);
    //m_pLayout->setContentsMargins(10, 10, 10, 10);
    this->setLayout(m_pLayout);

}

void LoginFrom::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        Login();
        break;
    default:
        break;
    }
}

void LoginFrom::InsertErrInfo()
{
    errlab ->setText("UserID or Password MissWrite!");
    errlab->setVisible(true);
}

//登录按钮
void LoginFrom::Login()
{
  QString username = m_pUserName->currentText();
  QString passwrd = m_pPassWord->text();
  int level = -3;
  if(DBInstanceManager::getDBInstance()->CheckUserNameandPasswordIsMatch(username,passwrd,level))
  {
        errlab->setVisible(false);
        AttributeInstance::GetInstance()->Power = level;
        m_pExitBtn->setVisible(true);
  }
  else
  {
      InsertErrInfo();
  }
}

void LoginFrom::Logout()
{
    AttributeInstance::GetInstance()->Power = 4;
    m_pExitBtn->setVisible(false);
    m_pPassWord->setText("");
    m_pUserName->setCurrentIndex(0);
}

CoProxyWidget *LoginFrom::Copy()
{
    LoginFrom *Logfrom = new LoginFrom();

    Logfrom->Copy(this);

    return Logfrom;
}

void LoginFrom::Copy(LoginFrom *loginfrom)
{
     CoProxyWidget::Copy(loginfrom);
}
