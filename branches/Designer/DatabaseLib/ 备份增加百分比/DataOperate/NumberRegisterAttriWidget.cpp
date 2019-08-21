#include "NumberRegisterAttriWidget.h"
#include <QIntValidator>
#include <QDoubleValidator>
#include <QMessageBox>
NumberRegisterAttriWidget::NumberRegisterAttriWidget(QWidget *parent ):QWidget(parent)
{
    QGroupBox *pGroup = new QGroupBox(tr("寄存器数值型"));
     pGroup->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
    QGridLayout *pGroupLayout = new QGridLayout();
    m_Varname =  new QLabel(tr(""));
    QLabel *varlabel = new QLabel(tr("变量名:"));
    QLabel *pchildtypeLabel = new QLabel(tr("子类型:"));
    QLabel *ppointnumLabel = new QLabel(tr("小数点位数:"));
    QLabel *pInitvalueLabel = new QLabel(tr("初值:"));
    QLabel *pupvalueLabel = new QLabel(tr("上限:"));
    QLabel *plowvalueLabel = new QLabel(tr("下限:"));
    pSaveToDBCheckbox = new QCheckBox(tr("是否保存到数据库"));

    pchildtypeEdit = new QLineEdit();
    pspointnumpinbox = new QSpinBox();
    pInitvalueEdit = new QLineEdit();
    pupvalueEdit = new QLineEdit();
    plowvalueEdit = new QLineEdit();
    pspointnumpinbox->setMaximum(4);
    pspointnumpinbox->setMinimum(0);
    pspointnumpinbox->setWrapping(true);
    QDoubleValidator *pDoubleValidator = new QDoubleValidator(this);
    pspointnumpinbox->setStyleSheet("QSpinBox{padding-right:36px;border:1px solid white;}");
    pInitvalueEdit->setValidator(pDoubleValidator);
    pupvalueEdit->setValidator(pDoubleValidator);
    plowvalueEdit->setValidator(pDoubleValidator);
    QPushButton *m_save = new QPushButton(this);
    m_save->setText(tr("保存属性"));
    m_save->setStyleSheet("QPushButton{width:48px;font:11px;border:1px solid white;border-radius:3px;}"\
                          "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                          "QPushButton:pressed{background-color:rgba(255,255,255,150);}");

    pGroupLayout->addWidget(varlabel,0,0,Qt::AlignRight);
    pGroupLayout->addWidget(m_Varname,0,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pchildtypeLabel,1,0,Qt::AlignRight);
    pGroupLayout->addWidget(pchildtypeEdit,1,1,Qt::AlignLeft);

    pGroupLayout->addWidget(ppointnumLabel,2,0,Qt::AlignRight);
   pGroupLayout->addWidget(pspointnumpinbox,2,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pInitvalueLabel,3,0,Qt::AlignRight);
    pGroupLayout->addWidget(pInitvalueEdit,3,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pupvalueLabel,4,0,Qt::AlignRight);
    pGroupLayout->addWidget(pupvalueEdit,4,1,Qt::AlignLeft);

    pGroupLayout->addWidget(plowvalueLabel,5,0,Qt::AlignRight);
    pGroupLayout->addWidget(plowvalueEdit,5,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pSaveToDBCheckbox,7,0,1,2);
    pGroupLayout->addWidget(m_save,8,0,1,2);

    //设置布局比例
    pGroupLayout->setColumnStretch(0,1);
    pGroupLayout->setColumnStretch(1,4);

    pGroup->setLayout(pGroupLayout);
    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(pGroup);
    this->setLayout(pMainLayout);
    connect(m_save,&QPushButton::clicked,this,&NumberRegisterAttriWidget::slotCountAttriChanged);
}

void NumberRegisterAttriWidget::SetData(VARDB_REGNUM_STRU stu)
{
    m_baselist<<QString::fromUtf8(stu.pkey)<<QString::fromUtf8(stu.type)<<\
                QString::fromUtf8(stu.date)<<QString::fromUtf8(stu.fkey)<<QString::fromUtf8(stu.note);
    m_Varname->setText(QString::fromUtf8(stu.pkey));
    pchildtypeEdit->setText(QString::fromUtf8(stu.childtype));
    pspointnumpinbox->setValue(stu.pointnum);
    pInitvalueEdit->setText(QString::number(stu.Initvalue));
    pupvalueEdit->setText(QString::number(stu.upvalue));
    plowvalueEdit->setText(QString::number(stu.lowvalue));
    pSaveToDBCheckbox->setChecked(stu.save);
}

void NumberRegisterAttriWidget::slotCountAttriChanged()
{
    //获取当前widget界面数据的变化,并发送出去
    VARDB_REGNUM_STRU regstru;
    strncpy(regstru.pkey,m_baselist[0].toStdString().c_str(),32);
    strncpy(regstru.type,m_baselist[1].toStdString().c_str(),32);
    strncpy(regstru.date,m_baselist[2].toStdString().c_str(),32);
    strncpy(regstru.fkey,m_baselist[3].toStdString().c_str(),32);
    strncpy(regstru.note,m_baselist[4].toStdString().c_str(),128);
    strncpy(regstru.childtype,pchildtypeEdit->text().toStdString().c_str(),16);
    regstru.pointnum = pspointnumpinbox->value();
    regstru.Initvalue = pInitvalueEdit->text().toDouble();
    regstru.upvalue = pupvalueEdit->text().toDouble();
    regstru.lowvalue = plowvalueEdit->text().toDouble();
    regstru.save = pSaveToDBCheckbox->isChecked();
    emit signalAttriDataChanged("数值型寄存器",&regstru);
}

