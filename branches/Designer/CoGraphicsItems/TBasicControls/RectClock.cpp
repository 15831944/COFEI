#include "RectClock.h"

#include <LibBase.h>
#include <QtMath>
#include <QDebug>
#include <QDateTime>
#include <CoGraphics/Base/Base.h>
TRectClock::TRectClock(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::RectClock);
}

TRectClock::~TRectClock()
{}

CoItem *TRectClock::Copy()
{
    TRectClock *clock = new TRectClock();

    clock->Copy(this);

    return clock;
}

void TRectClock::Copy(TRectClock *ClockFrom)
{
    if(NULL == ClockFrom)
    {
        return;
    }

    CoGraphics::Copy(ClockFrom);
}
void TRectClock::Draw(QPainter *painter)
{
    double Width = m_StopX - m_StartX;
    double Height = m_StopY - m_StartY;
    double CenterX = m_StartX + Width / 2;

    double Roundness = MIN(Width, Height) * 0.1; //圆角

    // 边框的外
    double Len = Height/2;
    QLinearGradient lg1(CenterX, m_StartY, CenterX, m_StopY);

    lg1.setColorAt(0, m_LineColor);
    lg1.setColorAt(1, TBase::GetNewColor(m_LineColor, -80));
    painter->setBrush(lg1);
    painter->drawRoundedRect(m_StartX, m_StartY, Width, Height, Roundness, Roundness);

    // 边框的内
    Len *= 0.1;
    QLinearGradient lg2(CenterX, m_StartY+Len, CenterX, m_StopY-Len);
    lg2.setColorAt(0, TBase::GetNewColor(m_LineColor, -100));
    lg2.setColorAt(1, m_LineColor);
    painter->setBrush(lg2);
    painter->drawRoundedRect(m_StartX+Len, m_StartY+Len, Width-2*Len, Height-2*Len, Roundness, Roundness);

    Len += Len *0.96;
    QLinearGradient lg3(CenterX, m_StartY+Len, CenterX, m_StopY-Len);
    lg3.setColorAt(0, TBase::GetNewColor(m_BackGroundColor, -10));
    lg3.setColorAt(1, m_BackGroundColor);
    painter->setBrush(lg3);
    painter->drawRoundedRect(m_StartX+Len, m_StartY+Len, Width-2*Len, Height-2*Len, Roundness, Roundness);

    QDateTime time = QDateTime::currentDateTime();
    QString timeDate= time.toString("yyyy-MM-dd");
    QString strShort = time.toString("hh:mm:ss");
    painter->setFont(m_font);
    painter->setPen(QPen(QBrush(Qt::blue), 3.0));
    painter->drawText(m_StartX+Len, m_StartY+Len, (Width-2*Len), (Height-2*Len)/2, Qt::AlignBottom |Qt::AlignHCenter, strShort);
    painter->drawText(m_StartX+Len, m_StartY + Height/2, (Width-2*Len), (Height-2*Len)/2, Qt::AlignCenter, timeDate);
}
