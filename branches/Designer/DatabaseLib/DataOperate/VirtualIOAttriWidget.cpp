#include "VirtualIOAttriWidget.h"
#include <QIntValidator>
#include <QMessageBox>
#include <Hardware/HardwareFrom.h>

VirtualIOAttriWidget::VirtualIOAttriWidget(QWidget *parent) : QWidget(parent)
{
    QGroupBox *pGroup = new QGroupBox(tr("开关类"));
    pGroup->setStyleSheet("QGroupBox{border:1px solid lightgray;margin-top: 1.5ex;}\
                          QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top left;left:10px;margin-left: 0px;padding:0 1px;}");
                          QGridLayout *pGroupLayout = new QGridLayout();
            QLabel *varlabel = new QLabel(tr("变量名:"));
    m_varname = new QLabel(tr(""));
    QLabel *pinitvalueLabel = new QLabel(tr("初值:"));
    pinitvalueEdit = new QLineEdit();
    m_ishwioCheckbox = new QCheckBox(tr("是否为硬件IO"));
    QLabel *iodaddrlabel = new QLabel(tr("硬件IO地址:"));
    m_IOaddrEdit = new QLineEdit();
    m_IOaddrEdit->setHidden(true);

    pSaveToDBCheckbox = new QCheckBox(tr("是否保存到数据库"));
    QIntValidator *pIntValidator = new QIntValidator(this);
    pinitvalueEdit->setValidator(pIntValidator);
    QPushButton *saveAttDataBtn = new QPushButton(this);
    saveAttDataBtn->setText(tr("保存属性"));
    saveAttDataBtn->setStyleSheet("QPushButton{font:11px;border:1px solid white;border-radius:3px;}"\
                                  "QPushButton:hover{background-color:rgba(255,255,255,110);}"\
                                  "QPushButton:pressed{background-color:rgba(255,255,255,150);}");
    pGroupLayout->addWidget(varlabel,0,0,Qt::AlignRight);
    pGroupLayout->addWidget(m_varname,0,1,Qt::AlignLeft);

    pGroupLayout->addWidget(pinitvalueLabel,1,0,Qt::AlignRight);
    pGroupLayout->addWidget(pinitvalueEdit,1,1,Qt::AlignLeft);
    pGroupLayout->addWidget(m_ishwioCheckbox,2,0,1,2);
    pGroupLayout->addWidget(iodaddrlabel,3,0,1,1);
    pGroupLayout->addWidget(m_IOaddrEdit,3,1,1,1);

    pGroupLayout->addWidget(pSaveToDBCheckbox,4,0,1,2);
    pGroupLayout->addWidget(saveAttDataBtn,5,0,1,2);
    //设置布局比例
    pGroupLayout->setColumnStretch(0,1);
    pGroupLayout->setColumnStretch(1,4);

    pGroup->setLayout(pGroupLayout);
    QVBoxLayout *pMainLayout = new QVBoxLayout();

    pMainLayout->addWidget(pGroup);
    this->setLayout(pMainLayout);
    connect(saveAttDataBtn,&QPushButton::clicked,this,&VirtualIOAttriWidget::slotVirtualIOAttriChanged);
    connect(m_ishwioCheckbox,&QCheckBox::clicked,this,&VirtualIOAttriWidget::slotshowhwIO);
}

void VirtualIOAttriWidget::SetData(VARDB_SWITCH_STRU stu)
{
    m_baselist<<QString::fromUtf8(stu.pkey)<<QString::fromUtf8(stu.type)<<\
                QString::fromUtf8(stu.date)<<QString::fromUtf8(stu.fkey)<<QString::fromUtf8(stu.note);
    m_varname->setText(QString::fromUtf8(stu.pkey));
    pinitvalueEdit->setText(QString::number(stu.initvalue));
    m_ishwioCheckbox->setChecked(stu.ishwIO);
    m_IOaddrEdit->setHidden(!stu.ishwIO);
    m_IOaddrEdit->setText(QString::fromUtf8(stu.IOaddr));
    pSaveToDBCheckbox->setChecked(stu.save);
}

void VirtualIOAttriWidget::slotVirtualIOAttriChanged()
{
    VARDB_SWITCH_STRU switstru;
    strncpy(switstru.pkey,m_baselist[0].toStdString().c_str(),32);
    strncpy(switstru.type,m_baselist[1].toStdString().c_str(),32);
    strncpy(switstru.date,m_baselist[2].toStdString().c_str(),32);
    strncpy(switstru.fkey,m_baselist[3].toStdString().c_str(),32);
    strncpy(switstru.note,m_baselist[4].toStdString().c_str(),128);
    strncpy(switstru.IOaddr,m_IOaddrEdit->text().toStdString().c_str(),16);
    switstru.initvalue = pinitvalueEdit->text().toInt();
    switstru.save = pSaveToDBCheckbox->isChecked();
    switstru.ishwIO = m_ishwioCheckbox->isChecked();
    if(CheckIOAddr())//硬件IO检查
    {
        emit signalAttriDataChanged("开关类",&switstru);
    }
    else{
        QMessageBox::information(NULL,tr("警告"),tr("硬件IO地址输入错误，请重新输入！"));
    }
}

void VirtualIOAttriWidget::slotshowhwIO(bool value)
{
    m_IOaddrEdit->setHidden(!value);
}

bool VirtualIOAttriWidget::CheckIOAddr()
{
    if(!m_ishwioCheckbox->isChecked())
    {
        return true;
    }
    bool exist  = false;
    Hardwarefrom *hwlib = new Hardwarefrom();
    QMap<QString,int> IOnameMap = hwlib->GetIoInfo();
    QString IOtext = m_IOaddrEdit->text();
    QStringList list = IOtext.split(".");
    if(!IOnameMap.isEmpty()&&list.size() == 2){
        for(auto  it = IOnameMap.begin() ; it != IOnameMap.end();++it){
            if(it.key() == list[0])//名字判断
            {
                if(it.value() > 0)  //字节位判断
                {
                    QString bytebit = list[1];
                    if(bytebit.toInt() < it.value()*8 && bytebit.toInt() >= 0)
                    {
                        exist = true;
                        break;
                    }
                }
            }
        }
    }
    return exist;
}
