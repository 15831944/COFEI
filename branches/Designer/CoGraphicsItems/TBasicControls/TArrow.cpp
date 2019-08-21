#include "TArrow.h"
#include <QDebug>
#include <QCursor>
#include <QGraphicsSceneEvent>
#include <math.h>
TArrow::TArrow(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Arrow);

    m_StartX = StartX;
    m_StartY = StartY;
    m_StopX = StopX;
    m_StopY = StopY;
    UpdatePos();
}

TArrow::~TArrow()
{}

QRectF TArrow::boundingRect() const
{
    qreal extra = (m_LineWidth + 15) / 2.0;
    QPolygonF mPolygonPoints;
    mPolygonPoints << QPointF(m_StartX,m_StartY) << QPointF(m_StopX,m_StopY);
    return QRectF(mPolygonPoints.boundingRect().topLeft(),mPolygonPoints.boundingRect().bottomRight())
        .normalized()
            .adjusted(-extra, -extra, extra, extra);
}
QPainterPath TArrow::shape() const
{
    QPainterPath path = QGraphicsItem::shape();
    path.addPolygon(m_arrowHead);
    return path;
}
void TArrow::UpdatePos()
{
    m_PosX = MIN(m_StartX, m_StopX);
    m_PosY = MIN(m_StartY, m_StopY);
    m_Width = ABS(m_StopX - m_StartX);
    m_Height = ABS(m_StopY - m_StartY);
}

void TArrow::SetStartPos(double StartX, double StartY)
{
    CoItem::SetStartPos(StartX, StartY);

    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    UpdatePos();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void TArrow::SetStopPos(double StopX, double StopY)
{
    CoItem::SetStopPos(StopX, StopY);

    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    UpdatePos();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void TArrow::SetStartPos(const QPointF &Start)
{
    CoItem::SetStartPos(Start);

    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    UpdatePos();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void TArrow::SetStopPos(const QPointF &Stop)
{
    CoItem::SetStopPos(Stop);

    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    UpdatePos();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

void TArrow::MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
{
    double SaveAngle = GetRotateAngle();

    if(0 != SaveAngle)
    {
        SetRotateAngle(0);
    }

    MovePos(NewStartX, NewStartY, NewStopX, NewStopY);
    UpdatePos();

    if(0 != SaveAngle)
    {
        SetRotateAngle(SaveAngle);
    }
}

CoItem *TArrow::Copy()
{
    TArrow *Arrow = new TArrow(m_StartX, m_StartY, m_StopX, m_StopY, m_LineStyle,
                            m_LineWidth, m_LineColor, m_BackGroundColor);

    Arrow->Copy(this);

    return Arrow;
}

void TArrow::Copy(TArrow *ArrowFrom)
{
    if(NULL == ArrowFrom)
    {
        return;
    }

    CoGraphics::Copy(ArrowFrom);

    this->UpdatePos();
}

void TArrow::UpdateMouse(QPointF &PosNow)
{
    QPointF Start = this->scenePos() + QPointF(m_StartX, m_StartY);
    QPointF Stop = this->scenePos() + QPointF(m_StopX, m_StopY);

    int Add = m_LineWidth + 2;

    QCursor mCursor;

    if(ABS(PosNow.x() - Start.x()) < Add && ABS(PosNow.y() - Start.y()) < Add)      // 左上
    {
        m_DropDirection = 8;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else if(ABS(PosNow.x() - Stop.x()) < Add && ABS(PosNow.y() - Stop.y()) < Add) // 右下
    {
        m_DropDirection = 4;
        mCursor.setShape(Qt::SizeFDiagCursor);
    }
    else
    {
        m_DropDirection = 0;
        mCursor.setShape(Qt::ArrowCursor);
    }

    if(Direc_Free == m_DropDirection)
    {
        MoveEnable(m_MoveAble);
    }
    else
    {
        MoveEnable(false);
    }
    setCursor(mCursor);
}

void TArrow::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_DropScale)
    {
        QPointF dis = event->scenePos() - m_DropPos;
        m_DropPos = event->scenePos();

        double DisX = dis.x() / 2;
        double DisY = dis.y() / 2;

        switch(m_DropDirection)
        {
        case 8: // 起点
            {
                m_StartX += DisX;
                m_StartY += DisY;
                m_StopX -= DisX;
                m_StopY -= DisY;

                UpdatePos();

                update(boundingRect());
                moveBy(DisX, DisY);

                break;
            }

        case 4: // 终点
            {
                m_StartX -= DisX;
                m_StartY -= DisY;
                m_StopX += DisX;
                m_StopY += DisY;

                UpdatePos();

                update(boundingRect());
                moveBy(DisX, DisY);

                break;
            }

        default:
            break;
        }
    }

    Update();

    if(m_MoveAble)
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void TArrow::Draw(QPainter *painter)
{
    QPen myPen = QPen(m_LineColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    myPen.setColor(m_LineColor);
    painter->setPen(myPen);
    painter->setBrush(m_LineColor);
    double x1, y1, x2, y2; //箭头的两点坐标

    CalcVertexes(m_StartX, m_StartY, m_StopX, m_StopY, x1, y1, x2, y2);
    m_arrowHead.clear();
    m_arrowHead.push_back(QPointF(x1,y1));
    m_arrowHead.push_back(QPointF(m_StopX,m_StopY));
    m_arrowHead.push_back(QPointF(x2,y2));

    painter->drawLine(m_StartX, m_StartY, m_StopX, m_StopY);
    painter->drawPolygon(m_arrowHead);
}
void TArrow::CalcVertexes(double startX, double startY, double stopX, double stopY,
                          double& destX1, double& destY1, double& destX2, double& destY2)
{
    double lenght = 15;//箭头长度，一般固定
    double degrees = 0.6;//箭头角度，一般固定

    double angle = atan2(stopY - startY, stopX - startX) + 3.14;//

    destX1 = stopX + lenght * cos(angle - degrees);
    destY1 = stopY + lenght * sin(angle - degrees);
    destX2 = stopX + lenght * cos(angle + degrees);
    destY2 = stopY + lenght * sin(angle + degrees);
}
