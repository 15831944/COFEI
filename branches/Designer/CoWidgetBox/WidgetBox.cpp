#include "WidgetBox.h"
#include "TitleBar.h"

CoWidgetBox::CoWidgetBox(QWidget *parent):
    QWidget(parent)
{
    Q_INIT_RESOURCE ( WidgetBox ) ;
    CoTitleBar *pTitleBar = new CoTitleBar(this);
    connect(pTitleBar,&CoTitleBar::SendHideEvent,this,&CoWidgetBox::HandleHideEvent);
    installEventFilter(pTitleBar);
    setWindowTitle("功能栏");
    m_pLayout = new QGridLayout();
    m_pLayout->addWidget(pTitleBar,0,0);
    m_pLayout->setSpacing(2);
    m_pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_pLayout);
}

void CoWidgetBox::AddWidget(QWidget *widget)
{
    if(widget != NULL)
    {
        m_pLayout->addWidget(widget,1,0);
    }
}

void CoWidgetBox::HandleHideEvent(bool ishide)
{
    emit TransferHideEvent(ishide);
}
