#ifndef TELLIPSE_H
#define TELLIPSE_H

#include <CoGraphics/Graphics.h>

/////////////////////////////////////////////////  圆形  ////////////////////////////////////////////////////
class TEllipse : public CoGraphics
{
    Q_OBJECT
public:
    TEllipse(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TEllipse();

public slots:
    CoItem *Copy();
    void Copy(TEllipse *EllipseFrom);

private:
    void Draw(QPainter *painter);
};

#endif // TELLIPSE_H
