#include <QDebug>
#include <QScrollArea>
#include "AttriSafeState.h"
#include "CoGraphics/Scene/Scene.h"
#include "../TBasicControls/SafeState.h"
#include "NewXML/DialogNewSafeXml.h"

AttriSafeState::AttriSafeState(QWidget *parent)
    : GraphicAttrWidget(parent)
{
    m_strSafeString = "";

    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pAttriWidget = CreateAttri();
    pTabWidget->addTab(pAttriWidget,tr("属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);

    m_pSubFlowLbl->hide();
    m_pSubFlowCmb->hide();
    connect(m_pStateCmb,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&AttriSafeState::StateChanged);
}

QWidget* AttriSafeState::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();

    QGridLayout *pLayout = new QGridLayout();
    pAttriWidget->setLayout(pLayout);

    m_pStateCmb = TBase::ComboBox(tr(""));

    QStringList list;
    list.clear();
    list << tr("总状态") << tr("子状态");
    m_pStateCmb->addItems(list);

    m_pSubFlowLbl = new QLabel(tr("子流程"));
    m_pSubFlowCmb = TBase::ComboBox(tr(""));
    m_pSafeDetailBtn = TBase::Button(tr("设置安全信息"),tr(""));
    connect(m_pSafeDetailBtn,&QPushButton::clicked,this,&AttriSafeState::ShowSafeDetail);
    m_pSafeDetailBtn->setFixedHeight(28);

    m_pCmbSelectPower->setEnabled(false);
    pLayout->addLayout(m_pGridLayout,0,0,1,4);
    pLayout->addWidget(new QLabel(tr("状态")),1,0,Qt::AlignLeft);
    pLayout->addWidget(m_pStateCmb,1,1,1,3);
    pLayout->addWidget(m_pSubFlowLbl,2,0,Qt::AlignLeft);
    pLayout->addWidget(m_pSubFlowCmb,2,1,1,3);
    pLayout->addWidget(m_pSafeDetailBtn,3,0,1,4);

    return pAttriWidget;
}

void AttriSafeState::SetItem(CoItemEx *item)
{
    GraphicAttrWidget::SetItem(item);
    TSafeState *safe = qobject_cast<TSafeState*>(item);

    uchar state = safe->GetSafeState();
    m_pStateCmb->setCurrentIndex(state);

    CoScene *pScene = static_cast<CoScene*>(item->GetCurScene());
    QStringList list = AttributeInstance::GetInstance()->flowMap[pScene->GetProjectName()];
    list.insert(0,QString(""));
    m_pSubFlowCmb->addItems(list);

    QString strFlowName = "";
    if(state > 0)
    {
        strFlowName = safe->GetFlowName();
        m_pSubFlowCmb->setCurrentText(strFlowName);
    }

    m_strSafeString = safe->GetSafeString();
    if(strFlowName.isEmpty() && state > 0)
        m_strSafeString = "";

    connect(m_pStateCmb,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,&AttriSafeState::StateChangedExt);
    connect(m_pSubFlowCmb,static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this,&AttriSafeState::FlowNameChanged);

    connect(this,&AttriSafeState::SendTAttribute,safe,&TSafeState::SetAttributeEvent);
}

void AttriSafeState::StateChanged(int index)
{
    if(0 == index)
    {
        m_pSubFlowLbl->hide();
        m_pSubFlowCmb->hide();
        m_pSubFlowCmb->setCurrentIndex(0);
    }
    else
    {
        m_pSubFlowLbl->show();
        m_pSubFlowCmb->show();
    }
}

void AttriSafeState::StateChangedExt(int index)
{
    QString text = "";
    if(0 == index)
        text = tr("总状态");
    else
        text = tr("子状态");

    m_strSafeString = "";
    emit SendTAttribute(CoAttributeWidget::SafeString,"",0);
    emit SendTAttribute(CoAttributeWidget::SafeState,QString::number(index),0);
    emit SendTAttribute(CoAttributeWidget::Text,text,0);
}

void AttriSafeState::FlowNameChanged(const QString &name)
{
    m_strSafeString = "";
    emit SendTAttribute(CoAttributeWidget::SafeString,"",0);
    emit SendTAttribute(CoAttributeWidget::FlowName,name,0);
}

void AttriSafeState::ShowSafeDetail()
{
    DialogNewSafeXml *sx = new DialogNewSafeXml(this);
    sx->SetText(m_strSafeString);
    connect(sx,&DialogNewSafeXml::SafeStringChanged,[=] (const QString& text)
    {
        m_strSafeString = text;
        emit SendTAttribute(CoAttributeWidget::SafeString,text,0);
    });
    sx->setModal(true);
    sx->show();
}
