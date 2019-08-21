#ifndef TFLOWJS_H
#define TFLOWJS_H
#include "flowbase.h"

class TFlowJS : public TFlowBase
{
    Q_OBJECT
public:
    TFlowJS(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowJS();

public slots:
    CoItem *Copy();
    void Copy(TFlowJS *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWJS_H
