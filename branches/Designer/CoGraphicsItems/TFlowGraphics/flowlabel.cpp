#include "flowlabel.h"
#include "../FlowPropery/PropertyItems/labelproperty.h"
flowlabel::flowlabel(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor):
TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::IDs);
    PropertyBase = new LabelProperty();
     connectWithText();
}

flowlabel::~flowlabel()
{

}

CoItem *flowlabel::Copy()
{
    flowlabel *flow = new flowlabel();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void flowlabel::Copy(flowlabel *FlowEndFrom)
{
    if(FlowEndFrom == NULL)
    {
        return;
    }
      CoFlowChart::Copy(FlowEndFrom);
}

void flowlabel::Draw(QPainter *painter)
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
