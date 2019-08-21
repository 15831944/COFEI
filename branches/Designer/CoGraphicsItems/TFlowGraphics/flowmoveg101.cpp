#include "flowmoveg101.h"
#include "../FlowPropery/PropertyItems/moveg101property.h"
TFlowMoveG101::TFlowMoveG101(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::FlowSport);
    PropertyBase = new MoveG101Property();
    connectWithText();
}

TFlowMoveG101::~TFlowMoveG101()
{}

CoItem *TFlowMoveG101::Copy()
{
    TFlowMoveG101 *flow = new TFlowMoveG101();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void TFlowMoveG101::Copy(TFlowMoveG101 *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void TFlowMoveG101::Draw(QPainter *painter)
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
