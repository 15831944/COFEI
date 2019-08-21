#ifndef TRECTCLOCK_H
#define TRECTCLOCK_H

#include <CoGraphics/Graphics.h>

class TRectClock : public CoGraphics
{
    Q_OBJECT
public:
    TRectClock(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
           const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
           const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);

    virtual ~TRectClock();

public slots:
    CoItem *Copy();
    void Copy(TRectClock *ClockFrom);

private:
    void Draw(QPainter *painter);
};

#endif // TRECTCLOCK_H
