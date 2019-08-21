#include "cspmotionctrwidget.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QValidator>
cspMotionctrWidget::cspMotionctrWidget(QWidget *parent ): QWidget(parent)
{
    createUI(parent);
}

cspMotionctrWidget::~cspMotionctrWidget()
{

}
void cspMotionctrWidget::createUI(QWidget *parent)
{
    //m_struVarValue = NULL;
    QVBoxLayout *Vmainlay = new QVBoxLayout;
    QGroupBox *group =new QGroupBox (tr("运动控制类"));
    group->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    QGridLayout *gridlay = new QGridLayout;

    QLabel *axisinfolabel = new QLabel ("包含轴ID:");
    m_axisid = new QLineEdit (parent);
    m_axisid->setMaxLength(128);
    QLabel *tmodlabel = new QLabel ("变量名:");
    m_namelabel  =new QLabel ("");
    QLabel *delaylabel = new QLabel ("延时:");
    m_delayline= new QLineEdit (parent);
    QLabel *timeout = new QLabel("超时:");
    m_timoutline = new QLineEdit(parent);
    QLabel *acclabel = new QLabel("加速度:");
    m_accline = new QLineEdit(parent);
    QLabel *declabel = new QLabel("减速度:");
    m_deccline = new QLineEdit(parent);
    QLabel *axisinfo = new QLabel("轴信息:");
    m_savetoDb = new QCheckBox(tr("是否保存到数据库"));
    gridlay->addWidget(tmodlabel,0,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_namelabel,0,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(axisinfolabel,1,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_axisid,1,1,1,1,Qt::AlignLeft);

    //  gridlay->addWidget(m_modcombox,1,1,1,2,Qt::AlignLeft);
    gridlay->addWidget(delaylabel,2,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_delayline,2,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(timeout,3,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_timoutline,3,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(acclabel,4,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_accline,4,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(declabel,5,0,1,1,Qt::AlignRight);
    gridlay->addWidget(m_deccline,5,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(axisinfo,6,0,1,1,Qt::AlignRight);
   // gridlay->addWidget(m_axisinfoline,6,1,1,1,Qt::AlignLeft);

    gridlay->addWidget(m_savetoDb,7,0,1,2);

    //设置布局比例
    gridlay->setColumnStretch(0,1);
    gridlay->setColumnStretch(1,4);
    gridlay->setAlignment(Qt::AlignLeft);
    group->setLayout(gridlay);
    Vmainlay->setMargin(20);
    Vmainlay->addWidget(group);
    this->setLayout(Vmainlay);
}
void cspMotionctrWidget::SetData(VARDB_LCSP_STRU stru)
{
//    m_namelabel->setText(QString::fromUtf8(stru.pkey));
//    m_accline->setText(QString::number(stru.acc));
//    m_deccline->setText(QString::number(stru.dec));
//    m_delayline->setText(QString::number(stru.delay));
//    m_timoutline->setText(QString::number(stru.timeOut));
//    m_axisid->setText(QString::fromUtf8(stru.axisidinfo));
//    m_savetoDb->setChecked(stru.save);
//    connect(m_axisid,&QLineEdit::textChanged,this,&cspMotionctrWidget::slotMotionCtrAttriChanged);
//    connect(m_delayline,&QLineEdit::textChanged,this,&cspMotionctrWidget::slotMotionCtrAttriChanged);
//    connect(m_timoutline,&QLineEdit::textChanged,this,&cspMotionctrWidget::slotMotionCtrAttriChanged);
//    connect(m_accline,&QLineEdit::textChanged,this,&cspMotionctrWidget::slotMotionCtrAttriChanged);
//    connect(m_deccline,&QLineEdit::textChanged,this,&cspMotionctrWidget::slotMotionCtrAttriChanged);
}
void cspMotionctrWidget::slotMotionCtrAttriChanged()
{
    //获取当前widget界面数据的变化,并发送出去
//    strncpy(m_struVarValue->pkey,m_baselist[0].toStdString().c_str(),32);
//    strncpy(m_struVarValue->type,m_baselist[1].toStdString().c_str(),32);
//    strncpy(m_struVarValue->date,m_baselist[2].toStdString().c_str(),32);
//    strncpy(m_struVarValue->fkey,m_baselist[3].toStdString().c_str(),32);
//    strncpy(m_struVarValue->note,m_baselist[4].toStdString().c_str(),128);
//    m_struVarValue->acc = m_accline->text().toInt();
//    m_struVarValue->dec = m_deccline->text().toInt();
//    strncpy(m_struVarValue->axisidinfo,m_axisid->text().toStdString().c_str(),128);
//    m_struVarValue->delay = m_delayline->text().toInt();
//    m_struVarValue->timeOut = m_timoutline->text().toInt();
//   emit signalAttriDataChanged("运动控制CSP模式",m_struVarValue);
}

