#include "tflowjs.h"
#include"../FlowPropery/PropertyItems/jsproperty.h"

TFlowJS::TFlowJS(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor) :
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Js);
    PropertyBase = new JSProperty();
    connectWithText();
}

TFlowJS::~TFlowJS()
{}

CoItem *TFlowJS::Copy()
{
    TFlowJS *Flow = new TFlowJS();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void TFlowJS::Copy(TFlowJS *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void TFlowJS::Draw(QPainter *painter)
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
