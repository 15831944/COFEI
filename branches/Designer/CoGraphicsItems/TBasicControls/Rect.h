#ifndef TRECT_H
#define TRECT_H

#include <CoGraphics/Graphics.h>

/////////////////////////////////////////////////  矩形  /////////////////////////////////////////////////////
class TRect : public CoGraphics
{
    Q_OBJECT
public:
    TRect(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TRect();

public slots:
    CoItem *Copy();
    void Copy(TRect *RectFrom);

private:
    void Draw(QPainter *painter);
};

#endif // TRECT_H
