#include "Flowflow.h"
#include <QDebug>
Flowflow::Flowflow(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor):
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::FlowCall);
    PropertyBase = new FlowProperty();
    connectWithText();
}

Flowflow::~Flowflow()
{

}

CoItem *Flowflow::Copy()
{
    Flowflow *flow = new Flowflow();

    flow->Copy(this);

    return flow;
}

void Flowflow::Copy(Flowflow *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void Flowflow::Draw(QPainter *painter)
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

