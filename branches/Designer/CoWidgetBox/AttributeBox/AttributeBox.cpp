#include "AttributeBox.h"
#include "TitleBar.h"
#include <QPushButton>
CoAttributeBox::CoAttributeBox(QWidget *parent):
    QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    CoTitleBar *pTitleBar = new CoTitleBar(this);
    connect(pTitleBar,&CoTitleBar::SendHideEvent,this,&CoAttributeBox::HandleHideEvent);
    installEventFilter(pTitleBar);
    m_widget = new QWidget(this);
    setWindowTitle("属性栏");
    m_pLayout = new QGridLayout();
    m_pLayout->addWidget(pTitleBar,0,0);
    m_pLayout->addWidget(m_widget,1,0);
    m_pLayout->setSpacing(2);
    m_pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_pLayout);
}

void CoAttributeBox::AddWidget(QWidget *widget)
{
    if(widget != NULL)
    {
        if(m_widget != NULL)
            m_widget->deleteLater();
        m_widget = widget;
        m_pLayout->addWidget(m_widget,1,0);
    }
}

void CoAttributeBox::HandleHideEvent(bool isHide)
{
    emit TransferHideEvent(isHide);
}
