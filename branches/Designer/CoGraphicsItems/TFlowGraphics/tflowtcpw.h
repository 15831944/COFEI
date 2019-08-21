#ifndef TFLOWTCPW_H
#define TFLOWTCPW_H

#include "flowbase.h"

class TFlowTCPW : public TFlowBase
{
    Q_OBJECT
public:
    TFlowTCPW(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
          const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
          const QColor &LineColor = Qt::black, const QColor &BackColor = Qt::white);

    virtual ~TFlowTCPW();

public slots:
    CoItem *Copy();
    void Copy(TFlowTCPW *Flow);

private:
    void Draw(QPainter *painter);
};

#endif // TFLOWTCPW_H
