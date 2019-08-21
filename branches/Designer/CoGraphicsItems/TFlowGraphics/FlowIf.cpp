#include "FlowIf.h"
#include "../FlowPropery/PropertyItems/ifproperty.h"
TFlowIf::TFlowIf(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::IF);
    PropertyBase = new IFProperty();
     connectWithText();
}

TFlowIf::~TFlowIf()
{}

CoItem *TFlowIf::Copy()
{
    TFlowIf *flow = new TFlowIf();

    flow->Copy(this);
    flow->SetBackGroundColor(m_BackGroundColor);
    flow->SetLineColor(m_LineColor);
    flow->PropertyBase->copyProperty(PropertyBase);
    return flow;
}

void TFlowIf::Copy(TFlowIf *flow)
{
    if(NULL == flow)
    {
        return;
    }

    CoFlowChart::Copy(flow);
}

void TFlowIf::Draw(QPainter *painter)
{
    double CenterX = m_StartX + (m_StopX - m_StartX) / 2;
    double CenterY = m_StartY + (m_StopY - m_StartY) / 2;  // 绘图区域的中心Y
    QPolygonF PolygonF;
    PolygonF.push_back(QPointF(m_StartX,CenterY));
    PolygonF.push_back(QPointF(CenterX,m_StartY));
    PolygonF.push_back(QPointF(m_StopX,CenterY));
    PolygonF.push_back(QPointF(CenterX,m_StopY));
    PolygonF.push_back(QPointF(m_StartX,CenterY));
    painter->drawPolyline(PolygonF);

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
