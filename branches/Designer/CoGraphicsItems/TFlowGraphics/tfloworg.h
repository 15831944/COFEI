#ifndef TFLOWORG_H
#define TFLOWORG_H

#include "flowbase.h"

class TFlowORG : public TFlowBase
{
    Q_OBJECT
public:
    TFlowORG(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowORG();

public slots:
    CoItem *Copy();
    void Copy(TFlowORG *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWORG_H
