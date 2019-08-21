#ifndef FLOWOUT_H
#define FLOWOUT_H
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/outproperty.h"

class FlowOut:public TFlowBase
{
    Q_OBJECT
public:
    FlowOut(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowOut();

public slots:
    CoItem *Copy();
    void Copy(FlowOut *FlowEndFrom);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWOUT_H
