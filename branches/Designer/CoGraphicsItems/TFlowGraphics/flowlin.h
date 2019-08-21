#ifndef FLOWLIN_H
#define FLOWLIN_H
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/linproperty.h"

class FlowLin:public TFlowBase
{  
    Q_OBJECT
public:
    FlowLin(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowLin();

public slots:
    CoItem *Copy();
    void Copy(FlowLin *FlowEndFrom);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWLIN_H
