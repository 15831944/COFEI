#include "Ellipse.h"
#include <QDebug>
TEllipse::TEllipse(double StartX, double StartY, double StopX, double StopY,
                   const Qt::PenStyle &LineStyle, const int LineWidth,
                   const QColor &LineColor, const QColor &BackColor) :
    CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Ellipse);
}

TEllipse::~TEllipse()
{}

CoItem *TEllipse::Copy()
{
    TEllipse *Ellipse = new TEllipse();

    Ellipse->Copy(this);

    return Ellipse;
}

void TEllipse::Copy(TEllipse *EllipseFrom)
{
    if(NULL == EllipseFrom)
    {
        return;
    }

    CoGraphics::Copy(EllipseFrom);
}

void TEllipse::Draw(QPainter *painter)
{
    painter->drawEllipse(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);
}


