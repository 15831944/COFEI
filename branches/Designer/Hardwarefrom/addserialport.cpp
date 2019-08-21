#include "addserialport.h"

AddSerialPort::AddSerialPort(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<HWCONFIGXML_COMPORT_STRU>("HWCONFIGXML_COMPORT_STRU");
    initUI();
}

void AddSerialPort::initUI()
{
    QString butssl = "QPushButton{color:black;border:0.5px solid white;border-radius:5px;}QPushButton:enabled:hover{\
            background: rgb(100, 160, 220);}";
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox;
    QLabel *ID = new QLabel(this);
    ID->setText("ID : ");
    ID->setFixedWidth(60);
    p_id = new QLineEdit(this);
    p_id->setFixedWidth(150);
    hbox = new QHBoxLayout();
    hbox->addWidget(ID);
    hbox->addWidget(p_id);
    vbox->addLayout(hbox);

    QLabel *seriaName = new QLabel(this);
    seriaName->setText("串口名 ：");
    seriaName->setFixedWidth(60);
    p_serialname = new QLineEdit(this);
    p_serialname->setFixedWidth(150);
    hbox = new QHBoxLayout();
    hbox->addWidget(seriaName);
    hbox->addWidget(p_serialname);
    vbox->addLayout(hbox);


    QLabel *headendname = new QLabel(this);
    headendname->setText("头尾设置 ：");
    headendname->setFixedWidth(60);
    p_headendname = new QLineEdit(this);
    p_headendname->setFixedWidth(150);
    p_headendname->setPlaceholderText("以;分割头尾字符例:!!;?*");
    hbox = new QHBoxLayout();
    hbox->addWidget(headendname);
    hbox->addWidget(p_headendname);
    vbox->addLayout(hbox);


//    QLabel* contain = new QLabel(this);
//    contain->setText("信息包含 ：");
//    contain->setFixedWidth(60);
//    p_contain = new QLineEdit(this);
//    p_contain->setFixedWidth(150);
//    hbox = new QHBoxLayout();
//    hbox->addWidget(contain);
//    hbox->addWidget(p_contain);
//    vbox->addLayout(hbox);

    QLabel *buadrate = new QLabel(this);
    buadrate->setText("波特率 ：");
    buadrate->setFixedWidth(60);
    p_buadrate = new QComboBox(this);
    QStringList buadratelist;
    buadratelist<<"1200"<<"4800"<<"9600"<<"19200"<<"115200"<<"128000";
    p_buadrate->addItems(buadratelist);
    p_buadrate->setFixedWidth(150);
    hbox = new QHBoxLayout(this);
    hbox->addWidget(buadrate);
    hbox->addWidget(p_buadrate);
    vbox->addLayout(hbox);

//    QLabel* stop = new QLabel(this);
//    stop->setText("结束符 ：");
//    stop->setFixedWidth(60);
//    p_stop = new QLineEdit(this);
//    p_stop->setFixedWidth(150);
//    hbox = new QHBoxLayout();
//    hbox->addWidget(stop);
//    hbox->addWidget(p_stop);
//    vbox->addLayout(hbox);

//    QLabel *stopchar = new QLabel(this);
//    stopchar->setText("停止位 ：");
//    stopchar->setFixedWidth(60);
//    p_stopchar = new QLineEdit(this);
//    p_stopchar->setFixedWidth(150);
//    hbox = new QHBoxLayout(this);
//    hbox->addWidget(stopchar);
//    hbox->addWidget(p_stopchar);
//    vbox->addLayout(hbox);

    QLabel *NumChar = new QLabel(this);
    NumChar->setText("数据位 ：");
    NumChar->setFixedWidth(60);
    p_NunChar = new QComboBox(this);
    p_NunChar->setFixedWidth(150);
    QStringList Numcharlist;
    Numcharlist<<"5"<<"6"<<"7"<<"8";
    p_NunChar->addItems(Numcharlist);
    hbox = new QHBoxLayout();
    hbox->addWidget(NumChar);
    hbox->addWidget(p_NunChar);
    vbox->addLayout(hbox);

//    QLabel *sendpath = new QLabel(this);
//    sendpath->setText("发送地址 ：");
//    sendpath->setFixedWidth(60);
//    p_sendpath = new QLineEdit(this);
//    p_sendpath->setFixedWidth(150);
//    hbox = new QHBoxLayout();
//    hbox->addWidget(sendpath);
//    hbox->addWidget(p_sendpath);
//    vbox->addLayout(hbox);

    QLabel *Paritycheck = new QLabel(this);
    Paritycheck->setText("奇偶校验 ：");
    Paritycheck->setFixedWidth(60);
    p_Paritycheck = new QComboBox(this);
    QStringList paritlist;
    paritlist<<"奇校验"<<"偶校验"<<"无校验";
    p_Paritycheck->addItems(paritlist);
    p_Paritycheck->setFixedWidth(150);
    hbox = new QHBoxLayout();
    hbox->addWidget(Paritycheck);
    hbox->addWidget(p_Paritycheck);
    vbox->addLayout(hbox);

//    QLabel *savepath = new QLabel(this);
//    savepath->setFixedWidth(60);
//    savepath->setText("保存地址 ：");
//    p_savepath = new QLineEdit(this);
//    p_savepath->setFixedWidth(150);
//    hbox = new QHBoxLayout();
//    hbox->addWidget(savepath);
//    hbox->addWidget(p_savepath);
//    vbox->addLayout(hbox);

    p_Okbut = new QPushButton(this);
    p_Okbut->setText("确定");
    p_Okbut->setStyleSheet(butssl);
    connect(p_Okbut,&QPushButton::clicked,this,&AddSerialPort::MakeSure);
    p_cleanbut = new QPushButton(this);
    p_cleanbut->setStyleSheet(butssl);
    p_cleanbut->setText("取消");
    //connect(p_cleanbut,&QPushButton::clicked,this,&QDialog::reject);
    hbox = new QHBoxLayout();
    hbox->addWidget(p_Okbut);
    hbox->addWidget(p_cleanbut);
    vbox->addLayout(hbox);
    this->setLayout(vbox);
}
HWCONFIGXML_COMPORT_STRU AddSerialPort::GetSerialPortParameter()
{
    HWCONFIGXML_COMPORT_STRU serialParameter;
    serialParameter.Id = p_id->text();
    serialParameter.baudRate = p_buadrate->currentText();
    serialParameter.portName = p_serialname->text();
    serialParameter.headendData = p_headendname->text();
    return serialParameter;
}

void AddSerialPort::MakeSure()
{
    emit Conmit_serialport(GetSerialPortParameter());
}

void AddSerialPort::SetSerialPortParameter(HWCONFIGXML_COMPORT_STRU serial)
{
    p_id->setText(serial.Id);
    p_buadrate->setCurrentText(serial.baudRate);
    p_serialname->setText(serial.portName);
    p_headendname->setText(serial.headendData);
}
