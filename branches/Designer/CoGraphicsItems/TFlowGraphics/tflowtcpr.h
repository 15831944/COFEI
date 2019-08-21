#ifndef TFLOWTCPR_H
#define TFLOWTCPR_H
#include "flowbase.h"

class TFlowTCPR : public TFlowBase
{
    Q_OBJECT
public:
    TFlowTCPR(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowTCPR();

public slots:
    CoItem *Copy();
    void Copy(TFlowTCPR *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWTCPR_H
