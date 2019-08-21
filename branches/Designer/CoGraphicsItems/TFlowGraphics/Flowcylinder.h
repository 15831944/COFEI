#ifndef FLOWCYLINDER_H
#define FLOWCYLINDER_H
#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/cylinderproperty.h"

class TFlowCyLinder : public TFlowBase
{
    Q_OBJECT
public:
    TFlowCyLinder(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowCyLinder();

public slots:
    CoItem *Copy();
    void Copy(TFlowCyLinder *flow);

private:
    void Draw(QPainter *painter);

};

#endif // FLOWCYLINDER_H
