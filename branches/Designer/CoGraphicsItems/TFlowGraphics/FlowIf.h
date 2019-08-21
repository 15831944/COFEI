#ifndef FLOWIF_H
#define FLOWIF_H

#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/ifproperty.h"
class TFlowIf : public TFlowBase
{
    Q_OBJECT
public:
    TFlowIf(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowIf();

public slots:
    CoItem *Copy();
    void Copy(TFlowIf *flow);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWIF_H
