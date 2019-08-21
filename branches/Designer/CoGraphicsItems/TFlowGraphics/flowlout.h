#ifndef FLOWLOUT_H
#define FLOWLOUT_H
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/loutproperty.h"

class FlowLout :public TFlowBase
{
    Q_OBJECT
public:
    FlowLout(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowLout();

public slots:
    CoItem *Copy();
    void Copy(FlowLout *FlowEndFrom);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWLOUT_H
