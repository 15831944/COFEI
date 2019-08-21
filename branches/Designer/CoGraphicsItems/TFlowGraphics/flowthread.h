#ifndef FLOWTHREAD_H
#define FLOWTHREAD_H
#include <CoGraphics/Graphics.h>
#include "flowbase.h"
#include "../FlowPropery/PropertyItems/threadproperty.h"
class TFlowThread:public TFlowBase
{
    Q_OBJECT
public:
    TFlowThread(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
               const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
               const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowThread();
public slots:
    CoItem *Copy();
    void Copy(TFlowThread *Flowthread);

private:
    void Draw(QPainter *painter);
};

#endif // FLOWTHREAD_H
