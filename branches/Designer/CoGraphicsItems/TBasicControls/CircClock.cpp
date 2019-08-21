#include "CircClock.h"

#include <LibBase.h>
#include <CoGraphics/Base/Base.h>
#include <QtMath>
#include <QDebug>
#include <QTime>
TCircClock::TCircClock(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_HourHandColor = QColor(157, 10, 187,100);
    m_MinuteHandColor = QColor(10, 187, 157, 191);
    m_SecondHandColor = QColor(127, 127, 0, 127);

    SetType(CoItem::CricClock);
}

TCircClock::~TCircClock()
{}

CoItem *TCircClock::Copy()
{
    TCircClock *clock = new TCircClock();

    clock->Copy(this);

    return clock;
}

void TCircClock::Copy(TCircClock *ClockFrom)
{
    if(NULL == ClockFrom)
    {
        return;
    }

    CoGraphics::Copy(ClockFrom);
    this->SetHourHandColor(ClockFrom->GetHourHandColor());
    this->SetMinuteHandColor(ClockFrom->GetMinuteHandColor());
    this->SetSecondHandColor(ClockFrom->GetSecondHandColor());
}

void TCircClock::SetHourHandColor(const QColor &Color)
{
    m_HourHandColor = Color;
}

QColor TCircClock::GetHourHandColor()
{
    return m_HourHandColor;
}
void TCircClock::SetMinuteHandColor(const QColor &Color)
{
    m_MinuteHandColor = Color;
}

QColor TCircClock::GetMinuteHandColor()
{
    return m_MinuteHandColor;
}
void TCircClock::SetSecondHandColor(const QColor &Color)
{
    m_SecondHandColor = Color;
}

QColor TCircClock::GetSecondHandColor()
{
    return m_SecondHandColor;
}
void TCircClock::Draw(QPainter *painter)
{
    double Width = m_StopX - m_StartX;
    double Height = m_StopY - m_StartY;
    double CenterX = m_StartX + Width / 2;
    double CenterY = m_StartY + Height / 2;
    painter->translate(CenterX, CenterY);
    int Radius = MIN(Width / 2, Height / 2);// 绘制圆形的半径
    QLinearGradient lg1(0, -Radius, 0, Radius);
    lg1.setColorAt(0, m_LineColor);
    lg1.setColorAt(1, TBase::GetNewColor(m_LineColor, -90));
    painter->setBrush(lg1);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);
    Radius *= 0.9;
    QLinearGradient lg2(0, -Radius, 0, Radius);
    lg2.setColorAt(0, TBase::GetNewColor(m_LineColor, -100));
    lg2.setColorAt(1, m_LineColor);
    painter->setBrush(lg2);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 灯的圆
    Radius *= 0.96;
    QRadialGradient rg(0, 0, Radius); // 辐射渐变
    rg.setColorAt(0, QColor(0, 205, 235));
    rg.setColorAt(0.6, QColor(0, 180, 210));
    rg.setColorAt(1, QColor(0, 146, 166));
    painter->setBrush(rg);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    static const QPoint hourHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -(4*Radius/10))
    };
    static const QPoint minuteHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -(6*Radius/10))
    };
    static const QPoint secondHand[3] = {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -(9*Radius/10 )),
    };

    QTime time = QTime::currentTime();
//    QFont font;
//    int fontsize = 0.1 * Radius + 1;
//    font.setPointSize(fontsize);
//    painter->setFont(font);
//    painter->setPen(Qt::red);
//    QString timeStr= time.toString();
//    painter->drawText(-Radius/2,Radius/3,Radius,Radius/3,Qt::AlignHCenter | Qt::AlignTop, timeStr);

    painter->setPen(Qt::NoPen);
    painter->setBrush(m_HourHandColor);
    painter->save();
    painter->rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter->drawConvexPolygon(hourHand, 3);
    painter->restore();

    painter->setPen(m_HourHandColor);
    for (int i = 0; i < 12; ++i)
    {
        painter->drawLine(Radius-9, 0, Radius, 0);
        if (i == 0)
            painter->drawText(-10,-(Radius-10),20,20,Qt::AlignHCenter | Qt::AlignTop,QString::number(12));
        else
            painter->drawText(-10,-(Radius-10),20,20,Qt::AlignHCenter | Qt::AlignTop,QString::number(i));
        painter->rotate(30.0);
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_MinuteHandColor);
    painter->save();
    painter->rotate(6.0 * (time.minute() + time.second() / 60.0));  //设旋转(角度 = 6° * (分钟 + 秒 / 60))
    painter->drawConvexPolygon(minuteHand, 3);
    painter->restore();

    painter->setPen(m_MinuteHandColor);
//    for (int j = 0; j < 60; ++j)
//    {
//        if ((j % 5) != 0)
//            painter->drawLine(Radius-4, 0, Radius, 0);
//        painter->rotate(6.0);
//    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_SecondHandColor);
    painter->save();
    painter->rotate(6.0 * time.second());
    painter->drawConvexPolygon(secondHand, 3);
    painter->restore();

    painter->setBrush(Qt::lightGray);             //画上中心原点
    painter->drawEllipse(QPoint(0,0),2,2);

    // 高光
    QPainterPath path;
    Radius *= 0.97;
    path.addEllipse(-Radius, -Radius*1.02, Radius << 1, Radius << 1);
    QPainterPath bigEllipse;

    // 减去高光下部的大圆
    Radius *= 2;
    bigEllipse.addEllipse(-Radius, -Radius * 0.49, Radius << 1, Radius << 1);
    path -= bigEllipse;

    QLinearGradient lg3(0, -Radius / 2, 0, 0);
    lg3.setColorAt(0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1, QColor(255, 255, 255, 30));
    painter->setBrush(lg3);
    painter->drawPath(path);
}
