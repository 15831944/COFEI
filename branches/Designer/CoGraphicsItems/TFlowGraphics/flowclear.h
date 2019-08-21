#ifndef TFLOWCLEAR_H
#define TFLOWCLEAR_H


#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/clearproperty.h"
class TFlowClear:public TFlowBase
{
    Q_OBJECT
public:
    TFlowClear(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowClear();
public slots:
    CoItem *Copy();
    void Copy(TFlowClear *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWCLEAR_H
