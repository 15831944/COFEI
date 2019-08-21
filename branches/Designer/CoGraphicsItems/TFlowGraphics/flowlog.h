#ifndef FLOWLOG_H
#define FLOWLOG_H

#include "flowbase.h"
class FlowLOG : public TFlowBase
{
    Q_OBJECT
public:
    FlowLOG(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowLOG();

public slots:
    CoItem *Copy();
    void Copy(FlowLOG *FlowStart);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWLOG_H
