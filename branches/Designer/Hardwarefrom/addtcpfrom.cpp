#include "addtcpfrom.h"

AddTcpFrom::AddTcpFrom(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<HWCONFIGXML_INTNET_STRU>("HWCONFIGXML_INTNET_STRU");
    initUI();

}

void AddTcpFrom::initUI()
{
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{\
            background: rgb(100, 160, 220);}";
    vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox1 = new QHBoxLayout();
    QLabel *p_id = new QLabel(this);
    p_id->setText("ID : ");
    p_id->setFixedWidth(60);
    ID = new QLineEdit(this);
    ID->setFixedWidth(150);
    hbox1->addWidget(p_id);
    hbox1->addWidget(ID);
    vbox->addLayout(hbox1);
    p_PcNameLab = new QLabel(this);
    p_PcNameLab->setText("IP：　");
    p_PcNameLab->setFixedWidth(60);
    p_PcNameLeit = new QLineEdit(this);
    connect(p_PcNameLeit,&QLineEdit::textEdited,this,&AddTcpFrom::IpLineEditClick);
    p_PcNameLeit->setFixedWidth(150);
    hbox1 = new QHBoxLayout();
    hbox1->addWidget(p_PcNameLab);
    hbox1->addWidget(p_PcNameLeit);
     vbox->addLayout(hbox1);

    p_PortNameLab  = new QLabel(this);
    p_PortNameLab->setText("端口号：　");
    p_PortNameLab->setFixedWidth(60);
    p_PortNameLeit = new QLineEdit(this);
    p_PortNameLeit->setFixedWidth(150);
    QHBoxLayout *hbox2 = new QHBoxLayout();
    hbox2->addWidget(p_PortNameLab);
    hbox2->addWidget(p_PortNameLeit);
    vbox->addLayout(hbox2);

//    p_ContansLab = new QLabel(this);
//    p_ContansLab->setText("信息包含：　");
//    p_ContansLab->setFixedWidth(60);
//    p_ContansLeit = new QLineEdit(this);
//    p_ContansLeit->setFixedWidth(150);
//    QHBoxLayout *hbox3 = new QHBoxLayout();
//    hbox3->addWidget(p_ContansLab);
//    hbox3->addWidget(p_ContansLeit);
//    vbox->addLayout(hbox3);

//    p_TimeOutLab = new QLabel(this);
//    p_TimeOutLab->setText("超时：　");
//    p_TimeOutLab->setFixedWidth(60);
//    p_TimeOutLeit = new QLineEdit(this);
//    p_TimeOutLeit->setFixedWidth(150);
//    QHBoxLayout *hbox4 = new QHBoxLayout();
//    hbox4->addWidget(p_TimeOutLab);
//    hbox4->addWidget(p_TimeOutLeit);
//    vbox->addLayout(hbox4);

//    QHBoxLayout * hbox;
//    QLabel * sendadrs = new QLabel(this);
//    sendadrs->setText("发送地址： ");
//    sendadrs->setFixedWidth(60);
//    p_SendAddress = new QLineEdit(this);
//    p_SendAddress->setFixedWidth(150);
//    hbox = new QHBoxLayout();
//    hbox->addWidget(sendadrs);
//    hbox->addWidget(p_SendAddress);
//    vbox->addLayout(hbox);

//    QLabel *resive = new QLabel(this);
//    resive->setFixedWidth(60);
//    resive->setText("Log： ");
//    p_RessiveAddress = new QLineEdit(this);
//    p_RessiveAddress->setFixedWidth(150);
//    hbox = new QHBoxLayout();
//    hbox->addWidget(resive);
//    hbox->addWidget(p_RessiveAddress);
//    vbox->addLayout(hbox);

    QHBoxLayout* hbox5 = new QHBoxLayout();
    p_Okbtun = new QPushButton(this);
    p_Okbtun->setStyleSheet(butssl);
    p_Okbtun->setText("确定");
    connect(p_Okbtun,&QPushButton::clicked,this,&AddTcpFrom::Commite);
    p_Nobtun = new QPushButton(this);
    p_Nobtun->setStyleSheet(butssl);
    p_Nobtun->setText("取消");
   // connect(p_Nobtun,&QPushButton::clicked,this,&QDialog::reject);
    hbox5->addWidget(p_Okbtun);
    //hbox5->addStretch(5);
    hbox5->addWidget(p_Nobtun);
    vbox->addLayout(hbox5);

    p_Discraption = new QLabel(this);
    p_Discraption->setText("说明： ");
    p_Discraption->setFixedHeight(30);
    vbox->addWidget(p_Discraption);
    this->setLayout(vbox);
}

void AddTcpFrom::Commite()
{
    HWCONFIGXML_INTNET_STRU paramer;
    paramer.Id = ID->text();
    paramer.ipAdd = p_PcNameLeit->text();
    if(p_PcNameLeit->text().size()<6)
    {
        paramer.serType = "服务器";
    }
    else
    {
        paramer.serType = "客户端";
    }
    paramer.portName = p_PortNameLeit->text();
    emit tcpparametar(paramer);
}

void AddTcpFrom::IpLineEditClick(const QString &str)
{
     Q_UNUSED(str);
    p_Discraption->setText("说明： Ip地址为空时，做服务器处理。");
}

void AddTcpFrom::SetTcpParameter(HWCONFIGXML_INTNET_STRU tcp)
{
  ID->setText(tcp.Id);
  p_PcNameLeit->setText(tcp.ipAdd);
  p_PortNameLeit->setText(tcp.portName);
}
