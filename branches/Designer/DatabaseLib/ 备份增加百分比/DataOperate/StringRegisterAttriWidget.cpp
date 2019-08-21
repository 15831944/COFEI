#include "StringRegisterAttriWidget.h"
StringRegisterAttriWidget::StringRegisterAttriWidget(QWidget *parent) : QWidget(parent)
{
    QGroupBox *pGroup = new QGroupBox(tr("寄存器字符串型"));
    pGroup->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    QGridLayout *pGroupLayout = new QGridLayout();

    QLabel *varlabel = new QLabel(tr("变量名:"));
    m_varname = new QLabel(tr(""));
    QLabel *pchildtypeLabel = new QLabel(tr("子类型:"));
    pSaveToDBCheckbox = new QCheckBox(tr("是否保存到数据库"));
    pchildtypeEdit = new QLineEdit();

    QLabel *ptextLabel = new QLabel(tr("值："));
    ptextvalEdit = new QLineEdit();
    QPushButton *save = new QPushButton(this);
    save->setText(tr("保存属性"));
    save->setStyleSheet("QPushButton{width:48px;font:11px;border:1px solid white;border-radius:3px;}"\
                        "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                        "QPushButton:pressed{background-color:rgba(255,255,255,150);}");
    pGroupLayout->addWidget(varlabel,0,0,Qt::AlignRight);
    pGroupLayout->addWidget(m_varname,0,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pchildtypeLabel,1,0,Qt::AlignRight);
    pGroupLayout->addWidget(pchildtypeEdit,1,1,Qt::AlignLeft);

    pGroupLayout->addWidget(ptextLabel,2,0,Qt::AlignRight);
    pGroupLayout->addWidget(ptextvalEdit,2,1,Qt::AlignLeft);
    pGroupLayout->addWidget(pSaveToDBCheckbox,3,0,1,2);
    pGroupLayout->addWidget(save,4,0,1,2);
    //设置布局比例
    pGroupLayout->setColumnStretch(0,1);
    pGroupLayout->setColumnStretch(1,4);

    pGroup->setLayout(pGroupLayout);
    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(pGroup);
    this->setLayout(pMainLayout);
    connect(save,&QPushButton::clicked,this,&StringRegisterAttriWidget::slotstringAttriChanged);
}

void StringRegisterAttriWidget::SetData(VARDB_REGTEXT_STRU stu)
{
    m_baselist<<QString::fromUtf8(stu.pkey)<<QString::fromUtf8(stu.type)<<\
                QString::fromUtf8(stu.date)<<QString::fromUtf8(stu.fkey)<<QString::fromUtf8(stu.note);
    m_varname->setText(QString::fromUtf8(stu.pkey));
    pchildtypeEdit->setText(QString::fromUtf8(stu.childtype));
    ptextvalEdit->setText(QString::fromUtf8(stu.textval));
    pSaveToDBCheckbox->setChecked(stu.save);
}

void StringRegisterAttriWidget::slotstringAttriChanged()
{
    //获取当前widget界面数据的变化,并发送出去
    VARDB_REGTEXT_STRU regtextstru;
    strncpy(regtextstru.pkey,m_baselist[0].toStdString().c_str(),32);
    strncpy(regtextstru.type,m_baselist[1].toStdString().c_str(),32);
    strncpy(regtextstru.date,m_baselist[2].toStdString().c_str(),32);
    strncpy(regtextstru.fkey,m_baselist[3].toStdString().c_str(),32);
    strncpy(regtextstru.note,m_baselist[4].toStdString().c_str(),128);
    strncpy(regtextstru.childtype,pchildtypeEdit->text().toStdString().c_str(),16);
    strncpy(regtextstru.textval,ptextvalEdit->text().toStdString().c_str(),64);
    regtextstru.save = pSaveToDBCheckbox->isChecked();
    emit signalAttriDataChanged("字符串型寄存器",&regtextstru);
}
