#include <QScrollArea>
#include <QGridLayout>
#include "../CoProxyItem/TouchPP.h"
#include "AttriTouchPP.h"

AttriTouchPP::AttriTouchPP(QWidget *parent)
    : GraphicAttrWidget(parent)
{
    QTabWidget *pTabWidget = new QTabWidget();
    pTabWidget->setFixedWidth(260);
    QWidget *pAttriWidget = new QWidget();
    pAttriWidget->setLayout(m_pGridLayout);
    pTabWidget->addTab(pAttriWidget,tr("属性"));

    QScrollArea *pMainScrollArea = new QScrollArea(this);
    pMainScrollArea->setWidget(pTabWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout();
    pMainLayout->addWidget(pMainScrollArea);
    this->setLayout(pMainLayout);
}

void AttriTouchPP::SetItem(CoItemEx *itm)
{
    GraphicAttrWidget::SetItem(itm);
    TouchPP *pp = qobject_cast<TouchPP*>(itm);
    connect(this,&AttriTouchPP::SendTAttribute,pp,&TouchPP::SetAttributeEvent);
}
