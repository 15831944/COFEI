#ifndef TFLOWBASE_H
#define TFLOWBASE_H
#include <CoGraphics/Graphics.h>
#include <QObject>
#include "../FlowPropery/libproperybase.h"
class TFlowBase : public CoFlowChart
{
    Q_OBJECT
public:
    TFlowBase(double StartX = 0, double StartY = 0, double StopX = 0, double StopY = 0,
                const Qt::PenStyle &LineStyle = Qt::SolidLine, const int LineWidth = 1,
                const QColor &LineColor = Qt::white, const QColor &BackColor = Qt::black);

public:
    LibProperyBase *PropertyBase;
    void connectWithText();
    void ShowText(QString code);
    QString Code;

    friend QDataStream& operator <<(QDataStream &out,TFlowBase &data);
    friend QDataStream& operator >>(QDataStream &in, TFlowBase &data);
};

#endif // TFLOWBASE_H