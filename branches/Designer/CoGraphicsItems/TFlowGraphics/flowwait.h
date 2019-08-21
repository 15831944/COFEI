#ifndef FLOWWAIT_H
#define FLOWWAIT_H

#include "flowbase.h"

class FlowWAIT : public TFlowBase
{
    Q_OBJECT
public:
    FlowWAIT(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowWAIT();

public slots:
    CoItem *Copy();
    void Copy(FlowWAIT *FlowEndFrom);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWWAIT_H
