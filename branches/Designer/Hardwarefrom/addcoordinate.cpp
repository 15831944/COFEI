#include "addcoordinate.h"

AddCoordinate::AddCoordinate(QWidget *parent) : QWidget(parent)
{
     qRegisterMetaType<COORDINATESTUR>("COORDINATESTUR");
    initUI();
}

void AddCoordinate::initUI()
{
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{\
            background: rgb(100, 160, 220);}";
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *Hbox =NULL;
    QLabel *name = new QLabel(this);
    name->setText("坐标名称 ：");
    name->setFixedWidth(60);
    p_IdName = new QLineEdit(this);
    p_IdName->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(name);
    Hbox->addWidget(p_IdName);
    vbox->addLayout(Hbox);

    QLabel *type = new QLabel(this);
    type->setText("正极限 ：");
    type->setFixedWidth(60);
    p_limitEnd = new QSpinBox(this);
    p_limitEnd->setFixedWidth(150);
    p_limitEnd->setMaximum(300000);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(type);
    Hbox->addWidget(p_limitEnd);
    vbox->addLayout(Hbox);

    QLabel *charnum = new QLabel(this);
    charnum->setText("负极限 ：");
    charnum->setFixedWidth(60);
    p_limitStart = new QSpinBox(this);
    p_limitStart->setMaximum(500000);
    p_limitStart->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(charnum);
    Hbox->addWidget(p_limitStart);
    vbox->addLayout(Hbox);

    QLabel *EleRatio = new QLabel(this);
    EleRatio->setText("包含轴 ：");
    EleRatio->setFixedWidth(60);
    p_Contain = new QLineEdit(this);
    p_Contain->setFixedWidth(150);
    Hbox = new QHBoxLayout();
    Hbox->addWidget(EleRatio);
    Hbox->addWidget(p_Contain);
    vbox->addLayout(Hbox);

    p_OkBut = new QPushButton(this);
    p_OkBut->setText("确定");
    p_OkBut->setStyleSheet(butssl);
    connect(p_OkBut,&QPushButton::clicked,this,&AddCoordinate::MakeSure);
    p_NoBut = new QPushButton(this);
    p_NoBut->setStyleSheet(butssl);
    p_NoBut->setText("取消");
    Hbox = new QHBoxLayout();
    Hbox->addWidget(p_OkBut);
    Hbox->addWidget(p_NoBut);
    vbox->addLayout(Hbox);
    this->setLayout(vbox);
}

void AddCoordinate::MakeSure()
{
   emit Submit(GetCoordinate());
}

COORDINATESTUR AddCoordinate::GetCoordinate()
{
    COORDINATESTUR coordiante;
    coordiante.contain = p_Contain->text();
    coordiante.coordname = p_IdName->text();
    coordiante.limtend = p_limitEnd->value();
    coordiante.limtstart = p_limitStart->value();
    return coordiante;
}

void AddCoordinate::SetParameter(COORDINATESTUR corstru)
{
    p_Contain->setText(corstru.contain);
    p_IdName->setText(corstru.coordname);
    p_limitEnd->setValue(corstru.limtend);
    p_limitStart->setValue(corstru.limtstart);
}
