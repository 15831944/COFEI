#ifndef TFLOWDS_H
#define TFLOWDS_H


#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/dsproperty.h"
class TFlowDS:public TFlowBase
{
    Q_OBJECT
public:
    TFlowDS(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowDS();
public slots:
    CoItem *Copy();
    void Copy(TFlowDS *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWDS_H
