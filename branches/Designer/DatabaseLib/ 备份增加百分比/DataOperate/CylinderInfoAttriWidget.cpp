#include "CylinderInfoAttriWidget.h"
#include <QTextEdit>
#include <QTextCodec>
#include <QIntValidator>
#include <QMessageBox>

CylinderInfoAttriWidget::CylinderInfoAttriWidget(QWidget *parent) : QWidget(parent)
{
    CreateCyAttriUI();
}

void CylinderInfoAttriWidget::CreateCyAttriUI()
{
    QGroupBox *group = new QGroupBox("气缸类");
    group->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    QVBoxLayout *mainlay = new QVBoxLayout;
   QLabel * varlabel = new QLabel (tr("变量名:"));
   m_Varname = new QLabel(tr(""));
   QHBoxLayout *varlay = new QHBoxLayout;
   varlay->addWidget(varlabel);
   varlay->addWidget(m_Varname);
   varlay->setAlignment(Qt::AlignLeft);
   mainlay->addLayout(varlay);
    QLabel * label = new QLabel ("线圈信息:");
    m_soltline = new QLineEdit(this);
    QHBoxLayout *soltlay = new QHBoxLayout;
    soltlay->addWidget(label);
    soltlay->addWidget(m_soltline);
    mainlay ->addLayout(soltlay);
    QHBoxLayout *inpointhlay = new QHBoxLayout;

    QLabel * baselabel = new QLabel ("原点:");
    m_baseLine = new QLineEdit(this);
    QLabel * movelabel = new QLabel ("动点:");
    m_moveLine = new QLineEdit(this);
    inpointhlay->addWidget(baselabel);
    inpointhlay->addWidget(m_baseLine);
    inpointhlay->addWidget(movelabel);
    inpointhlay->addWidget(m_moveLine);
    mainlay->addLayout(inpointhlay);
    QHBoxLayout *timehlay = new QHBoxLayout;
    QLabel * delaylabel = new QLabel ("延时:");
    m_delayLine = new QLineEdit(this);
    QLabel *timoutlabel = new QLabel ("超时:");
    m_timoutLine = new QLineEdit(this);
     QIntValidator *pIntValidator = new QIntValidator(this);
     m_delayLine->setValidator(pIntValidator);
     m_timoutLine->setValidator(pIntValidator);
    timehlay->addWidget(delaylabel);
    timehlay->addWidget(m_delayLine);
    timehlay->addWidget(timoutlabel);
    timehlay->addWidget(m_timoutLine);
    mainlay->addLayout(timehlay);

    QHBoxLayout *cyinfohlay = new QHBoxLayout;
    QLabel * infolabel = new QLabel ("信息:");
    m_infotext = new QTextEdit(this);

    m_infotext->setStyleSheet(tr("QTextEdit{width:70px;font:14px;border:1px solid white;border-radius:3px;}"\
                                 "QTextEdit{background-color:rgba(255,255,255,100);color:rgba(0,0,80,255)}"));
    cyinfohlay->addWidget(infolabel);
    cyinfohlay->addWidget(m_infotext);
    mainlay->addLayout(cyinfohlay);
    m_pSaveToDBCheckbox = new QCheckBox(tr("是否保存到数据库"));
    mainlay->addWidget(m_pSaveToDBCheckbox);
    m_save = new QPushButton(this);
    m_save->setText(tr("保存属性"));
    m_save->setStyleSheet("QPushButton{width:48px;font:11px;border:1px solid white;border-radius:3px;}"\
                          "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                          "QPushButton:pressed{background-color:rgba(255,255,255,150);}");
    mainlay->addWidget(m_save);
    group->setLayout(mainlay);
    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(group);
    this->setLayout(pMainLayout);
    connect(m_save,&QPushButton::clicked,this,&CylinderInfoAttriWidget::slotCylinderAttriChanged);
}

bool CylinderInfoAttriWidget::CheckChar(QString srcstr,QChar keychar)
{
    if(srcstr.size() <= 2)
    {
        return false;
    }
    else{
        if(srcstr[0]!=keychar)
        {
            return false;
        }
    }
    return true;
}

void CylinderInfoAttriWidget::SetData(VARDB_CY_STRU stru)
{
    m_baselist<<QString::fromUtf8(stru.pkey)<<QString::fromUtf8(stru.type)<<\
                QString::fromUtf8(stru.date)<<QString::fromUtf8(stru.fkey)<<QString::fromUtf8(stru.note);
    m_Varname->setText(QString::fromUtf8(stru.pkey));
    m_soltline->setText(QString::fromUtf8(stru.soltComp));
    m_baseLine->setText(QString::fromUtf8(stru.basePoint));
    m_moveLine->setText(QString::fromUtf8(stru.movePoint));
    m_delayLine->setText(QString::number(stru.delay));
    m_timoutLine->setText(QString::number(stru.timeOut));
    m_infotext->setText(QString::fromUtf8(stru.MsgMap));
    m_pSaveToDBCheckbox->setChecked(stru.save);
}
void CylinderInfoAttriWidget::slotCylinderAttriChanged()
{
    //获取当前widget界面数据的变化,并发送出去
    VARDB_CY_STRU cystru;
    strncpy(cystru.pkey,m_baselist[0].toStdString().c_str(),32);
    strncpy(cystru.type,m_baselist[1].toStdString().c_str(),32);
    strncpy(cystru.date,m_baselist[2].toStdString().c_str(),32);
    strncpy(cystru.fkey,m_baselist[3].toStdString().c_str(),32);
    strncpy(cystru.note,m_baselist[4].toStdString().c_str(),128);
    if(CheckChar(m_baseLine->text(),'E')&&CheckChar(m_moveLine->text(),'E')&&(m_baseLine->text()!=m_moveLine->text()))
    {
       if(!CheckChar(m_soltline->text(),'Q'))
       {
           QMessageBox::information(NULL,tr("警告"),tr("电磁阀线圈信息输入错误，请重新输入！"));
           return ;
       }
        strncpy(cystru.soltComp,m_soltline->text().toStdString().c_str(),32);
        strncpy(cystru.basePoint,m_baseLine->text().toStdString().c_str(),16);
        strncpy(cystru.movePoint,m_moveLine->text().toStdString().c_str(),16);
        cystru.delay = m_delayLine->text().toInt();
        cystru.timeOut = m_timoutLine->text().toInt();
        strncpy(cystru.MsgMap,m_infotext->toPlainText().toStdString().c_str(),64);
        cystru.save = m_pSaveToDBCheckbox->isChecked();
        emit signalAttriDataChanged("气缸类",&cystru);
    }
    else
    {
        QMessageBox::information(NULL,tr("警告"),tr("原点或动点输入错误，请重新输入！"));
        return ;
    }
}


