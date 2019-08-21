#include "tflowstart.h"
#include "../FlowPropery/PropertyItems/startproperty.h"

TFlowStart::TFlowStart(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::FlowStart);
    PropertyBase = new StartProperty();
     connectWithText();
}

TFlowStart::~TFlowStart()
{}

CoItem *TFlowStart::Copy()
{
    TFlowStart *Flow = new TFlowStart();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void TFlowStart::Copy(TFlowStart *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void TFlowStart::Draw(QPainter *painter)
{
    painter->drawEllipse(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);
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
