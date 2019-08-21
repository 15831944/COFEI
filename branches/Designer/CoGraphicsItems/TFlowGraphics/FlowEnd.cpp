#include "FlowEnd.h"
#include <QDebug>
#include "../FlowPropery/PropertyItems/endproperty.h"
TFlowEnd::TFlowEnd(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
    TFlowBase(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::FlowEnd);
    PropertyBase = new EndProperty();
     connectWithText();
}

TFlowEnd::~TFlowEnd()
{}

CoItem *TFlowEnd::Copy()
{
    TFlowEnd *Flow = new TFlowEnd();

    Flow->Copy(this);
    Flow->PropertyBase->copyProperty(PropertyBase);
    Flow->SetLineColor(m_LineColor);
    Flow->SetBackGroundColor(m_BackGroundColor);
    return Flow;
}

void TFlowEnd::Copy(TFlowEnd *FlowEnd)
{
    if(NULL == FlowEnd)
    {
        return;
    }

    CoFlowChart::Copy(FlowEnd);
}

void TFlowEnd::Draw(QPainter *painter)
{
    painter->drawEllipse(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);

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
