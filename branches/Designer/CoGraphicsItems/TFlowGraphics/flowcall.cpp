#include "flowcall.h"
#include "../FlowPropery/PropertyItems/callproperty.h"
TFlowCall::TFlowCall(double StartX, double StartY, double StopX, double StopY,
                       const Qt::PenStyle &LineStyle, const int LineWidth,
                       const QColor &LineColor, const QColor &BackColor):
 TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
     SetType(CoItem::FlowCall);
     PropertyBase = new CallProperty();
      connectWithText();
}

TFlowCall::~TFlowCall()
{

}

CoItem *TFlowCall::Copy()
{
    TFlowCall *Flow = new TFlowCall();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void TFlowCall::Copy(TFlowCall *Flow)
{
    if(NULL == Flow)
    {
        return;
    }

    CoFlowChart::Copy(Flow);
}

void TFlowCall::Draw(QPainter *painter)
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
