#include "Rect.h"

TRect::TRect(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Rect);
}

TRect::~TRect()
{}

CoItem *TRect::Copy()
{
    TRect *Rect = new TRect();

    Rect->Copy(this);

    return Rect;
}

void TRect::Copy(TRect *RectFrom)
{
    if(NULL == RectFrom)
    {
        return;
    }

    CoGraphics::Copy(RectFrom);
}

void TRect::Draw(QPainter *painter)
{
    painter->drawRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);
}
