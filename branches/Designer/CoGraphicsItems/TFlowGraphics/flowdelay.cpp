#include "flowdelay.h"
#include "../FlowPropery/PropertyItems/delayproperty.h"
TFlowDelay::TFlowDelay(double StartX, double StartY, double StopX, double StopY,
                       const Qt::PenStyle &LineStyle, const int LineWidth,
                       const QColor &LineColor, const QColor &BackColor):
 TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
     SetType(CoItem::Dely);
     PropertyBase = new DelayProperty();
      connectWithText();
}

TFlowDelay::~TFlowDelay()
{

}

CoItem *TFlowDelay::Copy()
{
    TFlowDelay *Flow = new TFlowDelay();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void TFlowDelay::Copy(TFlowDelay *Flow)
{
    if(NULL == Flow)
    {
        return;
    }

    CoFlowChart::Copy(Flow);
}

void TFlowDelay::Draw(QPainter *painter)
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
