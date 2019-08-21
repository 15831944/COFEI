#include "addiofrom.h"

AddIoFrom::AddIoFrom(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<HWCONFIGXML_SERINPUT_STRU>("HWCONFIGXML_SERINPUT_STRU");
    initUI();
    m_isAdd  = true;
}

void AddIoFrom::initUI()
{
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{\
            background: rgb(100, 160, 220);}";
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *Hbox =NULL;
    QLabel *name = new QLabel(this);
    name->setText("Io名称 ：");
    name->setFixedWidth(60);
    p_IdName = new QLineEdit(this);
    p_IdName->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(name);
    Hbox->addWidget(p_IdName);
    vbox->addLayout(Hbox);

    QLabel *type = new QLabel(this);
    type->setText("Io类型 ：");
    type->setFixedWidth(60);
    p_TpyeLint = new QLineEdit(this);
    p_TpyeLint->setText(tr("EC"));
    p_TpyeLint->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(type);
    Hbox->addWidget(p_TpyeLint);
    vbox->addLayout(Hbox);

    QLabel *charnum = new QLabel(this);
    charnum->setText("字节数 ：");
    charnum->setFixedWidth(60);
    p_charNum = new QSpinBox(this);
    p_charNum->setValue(1);
    p_charNum->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(charnum);
    Hbox->addWidget(p_charNum);
    vbox->addLayout(Hbox);

    p_OkBut = new QPushButton(this);
    p_OkBut->setText("确定");
    p_OkBut->setStyleSheet(butssl);
    connect(p_OkBut,&QPushButton::clicked,this,&AddIoFrom::MakeSure);
    p_NoBut = new QPushButton(this);
    p_NoBut->setStyleSheet(butssl);
    p_NoBut->setText("取消");
    Hbox = new QHBoxLayout();
    Hbox->addWidget(p_OkBut);
    Hbox->addWidget(p_NoBut);
    vbox->addLayout(Hbox);
    this->setLayout(vbox);
}

void AddIoFrom::MakeSure()
{
    if(m_isAdd)
        emit Submit(GetIoParameter());
    else
        emit Modify(m_preParam,GetIoParameter());
}

HWCONFIGXML_IO_STRU AddIoFrom::GetIoParameter()
{
    HWCONFIGXML_IO_STRU iostru;
    iostru.id = p_IdName->text();
    iostru.iByteNum = p_charNum->value();
    iostru.strType = p_TpyeLint->text();
    return iostru;
}

void AddIoFrom::SetIoParameter(HWCONFIGXML_IO_STRU Iostru)
{
    m_isAdd = false;
    m_preParam = Iostru;
    p_IdName->setText(Iostru.id);
    p_charNum->setValue(Iostru.iByteNum);
    p_TpyeLint->setText(Iostru.strType);

}
