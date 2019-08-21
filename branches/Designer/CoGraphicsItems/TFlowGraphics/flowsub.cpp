#include "flowsub.h"
#include "../FlowPropery/PropertyItems/subproperty.h"

FlowSub::FlowSub(double StartX, double StartY, double StopX, double StopY, const Qt::PenStyle &LineStyle,
                 const int LineWidth, const QColor &LineColor, const QColor &BackColor):
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
     SetType(CoItem::Flowflow);
     PropertyBase = new SubProperty();
     connectWithText();
}

FlowSub::~FlowSub()
{

}

CoItem *FlowSub::Copy()
{
    FlowSub *Flow = new FlowSub();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void FlowSub::Copy(FlowSub *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void FlowSub::Draw(QPainter *painter)
{
    painter->drawRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);
    double peice = (qAbs(m_StopX - m_StartX))/8.0;
    painter->drawRect(m_StartX + peice, m_StartY, (m_StopX - m_StartX - 2*peice), m_StopY - m_StartY);
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
