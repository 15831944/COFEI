#ifndef FLOWEND_H
#define FLOWEND_H
#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/endproperty.h"
class TFlowEnd:public TFlowBase
{
    Q_OBJECT
public:
    TFlowEnd(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowEnd();

public slots:
    CoItem *Copy();
    void Copy(TFlowEnd *FlowEndFrom);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWEND_H
