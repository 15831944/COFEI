#include "FlowMoveG00.h"
#include "FlowPropery/PropertyItems/moveg00property.h"
TFlowMoveG00::TFlowMoveG00(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::FlowSport);
    PropertyBase = new MoveG00Property();
    connectWithText();
}

TFlowMoveG00::~TFlowMoveG00()
{}

CoItem *TFlowMoveG00::Copy()
{
    TFlowMoveG00 *flow = new TFlowMoveG00();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void TFlowMoveG00::Copy(TFlowMoveG00 *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void TFlowMoveG00::Draw(QPainter *painter)
{
    painter->drawRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);

    QFont font = painter->font();
    font.setPointSize(12);
    painter->setFont(font);

    painter->drawText(m_StartX,
                        m_StartY,
                        m_StopX - m_StartX,
                        m_StopY - m_StartY,
                        Qt::AlignCenter,
                      Code);
}
