#ifndef FLOWSUB_H
#define FLOWSUB_H
#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/subproperty.h"

class FlowSub :public TFlowBase
{
    Q_OBJECT
public:
    FlowSub(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowSub();
public slots:
    CoItem *Copy();
    void Copy(FlowSub *flow);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWSUB_H
