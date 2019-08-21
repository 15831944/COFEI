#ifndef FLOWJOG_H
#define FLOWJOG_H

#include <QObject>
#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/jogproperty.h"
class FlowJog : public TFlowBase
{
    Q_OBJECT
public:
    FlowJog(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowJog();

public slots:
    CoItem *Copy();
    void Copy(FlowJog *FlowEndFrom);
private:
    void Draw(QPainter *painter);
};

#endif // FLOWJOG_H
