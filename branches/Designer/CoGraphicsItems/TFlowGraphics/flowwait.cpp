#include "flowwait.h"
#include"../FlowPropery/PropertyItems/waitproperty.h"

FlowWAIT::FlowWAIT(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor)
    :TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Wait);
    PropertyBase = new WAITProperty();
    connectWithText();
}

FlowWAIT::~FlowWAIT()
{

}

CoItem *FlowWAIT::Copy()
{
    FlowWAIT *flow = new FlowWAIT();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void FlowWAIT::Copy(FlowWAIT *FlowEndFrom)
{
  if(FlowEndFrom == NULL)
  {
      return;
  }
   CoFlowChart::Copy(FlowEndFrom);
}

void FlowWAIT::Draw(QPainter *painter)
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
