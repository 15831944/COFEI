#include "Slider.h"
#include <LibBase.h>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
TSlider::TSlider(double StartX, double StartY, double StopX, double StopY,
                 const Qt::PenStyle &LineStyle, const int LineWidth,
                 const QColor &LineColor, const QColor &BackColor) :
    CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::SliderF);
    m_Min = 0.0f ;
    m_Max = 99.0f;
    m_Value = 50.0f;
    m_SingleStep = 0.1f;
    m_State = None;
    m_Label = tr("滑块");
}

TSlider::~TSlider()
{}

CoItem *TSlider::Copy()
{
    TSlider *Slider = new TSlider();

    Slider->Copy(this);

    return Slider;
}

void TSlider::Copy(TSlider *SliderFrom)
{
    if(NULL == SliderFrom)
    {
        return;
    }
    CoWidgets::Copy(SliderFrom);
}

float TSlider::GetValidValue(float val, float min, float max)
{
    float tmp = MAX(val, min);
    return MIN(tmp, max);
}


void TSlider::SetRange(float min, float max)
{
    m_Min = min;
    m_Max = max;

    float val = MIN(m_Value,m_Max);
    val = MAX(val,m_Min);

    SetValue(val);
}

void TSlider::SetSingleStep(float step)
{
    m_SingleStep = step;
}

float TSlider::MinRange() const
{
    return m_Min;
}

float TSlider::MaxRange() const
{
    return m_Max;
}

float TSlider::Value()    const
{
    return m_Value;
}

void TSlider::SetLabel(const QString& label)
{
    m_Label = label;
    update();
}

void TSlider::SetValue(float val)
{
    if(ABS(m_Value - val) > (m_SingleStep / 100))
    {
        m_Value = val;
        emit valueChanged(val);
        update();
    }
}
void TSlider::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    CoItem::hoverLeaveEvent(event);
    m_State = None;
    update();
}


void TSlider::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(!m_MoveAble && !m_DragAble)
    {
        if(m_HandleRegion.contains(QPointF(event->scenePos().x() - m_StartX,event->scenePos().y() - m_StartY - 20)))
        {
            m_State = Hovered;
        }
        else
        {
            m_State = None;
        }
    }
    CoItem::mousePressEvent(event);
}

void TSlider::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(!m_MoveAble && !m_DragAble)
    {
        double Width = m_StopX - m_StartX;
        float move = (event->scenePos().x() - m_StartX) * (m_Max - m_Min) * 1.0/ Width + m_Min;
        if(m_State == Hovered)
        {
            float val = GetValidValue( move, m_Min,m_Max);
            SetValue(val);
        }
    }

    CoItem::mouseMoveEvent(event);
}

void TSlider::Draw(QPainter* painter)
{
    double Width = m_StopX - m_StartX;
    double Height = m_StopY - m_StartY;
    painter->translate(m_StartX, m_StartY);

    QFont font;
    int fontsize = Height/4 ;
    if(fontsize > 1)
        fontsize -= 1;
    font.setPointSize(fontsize);
    painter->setFont(font);
    QString valueString = QString::number(m_Value,'f',3);
    QFontMetrics metrics = painter->fontMetrics();
    int textWidth = metrics.width(valueString);
    int textHeight = metrics.height();

    painter->drawText(QRectF(2,0,textWidth,textHeight),valueString);

    int labelWidth = metrics.width(m_Label);
    int labelHeight = metrics.height();
    QRectF textRect = QRectF((Width/2 - labelWidth/2), 0,labelWidth,labelHeight);
    painter->drawText(textRect,m_Label);

    int minPos = ( m_Value - m_Min ) * Width / (m_Max - m_Min);

    if(minPos <= 4)
    {
        minPos = 4;
    }
    else if(minPos >= Width - 8)
    {
        minPos = Width - 8;
    }
    PaintColoredRect(QRect(4,Height/2,Width - 8,Height/3),Qt::gray,painter);
    PaintColoredRect(QRect(4,Height/2,minPos,Height/3),QColor(51,193,155),painter);
    m_HandleRegion = QRectF(minPos ,Height/3,Width/20,2*Height/3);

    QColor minColor  = (m_State == Hovered) ? QColor(51,153,155) : Qt::darkGray;
    PaintColoredRect(m_HandleRegion,minColor,painter);
}
void TSlider::PaintColoredRect(QRectF rect, QColor color ,QPainter* painter)
{
    painter->fillRect(rect,QBrush(color));
}
