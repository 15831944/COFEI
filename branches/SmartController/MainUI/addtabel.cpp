#include "addtabel.h"
#include <QGridLayout>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include<QDebug>
Addtabel::Addtabel(QVector<QString> str)
{
    m_strvec=str;
    //setWindowFlags(Qt::Window|Qt::WindowTitleHint|Qt::CustomizeWindowHint);
    this->setWindowFlags(windowFlags()&~ Qt::WindowMinMaxButtonsHint);
    //setWindowFlags(Qt::ToolTip);
   // resize(480,190);
    this->setFixedSize(480,190);
    //setMask(m_PixmapChange.mask());
    // setFixedHeight(sizeHint().height());
    QDesktopWidget *deskWgt = QApplication::desktop();
    QRect screenRect = deskWgt->screenGeometry();
    int screenWidth = screenRect.width() ;
    int screenHeight = screenRect.height();
    move(screenWidth/3,screenHeight/3);
    setStyleSheet(tr("background-color:rgb(40,42,51);color:white;"));
    initUi();
}

void Addtabel::initUi()
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *gbox = new QHBoxLayout(this);
    QFont ft (QFont("黑体",11));

    for(int i=0;i<m_strvec.size();++i)
    {
        m_label[i]=new QLabel();
        m_label[i]->setText(m_strvec[i]);
        m_label[i]->setFixedSize(90,30);
        m_label[i]->setAlignment(Qt::AlignHCenter);
        gbox->addWidget(m_label[i]);
    }
    vbox->addLayout(gbox);

    QHBoxLayout *hbox1 = new QHBoxLayout(this);

    for(int i=0;i<m_strvec.size();++i)
    {
        m_lineedit[i]=new QLineEdit();
        m_lineedit[i]->setFixedSize(90,30);
        hbox1->addWidget(m_lineedit[i]);
    }

    vbox->addLayout(hbox1);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    m_pok = new QPushButton();
    m_pok->setFont(ft);
    m_pok->setText("确定");
    connect(m_pok,&QPushButton::clicked,this,&Addtabel::sure);
    hbox->addWidget(m_pok);

    m_pcleal = new QPushButton();
    m_pcleal->setFont(ft);
    m_pcleal->setText("取消");
    connect(m_pcleal,&QPushButton::clicked,this,&QDialog::reject);
    hbox->addWidget(m_pcleal);

    vbox->addLayout(hbox);
    this->setLayout(vbox);
}

void Addtabel::sure()
{
    QStringList list;
    for(int i=0;i<m_strvec.size();++i)
    {
        list.append(m_lineedit[i]->text());
    }
   emit data(list);
        accept();

}

bool Addtabel::createmessage()
{
    QFont font;
    font.setPointSize(13);
    font.setFamily("黑体");
    QDialog box ;
    box.setWindowTitle("chenck");
    box.setFixedSize(256,200);
    box.setFont(font);

    QLabel *lable = new QLabel(&box);
    lable->setText("nihap");
    lable->setFont(font);
    lable->setWordWrap(true);
    lable->setFixedSize(250,100);
    QVBoxLayout *vbox = new QVBoxLayout(&box);
    box.setLayout(vbox);

    vbox->addWidget(lable);

    box.setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    QHBoxLayout *hBox = new QHBoxLayout(&box);
    QPushButton *btn = new QPushButton("确认",this);
    connect(btn,&QPushButton::clicked,&box,&QDialog::accept);
    btn->setFont(font);
    QPushButton *btn1 = new QPushButton("取消",this);
    connect(btn1,&QPushButton::clicked,&box,&QDialog::reject);
    btn1->setFont(font);
    hBox->addWidget(btn);
    hBox->addWidget(btn1);
    vbox->addLayout(hBox);
    if(QDialog::Accepted == box.exec())
        return true;

    return false;
}
