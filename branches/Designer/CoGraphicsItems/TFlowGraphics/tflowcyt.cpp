#include "tflowcyt.h"

TFlowCyt::TFlowCyt(double StartX, double StartY, double StopX, double StopY, const Qt::PenStyle &LineStyle,
                   const int LineWidth, const QColor &LineColor, const QColor &BackColor):
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
     SetType(CoItem::Cyt);
     PropertyBase = new Cytproperty();
     connectWithText();

}

TFlowCyt::~TFlowCyt()
{

}

CoItem *TFlowCyt::Copy()
{
    TFlowCyt *flow = new TFlowCyt();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void TFlowCyt::Copy(TFlowCyt *Flow)
{
    if(Flow == NULL)
    {
        return;
    }
    TFlowBase::Copy(Flow);
}

void TFlowCyt::Draw(QPainter *painter)
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
