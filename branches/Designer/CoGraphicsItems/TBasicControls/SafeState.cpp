#include <QDebug>
#include "SafeState.h"
#include "AttributeWidget.h"

TSafeState::TSafeState(double StartX, double StartY, double StopX, double StopY,
                       const Qt::PenStyle &LineStyle, const int LineWidth,
                       const QColor &LineColor, const QColor &BackColor,
                       const QString &Text) :
                  CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::SafeState);

    m_Text = Text;
    m_SafeState = 0;
    m_strFlowName = "";
    m_strSafeString = "";

}

CoItem *TSafeState::Copy()
{
    TSafeState *safeState = new TSafeState();
    safeState->Copy(this);
    return safeState;
}

void TSafeState::Copy(TSafeState *From)
{
    if(NULL == From)
        return;

    CoWidgets::Copy(From);
}

void TSafeState::Draw(QPainter *painter)
{
    painter->drawPixmap(m_StartX,m_StartY,m_StopX - m_StartX,m_StopY - m_StartY,
                        QPixmap(tr(":images/draw/safeState.png")));
    painter->setPen(QPen(Qt::white));
    painter->setFont(QFont("宋体", 13, QFont::Bold));
    painter->drawText(m_StartX,m_StartY,m_StopX - m_StartX,m_StopY - m_StartY,
                      Qt::AlignCenter,m_Text);
}

void TSafeState::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    Q_UNUSED(uc)
    switch (type) {
    case CoAttributeWidget::Text:
    {
        SetText(value);
        update();
    }
        break;
    case CoAttributeWidget::SafeState:
        SetSafeState(value.toShort());
        break;
    case CoAttributeWidget::FlowName:
        SetFlowName(value);
        break;
    case CoAttributeWidget::SafeString:
        SetSafeString(value);
        break;
    default:
        break;
    }
}

void TSafeState::SetSafeState(uchar state)
{
    m_SafeState = state;
}

uchar TSafeState::GetSafeState()
{
    return m_SafeState;
}

void TSafeState::SetFlowName(const QString &name)
{
    m_strFlowName = name;
}

QString TSafeState::GetFlowName()
{
    return m_strFlowName;
}

void TSafeState::SetSafeString(const QString &string)
{
    m_strSafeString = string;
}

QString TSafeState::GetSafeString()
{
    return m_strSafeString;
}

