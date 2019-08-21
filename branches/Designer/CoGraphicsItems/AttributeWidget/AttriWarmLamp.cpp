#include <QScrollArea>
#include <QGridLayout>
#include "../TBasicControls/WarnLamp.h"
#include "AttriWarmLamp.h"

AttriWarmLamp::AttriWarmLamp(QWidget *parent)
    : GraphicAttrWidget(parent)
{
    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pAttriWidget = CreateAttri();
    pTabWidget->addTab(pAttriWidget,tr("属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

QWidget *AttriWarmLamp::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();

    QGridLayout *pLayout = new QGridLayout();
    pAttriWidget->setLayout(pLayout);

    m_pRed = AttriBase::varSelectedAttri();
    m_pRed->SetSelectVarType(tr("Switch"));
    m_pRed->IsHWSwitch(true);
    m_pRed->setObjectName("red");

    m_pYellow = AttriBase::varSelectedAttri();
    m_pYellow->SetSelectVarType(tr("Switch"));
    m_pYellow->IsHWSwitch(true);
    m_pYellow->setObjectName("yellow");

    m_pGreen = AttriBase::varSelectedAttri();
    m_pGreen->SetSelectVarType(tr("Switch"));
    m_pGreen->IsHWSwitch(true);
    m_pGreen->setObjectName("green");

    m_pBuzzer = AttriBase::varSelectedAttri();
    m_pBuzzer->SetSelectVarType(tr("Switch"));
    m_pBuzzer->IsHWSwitch(true);
    m_pBuzzer->setObjectName("buzzer");

    pLayout->addLayout(m_pGridLayout,0,0,1,5);
    pLayout->addWidget(new QLabel(tr("红灯")),1,0,Qt::AlignLeft);
    pLayout->addWidget(m_pRed,1,1,1,4);
    pLayout->addWidget(new QLabel(tr("黄灯")),2,0,Qt::AlignLeft);
    pLayout->addWidget(m_pYellow,2,1,1,4);
    pLayout->addWidget(new QLabel(tr("绿灯")),3,0,Qt::AlignLeft);
    pLayout->addWidget(m_pGreen,3,1,1,4);
    pLayout->addWidget(new QLabel(tr("蜂鸣器")),4,0,Qt::AlignLeft);
    pLayout->addWidget(m_pBuzzer,4,1,1,4);

    return pAttriWidget;
}

void AttriWarmLamp::SetItem(CoItemEx *item)
{
    GraphicAttrWidget::SetItem(item);
    TWarnLamp *wl = qobject_cast<TWarnLamp*>(item);

    QVector<QString> vec = wl->GetVarName();
    m_pRed->SetVarName(vec.at(0));
    m_pYellow->SetVarName(vec.at(1));
    m_pGreen->SetVarName(vec.at(2));
    m_pBuzzer->SetVarName(vec.at(3));

    connect(m_pRed,&VarSelectedAttri::SendTextFinfished,this,&AttriWarmLamp::VarChanged);
    connect(m_pYellow,&VarSelectedAttri::SendTextFinfished,this,&AttriWarmLamp::VarChanged);
    connect(m_pGreen,&VarSelectedAttri::SendTextFinfished,this,&AttriWarmLamp::VarChanged);
    connect(m_pBuzzer,&VarSelectedAttri::SendTextFinfished,this,&AttriWarmLamp::VarChanged);
    connect(this,&AttriWarmLamp::SendTAttribute,wl,&TWarnLamp::SetAttributeEvent);
}

void AttriWarmLamp::VarChanged(const QString &varinfo)
{
    QStringList list = varinfo.split(',');
    if(list.size() > 1)
    {
        VarSelectedAttri *pVSA = qobject_cast<VarSelectedAttri*>(sender());
        QString objectname = pVSA->objectName();
        uchar uc = 0;
        if(objectname == "red")
            uc = 1;
        else if(objectname == "yellow")
            uc = 2;
        else if(objectname == "green")
            uc = 3;
        else if(objectname == "buzzer")
            uc = 4;

        QString name = list.at(0);
        emit SendTAttribute(CoAttributeWidget::VarName,name,uc);
    }
}
