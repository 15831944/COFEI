#include "login.h"
#include <QBitmap>
#include <QDesktopWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QStatusBar>
#include <DycLibBase/SecretManager/secretmanager.h>
LogIn::LogIn()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::ToolTip);
    resize(480,220);
    setMask(m_PixmapChange.mask());
    QDesktopWidget *deskWgt = QApplication::desktop();
    QRect screenRect = deskWgt->screenGeometry();
    int screenWidth = screenRect.width() ;
    int screenHeight = screenRect.height();
    move(screenWidth/3,screenHeight/3);
    setStyleSheet(tr("background-color:rgb(40,42,51);color:white;"));
    setWindowOpacity(0.9);
    InitUI();
    CreateMeCode();
    m_pStatus->showMessage("本设备未进行注册，请联系客服！");
}
void LogIn::CreateMeCode()
{
    SecretManager secretManager;
    m_pMechineCode->setText(secretManager.GetMeCode());
}

void LogIn::InitUI()
{
    QFont font;
    font.setPointSize(12);
    font.setFamily("黑体");
    QVBoxLayout *vbox = new QVBoxLayout(this);

    QHBoxLayout *hbox = new QHBoxLayout(this);
    QLabel *l = new QLabel(this);
    l->setAlignment(Qt::AlignLeft);
    l->setFont(font);
    l->setText("机器码:");
    hbox->addWidget(l);
    m_pMechineCode = new QLineEdit(this);
    m_pMechineCode->setFont(font);
    m_pMechineCode->setFixedSize(420,24);
    m_pMechineCode->setReadOnly(true);
    hbox->addWidget(m_pMechineCode);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout(this);
    l = new QLabel(this);
    l->setAlignment(Qt::AlignLeft);
    l->setFont(font);
    l->setText("注册码:");
    hbox->addWidget(l);
    m_pPollCode = new QLineEdit(this);
    m_pPollCode->setFont(font);
    m_pPollCode->setFixedSize(420,24);
    hbox->addWidget(m_pPollCode);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout(this);
    QPushButton *btn = new QPushButton(this);
    btn->setText("取消");
    btn->setFixedSize(100,30);
    connect(btn,&QPushButton::clicked,this,&QDialog::reject);
    hbox->addWidget(btn);
    btn = new QPushButton(this);
    btn->setText("注册");
    btn->setFixedSize(100,30);
    connect(btn,&QPushButton::clicked,this,&LogIn::OverAccept);
    hbox->addWidget(btn);
    vbox->addLayout(hbox);

    this->setLayout(vbox);
    m_pStatus = new QStatusBar(this);
    m_pStatus->setGeometry(QRect(5,200,490,20));
    font.setPointSize(12);
    m_pStatus->setFont(font);
    m_pStatus->setStyleSheet(tr("color:blue;"));
}
void LogIn::OverAccept()
{ 
    if(!m_pPollCode->text().isEmpty())
    {
        SecretManager secretManager;
        if(secretManager.CheckKey(m_pPollCode->text()))
        {
            secretManager.SetEnCode(m_pPollCode->text());
            accept();
        }
    }
    m_pStatus->showMessage("这是无效的注册码，请联系客服！");
}
