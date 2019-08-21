#ifndef TFLOWCOUNT_H
#define TFLOWCOUNT_H

#include <QObject>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/countproperty.h"
class TFlowCount : public TFlowBase
{

public:
    TFlowCount(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowCount();
public slots:
    CoItem *Copy();
    void Copy(TFlowCount *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWCOUNT_H
