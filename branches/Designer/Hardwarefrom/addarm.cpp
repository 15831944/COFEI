#include "addarm.h"

AddArm::AddArm(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<HWCONFIGXML_ARMINFO_STRU>("HWCONFIGXML_ARMINFO_STRU");
     innitUI();
}

void AddArm::innitUI()
{
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{\
            background: rgb(100, 160, 220);}";

    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *Hbox =NULL;
    QLabel *name = new QLabel(this);
    name->setText("轴名称 ：");
    name->setFixedWidth(60);
    p_IdName = new QLineEdit(this);
    p_IdName->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(name);
    Hbox->addWidget(p_IdName);
    vbox->addLayout(Hbox);

    QLabel *type = new QLabel(this);
    type->setText("臂长 ：");
    type->setFixedWidth(60);
    p_ArmLenght = new QSpinBox(this);
    p_ArmLenght->setFixedWidth(150);
    p_ArmLenght->setMaximum(300);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(type);
    Hbox->addWidget(p_ArmLenght);
    vbox->addLayout(Hbox);

    QLabel *charnum = new QLabel(this);
    charnum->setText("减速比 ：");
    charnum->setFixedWidth(60);
    p_Decelerationratio = new QSpinBox(this);
    p_Decelerationratio->setMaximum(500);
    p_Decelerationratio->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(charnum);
    Hbox->addWidget(p_Decelerationratio);
    vbox->addLayout(Hbox);

    QLabel *EleRatio = new QLabel(this);
    EleRatio->setText("电子齿轮比 ：");
    EleRatio->setFixedWidth(60);
    p_ElectronicRatio = new QSpinBox(this);
    p_ElectronicRatio->setMaximum(5000);
    p_ElectronicRatio->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(EleRatio);
    Hbox->addWidget(p_ElectronicRatio);
    vbox->addLayout(Hbox);

    p_OkBut = new QPushButton(this);
    p_OkBut->setText("确定");
    p_OkBut->setStyleSheet(butssl);
    connect(p_OkBut,&QPushButton::clicked,this,&AddArm::MakeSure);
    p_NoBut = new QPushButton(this);
    p_NoBut->setText("取消");
    p_NoBut->setStyleSheet(butssl);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(p_OkBut);
    Hbox->addWidget(p_NoBut);
    vbox->addLayout(Hbox);
    this->setLayout(vbox);
}

void AddArm::MakeSure()
{
    emit Submit_armInfo(GetArmParameter());
}

HWCONFIGXML_ARMINFO_STRU AddArm::GetArmParameter()
{
    HWCONFIGXML_ARMINFO_STRU armparameter;
    armparameter.Id = p_IdName->text();
    armparameter.decrat = p_Decelerationratio->value();
    armparameter.elegearat = p_ElectronicRatio->value();
    armparameter.armlen = p_ArmLenght->value();
    return armparameter;
}

void AddArm::SetArmParameter(HWCONFIGXML_ARMINFO_STRU arm)
{
    p_IdName->setText(arm.Id);
    p_Decelerationratio->setValue(arm.decrat);
    p_ElectronicRatio->setValue(arm.elegearat);
    p_ArmLenght->setValue(arm.armlen);
}
