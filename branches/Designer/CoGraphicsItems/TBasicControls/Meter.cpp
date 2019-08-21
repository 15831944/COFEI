#include "Meter.h"

#include <LibBase.h>
#include <CoGraphics/Base/Base.h>
#include <QtMath>
#include <QDebug>

TMeter::TMeter(double StartX, double StartY, double StopX, double StopY,
               const Qt::PenStyle &LineStyle, const int LineWidth,
               const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    m_Mode = MODE_COUNTER_WISE;
    m_ScaleSum = 8;
    m_StartAngle = 216;
    m_StepAngle = 36;
    m_StartValue = 0;
    m_StepValue = 1;

    m_PointerColor = QColor(0, 0, 190, 255);//分针颜色

    SetType(CoItem::Meter);
}

TMeter::~TMeter()
{}

CoItem *TMeter::Copy()
{
    TMeter *Meter = new TMeter();

    Meter->Copy(this);

    return Meter;
}

void TMeter::Copy(TMeter *MeterFrom)
{
    if(NULL == MeterFrom)
    {
        return;
    }

    CoGraphics::Copy(MeterFrom);

    this->SetMode(MeterFrom->GetMode());
    this->SetScaleSum(MeterFrom->GetScaleSum());
    this->SetStartAngle(MeterFrom->GetStartAngle());
    this->SetStepAngle(MeterFrom->GetStepAngle());
    this->SetStartValue(MeterFrom->GetStartValue());
    this->SetStepValue(MeterFrom->GetStepValue());
    this->SetPointerColor(MeterFrom->GetPointerColor());
}

void TMeter::SetMode(int Mode)
{
    if(Mode >= MODE_CLOSCK_WISE && Mode <= MODE_COUNTER_WISE)
    {
        m_Mode = Mode;
    }
}

int TMeter::GetMode()
{
    return m_Mode;
}

void TMeter::SetScaleSum(int Sum)
{
    m_ScaleSum = Sum;
}

int TMeter::GetScaleSum()
{
    return m_ScaleSum;
}

void TMeter::SetStartAngle(double Angle)
{
    m_StartAngle = Angle;
}

double TMeter::GetStartAngle()
{
    return m_StartAngle;
}

void TMeter::SetStepAngle(double Angle)
{
    m_StepAngle = Angle;
}

double TMeter::GetStepAngle()
{
    return m_StepAngle;
}

void TMeter::SetStartValue(double Value)
{
    m_StartValue = Value;
}

double TMeter::GetStartValue()
{
    return m_StartValue;
}

void TMeter::SetStepValue(double Value)
{
    m_StepValue = Value;
}

double TMeter::GetStepValue()
{
    return m_StepValue;
}

void TMeter::SetPointerColor(const QColor &Color)
{
    m_PointerColor = Color;
}

QColor TMeter::GetPointerColor()
{
    return m_PointerColor;
}

void TMeter::Draw(QPainter *painter)
{
    int fontsize;
    QFont font;
    double Width = m_StopX - m_StartX;
    double Height = m_StopY - m_StartY;
    double CenterX = m_StartX + Width / 2;   // 绘图区域的中心X
    double CenterY = m_StartY + Height / 2;  // 绘图区域的中心Y

    painter->translate(CenterX, CenterY);   // 把区域中心作为零点

    int Radius = MIN(Width / 2, Height / 2);// 绘制圆形的半径

    // 边框的外圆
    QLinearGradient lg1(0, -Radius, 0, Radius); // 线性渐变
    lg1.setColorAt(0, m_LineColor);
    lg1.setColorAt(1, TBase::GetNewColor(m_LineColor, -90));
    painter->setBrush(lg1);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    // 边框的内圆
    Radius *= 0.90;
    QLinearGradient lg2(0, -Radius, 0, Radius);
    lg2.setColorAt(0, TBase::GetNewColor(m_LineColor, -120));
    lg2.setColorAt(1, m_LineColor);
    painter->setBrush(lg2);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    Radius *= 0.96;
    QRadialGradient rg(0, 0, Radius); // 辐射渐变
    rg.setColorAt(0, TBase::GetNewColor(m_BackGroundColor, -10));
    rg.setColorAt(0.6, TBase::GetNewColor(m_BackGroundColor, -30));
    rg.setColorAt(1, TBase::GetNewColor(m_BackGroundColor, -60));
    painter->setBrush(rg);
    painter->drawEllipse(-Radius, -Radius, Radius << 1, Radius << 1);

    fontsize = 0.1 * Radius + 1;// 加1，避免为0
    font.setPointSize(fontsize);
    painter->setFont(font);

    double DrawAngle = m_StartAngle;
    int Direct = 0;

    if(MODE_COUNTER_WISE == m_Mode)
    {
        Direct = -1;
    }
    else
    {
        Direct = 1;
    }

    for(int i = 0; i < m_ScaleSum; i++)
    {
        double Angle = (DrawAngle + i * Direct * m_StepAngle) / 180 * 3.1415926535898;
        double x = qCos(Angle) * 0.7 * Radius;
        double y = -qSin(Angle)* 0.7 * Radius;

        if(x <= 0)
        {
            x -= (fontsize / 2);
            y -= (fontsize / 2);
            painter->drawText(x,
                            y,
                            Radius,
                            Radius,
                            Qt::AlignLeft | Qt::AlignTop,
                            QString::number(m_StartValue + i * m_StepValue));
        }
        else
        {
            x += (fontsize / 2);
            y += (fontsize * 4 / 5);
            painter->drawText(x - Radius,
                            y - Radius,
                            Radius,
                            Radius,
                            Qt::AlignRight | Qt::AlignBottom,
                            QString::number(m_StartValue + i * m_StepValue));
        }
    }

    // 绘制大刻度
    painter->save();
    painter->rotate(-DrawAngle); // 旋转回起点

    for(int i = 0; i < m_ScaleSum; i++)
    {
        painter->drawLine(0.8 * Radius, 0, Radius, 0);
        painter->rotate(-Direct * m_StepAngle);
    }
    painter->restore();

    // 绘制小刻度
    painter->save();
    painter->rotate(-DrawAngle); // 旋转回起点
    int Count = m_ScaleSum - 1;

    if(360 == m_ScaleSum * m_StepAngle)
    {
        Count = m_ScaleSum;
    }

    for(int i = 0 ; i < Count; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(0 != j)
            {
                painter->drawLine(0.9 * Radius, 0, Radius, 0);
            }

            painter->rotate(-Direct * m_StepAngle / 5);
        }
    }
    painter->restore();

    // 绘制指针
    double PointerWidth = Radius * 0.1; // 指针宽度

    const QPoint minuteHand[3] =
    {
        QPoint(-Radius * 0.2, -PointerWidth),
        QPoint(Radius * 0.7, 0),
        QPoint(-Radius * 0.2, PointerWidth)
    };

    double Value = m_Value;

    if(Value < m_StartValue)
    {
        Value = m_StartValue;
    }
    else if(Value > (m_StartValue + m_StepValue * m_ScaleSum))
    {
        Value = m_StartValue + m_StepValue * m_ScaleSum;
    }

    int Pointer = DrawAngle + Direct * (Value - m_StartValue) / m_StepValue * m_StepAngle;

    painter->setPen(Qt::NoPen);
    painter->setBrush(m_PointerColor);

    painter->save();
    painter->rotate(-Pointer);
    painter->drawConvexPolygon(minuteHand, 3);
    painter->restore();
    painter->setBrush(Qt::lightGray);             //画上中心原点
    painter->drawEllipse(QPoint(0,0),2,2);
}
