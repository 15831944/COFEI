#ifndef FLOWLABEL_H
#define FLOWLABEL_H
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/labelproperty.h"

class flowlabel :public TFlowBase
{
    Q_OBJECT
public:
    flowlabel(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~flowlabel();

public slots:
    CoItem *Copy();
    void Copy(flowlabel *FlowEndFrom);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWLABEL_H
