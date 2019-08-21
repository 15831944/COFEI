#include "flowin.h"
#include "../FlowPropery/PropertyItems/inproperty.h"
FlowIn::FlowIn(double StartX, double StartY, double StopX, double StopY, const Qt::PenStyle &LineStyle,
               const int LineWidth, const QColor &LineColor, const QColor &BackColor):
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::In);
    PropertyBase = new InProperty();
    connectWithText();
}

FlowIn::~FlowIn()
{

}

CoItem *FlowIn::Copy()
{
    FlowIn *flow = new FlowIn();

    flow->Copy(this);
    flow->PropertyBase->copyProperty(PropertyBase);
    flow->SetLineColor(m_LineColor);
    flow->SetBackGroundColor(m_BackGroundColor);
    return flow;
}

void FlowIn::Copy(FlowIn *FlowEndFrom)
{
    if(NULL == FlowEndFrom)
    {
        return;
    }

    CoFlowChart::Copy(FlowEndFrom);
}
double FlowIn::GetCurrentStartX()
{
    double piece = (qAbs(m_StopX - m_StartX))/6.0;
    double d = mapToScene(m_StartX, m_StartY).x();
    return d + piece;
}
double FlowIn::GetCurrentStopX()
{
    double piece = (qAbs(m_StopX - m_StartX))/6.0;
    double d = mapToScene(m_StopX, m_StopY).x();
    return d - piece;
}
void FlowIn::Draw(QPainter *painter)
{
    //painter->drawRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);
    double piece = (qAbs(m_StopX - m_StartX))/3.0;
    QPolygonF PolygonF;
    PolygonF.push_back(QPointF(m_StartX + piece,m_StartY));
    PolygonF.push_back(QPointF(m_StopX,m_StartY));
    PolygonF.push_back(QPointF(m_StopX-piece,m_StopY));
    PolygonF.push_back(QPointF(m_StartX,m_StopY));
    PolygonF.push_back(QPointF(m_StartX + piece,m_StartY));
    painter->drawPolyline(PolygonF);
    QFont font = painter->font();
    font.setPointSize(13);
    painter->setFont(font);

    painter->drawText(m_StartX,
                        m_StartY,
                        m_StopX - m_StartX,
                        m_StopY - m_StartY,
                        Qt::AlignCenter,
                      Code);
}
