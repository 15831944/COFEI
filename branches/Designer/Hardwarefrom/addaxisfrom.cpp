#include "addaxisfrom.h"
#include <QDebug>
AddAxisFrom::AddAxisFrom(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<HWCONFIGXML_SEROUTPUT_STRU>("HWCONFIGXML_SEROUTPUT_STRU");
    innitUI();
}

void AddAxisFrom::innitUI()
{
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{\
            background: rgb(100, 160, 220);}";
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox = NULL;
    QLabel * name = new QLabel(this);
    name->setText("轴名称 ：");
    name->setFixedWidth(60);
    p_namelet = new QLineEdit(this);
    p_namelet->setFixedWidth(150);
    QLabel *mode = new QLabel(this);
    mode->setText("模式 ：");
    mode->setFixedWidth(60);
    p_Mode = new QSpinBox(this);
    p_Mode->setMaximum(10);
    p_Mode->setMinimum(0);
    p_Mode->setValue(1);
    hbox = new QHBoxLayout();
    hbox->addWidget(name);
    hbox->addWidget(p_namelet);
    vbox->addLayout(hbox);
    hbox = new QHBoxLayout();
    hbox->addWidget(mode);
    hbox->addWidget(p_Mode);
    vbox->addLayout(hbox);

    QLabel * LimetUp_IO = new QLabel(this);
    LimetUp_IO->setText("正极限 ：");
    LimetUp_IO->setFixedWidth(60);
    p_Limit_lowIo = new QLineEdit(this);
    p_Limit_lowIo->setFixedWidth(150);
    QLabel *LimetLow_IO = new QLabel(this);
    LimetLow_IO->setText("负极限 ：");
    LimetLow_IO->setFixedWidth(60);
    p_Limit_upIo = new QLineEdit(this);
    p_Limit_upIo->setFixedWidth(150);
    hbox = new QHBoxLayout();
    hbox->addWidget(LimetUp_IO);
    hbox->addWidget(p_Limit_lowIo);
    vbox->addLayout(hbox);
    hbox = new QHBoxLayout();
    hbox->addWidget(LimetLow_IO);
    hbox->addWidget(p_Limit_upIo);
    vbox->addLayout(hbox);

    QLabel *Orglab = new QLabel(this);
    Orglab->setText("原点 ：");
    Orglab->setFixedWidth(60);
    p_mOrg = new QLineEdit(this);
    p_mOrg->setFixedWidth(150);
    hbox = new QHBoxLayout();
    hbox->addWidget(Orglab);
    hbox->addWidget(p_mOrg);
    vbox->addLayout(hbox);

    QLabel * LimetUp_val = new QLabel(this);
    LimetUp_val->setText("软极限原点 ：");
    LimetUp_val->setFixedWidth(60);
    p_Limit_lowVal = new QSpinBox(this);
    p_Limit_lowVal->setMaximum(9999999);
    p_Limit_lowVal->setMinimum(-999999);
    p_Limit_lowVal->setFixedWidth(150);
    QLabel *LimetLow_val = new QLabel(this);
    LimetLow_val->setText("软极限终点 ：");
    LimetLow_val->setFixedWidth(60);
    p_Limit_upVal = new QSpinBox(this);
    p_Limit_upVal->setMaximum(9999999);
    p_Limit_upVal->setMinimum(-99999999);
    p_Limit_upVal->setFixedWidth(150);
    hbox = new QHBoxLayout();
    hbox->addWidget(LimetUp_val);
    hbox->addWidget(p_Limit_lowVal);
       vbox->addLayout(hbox);
       hbox = new QHBoxLayout();
    hbox->addWidget(LimetLow_val);
    hbox->addWidget(p_Limit_upVal);
    vbox->addLayout(hbox);

    // 偏差 加速度系数
    QLabel * offist = new QLabel(this);
    offist->setText("偏差 ：");
    offist->setFixedWidth(60);
    p_Offsict = new QSpinBox(this);
    p_Offsict->setFixedWidth(150);
    p_Offsict->setMaximum(500);
    p_Offsict->setMinimum(-500);
    p_Offsict->setValue(10);
    QLabel * addval = new QLabel(this);
    addval->setText("速度系数 ：");
    addval->setFixedWidth(60);
    p_AddVal = new QSpinBox(this);
    p_AddVal->setMaximum(5000000);
    p_AddVal->setMinimum(1);
    p_AddVal->setValue(1);
    hbox = new QHBoxLayout();
    hbox->addWidget(offist);
    hbox->addWidget(p_Offsict);
    vbox->addLayout(hbox);
    QLabel *accdcc = new QLabel(this);
    accdcc->setText("加减速系数");
    accdcc->setFixedWidth(60);
    m_pDccAccValue = new QSpinBox(this);
    m_pDccAccValue->setMaximum(5000000);
    m_pDccAccValue->setValue(100);
    hbox = new QHBoxLayout();
    hbox->addWidget(accdcc);
    hbox->addWidget(m_pDccAccValue);
    vbox->addLayout(hbox);

    hbox = new QHBoxLayout();
    hbox->addWidget(addval);
    hbox->addWidget(p_AddVal);
    vbox->addLayout(hbox);

    p_Okbutn = new QPushButton(this);
    p_Okbutn->setText("确定");
    p_Okbutn->setStyleSheet(butssl);
    connect(p_Okbutn,&QPushButton::clicked,this,&AddAxisFrom::MakeSure);
    p_Cancelbut = new QPushButton(this);
    p_Cancelbut->setStyleSheet(butssl);
    p_Cancelbut->setText("取消");
    //connect(p_Cancelbut,&QPushButton::clicked,this,&QDialog::reject);
    hbox = new QHBoxLayout();
    hbox->addWidget(p_Okbutn);
    hbox->addWidget(p_Cancelbut);
    vbox->addLayout(hbox);
    this->setLayout(vbox);
}

void AddAxisFrom::MakeSure()
{
   // accept();
    emit Submit_axisinfo( GetAxisParameter());
}

HWCONFIGXML_SEROUTPUT_STRU AddAxisFrom::GetAxisParameter()
{
    HWCONFIGXML_SEROUTPUT_STRU axis_stru;
    axis_stru.id = p_namelet->text();
    axis_stru.swLimitStart = p_Limit_lowVal->text();
    axis_stru.swLimitEnd = p_Limit_upVal->text();
    axis_stru.hwLimitStart = p_Limit_lowIo->text();
    axis_stru.hwLimitEnd = p_Limit_upIo->text();
    axis_stru.lead = p_AddVal->value();
    axis_stru.modelType = QString("%1").arg(p_Mode->value());
    axis_stru.error = p_Offsict->value();
    axis_stru.adled = m_pDccAccValue->value();
    axis_stru.orgLimit = p_mOrg->text();
    return axis_stru;
}

void AddAxisFrom::SetAxisParameter(HWCONFIGXML_SEROUTPUT_STRU Axis)
{
    p_namelet->setText(Axis.id);
    p_Limit_lowVal->setValue(Axis.swLimitStart.toDouble());
    p_Limit_upVal->setValue(Axis.swLimitEnd.toDouble());
    p_Limit_lowIo->setText(Axis.hwLimitStart);
    p_Limit_upIo->setText(Axis.hwLimitEnd);
    p_AddVal->setValue(Axis.lead);
    p_Mode->setValue(Axis.modelType.toInt());
    p_Offsict->setValue(Axis.error);
    m_pDccAccValue->setValue(Axis.adled);
    p_mOrg->setText(Axis.orgLimit);

}
