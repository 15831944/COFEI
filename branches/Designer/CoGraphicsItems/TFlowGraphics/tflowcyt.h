#ifndef TFLOWCYT_H
#define TFLOWCYT_H

#include <QObject>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/cytproperty.h"

class TFlowCyt : public TFlowBase
{
 Q_OBJECT
public:
    TFlowCyt(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowCyt();
public slots:
    CoItem *Copy();
    void Copy(TFlowCyt *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWCYT_H
