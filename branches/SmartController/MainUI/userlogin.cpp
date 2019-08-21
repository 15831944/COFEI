#include "userlogin.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QBitmap>
#include <QHorizontalBarSeries>
#include <QMessageBox>
#include <QSqlError>
#include <DycLibBase/SecretManager/secretmanager.h>
UserLogin::UserLogin()
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setWindowFlags(Qt::ToolTip);
    resize(480,220);
    setMask(m_PixmapChange.mask());
   // setFixedHeight(sizeHint().height());
    QDesktopWidget *deskWgt = QApplication::desktop();
    QRect screenRect = deskWgt->screenGeometry();
    int screenWidth = screenRect.width() ;
    int screenHeight = screenRect.height();
    move(screenWidth/3,screenHeight/3);
    setStyleSheet(tr("background-color:rgb(40,42,51);color:white;"));
    setWindowOpacity(0.9);
    InitUI();
    DataBaseManager::GetInstance()->CreateDBTable("create table user(id int primary key, UserName varchar(100), PasWord varchar(100));");
}

UserLogin::~UserLogin()
{
   db.close();
}

void UserLogin::InitUI()
{
    QFont ft ("Microsoft YaHei",12,55); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
     QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox1 = new QHBoxLayout();
     m_puser = new Lineedit(this);
     m_puser->setMinimumHeight(32);
     connect(m_puser,&Lineedit::clicked,this,&UserLogin::clearuser);
    m_puserlable = new QLabel(this);
    hbox1->addStretch();
    m_puserlable->setText("账号: ");
    m_puserlable->setFont(ft);
    hbox1->addWidget(m_puserlable);
    hbox1->addWidget(m_puser);
    hbox1->addStretch();
    vbox->addLayout(hbox1);

    QHBoxLayout *hbox2 = new QHBoxLayout();
    m_pPasword = new Lineedit(this);
    m_pPasword->setMinimumHeight(32);
    connect(m_pPasword,&Lineedit::clicked,this,&UserLogin::clearpas);
    m_pPasword->setEnabled(true);
    m_pPasword->setFocusPolicy(Qt::ClickFocus);
    m_pPasword->setReadOnly(false);
    m_pPasword->setEchoMode(QLineEdit::Password);

    m_pPaswordlable = new QLabel(this);
    m_pPaswordlable->setText("密码: ");
    m_pPaswordlable->setFont(ft);
    hbox2->addStretch();
    hbox2->addWidget(m_pPaswordlable);
    hbox2->addWidget(m_pPasword);
    hbox2->addStretch();
    vbox->addLayout(hbox2);

    QHBoxLayout *hbox3 = new QHBoxLayout();
    m_plogin = new QPushButton(this);
    connect(m_plogin,&QPushButton::clicked,this,&UserLogin::login);
    m_pcancel = new QPushButton(this);
    connect(m_pcancel,&QPushButton::clicked,this,&QDialog::reject);
    m_pcancel->setText("取消");
    m_pcancel->setFont(ft);
    m_pcancel->setFocus();
    m_plogin->setText("登录");
    m_plogin->setFont(ft);
    hbox3->addStretch();
    hbox3->addWidget(m_plogin);
    hbox3->addWidget(m_pcancel);
    hbox3->addStretch();
    hbox3->setSpacing(50);

    ft.setPixelSize(18);
    QLabel *logbel = new QLabel(this);
    logbel->setFont(ft);
    logbel->setText("Login..");
    logbel->setGeometry(20,15,70,50);

    user = new QLabel(this);
    user->setStyleSheet("QLabel{border-image: url(:/images/skin/icons/light/user.png);}");
    user->setGeometry(200,39,24,24);

    pas = new QLabel(this);
    pas->setStyleSheet("QLabel{border-image: url(:/images/skin/icons/light/lock.png);}");
    pas->setGeometry(200,100,24,24);

    tips = new QLabel(this);
    tips->setGeometry(340,98,70,30);
    ft.setPointSize(12);
    tips->setText("<u>忘记密码?</u>");
    tips->setFont(ft);

    vbox->addLayout(hbox3);
    this->setLayout(vbox);
    m_puser->installEventFilter(this);
    m_pPasword->installEventFilter(this);

}

void UserLogin::login()
{
   QString username = m_puser->text();
   QString PasWord = m_pPasword->text();
  // PasWord = AddCode(m_pPasword->text());
   CheckLogin(username,PasWord);
}

void UserLogin::clearuser()
{
    user->setVisible(false);
    if(m_pPasword->text() == "")
    {
        pas->setVisible(true);
    }
}

void UserLogin::clearpas()
{
     pas->setVisible(false);
     if(m_puser->text() == "")
     {
         user->setVisible(true);
     }

}

bool UserLogin::changepasword(QString username, QString newpasword)
{
    QSqlQuery sqlquery;
    bool isok = sqlquery.exec(QString("UPDATE user SET PasWord = '%1' WHERE UserName = '%2' ").arg(newpasword).arg(username));
    return isok;
}

QString UserLogin::AddCode(QString str)
{
    QByteArray buff = str.toLatin1();
    if(buff.size()>10)
    {
        return "";
    }
   for(int i = 0; i<buff.size();i++)
   {
        buff[i] = buff[i] + uc[i];
   }
    return buff;
}

QString UserLogin::DeCode(QString str)
{
    QByteArray buff = str.toLatin1();
   for(int i = 0; i<buff.size();i++)
   {
        buff[i] = buff[i] - uc[i];
   }
    return buff;

}

void UserLogin::CheckLogin(QString Uname, QString Paws)
{
    bool isok = false;
    QSqlQuery sql;
//    bool a = sql.exec(("SELECT id,UserName,PasWord FROM user"));
    sql =  DataBaseManager::GetInstance()->ExcQueryDb("SELECT id,UserName,PasWord FROM user");
//    if(false == a)// dont judge the connect faile or sucessful
//    {
//        tips->text()=="数据库错误";
//        tips->setStyleSheet("color:red");
//        return;
//    }
    while(sql.next())
    {
        if(Uname.compare(sql.value("UserName").toString()) == 0 &&
                Paws.compare(sql.value("PasWord").toString())==0)
        {
            tips->setText("登录成功!");
            tips->setStyleSheet("color:white");
            isok = true;//登录成功
            accept();
            break;
        }
    }
    if(true != isok)
    {
        //登录失败
        tips->setText("登录失败!");
        tips->setStyleSheet("color:red");
        m_pPasword->setText("");
    }
}

bool UserLogin::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_pPasword || obj == m_puser )
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
            if(key_event->key() == Qt::Key_Tab)
            {
                bool pasword_focus = m_pPasword->hasFocus();
                bool puser_focus = m_puser->hasFocus();
                if(pasword_focus)
                {
                    focusNextChild();
                    m_puser->setFocus();
                }
                else if(puser_focus)
                {
                    focusNextChild();
                    m_pPasword->setFocus();
                }
                else
                {
                    focusNextChild();
                }
                return true;
            }
        }
    }
    return QDialog::eventFilter(obj, event);
}
