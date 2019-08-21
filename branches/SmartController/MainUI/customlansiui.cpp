#include "customlansiui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <MyConfig_Shared>
#include <databasemanager.h>
#include <QSettings>
CustomLanSiUI::CustomLanSiUI(QWidget *parent) : QWidget(parent)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(TimerUpDateCusUI()));
    m_font.setPointSize(11);
    m_font.setFamily("黑体");
    this->setFont(m_font);
    m_lineEditMap.clear();
    CreateCusUI();
    InitData();
    TimerUpDateCusUI();
}

void CustomLanSiUI::CreateCusUI()
{
    QVBoxLayout *vBoxMain = new QVBoxLayout(this);
    QWidget *widget = new QWidget(this);
    widget->setStyleSheet(tr("background-color:rgb(40,42,51);color:white;border-radius:15px;"));
    vBoxMain->addWidget(widget);
    this->setLayout(vBoxMain);
    QVBoxLayout *vBox = new QVBoxLayout();
    widget->setLayout(vBox);

    QHBoxLayout *hBoxCT = new QHBoxLayout();
    //vBox->setSizeConstraint(QLayout::SetFixedSize);
    hBoxCT->addWidget(CreateCusLabel("CTA:"));
    hBoxCT->addWidget(CreateCusLineEdit("CT1"));
    hBoxCT->addWidget(CreateCusLabel(" "));
    hBoxCT->addWidget(CreateCusLabel("CTB:"));
    hBoxCT->addWidget(CreateCusLineEdit("CT2"));
    hBoxCT->addWidget(CreateCusLabel(" "));
    vBox->addLayout(hBoxCT);

    QHBoxLayout *hBoxUPH = new QHBoxLayout();
    hBoxUPH->addWidget(CreateCusLabel("总产值A:"));
    hBoxUPH->addWidget(CreateCusLineEdit("UPH1"));
    hBoxUPH->addWidget(CreateCusBTN("A总产值清零"));
    hBoxUPH->addWidget(CreateCusLabel("总产值B:"));
    hBoxUPH->addWidget(CreateCusLineEdit("UPH2"));
    hBoxUPH->addWidget(CreateCusBTN("B总产值清零"));
    vBox->addLayout(hBoxUPH);

    QHBoxLayout *hBoxBoth = new QHBoxLayout();
    hBoxBoth->addWidget(CreateCusLabel("两班产值A"));
    hBoxBoth->addWidget(CreateCusLineEdit("两班产值1"));
    hBoxBoth->addWidget(CreateCusLabel(" "));
    hBoxBoth->addWidget(CreateCusLabel("两班产值B"));
    hBoxBoth->addWidget(CreateCusLineEdit("两班产值2"));
    hBoxBoth->addWidget(CreateCusLabel(" "));
    vBox->addLayout(hBoxBoth);

    QHBoxLayout *hBoxPre = new QHBoxLayout();
    hBoxPre->addWidget(CreateCusLabel("前一班产值A"));
    hBoxPre->addWidget(CreateCusLineEdit("PreCountA"));
    hBoxPre->addWidget(CreateCusLabel(" "));
    hBoxPre->addWidget(CreateCusLabel("前一班产值B"));
    hBoxPre->addWidget(CreateCusLineEdit("PreCountB"));
    hBoxPre->addWidget(CreateCusLabel(" "));
    vBox->addLayout(hBoxPre);

    QHBoxLayout *hBoxCur = new QHBoxLayout();
    hBoxCur->addWidget(CreateCusLabel("当班产值A"));
    hBoxCur->addWidget(CreateCusLineEdit("CurCount1"));
    hBoxCur->addWidget(CreateCusLabel(" "));
    hBoxCur->addWidget(CreateCusLabel("当班产值B"));
    hBoxCur->addWidget(CreateCusLineEdit("CurCount2"));
    hBoxCur->addWidget(CreateCusLabel(" "));
    vBox->addLayout(hBoxCur);

    QHBoxLayout *hBoxBtn = new QHBoxLayout();
    hBoxBtn->addWidget(CreateCusBTN("换班清零"));
    vBox->addLayout(hBoxBtn);
}

void CustomLanSiUI::InitData()
{
    m_Id.clear();
    QSettings *configini = new QSettings("Cowain","MainUI");
    if(!configini->contains("Config/PreCountA"))
    {
        configini->setValue("Config/PreCountA",0);
    }
    m_lineEditMap["PreCountA"]->setText(configini->value("Config/PreCountA").toString());

    if(!configini->contains("Config/PreCountB"))
    {
        configini->setValue("Config/PreCountB",0);
    }
    m_lineEditMap["PreCountB"]->setText(configini->value("Config/PreCountB").toString());

    if(!configini->contains("Config/CurCountA"))
    {
        configini->setValue("Config/CurCountA",0);
    }
    m_CurCountA = configini->value("Config/CurCountA").toUInt();

    if(!configini->contains("Config/CurCountB"))
    {
        configini->setValue("Config/CurCountB",0);
    }
    m_CurCountB = configini->value("Config/CurCountB").toUInt();

    m_lineEditMap["CurCount1"]->setText(QString("%1").arg(m_CurCountA));
    m_lineEditMap["CurCount2"]->setText(QString("%1").arg(m_CurCountB));

    QSqlQuery query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT ID,addCount,Info,maxCount,actCount,type FROM lifeManagement");
    int index = 0;
    while(query.next())
    {
        m_Id << query.value("ID").toString();
        QString strCnt = query.value("actCount").toString();
        QString strUPH = QString("UPH%1").arg(++index);
        if(m_lineEditMap.contains(strUPH))
        {
            m_lineEditMap[strUPH]->setText(strCnt);
        }
    }
}

QLabel *CustomLanSiUI::CreateCusLabel(const QString str)
{
    QLabel *label = new QLabel();
    label->setFont(m_font);
    label->setFixedSize(85,35);
    label->setText(str);
    label->setAlignment(Qt::AlignCenter);
    return label;
}

QLineEdit *CustomLanSiUI::CreateCusLineEdit(const QString str)
{
    QLineEdit *txt = new QLineEdit(this);
    txt->setStyleSheet("border:1px groove lightgray;border-radius:10px;");
    txt->setFont(m_font);
    txt->setFixedSize(85,35);
    txt->setReadOnly(true);
    m_lineEditMap.insert(str,txt);
    return txt;
}

QPushButton *CustomLanSiUI::CreateCusBTN(const QString str)
{
    QPushButton *btn = new QPushButton();
    btn->setText(str);
    btn ->setFixedSize(85,35);
    btn->setStyleSheet("QPushButton{background:transparent;border:1px groove lightgray;border-radius:8px;}"
                       "QPushButton:hover{background-color:rgb(155,155,155);}""QPushButton:pressed{background-color:rgb(155,155,255)}");
    btn->setFont(m_font);
    connect(btn,&QPushButton::clicked,this,&CustomLanSiUI::ReliefZero);
    return btn;
}

void CustomLanSiUI::TimerUpDateCusUI()
{
    QSqlQuery query;
    query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT ID,CycTime,EndFlag FROM TimeManageemet");
    int index = 0;
    while(query.next())
    {
        QString strCyct = query.value("CycTime").toString()+" s";
        QString strCt = QString("CT%1").arg(++index);
        if(m_lineEditMap.contains(strCt))
        {
            m_lineEditMap[strCt]->setText(strCyct);
        }
    }

    query = DataBaseManager::GetInstance()->ExcQueryDb("SELECT ID,addCount,Info,maxCount,actCount,type FROM lifeManagement");
    index = 0;
    while(query.next())
    {
        QString strCnt = query.value("actCount").toString();
        QString strUPH = QString("UPH%1").arg(++index);
        QString strCurCnt = QString("CurCount%1").arg(index);
        if(m_lineEditMap.contains(strUPH))
        {
            uint iNum = strCnt.toUInt() - m_lineEditMap[strUPH]->text().toUInt();
            if(iNum > 0)
            {
                QSettings *configini = new QSettings("Cowain","MainUI");
                if(1 == index)
                {
                    m_CurCountA += iNum;
                    m_lineEditMap[strCurCnt]->setText(QString("%1").arg(m_CurCountA));
                    configini->setValue("Config/CurCountA",m_CurCountA);
                }
                else
                {
                    m_CurCountB += iNum;
                    m_lineEditMap[strCurCnt]->setText(QString("%1").arg(m_CurCountB));
                    configini->setValue("Config/CurCountB",m_CurCountB);
                }
            }
            m_lineEditMap[strUPH]->setText(strCnt);
        }
    }
    m_lineEditMap["两班产值1"]->setText(QString("%1").arg(m_lineEditMap["PreCountA"]->text().toUInt()+
                                    m_lineEditMap["CurCount1"]->text().toUInt()));
    m_lineEditMap["两班产值2"]->setText(QString("%1").arg(m_lineEditMap["PreCountB"]->text().toUInt()+
                                    m_lineEditMap["CurCount2"]->text().toUInt()));
}

void CustomLanSiUI::ReliefZero()
{
    QPushButton *qBtn = qobject_cast<QPushButton*>(sender());
    QString text = qBtn->text();
    if(text.compare("换班清零") == 0)
    {
        QSettings *configini = new QSettings("Cowain","MainUI");
        configini->setValue("Config/PreCountA",m_lineEditMap["CurCount1"]->text());
        configini->setValue("Config/PreCountB",m_lineEditMap["CurCount2"]->text());
        configini->setValue("Config/CurCountB",0);
        configini->setValue("Config/CurCountA",0);
        m_CurCountA = 0;
        m_CurCountB = 0;
        m_lineEditMap["PreCountA"]->setText(m_lineEditMap["CurCount1"]->text());
        m_lineEditMap["CurCount1"]->setText("0");
        m_lineEditMap["PreCountB"]->setText(m_lineEditMap["CurCount2"]->text());
        m_lineEditMap["CurCount2"]->setText("0");

        m_lineEditMap["两班产值1"]->setText(QString("%1").arg(m_lineEditMap["PreCountA"]->text().toUInt()+
                                        m_lineEditMap["CurCount1"]->text().toUInt()));
        m_lineEditMap["两班产值2"]->setText(QString("%1").arg(m_lineEditMap["PreCountB"]->text().toUInt()+
                                        m_lineEditMap["CurCount2"]->text().toUInt()));
    }
    else if(text.compare("A总产值清零") == 0)
    {
        if(m_Id.count() > 0)
        {
            QStringList key;
            QStringList value;
            key<<QString(tr("actCount"));
            value<<QString("%1").arg(0);
            QString exp = QString("ID = '%1'").arg(m_Id[0]);
            DataBaseManager::GetInstance()->ExcUpdateDb("lifeManagement",key,value,exp);
            m_lineEditMap["UPH1"]->setText("0");
        }
    }
    else if(text.compare("B总产值清零") == 0)
    {
        if(m_Id.count() > 1)
        {
            QStringList key;
            QStringList value;
            key<<QString(tr("actCount"));
            value<<QString("%1").arg(0);
            QString exp = QString("ID = '%1'").arg(m_Id[1]);
            DataBaseManager::GetInstance()->ExcUpdateDb("lifeManagement",key,value,exp);
            m_lineEditMap["UPH2"]->setText("0");
        }
    }

}

void CustomLanSiUI::GetUIEnableSig(bool isenable)
{
    if(isenable)
    {
        m_pTimer->stop();
    }
    else
    {
        m_pTimer->start(500);
    }
}
