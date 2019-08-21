#include "tflowbase.h"



TFlowBase::TFlowBase(double StartX, double StartY, double StopX, double StopY, const Qt::PenStyle &LineStyle,
                     const int LineWidth, const QColor &LineColor, const QColor &BackColor):
    CoFlowChart(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    PropertyBase = NULL;

}

void TFlowBase::ShowText(QString code)
{
    Code = code;
}

void TFlowBase::connectWithText()
{
    connect(PropertyBase,&LibProperyBase::ValueChange,this,&TFlowBase::ShowText);
    PropertyBase->setID("-1");

}
