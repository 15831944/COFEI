#ifndef TFLOWDELAY_H
#define TFLOWDELAY_H


#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/delayproperty.h"
class TFlowDelay:public TFlowBase
{
    Q_OBJECT
public:
    TFlowDelay(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowDelay();
public slots:
    CoItem *Copy();
    void Copy(TFlowDelay *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWDELAY_H
