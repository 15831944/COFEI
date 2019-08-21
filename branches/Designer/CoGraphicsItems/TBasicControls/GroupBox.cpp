#include "GroupBox.h"
#include <LibBase.h>
#include <CoGraphics/Scene/Scene.h>
#include <QStyleOptionGraphicsItem>
#include <QDebug>

TGroupBox::TGroupBox(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor) :
    CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::GroupBox);
    m_Text = tr("GroupBox");
}

TGroupBox::~TGroupBox()
{}

CoItem *TGroupBox::Copy()
{
    TGroupBox *Group = new TGroupBox();

    Group->Copy(this);

    return Group;
}

void TGroupBox::Copy(TGroupBox *GroupFrom)
{
    if(NULL == GroupFrom)
    {
        return;
    }
    this->SetTitle(GroupFrom->GetTitle());
    CoGraphics::Copy(GroupFrom);
}

void TGroupBox::SetAttributeEvent(const int type, const QString value, uchar uc)
{
    Q_UNUSED(type)
    Q_UNUSED(uc)
    SetTitle(value);
}

void TGroupBox::SetTitle(QString Text)
{
    m_Text = Text;
    Update();
}

QString TGroupBox::GetTitle()
{
    return m_Text;
}
void TGroupBox::SetLineColor(const QColor &color)
{
    CoItem::SetLineColor(color);
}

void TGroupBox::SetLineColor(int r, int g, int b, int a)
{
    this->SetLineColor(QColor(r, g, b, a));
}

void TGroupBox::SetLineStyle(const Qt::PenStyle &style)
{
    CoItem::SetLineStyle(style);
}

void TGroupBox::SetLineStyle(int style)
{
    this->SetLineStyle(Qt::PenStyle(style));
}

void TGroupBox::SetLineWidth(int width)
{
    CoItem::SetLineWidth(width);
}

void TGroupBox::Draw(QPainter* painter)
{
    QPainterPath bigRect;
    bigRect.addRoundedRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY -m_StartY,15,15);

    QPainterPath smallRect;
    smallRect.addRoundedRect(m_StartX+m_LineWidth, m_StartY+m_LineWidth ,
                             m_StopX - m_StartX-m_LineWidth*2, m_StopY -m_StartY-m_LineWidth*2,15,15);

    QFontMetrics fm(m_font);
    QRect rec = fm.boundingRect(m_Text);
    QPainterPath textRect;
    textRect.addRect(m_StartX+10, m_StartY,rec.width(),m_LineWidth);

    QPainterPath path = bigRect - smallRect - textRect;
    painter->drawPath(path);

    painter->drawText(m_StartX+10, m_StartY-12,rec.width(),24,Qt::AlignCenter,m_Text);
}

QRectF TGroupBox::boundingRect() const
{
    QRectF mRect(m_StartX, m_StartY, m_StopX - m_StartX, m_StopY - m_StartY);

    const int padding = (m_LineWidth + 1)/2;
    mRect.adjust(-padding, -(padding*2), +padding, +padding);

    return mRect;
}
