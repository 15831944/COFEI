#include "Arrow.h"
#include <QDebug>
#include <QCursor>
#include <QGraphicsSceneEvent>
#include "Scene/Scene.h"
#include <math.h>
#include <QMenu>
const int Distance = 12;
CoArrow::CoArrow(CoFlowChart * startItem, CoFlowChart * endItem)
{
    m_isYes = true;
    m_StartItem = startItem;
    m_EndItem = endItem;
    m_StartPost = m_StartItem->m_ArrowPost;
    m_EndPost = m_EndItem->m_ArrowPost;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_Color = Qt::gray;
    //setPen(QPen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

CoArrow::~CoArrow()
{}

QRectF CoArrow::boundingRect() const
{
    qreal extra = (pen().width() + 10) / 2.0;
    QPolygonF mPolygonPoints;
    mPolygonPoints << m_p1 << m_p2 << m_p3 << m_p4 << m_p5 << m_p6;
    return QRectF(mPolygonPoints.boundingRect().topLeft(),mPolygonPoints.boundingRect().bottomRight())
        .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath CoArrow::shape() const
{
    QPainterPath path = QGraphicsItem::shape();
    path.addPolygon(m_arrowHead);
    return path;
}
void CoArrow::UpdatePos()
{
    if(m_StartItem == NULL || m_EndItem == NULL)
        return;
    StartEndPos();
    setLine(QLineF(m_p1,m_p6));
    update();
}

CoFlowChart *CoArrow::GetStartItem()
{
    return m_StartItem;
}

CoFlowChart *CoArrow::GetEndItem()
{
    return m_EndItem;
}

int CoArrow::GetStartPost()
{
    return m_StartPost;
}

int CoArrow::GetEndPost()
{
    return m_EndPost;
}

bool CoArrow::GetYNPolicy()
{
    return m_isYes;
}

void CoArrow::SetYNPolicy(bool isyn)
{
    m_isYes = isyn;
}

void CoArrow::SetYes()
{
    m_isYes = true;
}

void CoArrow::SetNo()
{
    m_isYes = false;
}
void CoArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if (m_StartItem->collidesWithItem(m_EndItem))
        return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    double x1, y1, x2, y2; //箭头的两点坐标

    CalcVertexes(m_p5.x(), m_p5.y(), m_p6.x(), m_p6.y(), x1, y1, x2, y2);
    m_arrowHead.clear();
    m_arrowHead.push_back(QPointF(x1,y1));
    m_arrowHead.push_back(QPointF(m_p6.x(), m_p6.y()));
    m_arrowHead.push_back(QPointF(x2,y2));

    QPen mPen = QPen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    if(!m_isYes)
        mPen.setStyle(Qt::DashDotLine);
    if(isSelected())
    {
        mPen.setColor(Qt::red);
        mPen.setWidth(3);
        painter->setBrush(Qt::red);
    }
    else
    {
        painter->setBrush(m_Color);
    }
    painter->setPen(mPen);

    painter->drawLine(m_p1,m_p2);
    painter->drawLine(m_p2,m_p3);
    painter->drawLine(m_p3,m_p4);
    painter->drawLine(m_p4,m_p5);
    painter->drawLine(m_p5,m_p6);
    painter->drawPolygon(m_arrowHead);

}
void CoArrow::CalcVertexes(double startX, double startY, double stopX, double stopY,
                          double& destX1, double& destY1, double& destX2, double& destY2)
{
    double lenght = 10;//箭头长度，一般固定
    double degrees = 0.6;//箭头角度，一般固定

    double angle = atan2(stopY - startY, stopX - startX) + 3.14159;//

    destX1 = stopX + lenght * cos(angle - degrees);
    destY1 = stopY + lenght * sin(angle - degrees);
    destX2 = stopX + lenght * cos(angle + degrees);
    destY2 = stopY + lenght * sin(angle + degrees);
}

void CoArrow::StartEndPos()
{
    if(m_StartPost == CoItem::Arrow_Left)
    {
        m_p1 = QPointF(m_StartItem->GetCurrentStartX(),m_StartItem->GetCurrentCenterY());
        m_p2 = QPointF(m_p1.x()-Distance/2,m_p1.y());
        SetLeftEndItemPos();
    }
    else if(m_StartPost == CoItem::Arrow_Right)
    {
        m_p1 = QPointF(m_StartItem->GetCurrentStopX(),m_StartItem->GetCurrentCenterY());
        m_p2 = QPointF(m_p1.x()+Distance/2,m_p1.y());
        SetRightEndItemPos();
    }
    else if(m_StartPost == CoItem::Arrow_Top)
    {
        m_p1 = QPointF(m_StartItem->GetCurrentCenterX(),m_StartItem->GetCurrentStartY());
        m_p2 = QPointF(m_p1.x(),m_p1.y()-Distance/2);
        SetTopEndItemPos();
    }
    else if(m_StartPost == CoItem::Arrow_Bottom)
    {
        m_p1 = QPointF(m_StartItem->GetCurrentCenterX(),m_StartItem->GetCurrentStopY());
        m_p2 = QPointF(m_p1.x(),m_p1.y()+Distance/2);
        SetBottomEndItemPos();
    }

}

void CoArrow::SetLeftEndItemPos()
{
    if(m_EndPost == CoItem::Arrow_Left)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStartX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()-Distance,m_p6.y());

        if(m_p2.y() <= m_EndItem->GetCurrentStopY() && m_p2.y() >= m_EndItem->GetCurrentStartY())
        {
            double minY = MIN(m_EndItem->GetCurrentStartY(),m_StartItem->GetCurrentStartY()) - Distance;
            m_p3 = QPointF(m_p2.x(),minY);
            m_p4 = QPointF(m_p5.x(),minY);
        }
        else
        {
            if(m_p2.x() >= m_p5.x())
            {
                m_p3 = QPointF(m_p5.x(),m_p2.y());
                m_p4 = QPointF(m_p5.x(),m_p2.y());
            }
            else
            {
                m_p3 = QPointF(m_p2.x(),m_p5.y());
                m_p4 = QPointF(m_p2.x(),m_p5.y());
            }
        }
    }
    else if(m_EndPost == CoItem::Arrow_Right)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStopX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()+Distance,m_p6.y());
        if(m_p2.x() >= m_p5.x())
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
        else
        {
            double centerY = (m_p1.y()+ m_p6.y())/2;
            if(centerY >= m_StartItem->GetCurrentStartY() && centerY <= m_StartItem->GetCurrentStopY())
            {
                double minY = MIN(m_EndItem->GetCurrentStartY(),m_StartItem->GetCurrentStartY()) - Distance;
                m_p3 = QPointF(m_p2.x(),minY);
                m_p4 = QPointF(m_p5.x(),minY);
            }
            else
            {
                m_p3 = QPointF(m_p2.x(),centerY);
                m_p4 = QPointF(m_p5.x(),centerY);
            }
        }

    }
    else if(m_EndPost == CoItem::Arrow_Top)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStartY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()-Distance);
        if(m_p5.y() <= m_StartItem->GetCurrentStopY() && m_p5.y() >= m_StartItem->GetCurrentStartY() &&
                m_p5.x() > m_StartItem->GetCurrentStopX())
        {
            double minY = m_StartItem->GetCurrentStartY()-Distance;
            m_p3 = QPointF(m_p2.x(), minY);
            m_p4 = QPointF(m_p5.x(),minY);
        }
        else
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
    }
    else if(m_EndPost == CoItem::Arrow_Bottom)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStopY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()+Distance);
        if(m_p5.y() <= m_StartItem->GetCurrentStopY() && m_p5.y() >= m_StartItem->GetCurrentStartY() &&
                m_p5.x() > m_StartItem->GetCurrentStopX())
        {
            double maxY = m_StartItem->GetCurrentStopY()+Distance;
            m_p3 = QPointF(m_p2.x(), maxY);
            m_p4 = QPointF(m_p5.x(),maxY);
        }
        else
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
    }
}

void CoArrow::SetRightEndItemPos()
{
    if(m_EndPost == CoItem::Arrow_Left)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStartX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()-Distance,m_p6.y());
        if(m_p2.x() <= m_p5.x())
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
        else
        {
            double centerY = (m_p1.y()+ m_p6.y())/2;
            if(centerY >= m_StartItem->GetCurrentStartY() && centerY <= m_StartItem->GetCurrentStopY())
            {
                double minY = MIN(m_EndItem->GetCurrentStartY(),m_StartItem->GetCurrentStartY()) - Distance;
                m_p3 = QPointF(m_p2.x(),minY);
                m_p4 = QPointF(m_p5.x(),minY);
            }
            else
            {
                m_p3 = QPointF(m_p2.x(),centerY);
                m_p4 = QPointF(m_p5.x(),centerY);
            }
        }
    }
    else if(m_EndPost == CoItem::Arrow_Right)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStopX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()+Distance,m_p6.y());

        if(m_p2.y() <= m_EndItem->GetCurrentStopY() && m_p2.y() >= m_EndItem->GetCurrentStartY())
        {
            double minY = MIN(m_EndItem->GetCurrentStartY(),m_StartItem->GetCurrentStartY()) - Distance;
            m_p3 = QPointF(m_p2.x(),minY);
            m_p4 = QPointF(m_p5.x(),minY);
        }
        else
        {
            if(m_p2.x() <= m_p5.x())
            {
                m_p3 = QPointF(m_p5.x(),m_p2.y());
                m_p4 = QPointF(m_p5.x(),m_p2.y());
            }
            else
            {
                m_p3 = QPointF(m_p2.x(),m_p5.y());
                m_p4 = QPointF(m_p2.x(),m_p5.y());
            }
        }

    }
    else if(m_EndPost == CoItem::Arrow_Top)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStartY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()-Distance);
        if(m_p5.y() <= m_StartItem->GetCurrentStopY() && m_p5.y() >= m_StartItem->GetCurrentStartY() &&
                 m_p5.x() < m_StartItem->GetCurrentStartX())
        {
            double minY = m_StartItem->GetCurrentStartY()-Distance;
            m_p3 = QPointF(m_p2.x(), minY);
            m_p4 = QPointF(m_p5.x(),minY);
        }
        else
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
    }
    else if(m_EndPost == CoItem::Arrow_Bottom)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStopY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()+Distance);
        if(m_p5.y() <= m_StartItem->GetCurrentStopY() && m_p5.y() >= m_StartItem->GetCurrentStartY() &&
                m_p5.x() < m_StartItem->GetCurrentStartX())
        {
            double maxY = m_StartItem->GetCurrentStopY()+Distance;
            m_p3 = QPointF(m_p2.x(), maxY);
            m_p4 = QPointF(m_p5.x(),maxY);
        }
        else
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
    }
}

void CoArrow::SetTopEndItemPos()
{
    if(m_EndPost == CoItem::Arrow_Left)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStartX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()-Distance,m_p6.y());
        if(m_p5.x() <= m_StartItem->GetCurrentStopX() && m_p5.x() >= m_StartItem->GetCurrentStartX() &&
                m_p5.y() >= m_StartItem->GetCurrentStopY())
        {
            double minX = m_StartItem->GetCurrentStartX() - Distance;
            m_p3 = QPointF(minX,m_p2.y());
            m_p4 = QPointF(minX,m_p5.y());
        }
        else
        {
            m_p3 = QPointF(m_p5.x(),m_p2.y());
            m_p4 = QPointF(m_p5.x(),m_p2.y());
        }
    }
    else if(m_EndPost == CoItem::Arrow_Right)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStopX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()+Distance,m_p6.y());
        if(m_p5.x() <= m_StartItem->GetCurrentStopX() && m_p5.x() >= m_StartItem->GetCurrentStartX() &&
                m_p5.y() >= m_StartItem->GetCurrentStopY())
        {
            double minX = m_StartItem->GetCurrentStopX() + Distance;
            m_p3 = QPointF(minX,m_p2.y());
            m_p4 = QPointF(minX,m_p5.y());
        }
        else
        {
            m_p3 = QPointF(m_p5.x(),m_p2.y());
            m_p4 = QPointF(m_p5.x(),m_p2.y());
        }

    }
    else if(m_EndPost == CoItem::Arrow_Top)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStartY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()-Distance);
        if(m_p2.x() <= m_EndItem->GetCurrentStopX() && m_p2.x() >= m_EndItem->GetCurrentStartX())
        {
            double maxX = MAX(m_EndItem->GetCurrentStopX(),m_StartItem->GetCurrentStopX())+ Distance;
            m_p3 = QPointF(maxX,m_p2.y());
            m_p4 = QPointF(maxX,m_p5.y());
        }
        else
        {
            if(m_p2.y() <= m_p5.y())
            {
                m_p3 = QPointF(m_p5.x(),m_p2.y());
                m_p4 = QPointF(m_p5.x(),m_p2.y());
            }
            else
            {
                m_p3 = QPointF(m_p2.x(),m_p5.y());
                m_p4 = QPointF(m_p2.x(),m_p5.y());
            }
        }
    }
    else if(m_EndPost == CoItem::Arrow_Bottom)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStopY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()+Distance);
        if(m_p2.y() >= m_p5.y())
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
        else
        {
            double centerX = (m_p1.x()+ m_p6.x())/2;
            if(centerX >= m_StartItem->GetCurrentStartX() && centerX <= m_StartItem->GetCurrentStopX())
            {
                double maxX = MIN(m_EndItem->GetCurrentStopX(),m_StartItem->GetCurrentStopX()) + Distance;
                m_p3 = QPointF(maxX ,m_p2.y());
                m_p4 = QPointF(maxX ,m_p5.y());
            }
            else
            {
                m_p3 = QPointF(centerX,m_p2.y());
                m_p4 = QPointF(centerX,m_p5.y());
            }
        }
    }
}

void CoArrow::SetBottomEndItemPos()
{
    if(m_EndPost == CoItem::Arrow_Left)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStartX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()-Distance,m_p6.y());
        if(m_p5.x() <= m_StartItem->GetCurrentStopX() && m_p5.x() >= m_StartItem->GetCurrentStartX() &&
                m_p5.y() <= m_StartItem->GetCurrentStartY())
        {
            double minX = m_StartItem->GetCurrentStartX() - Distance;
            m_p3 = QPointF(minX,m_p2.y());
            m_p4 = QPointF(minX,m_p5.y());
        }
        else
        {
            m_p3 = QPointF(m_p5.x(),m_p2.y());
            m_p4 = QPointF(m_p5.x(),m_p2.y());
        }
    }
    else if(m_EndPost == CoItem::Arrow_Right)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentStopX(),m_EndItem->GetCurrentCenterY());
        m_p5 = QPointF(m_p6.x()+Distance,m_p6.y());
        if(m_p5.x() <= m_StartItem->GetCurrentStopX() && m_p5.x() >= m_StartItem->GetCurrentStartX() &&
                m_p5.y() <= m_StartItem->GetCurrentStartY())
        {
            double minX = m_StartItem->GetCurrentStopX() + Distance;
            m_p3 = QPointF(minX,m_p2.y());
            m_p4 = QPointF(minX,m_p5.y());
        }
        else
        {
            m_p3 = QPointF(m_p5.x(),m_p2.y());
            m_p4 = QPointF(m_p5.x(),m_p2.y());
        }

    }
    else if(m_EndPost == CoItem::Arrow_Top)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStartY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()-Distance);

        if(m_p2.y() < m_p5.y())
        {
            m_p3 = QPointF(m_p2.x(),m_p5.y());
            m_p4 = QPointF(m_p2.x(),m_p5.y());
        }
        else
        {
            double centerX = (m_p1.x()+ m_p6.x())/2;
            if(centerX >= m_StartItem->GetCurrentStartX() && centerX <= m_StartItem->GetCurrentStopX())
            {
                double maxX = MIN(m_EndItem->GetCurrentStopX(),m_StartItem->GetCurrentStopX()) + Distance;
                m_p3 = QPointF(maxX ,m_p2.y());
                m_p4 = QPointF(maxX ,m_p5.y());
            }
            else
            {
                m_p3 = QPointF(centerX,m_p2.y());
                m_p4 = QPointF(centerX,m_p5.y());
            }
        }


    }
    else if(m_EndPost == CoItem::Arrow_Bottom)
    {
        m_p6 = QPointF(m_EndItem->GetCurrentCenterX(),m_EndItem->GetCurrentStopY());
        m_p5 = QPointF(m_p6.x(),m_p6.y()+Distance);
        if(m_p2.x() <= m_EndItem->GetCurrentStopX() && m_p2.x() >= m_EndItem->GetCurrentStartX())
        {
            double maxX = MAX(m_EndItem->GetCurrentStopX(),m_StartItem->GetCurrentStopX())+ Distance;
            m_p3 = QPointF(maxX,m_p2.y());
            m_p4 = QPointF(maxX,m_p5.y());
        }
        else
        {
            if(m_p2.y() >= m_p5.y())
            {
                m_p3 = QPointF(m_p5.x(),m_p2.y());
                m_p4 = QPointF(m_p5.x(),m_p2.y());
            }
            else
            {
                m_p3 = QPointF(m_p2.x(),m_p5.y());
                m_p4 = QPointF(m_p2.x(),m_p5.y());
            }
        }
    }
}

bool CoArrow::IntersectPos(QLineF sourceline ,QPointF &interPos)
{
    QPointF lefttop = QPointF(m_EndItem->GetCurrentStartX(),m_EndItem->GetCurrentStartY());
    QPointF righttop = QPointF(m_EndItem->GetCurrentStopX(),m_EndItem->GetCurrentStartY());
    QPointF leftbottom = QPointF(m_EndItem->GetCurrentStartX(),m_EndItem->GetCurrentStopY());
    QPointF rightbottom = QPointF(m_EndItem->GetCurrentStopX(),m_EndItem->GetCurrentStopY());

    QLineF::IntersectType type = sourceline.intersect(QLineF(lefttop,leftbottom), &interPos);
    if (type == QLineF::BoundedIntersection)
    {
        interPos.setX(interPos.x()-Distance);
        return true;
    }

    type = sourceline.intersect(QLineF(righttop,rightbottom), &interPos);
    if (type == QLineF::BoundedIntersection)
    {
        interPos.setX(interPos.x()+Distance);
        return true;
    }

    type = sourceline.intersect(QLineF(lefttop,righttop), &interPos);
    if (type == QLineF::BoundedIntersection)
    {
        interPos.setY(interPos.y()-Distance);
        return true;
    }

    type = sourceline.intersect(QLineF(leftbottom,rightbottom), &interPos);
    if (type == QLineF::BoundedIntersection)
    {
        interPos.setY(interPos.y()+Distance);
        return true;
    }

    return false;
}

void CoArrow::SetPoints(const double & fisrt, const double &second, const double custom, const char dice)
{
    if(custom == -1)
    {
        m_p3 = QPointF(fisrt ,second);
        m_p4 = QPointF(fisrt ,second);
    }
    else
    {
        if(dice == 0)
        {
            m_p3 = QPointF(fisrt ,custom);
            m_p4 = QPointF(second ,custom);
        }
        else
        {
            m_p3 = QPointF(custom ,fisrt);
            m_p4 = QPointF(custom ,second);
        }
    }
}

QMenu *CoArrow::CreateMenus()
{
    QMenu *menu = new QMenu();
    menu->setFont(QFont("黑体",11));
    QAction *_Yes = new QAction(QObject::tr("Yes属性"),this);
    connect(_Yes, &QAction::triggered, this, &CoArrow::SetYes);
    QAction *_No = new QAction(QObject::tr("No属性"),this);
    connect(_No, &QAction::triggered, this, &CoArrow::SetNo);

    menu->addAction(_Yes);
    menu->addAction(_No);
    return menu;
}

void CoArrow::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    CreateMenus()->exec(event->screenPos());
}
