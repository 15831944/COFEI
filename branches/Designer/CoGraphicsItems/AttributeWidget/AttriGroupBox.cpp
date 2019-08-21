#include <QScrollArea>
#include "../TBasicControls/GroupBox.h"
#include "AttriGroupBox.h"

AttriGroupBox::AttriGroupBox(QWidget *parent)
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

QWidget *AttriGroupBox::CreateAttri()
{
    QWidget *pAttriWidget = new QWidget();
    QHBoxLayout *pLayout = new QHBoxLayout();
    pAttriWidget->setLayout(pLayout);

    m_pLineEditTxt = TBase::LineEdit(tr(""));
    pLayout->addWidget(new QLabel(tr("标题")));
    pLayout->addWidget(m_pLineEditTxt);

    return pAttriWidget;
}

void AttriGroupBox::SetItem(CoItemEx *item)
{
    GraphicAttrWidget::SetItem(item);
    TGroupBox *grp = qobject_cast<TGroupBox*>(item);
    m_pLineEditTxt->setText(grp->GetTitle());

    connect(m_pLineEditTxt,&QLineEdit::editingFinished,this,&AttriGroupBox::TextChanged);
    connect(this,&AttriGroupBox::SendTAttribute,grp,&TGroupBox::SetAttributeEvent);
}

void AttriGroupBox::TextChanged()
{
    emit SendTAttribute(CoAttributeWidget::Title,m_pLineEditTxt->text(),0);
}
