#include "flowout.h"

#include "../FlowPropery/PropertyItems/outproperty.h"

FlowOut::FlowOut(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor)
    :TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Out);
    PropertyBase = new OutProperty();
    connectWithText();
}

FlowOut::~FlowOut()
{

}

CoItem *FlowOut::Copy()
{
    FlowOut *flow = new FlowOut();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void FlowOut::Copy(FlowOut *FlowEndFrom)
{
  if(FlowEndFrom == NULL)
  {
      return;
  }
   CoFlowChart::Copy(FlowEndFrom);
}

void FlowOut::Draw(QPainter *painter)
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
