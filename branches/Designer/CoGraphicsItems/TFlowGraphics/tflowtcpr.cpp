#include "tflowtcpr.h"
#include"../FlowPropery/PropertyItems/tcprproperty.h"


TFlowTCPR::TFlowTCPR(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor) :
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Tcp_R);
    PropertyBase = new TCPRProperty();
    connectWithText();
}

TFlowTCPR::~TFlowTCPR()
{}

CoItem *TFlowTCPR::Copy()
{
    TFlowTCPR *Flow = new TFlowTCPR();

    Flow->Copy(this);
    Flow->SetBackGroundColor(m_BackGroundColor);
    Flow->SetLineColor(m_LineColor);
    Flow->PropertyBase->copyProperty(PropertyBase);
    return Flow;
}

void TFlowTCPR::Copy(TFlowTCPR *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void TFlowTCPR::Draw(QPainter *painter)
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
