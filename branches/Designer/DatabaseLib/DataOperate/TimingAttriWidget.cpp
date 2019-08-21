#include "TimingAttriWidget.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QDebug>
TimingAttriWidget::TimingAttriWidget(QWidget *parent) : QWidget(parent)
{
    QGroupBox *pGroup = new QGroupBox(tr("计时类"));
    pGroup->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    QGridLayout *pGroupLayout = new QGridLayout();
    m_varname = new QLabel(tr(""));
    QLabel *varlabel = new QLabel(tr("变量名:"));
    QLabel *pstartIOLabel = new QLabel(tr("起始IO:"));
    QLabel *pInitvalueLabel = new QLabel(tr("初始值:"));
    QLabel *pSetvalueLabel = new QLabel(tr("设置值:"));
    QLabel *pCurvalueLabel = new QLabel(tr("当前值:"));
    QLabel *pEndIOLabel = new QLabel(tr("关联IO:"));
    pSaveToDBCheckbox = new QCheckBox(tr("是否保存到数据库"));

    pstartIOEdit = new QLineEdit();
    pInitvalueEdit = new QLineEdit();
    pSetvalueEdit = new QLineEdit();
    pCurvalueEdit = new QLineEdit();
    pEndIOEdit = new QLineEdit();
    QIntValidator *pIntValidator = new QIntValidator(this);
    pInitvalueEdit->setValidator(pIntValidator);
    pSetvalueEdit->setValidator(pIntValidator);
    pCurvalueEdit->setValidator(pIntValidator);
    QPushButton *m_save = new QPushButton(this);
     m_save->setText(tr("保存属性"));
     m_save->setStyleSheet("QPushButton{width:48px;font:11px;border:1px solid white;border-radius:3px;}"\
                           "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                           "QPushButton:pressed{background-color:rgba(255,255,255,150);}");

    pGroupLayout->addWidget(varlabel,0,0,Qt::AlignRight);
    pGroupLayout->addWidget(m_varname,0,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pstartIOLabel,1,0,Qt::AlignRight);
    pGroupLayout->addWidget(pstartIOEdit,1,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pInitvalueLabel,2,0,Qt::AlignRight);
    pGroupLayout->addWidget(pInitvalueEdit,2,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pSetvalueLabel,3,0,Qt::AlignRight);
    pGroupLayout->addWidget(pSetvalueEdit,3,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pCurvalueLabel,4,0,Qt::AlignRight);
    pGroupLayout->addWidget(pCurvalueEdit,4,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pEndIOLabel,6,0,Qt::AlignRight);
    pGroupLayout->addWidget(pEndIOEdit,6,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pSaveToDBCheckbox,7,0,1,2);

   pGroupLayout->addWidget(m_save,8,0,1,2);
    //设置布局比例
    pGroupLayout->setColumnStretch(0,1);
    pGroupLayout->setColumnStretch(1,4);

    pGroup->setLayout(pGroupLayout);
    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(pGroup);
    this->setLayout(pMainLayout);
    connect(m_save,&QPushButton::clicked,this,&TimingAttriWidget::slotTimeAttriChanged);
}

void TimingAttriWidget::SetData(VARDB_TIME_STRU stu)
{
    m_baselist<<QString::fromUtf8(stu.pkey)<<QString::fromUtf8(stu.type)<<\
                QString::fromUtf8(stu.date)<<QString::fromUtf8(stu.fkey)<<QString::fromUtf8(stu.note);
    m_varname->setText(QString::fromUtf8(stu.pkey));
    pstartIOEdit->setText(QString::fromUtf8(stu.startIO));
    pInitvalueEdit->setText(QString::number(stu.initvalue));
    pSetvalueEdit->setText(QString::number(stu.setvalue));
    pCurvalueEdit->setText(QString::number(stu.curvalue));
    pEndIOEdit->setText(QString::fromUtf8(stu.endIO));
    pSaveToDBCheckbox->setChecked(stu.save);
}

void TimingAttriWidget::slotTimeAttriChanged()
{
    VARDB_TIME_STRU timstru;
    strncpy(timstru.pkey,m_baselist[0].toStdString().c_str(),32);
    strncpy(timstru.type,m_baselist[1].toStdString().c_str(),32);
    strncpy(timstru.date,m_baselist[2].toStdString().c_str(),32);
    strncpy(timstru.fkey,m_baselist[3].toStdString().c_str(),32);
    strncpy(timstru.note,m_baselist[4].toStdString().c_str(),128);
    if(!CheckChar(pstartIOEdit->text()))
    {
        QMessageBox::information(NULL,tr("警告"),tr("起始IO输入错误，请重新输入！"));
        return ;
    }
    if(!CheckChar(pEndIOEdit->text()))
    {
        QMessageBox::information(NULL,tr("警告"),tr("关联IO输入错误，请重新输入！"));
        return ;
    }
    strncpy(timstru.startIO,pstartIOEdit->text().toStdString().c_str(),16);
    timstru.initvalue = pInitvalueEdit->text().toInt();
    timstru.setvalue = pSetvalueEdit->text().toInt();
    timstru.curvalue = pCurvalueEdit->text().toInt();
    strncpy(timstru.endIO,pEndIOEdit->text().toStdString().c_str(),16);
    memset(timstru.funtype,0,8);
    timstru.save = pSaveToDBCheckbox->isChecked();
    emit signalAttriDataChanged("计时类",&timstru);
}

bool TimingAttriWidget::CheckChar(QString srcstr)
{
    if(srcstr.size() <= 2)
    {
        return false;
    }
    else{
        if(!srcstr.contains("E")&& !srcstr.contains("Q"))
        {
            return false;
        }
    }
    return true;
}

