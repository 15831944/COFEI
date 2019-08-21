#include "Line.h"

#include <QCursor>
#include <QGraphicsSceneEvent>

TLine::TLine(double StartX, double StartY, double StopX, double StopY,
             const Qt::PenStyle &LineStyle, const int LineWidth,
             const QColor &LineColor, const QColor &BackColor) :
        CoGraphics(StartX, StartY, StopX, StopY, LineStyle, LineWidth, LineColor, BackColor)
{
    SetType(CoItem::Line);

    m_StartX = StartX;
    m_StartY = StartY;
    m_StopX = StopX;
    m_StopY = StopY;

    UpdatePos();
}

TLine::~TLine()
{}

QRectF TLine::boundingRect() const
{
    QRectF mRect(m_PosX, m_PosY, m_Width, m_Height);

    const int padding = (m_LineWidth + 1) / 2;
    mRect.adjust(-padding, -padding, +padding, +padding);

    return mRect;
}

void TLine::UpdatePos()
{
    m_PosX = MIN(m_StartX, m_StopX);
    m_PosY = MIN(m_StartY, m_StopY);
    m_Width = ABS(m_StopX - m_StartX);
    m_Height = ABS(m_StopY - m_StartY);
}

void TLine::SetStartPos(double StartX, double StartY)
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

void TLine::SetStopPos(double StopX, double StopY)
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

void TLine::SetStartPos(const QPointF &Start)
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

void TLine::SetStopPos(const QPointF &Stop)
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

void TLine::MoveTo(double NewStartX, double NewStartY, double NewStopX, double NewStopY)
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

CoItem *TLine::Copy()
{
    TLine *Line = new TLine(m_StartX, m_StartY, m_StopX, m_StopY, m_LineStyle,
                            m_LineWidth, m_LineColor, m_BackGroundColor);

    Line->Copy(this);

    return Line;
}

void TLine::Copy(TLine *LineFrom)
{
    if(NULL == LineFrom)
    {
        return;
    }

    CoGraphics::Copy(LineFrom);

    this->UpdatePos();
}

void TLine::UpdateMouse(QPointF &PosNow)
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

void TLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void TLine::Draw(QPainter *painter)
{
    painter->drawLine(m_StartX, m_StartY, m_StopX, m_StopY);
}
