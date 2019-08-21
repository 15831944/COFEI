#ifndef FLOWIN_H
#define FLOWIN_H
#include "flowbase.h"

class FlowIn:public TFlowBase
{
    Q_OBJECT
public:
    FlowIn(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
             const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
             const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~FlowIn();

public slots:
    CoItem *Copy();
    void Copy(FlowIn *FlowEndFrom);
    virtual double GetCurrentStartX();
    virtual double GetCurrentStopX();

private:
    void Draw(QPainter *painter);
};

#endif // FLOWIN_H
