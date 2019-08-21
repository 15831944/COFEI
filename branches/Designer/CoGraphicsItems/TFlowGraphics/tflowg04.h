#ifndef TFLOWG04_H
#define TFLOWG04_H
#include "flowbase.h"

class TFlowG04 : public TFlowBase
{
    Q_OBJECT
public:
    TFlowG04(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowG04();

public slots:
    CoItem *Copy();
    void Copy(TFlowG04 *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWG04_H
