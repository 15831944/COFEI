#include "flowbase.h"


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

QDataStream &operator <<(QDataStream &out, TFlowBase &data)
{

    out<<data.GetId()<< data.GetStartX()<<data.GetStartY()<<data.GetStopX()<<data.GetStopY()
      <<data.GetLineStyle()<<data.GetLineWidth()<<data.GetLineColor()
     <<data.GetBackGroundColor()<<data.m_ArrowPost<<(*(data.PropertyBase));
    return out;
}

QDataStream &operator >>(QDataStream &in, TFlowBase &data)
{
    double StartX, StartY, StopX, StopY;
    int LineWidth;
    QColor LineColor,BackColor;
    int penstyle;
    int id;

    in>>id>>StartX>>StartY>>StopX>>StopY>>penstyle>>LineWidth>>LineColor
    >>BackColor>>data.m_ArrowPost>>(*(data.PropertyBase));
    data.SetId(id);
    data.SetStartPos(StartX,StartY);
    data.SetStopPos(StopX,StopY);
    data.SetLineStyle(penstyle);
    data.SetLineWidth(LineWidth);
    data.SetLineColor(LineColor);
    data.SetBackGroundColor(BackColor);
    return in;
}
