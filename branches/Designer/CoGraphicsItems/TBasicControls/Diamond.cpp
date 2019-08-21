#include "Diamond.h"
#include <QKeyEvent>
TDiamond::TDiamond(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Diamond);
    m_Text = tr("");
}

TDiamond::~TDiamond()
{}

CoItem *TDiamond::Copy()
{
    TDiamond *Diam = new TDiamond();

    Diam->Copy(this);

    return Diam;
}

void TDiamond::Copy(TDiamond *DiamFrom)
{
    if(NULL == DiamFrom)
    {
        return;
    }

    CoGraphics::Copy(DiamFrom);
}

void TDiamond::SetText(const QString &Text)
{
    m_Text = Text;
}

QString TDiamond::GetText()
{
    return m_Text;
}

void TDiamond::Draw(QPainter *painter)
{
    double CenterX = m_StartX + (m_StopX - m_StartX) / 2;
    double CenterY = m_StartY + (m_StopY - m_StartY) / 2;  // 绘图区域的中心Y
    QPolygonF PolygonF;
    PolygonF.push_back(QPointF(m_StartX,CenterY));
    PolygonF.push_back(QPointF(CenterX,m_StartY));
    PolygonF.push_back(QPointF(m_StopX,CenterY));
    PolygonF.push_back(QPointF(CenterX,m_StopY));
    PolygonF.push_back(QPointF(m_StartX,CenterY));
    painter->drawPolyline(PolygonF);
    painter->drawText(m_StartX + 6,CenterY - 6,(m_StopX - m_StartX) -12,24,Qt::AlignHCenter,m_Text);

}

bool TDiamond::SetSelectAble(bool Able)
{
    Q_UNUSED(Able);

    return false;
}

bool TDiamond::GetSelectAble()
{
    return m_SelectAble;
}

void TDiamond::keyReleaseEvent(QKeyEvent *event)
{
    if(Qt::Key_Backspace != event->key())
    {
        if(event->text().length() > 0)
        {
            m_Text += event->text();
        }
    }
    else
    {
        if(m_Text.length() > 0)
        {
            m_Text = m_Text.left(m_Text.length() - 1);
        }
    }

    Update();

    QGraphicsItem::keyReleaseEvent(event);
}
