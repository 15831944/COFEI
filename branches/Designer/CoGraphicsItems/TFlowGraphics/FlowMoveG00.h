#ifndef FLOWMOVE_H
#define FLOWMOVE_H

#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/moveg00property.h"
class TFlowMoveG00 : public TFlowBase
{
    Q_OBJECT
public:
    TFlowMoveG00(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowMoveG00();

public slots:
    CoItem *Copy();
    void Copy(TFlowMoveG00 *flow);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWMOVE_H
