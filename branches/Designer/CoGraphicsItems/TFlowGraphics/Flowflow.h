#ifndef FLOWFLOW_H
#define FLOWFLOW_H

#include <CoGraphics/Graphics.h>
#include <TFlowGraphics/tflowbase.h>
#include <PropertyItems/flowproperty.h>
class Flowflow :public TFlowBase
{
    Q_OBJECT
public:
    explicit Flowflow(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                      const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                      const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);
    virtual ~Flowflow();

public slots:
    CoItem *Copy();
    void Copy(Flowflow *flow);

private:
    void Draw(QPainter *painter);

};

#endif // FLOWFLOW_H
