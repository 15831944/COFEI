#ifndef TFLOWUART_H
#define TFLOWUART_H

#include <QObject>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/uartproperty.h"
class TFlowUart : public TFlowBase
{
 Q_OBJECT
public:
    TFlowUart(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowUart();
public slots:
    CoItem *Copy();
    void Copy(TFlowUart *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWUART_H
