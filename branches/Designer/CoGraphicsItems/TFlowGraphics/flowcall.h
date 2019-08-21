#ifndef TFLOWCALL_H
#define TFLOWCALL_H


#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/callproperty.h"
class TFlowCall:public TFlowBase
{
    Q_OBJECT
public:
    TFlowCall(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowCall();
public slots:
    CoItem *Copy();
    void Copy(TFlowCall *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWCALL_H
