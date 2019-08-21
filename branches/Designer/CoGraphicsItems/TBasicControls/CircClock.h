#ifndef TCIRCCLOCK_H
#define TCIRCCLOCK_H

#include <CoGraphics/Graphics.h>

class TCircClock : public CoGraphics
{
    Q_OBJECT
public:
    TCircClock(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);

    virtual ~TCircClock();

public slots:
    CoItem *Copy();
    void Copy(TCircClock *ClockFrom);

    void SetHourHandColor(const QColor &Color);
    QColor GetHourHandColor();
    void SetMinuteHandColor(const QColor &Color);
    QColor GetMinuteHandColor();
    void SetSecondHandColor(const QColor &Color);
    QColor GetSecondHandColor();

private:
    QColor m_HourHandColor;
    QColor m_MinuteHandColor;
    QColor m_SecondHandColor;
    void Draw(QPainter *painter);
};

#endif // TCIRCCLOCK_H
