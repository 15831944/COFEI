#include "TitleBar.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
CoTitleBar::CoTitleBar(QWidget *parent) :
    QWidget(parent)
{
    ishide = false;
    m_pHideBtn = new QPushButton(this);
    m_pHideBtn->setIcon(QIcon(":/images/unFix.png"));
    m_pHideBtn->setIconSize(QSize(24,24));
    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->setFont(QFont("黑体",11));
    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pHideBtn->setFixedSize(27, 24);
    m_pHideBtn->setToolTip("隐藏");
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addWidget(m_pHideBtn);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 4);
    setLayout(pLayout);

    connect(m_pHideBtn, &QPushButton::clicked, this,&CoTitleBar::onClicked);
    this->setFixedHeight(30);
}

void CoTitleBar::onClicked()
{
    emit SendHideEvent(true);
}
bool CoTitleBar::eventFilter(QObject *obj, QEvent *event)
{
    int itype = event->type();
    switch (itype)
    {
        case QEvent::WindowTitleChange:
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget)
            {
                m_pTitleLabel->setText(pWidget->windowTitle());
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
