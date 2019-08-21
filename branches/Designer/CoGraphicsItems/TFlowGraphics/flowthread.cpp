#include "flowthread.h"
#include "../FlowPropery/PropertyItems/threadproperty.h"

TFlowThread::TFlowThread(double StartX, double StartY, double StopX, double StopY,
                       const Qt::PenStyle &LineStyle, const int LineWidth,
                       const QColor &LineColor, const QColor &BackColor):
 TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
     SetType(CoItem::FlowThread);
     PropertyBase = new ThreadProperty();
      connectWithText();
}

TFlowThread::~TFlowThread()
{

}

CoItem *TFlowThread::Copy()
{
    TFlowThread *Flow = new TFlowThread();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void TFlowThread::Copy(TFlowThread *Flowthread)
{
    if(NULL == Flowthread)
    {
        return;
    }

    CoFlowChart::Copy(Flowthread);
}

void TFlowThread::Draw(QPainter *painter)
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
