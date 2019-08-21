#ifndef TFLOWSTART_H
#define TFLOWSTART_H
#include <CoGraphics/Graphics.h>
#include "flowbase.h"
class TFlowStart:public TFlowBase
{
    Q_OBJECT
public:
    TFlowStart(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowStart();

public slots:
    CoItem *Copy();
    void Copy(TFlowStart *FlowStart);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWSTART_H
