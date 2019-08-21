#include "ComboBox.h"
#include <LibBase.h>
#include <CoGraphics/Base/Base.h>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

TComboBox::TComboBox(double StartX, double StartY, double StopX, double StopY,
                     const Qt::PenStyle &LineStyle, const int LineWidth,
                     const QColor &LineColor, const QColor &BackColor) :
        CoWidgets(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::ComboBox);
    m_CurText = tr("下拉框");
    m_TextVec.clear();
    m_TextVec.append(m_CurText);
    m_TextVec.append("下拉框1");
    m_isDown = false;
    m_isDrop = false;
    m_CurStopY = StopY;
}

TComboBox::~TComboBox()
{}

CoItem *TComboBox::Copy()
{
    TComboBox *ComboBox = new TComboBox();

    ComboBox->Copy(this);

    return ComboBox;
}
void TComboBox::SetText(QString Text)
{
    m_CurText = Text;
    m_TextVec.append(m_CurText);
}

QString TComboBox::GetText()
{
    return m_CurText;
}
void TComboBox::Copy(TComboBox *ComboBoxFrom)
{
    if(NULL == ComboBoxFrom)
    {
        return;
    }
    CoWidgets::Copy(ComboBoxFrom);
}

void TComboBox::Draw(QPainter *painter)
{
    double Width = m_StopX - m_StartX;        // 绘图区域的宽度
    double Height = m_StopY - m_StartY;       // 绘图区域的高度
    double CenterX = m_StartX + Width / 2;   // 绘图区域的中心X

    painter->setPen(QPen(QBrush(Qt::gray), 2.0));
    QLinearGradient lg1(CenterX, m_StartY, CenterX, m_StopY);
    lg1.setColorAt(0, QColor(166, 166, 166));
    lg1.setColorAt(0.4, QColor(166, 166, 166));
    lg1.setColorAt(1, QColor(255, 255, 255));


    painter->setBrush(lg1);
    QPainterPath OutLine;
    OutLine.addRect(m_StartX, m_StartY, 4*Width/5, Height);
    painter->drawPath(OutLine);
    painter->fillPath(OutLine, lg1);
    QLinearGradient lg2(CenterX, m_StartY, CenterX, m_StopY);
    lg2.setColorAt(0, QColor(255, 255, 255));
    lg2.setColorAt(0.4, QColor(255, 255, 255));
    lg2.setColorAt(1, QColor(166, 166, 166));
    painter->setBrush(lg2);
    QPainterPath downLine;
    downLine.addRect(m_StartX + 4*Width/5, m_StartY, Width/5, Height);
    painter->drawPath(downLine);
    painter->fillPath(downLine, lg2);

    QPainterPath downflag;
    QPolygonF polygonF;
    polygonF.push_back(QPointF(m_StartX+24*Width/25,m_StartY+Height/8));
    polygonF.push_back(QPointF(m_StartX+21*Width/25,m_StartY+Height/8));
    polygonF.push_back(QPointF(m_StartX+45*Width/50,m_StartY+7*Height/8));
    downflag.addPolygon(polygonF);
    painter->drawPath(downflag);
    painter->fillPath(downflag, QBrush(Qt::black));

    //文字
    painter->setPen(QPen(QBrush(Qt::blue), 2.0));
    painter->drawText(m_StartX, m_StartY, 9*Width/10, Height, Qt::AlignCenter, m_CurText);
    if(m_isDown)
    {
        for(int i = 0; i < m_TextVec.size();++i)
        {
            QLinearGradient lg(CenterX, m_StartY, CenterX, m_StopY);
            lg.setColorAt(0, QColor(106, 106, 106));
            lg.setColorAt(0.4, QColor(166, 166, 166));
            lg.setColorAt(1, QColor(255, 255, 255));
            QPainterPath down;
            down.addRect(m_StartX, m_StartY+Height*(i+1), 4*Width/5, Height);
            painter->drawPath(down);
            painter->fillPath(down, lg);

            painter->drawText(m_StartX, m_StartY+Height*(i+1), 9*Width/10, Height, Qt::AlignCenter, m_TextVec[i]);
        }
    }
}

void TComboBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(Qt::LeftButton == event->button())
    {
        QPointF PressStart = event->scenePos();
        double Width = GetCurrentStopX() - GetCurrentStartX();
        if(PressStart.x() > GetCurrentStartX()+21*Width/25 && PressStart.x() < GetCurrentStartX()+24*Width/25)
        {
            m_isDown = true;
        }
        else if(PressStart.y() > m_StopY && m_isDown)
        {
            m_isDown = false;
            double H = PressStart.y() - m_StopY;
            int i = (int)(H/(m_StopY-m_StartY));
            if(i < m_TextVec.size())
                m_CurText = m_TextVec[i];
        }
    }
    CoItem::mousePressEvent(event);
}
void TComboBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    CoItem::mouseMoveEvent(event);
    if(m_DropScale && m_DropDirection != Direc_Free)
    {
        m_isDrop = true;
    }
}

void TComboBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    scene()->update(GetCurrentStartX() -m_LineWidth,GetCurrentStartY()-m_LineWidth,
                    GetCurrentStopX() - GetCurrentStartX() + 2*m_LineWidth,
                    (GetCurrentStopY() - GetCurrentStartY())*(m_TextVec.size()+1)+2*m_LineWidth);
    CoItem::mouseReleaseEvent(event);
}
QRectF TComboBox::boundingRect() const
{
    double Height = m_StopY - m_StartY;
    if(m_isDown)
        Height *= (m_TextVec.size()+1);
    QRectF mRect(m_StartX, m_StartY, m_StopX - m_StartX, Height);
    const int padding = (m_LineWidth + 1)/2;
    mRect.adjust(-padding, -padding, +padding, +padding);

    return mRect;
}
