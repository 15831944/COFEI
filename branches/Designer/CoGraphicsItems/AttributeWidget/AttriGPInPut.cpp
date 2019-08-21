#include <QScrollArea>
#include <QGridLayout>
#include "SelectVar/DialogSelectVar.h"
#include "../TBasicControls/GPInPut.h"
#include "AttriGPInPut.h"

AttriGPInPut::AttriGPInPut(QWidget *parent):
    GraphicAttrWidget(parent)
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

QWidget *AttriGPInPut::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();

    QGridLayout *pLayout = new QGridLayout();
    pAttriWidget->setLayout(pLayout);

    m_pCmbIOName = TBase::ComboBox(tr(""));
    QStringList list;
    list.append(" ");
    for(int i=0; i<10;i++)
        list.append(tr("ME%1").arg(i));
    m_pCmbIOName->addItems(list);

    m_pCmbFrequency = TBase::ComboBox(tr(""));
    list.clear();
    list << tr("200") << tr("500") << tr("1000");
    m_pCmbFrequency->addItems(list);

    pLayout->addLayout(m_pGridLayout,0,0,1,5);
    pLayout->addWidget(new QLabel(tr("GPIO读")),1,0,Qt::AlignLeft);
    pLayout->addWidget(m_pCmbIOName,1,1,1,3);
    pLayout->addWidget(new QLabel(tr("刷新频率")),2,0,Qt::AlignLeft);
    pLayout->addWidget(m_pCmbFrequency,2,1,1,3);
    pLayout->addWidget(new QLabel(tr("ms")),2,4);
    return pAttriWidget;
}

void AttriGPInPut::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TGPInPut *in = qobject_cast<TGPInPut*>(itm);

    m_pCmbIOName->setCurrentText(in->GetIOName());
    m_pCmbFrequency->setCurrentText(QString::number(in->GetAutoTime()));
    connect(m_pCmbIOName,&QComboBox::currentTextChanged,this,&AttriGPInPut::IONameChanged);
    connect(m_pCmbFrequency,&QComboBox::currentTextChanged,this,&AttriGPInPut::RefreshFrequencyChanged);
    connect(this,&AttriGPInPut::SendTAttribute,in,&TGPInPut::SetAttributeEvent);
}

void AttriGPInPut::IONameChanged(const QString text)
{
    emit SendTAttribute(CoAttributeWidget::VarName,text,0);
}

void AttriGPInPut::RefreshFrequencyChanged(const QString &text)
{
    emit SendTAttribute(CoAttributeWidget::RefreshFreq,text,0);
}


