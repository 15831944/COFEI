#ifndef FLOWMOVEG101_H
#define FLOWMOVEG101_H


#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/moveg101property.h"
class TFlowMoveG101 : public TFlowBase
{
    Q_OBJECT
public:
    TFlowMoveG101(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowMoveG101();

public slots:
    CoItem *Copy();
    void Copy(TFlowMoveG101 *flow);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWMOVEG101_H
