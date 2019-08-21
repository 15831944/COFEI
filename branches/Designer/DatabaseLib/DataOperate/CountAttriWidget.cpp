#include "CountAttriWidget.h"
// #include <QMetaType>
#include <QMessageBox>

CountAttriWidget::CountAttriWidget()
{
    QGroupBox *pGroup = new QGroupBox(tr("计数类"));
    pGroup->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    QGridLayout *pGroupLayout = new QGridLayout();
    QLabel *varname = new QLabel(tr("变量名:"));
    m_Varname = new QLabel(tr(""));
    QLabel *pfuntypeLabel = new QLabel(tr("计数类型:"));
    QLabel *pstartIOLabel = new QLabel(tr("起始IO:"));
    QLabel *pInitvalueLabel = new QLabel(tr("初始值:"));
    QLabel *pSetvalueLabel = new QLabel(tr("设置值:"));
    QLabel *pCurvalueLabel = new QLabel(tr("当前值:"));
    QLabel *pAddvalueLabel = new QLabel(tr("增量:"));
    QLabel *pEndIOLabel = new QLabel(tr("关联IO:"));
    pSaveToDBCheckbox = new QCheckBox(tr("是否保存到数据库"));
   QPushButton *m_save = new QPushButton(this);
    m_save->setText(tr("保存属性"));
    m_save->setStyleSheet("QPushButton{width:48px;font:11px;border:1px solid white;border-radius:3px;}"\
                          "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                          "QPushButton:pressed{background-color:rgba(255,255,255,150);}");
    QStringList typelist;
    typelist<<"0"<<"1";
    m_funtype = new QComboBox();
    m_funtype->setStyleSheet(tr("border: 1px solid white;border-radius: 3px;padding: 1px 2px 1px 2px;min-width: 48;"));
    m_funtype->addItems(typelist);
    pstartIOEdit = new QLineEdit();
    pInitvalueEdit = new QLineEdit();
    pSetvalueEdit = new QLineEdit();
    pCurvalueEdit = new QLineEdit();
    pAddvalueEdit = new QLineEdit();
    pEndIOEdit = new QLineEdit();
    QIntValidator *pIntValidator = new QIntValidator(this);
    pInitvalueEdit->setValidator(pIntValidator);
    pSetvalueEdit->setValidator(pIntValidator);
    pCurvalueEdit->setValidator(pIntValidator);
    pAddvalueEdit->setValidator(pIntValidator);
    pGroupLayout->addWidget(varname,0,0,Qt::AlignRight);
    pGroupLayout->addWidget(m_Varname,0,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pfuntypeLabel,1,0,Qt::AlignRight);
    pGroupLayout->addWidget(m_funtype,1,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pstartIOLabel,2,0,Qt::AlignRight);
    pGroupLayout->addWidget(pstartIOEdit,2,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pInitvalueLabel,3,0,Qt::AlignRight);
    pGroupLayout->addWidget(pInitvalueEdit,3,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pSetvalueLabel,4,0,Qt::AlignRight);
    pGroupLayout->addWidget(pSetvalueEdit,4,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pCurvalueLabel,5,0,Qt::AlignRight);
    pGroupLayout->addWidget(pCurvalueEdit,5,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pAddvalueLabel,6,0,Qt::AlignRight);
    pGroupLayout->addWidget(pAddvalueEdit,6,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pEndIOLabel,7,0,Qt::AlignRight);
    pGroupLayout->addWidget(pEndIOEdit,7,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pSaveToDBCheckbox,8,0,1,2);
    pGroupLayout->addWidget(m_save,9,0,1,2);

    //设置布局比例
    pGroupLayout->setColumnStretch(0,1);
    pGroupLayout->setColumnStretch(1,4);

    pGroup->setLayout(pGroupLayout);
    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(pGroup);
    this->setLayout(pMainLayout);
    connect(m_save,&QPushButton::clicked,this,&CountAttriWidget::slotCountAttriChanged);
}
void CountAttriWidget::SetData(VARDB_COUNT_STRU stu)
{
    m_baselist<<QString::fromUtf8(stu.pkey)<<QString::fromUtf8(stu.type)<<\
                QString::fromUtf8(stu.date)<<QString::fromUtf8(stu.fkey)<<QString::fromUtf8(stu.note);
    m_funtype->setCurrentText(QString::fromUtf8(stu.funtype));
    m_Varname->setText(QString::fromUtf8(stu.pkey));
    pstartIOEdit->setText(QString::fromUtf8(stu.startIO));
    pInitvalueEdit->setText(QString::number(stu.initvalue));
    pSetvalueEdit->setText(QString::number(stu.setvalue));
    pCurvalueEdit->setText(QString::number(stu.curvalue));
    pAddvalueEdit->setText(QString::number(stu.addvalue));
    pEndIOEdit->setText(QString::fromUtf8(stu.endIO));
    pSaveToDBCheckbox->setChecked(stu.save);
}
void CountAttriWidget::slotCountAttriChanged()
{
    VARDB_COUNT_STRU cunt;
    //获取当前widget界面数据的变化,并发送出去
    strncpy(cunt.pkey,m_baselist[0].toStdString().c_str(),32);
    strncpy(cunt.type,m_baselist[1].toStdString().c_str(),32);
    strncpy(cunt.date,m_baselist[2].toStdString().c_str(),32);
    strncpy(cunt.fkey,m_baselist[3].toStdString().c_str(),32);
    strncpy(cunt.note,m_baselist[4].toStdString().c_str(),128);
//    if(!CheckChar(pstartIOEdit->text()))
//    {
//        QMessageBox::information(NULL,tr("警告"),tr("起始IO输入错误，请重新输入！"));
//        return ;
//    }
//    if(!CheckChar(pEndIOEdit->text()))
//    {
//        QMessageBox::information(NULL,tr("警告"),tr("关联IO输入错误，请重新输入！"));
//        return ;
//    }
    strncpy(cunt.funtype,m_funtype->currentText().toStdString().c_str(),8);
    strncpy(cunt.startIO,pstartIOEdit->text().toStdString().c_str(),16);
    cunt.initvalue = pInitvalueEdit->text().toInt();
    cunt.setvalue = pSetvalueEdit->text().toInt();
    cunt.curvalue = pCurvalueEdit->text().toInt();
    cunt.addvalue = pAddvalueEdit->text().toInt();
    strncpy(cunt.endIO,pEndIOEdit->text().toStdString().c_str(),16);
    cunt.save = pSaveToDBCheckbox->isChecked();
    emit signalAttriDataChanged("计数类",&cunt);
}

bool CountAttriWidget::CheckChar(QString srcstr)
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


