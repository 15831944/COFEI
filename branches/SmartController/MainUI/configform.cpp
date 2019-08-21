#include "configform.h"
#include <QGridLayout>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QSettings>
#include <MyConfig_Shared>
#include <QMessageBox>
#include <QComboBox>
#include <QTextEdit>
#include <QProcess>
#include <QStandardPaths>
#include <QDebug>

ConfigForm::ConfigForm(QVector<UIXML_STRU> vec,QWidget *parent) : QWidget(parent)
{
    m_uivec = vec;
    CreateUI();
    SetEditText();
    this->setGeometry(5,5,parent->width()-5,parent->height()-5);
}

ConfigForm::~ConfigForm()
{
}
void ConfigForm::CreateUI()
{
    QFont font;
    font.setPointSize(13);
    font.setFamily("黑体");
    QVBoxLayout *vBox = new QVBoxLayout(this);
    vBox->setSpacing(20);
//    vBox->setAlignment(Qt::AlignTop);

//    QHBoxLayout *hBox = new QHBoxLayout();
//    hBox->setAlignment(Qt::AlignLeft);
//    QPushButton *btnOP = new QPushButton(this);
//    btnOP->setFixedSize(100,30);
//    btnOP->setFont(font);
//    btnOP->setText("用户提权");
//    connect(btnOP,&QPushButton::clicked,this,&ConfigForm::ChmodUsr);
//    hBox->addWidget(btnOP);
//    vBox->addLayout(hBox);

    QGridLayout *baseLayout = new QGridLayout();
    int ir = 0;
    for(int i(0);i < m_uivec.count();++i)
    {
        if(m_uivec[i].className == "Text")
        {
            QWidget *w = new QWidget(this);
            QLabel *l = new QLabel(w);
            l->setAlignment(Qt::AlignLeft);
            l->setFont(font);
            l->setText(m_uivec[i].strText);
            l->setFixedWidth(100);
            QLineEdit *let = new QLineEdit(w);
            let->setFont(font);
            let->resize(m_uivec[i].csize);
            let->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:5px;"));
            let->move(l->width()+1,2);
            m_lineEditMap.insert(m_uivec[i].address,let);
            w->setFixedSize(101+m_uivec[i].csize.width(),m_uivec[i].csize.height()+10);

            static int ic = 0;

            baseLayout->addWidget(w,ir,ic++);
            if(ic > 3)
            {
                ++ir;
                ic = 0;
            }
        }
        else if ("托盘重置按钮" == m_uivec[i].className)
        {
            QComboBox *comBo = new QComboBox(this);
            comBo->setGeometry(50, 50, 200, 30);
            for (auto item = ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_PalletInfo.begin();
                 item != ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_PalletInfo.end(); item++)
            {
                comBo->addItem(item.key());
            }

            baseLayout->addWidget(comBo);

            connect(comBo, SIGNAL(activated(const QString &)), this, SLOT(ShowMessage(const QString &)));
        }
    }
    vBox->addLayout(baseLayout);

    QHBoxLayout *hBox1 = new QHBoxLayout();
    QPushButton *btn = new QPushButton(this);
    btn->setFixedSize(100,30);
    btn->setFont(font);
    btn->setText("保存");
    connect(btn,&QPushButton::clicked,this,&ConfigForm::SaveEditText);
    hBox1->addWidget(btn);
    vBox->addLayout(hBox1);

    this->setLayout(vBox);
}
void ConfigForm::ChmodUsr()
{
    QPushButton *pBtn = qobject_cast<QPushButton*>(sender());
    QProcess *poc = new QProcess;
    QString path = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/用户文件夹";
    if(pBtn->text() == "用户提权")
    {
        QString str = "chattr -R -ai " + path;
        poc->start(str);
        pBtn->setText("用户加密");
    }
    else
    {
        QString str = "chattr -R +ai " + path;
        poc->start(str);
        pBtn->setText("用户提权");
    }
}
void ConfigForm::SetEditText()
{
    QSettings *configini = new QSettings("Cowain","MainUI");
    ConfigData::GetInstance()->m_cusTime = configini->value("Config/CusTime").toUInt();
    ConfigData::GetInstance()->productTolCount = configini->value("Config/Tote").toUInt();
    for(auto item = m_lineEditMap.begin();item != m_lineEditMap.end();++item)
    {
        if(item.key() == "速度")
        {
            item.value()->setText( configini->value("Config/Speed").toString());
            ConfigData::GetInstance()->iSpeed = configini->value("Config/Speed").toUInt();
        }
        else if(item.key() == "加速")
        {
            item.value()->setText(configini->value("Config/Accel").toString());
            ConfigData::GetInstance()->iAccel = configini->value("Config/Accel").toUInt();
        }
        else if(item.key() == "减速")
        {
            item.value()->setText(configini->value("Config/Dccel").toString());
            ConfigData::GetInstance()->iDccel = configini->value("Config/Dccel").toUInt();
        }
        else if(item.key() == "产量")
        {
            item.value()->setText(configini->value("Config/Products").toString());
            ConfigData::GetInstance()->productTempTotal= ConfigData::GetInstance()->productTotal = configini->value("Config/Products").toUInt();
        }
        else if(item.key() == "打磨时间")
        {
            item.value()->setText( configini->value("Config/PoTime").toString());
            ConfigData::GetInstance()->icspSpeed = configini->value("Config/PoTime").toUInt();
        }
        else if(item.key() == "打磨次数")
        {
            item.value()->setText(configini->value("Config/PoCount").toString());
            ConfigData::GetInstance()->icspAccel = configini->value("Config/PoCount").toUInt();
        }
    }
}

void ConfigForm::SaveEditText()
{
    QSettings *configini = new QSettings("Cowain","MainUI");
    for(auto item = m_lineEditMap.begin();item != m_lineEditMap.end();++item)
    {
        if(item.key() == "速度")
        {
            configini->setValue("Config/Speed",item.value()->text().toUInt());
            ConfigData::GetInstance()->iSpeed = item.value()->text().toUInt();
        }
        else if(item.key() == "加速")
        {
            configini->setValue("Config/Accel",item.value()->text().toUInt());
            ConfigData::GetInstance()->iAccel = configini->value("Config/Accel").toUInt();
        }
        else if(item.key() == "减速")
        {
            configini->setValue("Config/Dccel",item.value()->text().toUInt());
            ConfigData::GetInstance()->iDccel = configini->value("Config/Dccel").toUInt();
        }
        else if(item.key() == "产量")
        {
            configini->setValue("Config/Products",item.value()->text().toUInt());
            ConfigData::GetInstance()->productTempTotal = ConfigData::GetInstance()->productTotal = configini->value("Config/Products").toUInt();
        }    
        else if(item.key() == "打磨时间")
        {
            configini->setValue("Config/PoTime",item.value()->text().toUInt());
            ConfigData::GetInstance()->icspSpeed = item.value()->text().toUInt();
        }
        else if(item.key() == "打磨次数")
        {
            configini->setValue("Config/PoCount",item.value()->text().toUInt());
            ConfigData::GetInstance()->icspAccel = item.value()->text().toUInt();
        }
    }
    return;
}

void ConfigForm::ShowMessage(const QString str)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::warning(this, tr("重置托盘"),
                                   QString(tr("回到Home位置并重新计数.\n确认托盘停止后再执行此操作．")),
                                   QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes) {

        QMessageBox::information(this, tr("重置托盘"), QString(tr("即将重置托盘")), QMessageBox::Yes | QMessageBox::No);
        FunModuleInterface *p = ConfigData::GetInstance()->childPluginMap["主程序"]["托盘指令"];

        SWCONFIGXML_STRU ps;
        ps.palletCmdParam.name = "G03";
        ps.palletCmdParam.param1 = str;
        ps.palletCmdParam.param2 = "(0;0;0)";

        QString msg;

        p->Initialization();
        p->SetModuleParam(ps);
        p->RunModule();
        p->GetExcResult(msg);
        p->StopModule();

        QMessageBox::information(this, tr("重置托盘"), msg, QMessageBox::Yes | QMessageBox::No);
    }
}
